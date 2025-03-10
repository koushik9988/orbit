#include "particle.h"
#include "world.h"

Particle::Particle(int id, double mass,  double x, double y, double vx, double vy, World &world):world(world)
{
    this->id= id;
    this->mass = mass;
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy; 
}

Particle::~Particle()
{

}

//method function implemenation
void Particle::update(double fx, double fy)
{
    //update velocity
    vx += (fx/mass)*world.DT;
    vy += (fy/mass)*world.DT;

    //update position
    x += vx*world.DT;
    y += vy*world.DT;
}


double Particle::calculateKE()
{
    double v2 = vx * vx + vy * vy;
    return 0.5 * mass * v2;
}

