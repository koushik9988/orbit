import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

class Object:
    def __init__(self, name, mass, size, sig, x, y, vx, vy):
        self.name = name
        self.mass = mass
        self.size = size
        self.sig = sig
        self.x = x
        self.y = y
        self.vx = vx
        self.vy = vy
        self.trail_x = []
        self.trail_y = []

au = 1.5e11
m_sun = 1.989e30  # kg
m_earth = 5.972e24  # kg
G = 6.67e-11  # m^3 kg^-1 s^-2

r_sun = 6.95e8
r_earth = 6371000

Lx = 10 * au
Ly = 10 * au

fig, ax = plt.subplots()


#---------massive --------------
#sun-1
x0 = 0*au
y0 = 0*au
vx0 = 0
vy0 = 0
size0 = 1*r_sun
m0 = 1*m_sun/2
sig0 = 50 #10*int(size0/r_earth)


#sun-2
x1 = -2*au
y1 = 5*au
vx1 = 0
vy1 = -1e3
size1 = r_sun
m1 = 1*m_sun/2
sig1 = 50 #10*int(size1/r_earth)

#sun-3
x5 = -7*au
y5 = 0*au
vx5 = 0
vy5 = 1e3
size5 = r_sun
m5 = 1*m_sun/2
sig5 = 50 #10*int(size1/r_earth)


#---------------------------------------
# planet-1
x2 = -.7*au
y2 = 0*au
vx2 = 0*4e4
vy2 = 2e4
m2 = 2*m_earth
size2 = r_earth
sig2 = 10   #10*int(size2/r_earth)

#planet-2
x3 = 0*au
y3 = 3*au
vx3 = 1e5
vy3 = 0
size3 = r_earth
m3 = 100*m_earth
sig3 = 5  #10*int(size3/r_earth)

#planet-3
x4 = 0*au
y4 = 4*au
vx4 = 1e5
vy4 = 0
size4 = 1*r_earth
m4 = 10*m_earth
sig4 = 10   #10*int(size4/r_earth)

sun1 = Object("sun1", m0, size0, sig0, x0, y0, vx0, vy0)
sun2 = Object("sun2", m1, size1, sig1, x1, y1, vx1, vy1)
sun3 = Object("sun3", m5, size5, sig5, x5, y5, vx5, vy5)
planet1 = Object("planet1", m2,  size2, sig2 ,  x2, y2, vx2, vy2)
planet2 = Object("planet2", m3, size3,  sig3, x3, y3, vx3, vy3)
planet3 = Object("planet3", m4, size4, sig4,  x4, y4, vx4, vy4)
planet4 = Object("planet4", m4, size4, sig4,  x4, (4.5/4)*y4, vx4, vy4)


#list_obj = [sun2, planet1, planet2]
list_obj = [sun1, sun2, planet1]
#list_obj = [sun1, planet1, planet2, planet3, planet4]

def compute_force(obj, list_obj):
    fx = 0
    fy = 0
    for i in list_obj:
        if obj != i:
            dx = obj.x - i.x
            dy = obj.y - i.y
            r = np.sqrt(dx**2 + dy**2)
            f = - G * obj.mass * i.mass / r**2
            fx += f * dx / r
            fy += f * dy / r
    return fx, fy

def compute_distance(obj1,obj2):
    dx = obj1.x - obj2.x
    dy = obj1.y - obj2.y
    return np.sqrt(dx**2 + dy**2)


def move(obj, fx, fy, dt):
    obj.vx += (fx / obj.mass) * dt
    obj.vy += (fy / obj.mass) * dt
    obj.x += obj.vx * dt
    obj.y += obj.vy * dt
    obj.trail_x.append(obj.x)
    obj.trail_y.append(obj.y)

def collision(obj, list_obj):
    objects_to_remove = []
    for i in list_obj:
        if obj != i:
            r = compute_distance(obj, i)
            if r <= obj.size + i.size:
                # inelastic collisions
                if obj.mass >= i.mass:
                    obj.vx = (obj.mass * obj.vx + i.mass * i.vx) / (obj.mass + i.mass) # using momentum conservation
                    obj.vy = (obj.mass * obj.vy + i.mass * i.vy) / (obj.mass + i.mass)
                    obj.mass += i.mass
                    objects_to_remove.append(i)
                else:
                    i.vx = (obj.mass * obj.vx + i.mass * i.vx) / (obj.mass + i.mass)
                    i.vy = (obj.mass * obj.vy + i.mass * i.vy) / (obj.mass + i.mass)
                    i.mass += obj.mass
                    objects_to_remove.append(obj)
    for obj in objects_to_remove:
        list_obj.remove(obj)


t = 0
dt = 3600*24 #Time step in seconds (5 days)
sim_time = 3600 * 24 * 365 *10 #days
while t < sim_time:  #Simulating 10 years
    for obj in list_obj:
        fx, fy = compute_force(obj, list_obj)
        move(obj, fx, fy, dt)
        collision(obj,list_obj)
    t += dt

    ax.clear()
    for obj in list_obj:
        ax.scatter(obj.x, obj.y, s = obj.sig , label = obj.name)
        ax.plot(obj.trail_x, obj.trail_y)
    ax.set_xlim([-Lx,Lx])
    ax.set_xlim([-Ly,Ly])
    ax.legend()
    #ax.set_title(f'Time: {int(sim_time * t / (dt ))} days')
    ax.set_title(f'Time: {(t / (3600*24))} days')
    plt.pause(1e-3)
    if(t % (3600*24*30) == 0):
        {
            plt.savefig(f'photo_{int(t / (3600 * 24))}.png')  # Save the photo
        }
    
plt.show()
