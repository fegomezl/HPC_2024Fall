import numpy as np
import matplotlib.pyplot as plt

def get_profiling(filename):
    data = np.genfromtxt(filename)
    
    times = []
    for n in range(1,33):
        times.append(min(data[:,1][np.where(data[:,0] == 1.0*n)]))
    speedup = [times[0]/times[n] for n in range(len(times))]
    efficiency = [speedup[n]/(n+1) for n in range(len(speedup))]
    return speedup, efficiency

speedup, efficiency = get_profiling("hw3/results/rk_4096.txt")

x = np.linspace(1,len(speedup),100)
plt.plot(x, x, color="black")
plt.plot(range(1,len(speedup)+1), speedup, marker="o")
plt.show()

plt.axhline(y=1, color="black")
plt.plot(range(1,len(efficiency)+1), efficiency, marker="o")
plt.show()