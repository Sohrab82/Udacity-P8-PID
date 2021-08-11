import matplotlib.pyplot as plt
import numpy as np


with open("build/output.csv", 'r') as f:
    lines = f.readlines()
lines = [line.strip().split(',') for line in lines]

results = []
for line in lines:
    l = []
    for el in line:
        el = float(el)
        l.append(el)
    results.append(l)

lines = np.array(results)
print(lines)

fig = plt.figure(figsize=(15, 15))
plt.subplot(311)
plt.plot(lines[:, 0], '*')
plt.title('CTE')

plt.subplot(312)
plt.plot(lines[:, 1], '*')
plt.title('steering angle')

plt.subplot(313)
plt.plot(lines[:, 2], '*')
plt.title('speed')
plt.savefig('output_images/results.jpg')
plt.show()
