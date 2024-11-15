import numpy as np
from glob import glob
import matplotlib.pyplot as plt
import matplotlib
matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'

def get_profiling(filename):
    files = glob(filename)
    speedup = np.zeros(32)
    for file in files:
        data = np.genfromtxt(file, skip_header=1)
        times = data[:,1]
        speedup = [max(speedup[n], times[0]/times[n]) for n in range(len(times))]
    efficiency = [speedup[n]/(n+1) for n in range(len(speedup))]
    return speedup, efficiency

#--------------------------------------------------------------------------------------------

fig, axs = plt.subplots(2, 1)

x = np.linspace(1,32,100)
axs[0].plot(x, x, color="black")
axs[1].axhline(y = 1, color="black")

for N in [256, 512, 700]:
    speedup, efficiency = get_profiling("hw4/results/mmult_{}*".format(N))
    
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
fig.savefig("hw4/mmult.eps")
plt.show()

#--------------------------------------------------------------------------------------------

fig, axs = plt.subplots(2, 1)

x = np.linspace(1,32,100)
axs[0].plot(x, x, color="black")
axs[1].axhline(y=1, color="black")

for N in [6, 7, 8]:
    speedup, efficiency = get_profiling("hw4/results/trape_10_{}*".format(N))
    
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
fig.savefig("hw4/trap.eps")
plt.show()

#--------------------------------------------------------------------------------------------

fig, axs = plt.subplots(2, 1)

x = np.linspace(1,32,100)
axs[0].plot(x, x, color="black")
axs[1].axhline(y=1, color="black")

for N in [2048, 4096, 8192]:
    speedup, efficiency = get_profiling("hw4/results/rk4_{}*".format(N))
    
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
fig.savefig("hw4/rk4.eps")
plt.show()