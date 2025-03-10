#include "world.h"

World::World(int Lx, int Ly)
{
    this->Lx = Lx;
    this->Ly = Ly;
}

std::tuple<double, double> World::ComputeForce(Particle& p0, std::vector<Particle> &particle_list)
{
    double fx = 0.0, fy = 0.0;
    for(auto& p : particle_list)
    {
        if(p0.id != p.id)
        {
            double dx = p0.x - p.x;
            double dy = p0.y - p.y;
            double r  = sqrt(dx*dx + dy*dy);
            if (r > 1e-6)
            {
                double f = -(p0.mass * p.mass) / (r * r);
                fx += f * dx / r;
                fy += f * dy / r;
            }
        }
    }
    return std::make_tuple(fx, fy);
}

// Calculate total potential energy of the system
double World::ComputePE(const std::vector<Particle> &particle_list)
{
    double total_pe = 0.0;

    for (int i = 0; i < particle_list.size(); ++i)
    {
        for (int j = i + 1; j < particle_list.size(); ++j)
        {
            const Particle& p1 = particle_list[i];
            const Particle& p2 = particle_list[j];

            double dx = p1.x - p2.x;
            double dy = p1.y - p2.y;
            double r = sqrt(dx * dx + dy * dy);

            if (r > 1e-6) {  // Avoid division by zero
                // Version 1: Matching your force convention (no explicit G)
                total_pe -= (p1.mass * p2.mass) / r;

                // Version 2: With explicit G (uncomment if needed)
                // total_pe -= (G * p1.mass * p2.mass) / r;
            }
        }
    }
    return total_pe;
}
