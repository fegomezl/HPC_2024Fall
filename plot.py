import numpy as np
from glob import glob
import matplotlib.pyplot as plt
import matplotlib
matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'


def get_profiling(filename):
    data = np.genfromtxt(filename)
    
    times = []
    for n in range(1,33):
        times.append(min(data[:,1][np.where(data[:,0] == 1.0*n)]))
    speedup = [times[0]/times[n] for n in range(len(times))]
    efficiency = [speedup[n]/(n+1) for n in range(len(speedup))]
    return speedup, efficiency


files = glob('hw3/results/mmult*')
for file in files:
    data = np.genfromtxt(file)
print(len(data))

'''
#--------------------------------------------------------------------------------------------

fig, axs = plt.subplots(2, 1)

x = np.linspace(1,32,100)
axs[0].plot(x, x, color="black")
axs[1].axhline(y=1, color="black")

for N in [1024, 2048, 4096]:
    speedup, efficiency = get_profiling("results/mmult_{}.txt".format(N))
    
    axs[0].plot(range(1,len(speedup)+1), speedup, marker="o", label="N={}".format(N))
    axs[1].plot(range(1,len(efficiency)+1), efficiency, marker="o", label="N={}".format(N))

axs[0].set_xlabel("Threads")
axs[1].set_xlabel("Threads")
axs[0].set_ylabel("Speedup")
axs[1].set_ylabel("Efficiency")
axs[0].set_xlim(1,32)
axs[1].set_xlim(1,32)
axs[0].legend()
axs[1].legend()

fig.tight_layout()
fig.savefig("mmult.eps")
plt.show()

#--------------------------------------------------------------------------------------------

fig, axs = plt.subplots(2, 1)

x = np.linspace(1,32,100)
axs[0].plot(x, x, color="black")
axs[1].axhline(y=1, color="black")

for N in [6, 7, 8]:
    speedup, efficiency = get_profiling("results/rect_10_{}.txt".format(N))
    
    axs[0].plot(range(1,len(speedup)+1), speedup, marker="o", label="N={}".format(10**N))
    axs[1].plot(range(1,len(efficiency)+1), efficiency, marker="o", label="N={}".format(10**N))

axs[0].set_xlabel("Threads")
axs[1].set_xlabel("Threads")
axs[0].set_ylabel("Speedup")
axs[1].set_ylabel("Efficiency")
axs[0].set_xlim(1,32)
axs[1].set_xlim(1,32)
axs[0].legend()
axs[1].legend()

fig.tight_layout()
fig.savefig("rect.eps")
plt.show()

#--------------------------------------------------------------------------------------------

fig, axs = plt.subplots(2, 1)

x = np.linspace(1,32,100)
axs[0].plot(x, x, color="black")
axs[1].axhline(y=1, color="black")

for N in [4096, 8192, 15000]:
    speedup, efficiency = get_profiling("results/rk_{}.txt".format(N))
    
    axs[0].plot(range(1,len(speedup)+1), speedup, marker="o", label="N={}".format(N))
    axs[1].plot(range(1,len(efficiency)+1), efficiency, marker="o", label="N={}".format(N))

axs[0].set_xlabel("Threads")
axs[1].set_xlabel("Threads")
axs[0].set_ylabel("Speedup")
axs[1].set_ylabel("Efficiency")
axs[0].set_xlim(1,32)
axs[1].set_xlim(1,32)
axs[0].legend()
axs[1].legend()

fig.tight_layout()
fig.savefig("rk.eps")
plt.show()
'''