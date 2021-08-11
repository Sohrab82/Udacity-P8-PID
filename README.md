# Udacity Project: PID
Self-Driving Car Engineer Nanodegree Program

---

[//]: # (Image References)

[image1]: ./output_images/p_005.jpg "Images"
[image2]: ./output_images/p_05.jpg "Training data distrib"
[image3]: ./output_images/p_05_d_05.jpg "Validation data distrib"
[image4]: ./output_images/p_05_d_5.jpg "Testing data distrib"
[image41]: ./output_images/sample_30.jpg "speed 80"
[image5]: ./output_images/lenet.jpg "lenet train"
[image6]: ./output_images/incept.jpg "incept train"
[image7]: ./output_images/new_images.jpg "new images"

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

Fellow students have put together a guide to Windows set-up for the project [here](https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/files/Kidnapped_Vehicle_Windows_Setup.pdf) if the environment you have set up for the Sensor Fusion projects does not work for this project. There's also an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3).

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## PID hyperparameters

In the following, starting values for the hyperparameters, and the effects of changing them is presented.
* `Rise Time`: how fast the system reacts to changes and compensates for errors
* `Overshoot`: compensating the error but losing control afterwards and going right and left 


| Closed Loop Response | Rise Time    | Overshoot | Settling Time | Steady State Error |
| -------------------- | ------------ | --------- | ------------- | ------------------ |
| Increase Kp          | Decrease     | Increase  | Small change  | Decrease           |
| Increase Ki          | Decrease     | Increase  | Increase      | Eliminate          |
| Increase Kd          | Small change | Decrease  | Decrease      | Small change       |


 Starting with PID controller parameters (0.05, 0,0), works to some extent, but the error grows as the car moves forward and it is hard for the car to follow the sharp curves.
![alt text][image1]
  
Increasing the P parameter to 0.05 makes the response sharper, the car starts to sway side to side, and eventually drives out of the road. It can be seen that CTE reaches 5 much faster compared to previous case of P=0.05, and the steering angle correction goes out of range [-1, 1].
![alt text][image2]
* The sharp response represents a small "Rise Time",
* the swaying represents a long settling time
* the out-of-range steering angles shows overshoot

By referring to the table above, it can be seen that adding a D-parameter can reduce both overshoot and settling time.
Results for (0.5, 0, 0.5) can be seen here
![alt text][image3]
As expected, the overshoot and swaying was reduced, but the car still drove out of the road.

By increasing the D-parameter to 5, the following results were obtained
![alt text][image4]

The error (CTE) is contained and the steering angle has a nicer response. The car manages to go around the track without going off-road, but has a sharp response around the corners.

To study the effects of I-parameter, I=0.5 (0.5, 0.5, 5) was considered and the car drove off-road right away.

Reducing I-parameter to 0.005 fixed the issue, but in this case, as the car in the simulator has no consistent errors, the I-parameter is not beneficial since the I-parameter more or less has the same effects of the D-parameter except in compensating the steady-state errors.



