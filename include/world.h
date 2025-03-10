#ifndef _World_
#define _World_

#include <iostream>
#include <vector>
#include "particle.h"
#include <cmath>
#include <tuple>

class Particle;

class World
{
    public:
    int Lx,Ly;
    double DT;
    std::vector<Particle> particle_list;
    World(int Lx, int Ly);
    double sim_time;
    int sim_step;
    std::string norm;

    std::tuple<double, double> ComputeForce(Particle &p0, std::vector<Particle> &particle_list);
    double ComputePE(const std::vector<Particle> &particle_list);
    
};


namespace constants  // Changed from 'const' to 'constants'
{
    // Gravitational constant (m³ kg⁻¹ s⁻²)
    const double G = 6.67e-11;
    // Astronomical length (m, roughly 1 AU)
    const double L = 1.5e11;
    // Mass of Earth (kg)
    const double M_earth = 5.97e24;
    // Mass of Sun (kg)
    const double M_sun = 1.989e30;
}

#endif