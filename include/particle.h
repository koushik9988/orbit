#ifndef _Particle_
#define _Particle_

#include <iostream>
#include <vector>

class World;

class Particle
{
    public:
    double x, y, vx, vy;
    double mass;
    int id;
    Particle(int id, double x, double y, double vx, double vy, double mass, World &world);
    ~Particle();

    void update(double fx, double fy);

    double calculateKE();
    
    private:
    World &world;
};

#endif