import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, FFMpegWriter
import sys
import os.path
from os.path import join as pjoin

file1 = 'sun1.txt'
file2 = 'sun2.txt'
file3 = 'sun3.txt'
file4 = 'ea.txt'
file5 = 'ea1.txt'

path = sys.argv[1]

if os.path.exists(pjoin(path, file1)):
    x1, y1 = np.loadtxt(pjoin(path, file1), unpack=True)
else:
    print('No data')
    exit()

if os.path.exists(pjoin(path, file2)):
    x2, y2 = np.loadtxt(pjoin(path, file2), unpack=True)
else:
    print('No data')
    exit()

if os.path.exists(pjoin(path, file3)):
    x3, y3 = np.loadtxt(pjoin(path, file3), unpack=True)
else:
    print('No data')
    exit()

if os.path.exists(pjoin(path, file4)):
    x4, y4 = np.loadtxt(pjoin(path, file4), unpack=True)
else:
    print('No data')
    exit()

if os.path.exists(pjoin(path, file5)):
    x5, y5 = np.loadtxt(pjoin(path, file5), unpack=True)
else:
    print('No data')
    exit()



# Define empty lists for trails
trail1_x, trail1_y = [], []
trail2_x, trail2_y = [], []
trail3_x, trail3_y = [], []
trail4_x, trail4_y = [], []
trail5_x, trail5_y = [], []

# Circle objects for trail tips (initially empty)
circle1, circle2, circle3, circle4 = None, None, None, None

# Creating the plot
fig, ax = plt.subplots(figsize=(10, 8))

# Setting limits
ax.set_xlim([-2, 2])
ax.set_ylim([-2, 2])

G = 6.67e-11
#characteristic lenghts
L = 1.5e11 #1 a.u
#characteristic mass
M = 5.97e24 #mass of earth in kg
M_sun = 1.989e30



sim_time = 50
n_step = 10000
dt = sim_time / n_step

#unmormallized time in seconds
dt_un = 1/(np.sqrt(G*M_sun/L**3))

dt_un = dt_un / (60 * 60 * 24)*dt

#print(int(700/dt))



def animate(i):

    global circle1, circle2, circle3, circle4 
    # Update trails
    trail1_x.append(x1[i])
    trail1_y.append(y1[i])
    trail2_x.append(x2[i])
    trail2_y.append(y2[i])
    trail3_x.append(x3[i])
    trail3_y.append(y3[i])
    trail4_x.append(x4[i])
    trail4_y.append(y4[i])

    # Update or create circle objects for trail tips
    if circle1 is None:
        circle1 = plt.Circle((trail1_x[-1], trail1_y[-1]), 0.01, color='red')
        ax.add_patch(circle1)
    else:
        circle1.center = (trail1_x[-1], trail1_y[-1])

    if circle2 is None:
        circle2 = plt.Circle((trail2_x[-1], trail2_y[-1]), 0.01, color='green')
        ax.add_patch(circle2)
    else:
        circle2.center = (trail2_x[-1], trail2_y[-1])

    if circle3 is None:
        circle3 = plt.Circle((trail3_x[-1], trail3_y[-1]), 0.01, color='blue')
        ax.add_patch(circle3)
    else:
        circle3.center = (trail3_x[-1], trail3_y[-1])

    if circle4 is None:
        circle4 = plt.Circle((trail4_x[-1], trail4_y[-1]), 0.01, color='black')
        ax.add_patch(circle4)
    else:
        circle4.center = (trail4_x[-1], trail4_y[-1])

    # Plot trails (optional, can be commented out if only circles are desired)
    ax.plot(trail1_x, trail1_y, alpha=0.7, color='red', linewidth=0.5)  # Adjust alpha for trail fading
    ax.plot(trail2_x, trail2_y, alpha=0.7, color='green', linewidth=0.5)
    ax.plot(trail3_x, trail3_y, alpha=0.7, color='blue', linewidth=0.5)
    ax.plot(trail4_x, trail4_y, alpha=0.7, color='black', linewidth=0.5)

    return circle1, circle2, circle3, circle4

# Creating the animation
ani = FuncAnimation(fig, animate, frames=len(x1), interval=10, blit=True)

plt.legend()
plt.show()

writer = FFMpegWriter(fps=10, metadata=dict(artist='kaushik'), bitrate=8000)
ani.save('orbit.mp4', writer='ffmpeg', fps=10, dpi=300)
