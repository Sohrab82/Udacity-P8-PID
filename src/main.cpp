#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s)
{
    auto found_null = s.find("null");
    auto b1 = s.find_first_of("[");
    auto b2 = s.find_last_of("]");
    if (found_null != string::npos)
    {
        return "";
    }
    else if (b1 != string::npos && b2 != string::npos)
    {
        return s.substr(b1, b2 - b1 + 1);
    }
    return "";
}

int main()
{
    uWS::Hub h;
    PID pid;
    // start (0.05, 0,0) works to some extent
    // add ki = 0.0001 overshoot and twirlling
    // reduce kp to 0.3

    // rewrite the file
    // pid.Init(.03, 0.00005, 0.3);//works but caanot follow sharp turns
    std::ofstream out_file("output.csv", std::ios::out);
    out_file.close();

    std::vector<float> steerings;
    pid.Init(0.5, 0.0, 5); //works but caanot follow sharp turns, a bit better following
    // pid.Init(.1, 0.00005, 0.3);   // too sensitive to curvature /error
    // pid.Init(.075, 0.00005, 0.3); // too sensitive to curvature /error starts twirlling
    // pid.Init(.05, 0.0005, 0.8);   // better perfomance but failed at too sharp corners
    // pid.Init(.08, 0.0005, 0.8);   // better perfomance , still slow to react, added steady state error (twirlling on the bridge)
    // pid.Init(.14, 0.01, 12);

    // pid.Init(0.1, 0.05, 2.);
    h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                       uWS::OpCode opCode)
                {
                    std::ofstream out_file("output.csv", std::ios::out | std::ios::app);
                    // "42" at the start of the message means there's a websocket message event.
                    // The 4 signifies a websocket message
                    // The 2 signifies a websocket event
                    if (length && length > 2 && data[0] == '4' && data[1] == '2')
                    {
                        auto s = hasData(string(data).substr(0, length));

                        if (s != "")
                        {
                            auto j = json::parse(s);

                            string event = j[0].get<string>();

                            if (event == "telemetry")
                            {
                                // j[1] is the data JSON object
                                double cte = std::stod(j[1]["cte"].get<string>());

                                double speed = std::stod(j[1]["speed"].get<string>());
                                double angle = std::stod(j[1]["steering_angle"].get<string>());
                                double steer_value;
                                pid.UpdateError(cte);
                                steer_value = pid.TotalError();
                                static_cast<void>(angle);
                                // DEBUG
                                std::cout << "CTE: " << cte << " Steering Value: " << steer_value
                                          << std::endl;
                                if (steer_value > 1.0)
                                    steer_value = 1.0;
                                if (steer_value < -1.0)
                                    steer_value = -1.0;
                                // moving average filter: did not work. added a lot of veering
                                // float cumsum = 0.0;
                                // for (size_t i = 0; i < steerings.size(); i++)
                                //     cumsum += steerings[i];
                                // cumsum += steer_value;
                                // steer_value = cumsum / (steerings.size() + 1);
                                // steerings.push_back(steer_value);
                                // if (steerings.size() > 3)
                                // {
                                //     steerings.erase(steerings.begin());
                                // }
                                out_file << cte << "," << steer_value << "," << speed << std::endl;
                                json msgJson;
                                msgJson["steering_angle"] = steer_value;
                                msgJson["throttle"] = 0.3;
                                auto msg = "42[\"steer\"," + msgJson.dump() + "]";
                                // std::cout << msg << std::endl;
                                ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
                            } // end "telemetry" if
                        }
                        else
                        {
                            // Manual driving
                            string msg = "42[\"manual\",{}]";
                            ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
                        }
                    } // end websocket message if
                });   // end h.onMessage

    h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req)
                   { std::cout << "Connected!!!" << std::endl; });

    h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                           char *message, size_t length)
                      {
                          ws.close();
                          std::cout << "Disconnected" << std::endl;
                      });

    int port = 4567;
    if (h.listen(port))
    {
        std::cout << "Listening to port " << port << std::endl;
    }
    else
    {
        std::cerr << "Failed to listen to port" << std::endl;
        return -1;
    }

    h.run();
}