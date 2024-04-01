import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, FFMpegWriter
import sys
import os.path
from os.path import join as pjoin

path = sys.argv[1]

#file_names = ['sun.txt', 'mercury.txt', 'venus.txt', 'earth.txt', 'moon.txt'] 
file_names = ['sun1.txt', 'sun2.txt', 'sun3.txt', 'ea.txt', 'ea1.txt']   

fig, ax = plt.subplots(figsize=(10, 8))
trails = []
scatters = []

for file_name in file_names:
    file_path = pjoin(path, file_name)
    if os.path.exists(file_path):
        x, y = np.loadtxt(file_path, unpack=True)
        trail, = ax.plot([], [], label=file_name[:-4])
        trails.append(trail)
        scatter = ax.scatter([], [])
        scatters.append(scatter)
    else:
        print(f'No data for {file_name}')
        trails.append(None)
        scatters.append(None)

ax.set_xlim([-3, 3])
ax.set_ylim([-3, 3])
ax.legend()

G = 6.67e-11
L = 1.5e11
M = 5.97e24
M_sun = 1.989e30

sim_time = 1
n_step = 100000
dt = sim_time / n_step

dt_un = 1 / (np.sqrt(G * M / L ** 3))
dt_un = dt_un / (60 * 60 * 24) * dt

def animate(i):
    for idx, file_name in enumerate(file_names):
        if trails[idx] is not None:
            file_path = pjoin(path, file_name)
            if os.path.exists(file_path):
                x, y = np.loadtxt(file_path, unpack=True)
                trails[idx].set_data(x[:i], y[:i])
                scatters[idx].set_offsets([[x[i - 1], y[i - 1]]])
    ax.set_title(f'Time: {i*dt_un} days')
    return trails + scatters

ani = FuncAnimation(fig, animate, frames=n_step, interval=10, blit=True)

plt.show()

writer = FFMpegWriter(fps=10, metadata=dict(artist='kaushik'), bitrate=8000)
ani.save('orbit.mp4', writer='ffmpeg', fps=10, dpi=300)
