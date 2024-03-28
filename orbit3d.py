import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

class Object:
    def __init__(self, name, mass, size, sig, x, y, z, vx, vy, vz):
        self.name = name
        self.mass = mass
        self.size = size
        self.sig = sig
        self.x = x
        self.y = y
        self.z = z
        self.vx = vx
        self.vy = vy
        self.vz = vz
        self.trail_x = []
        self.trail_y = []
        self.trail_z = []

au = 1.5e11
m_sun = 1.989e30  # kg
m_earth = 5.972e24  # kg
G = 6.67e-11  # m^3 kg^-1 s^-2

r_sun = 6.95e8
r_earth = 6371000

L = 8
Lx = L * au
Ly = L * au
Lz = L * au

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Objects
# sun-1
sun1 = Object("sun1", 2*m_sun, r_sun, 50, 0*au, 0*au, 0*au, 0, 0, 0)
# planet-1
planet1 = Object("planet1", m_earth, r_earth, 10, 2*au, 2*au, 1*au, 0*4e4, 2e4, 0)

planet2 = Object("planet2", 1*m_earth, r_earth, 10, 4*au, 3*au, -1*au, 0*4e3, 8*1e3, 0)

planet3 = Object("planet3", 2*m_earth, r_earth, 10, 0*au, 0*au, 3*au, 1*1e4, 0*2e5, 0)

list_obj = [sun1, planet1, planet2, planet3 ]

def compute_force(obj, list_obj):
    fx, fy, fz = 0, 0, 0
    for i in list_obj:
        if obj != i:
            dx = obj.x - i.x
            dy = obj.y - i.y
            dz = obj.z - i.z
            r = np.sqrt(dx**2 + dy**2 + dz**2)
            f = - G * obj.mass * i.mass / r**2
            fx += f * dx / r
            fy += f * dy / r
            fz += f * dz / r
    return fx, fy, fz

def compute_distance(obj1, obj2):
    dx = obj1.x - obj2.x
    dy = obj1.y - obj2.y
    dz = obj1.z - obj2.z
    return np.sqrt(dx**2 + dy**2 + dz**2)

def move(obj, fx, fy, fz, dt):
    obj.vx += (fx / obj.mass) * dt
    obj.vy += (fy / obj.mass) * dt
    obj.vz += (fz / obj.mass) * dt
    obj.x += obj.vx * dt
    obj.y += obj.vy * dt
    obj.z += obj.vz * dt
    obj.trail_x.append(obj.x)
    obj.trail_y.append(obj.y)
    obj.trail_z.append(obj.z)

def collision(obj, list_obj):
    objects_to_remove = []
    for i in list_obj:
        if obj != i:
            r = compute_distance(obj, i)
            if r <= obj.size + i.size:
                # inelastic collisions
                if obj.mass >= i.mass:
                    obj.vx = (obj.mass * obj.vx + i.mass * i.vx) / (obj.mass + i.mass)  # using momentum conservation
                    obj.vy = (obj.mass * obj.vy + i.mass * i.vy) / (obj.mass + i.mass)
                    obj.vz = (obj.mass * obj.vz + i.mass * i.vz) / (obj.mass + i.mass)
                    obj.mass += i.mass
                    objects_to_remove.append(i)
                else:
                    i.vx = (obj.mass * obj.vx + i.mass * i.vx) / (obj.mass + i.mass)
                    i.vy = (obj.mass * obj.vy + i.mass * i.vy) / (obj.mass + i.mass)
                    i.vz = (obj.mass * obj.vz + i.mass * i.vz) / (obj.mass + i.mass)
                    i.mass += obj.mass
                    objects_to_remove.append(obj)
    for obj in objects_to_remove:
        list_obj.remove(obj)

t = 0
dt = 3600*24*1 # Time step in seconds (5 days)
sim_time = 3600 * 24 * 365 * 100  # 10 years


while t < sim_time:
    for obj in list_obj:
        fx, fy, fz = compute_force(obj, list_obj)
        move(obj, fx, fy, fz, dt)
        collision(obj, list_obj)
    t += dt

    ax.clear()
    for obj in list_obj:
        ax.scatter3D(obj.x, obj.y, obj.z, label = obj.name)
        ax.plot3D(obj.trail_x, obj.trail_y, obj.trail_z)
    ax.set_xlim([-Lx, Lx])
    ax.set_ylim([-Ly, Ly])
    ax.set_zlim([-Lz, Lz])
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    ax.set_title(f'Time: {t / (3600*24)} days')
    ax.legend()
    plt.pause(0.001)

plt.show()
