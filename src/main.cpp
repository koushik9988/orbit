#include <vector>
#include "particle.h"
#include "iniparser.h"
#include "world.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;
using namespace constants;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "ERROR! At least one argument expected." << std::endl;
        return EXIT_FAILURE;
    }

    const std::string filename = argv[1];
    auto IniData = INIParser::parse(filename);

    auto object_section = IniData["Objects"];
    int number_of_object = object_section.size();

    // Pre-allocate vectors
    std::vector<int> id;
    std::vector<double> mass, x, y, vx, vy;

    id.reserve(number_of_object);
    mass.reserve(number_of_object);
    x.reserve(number_of_object);
    y.reserve(number_of_object);
    vx.reserve(number_of_object);
    vy.reserve(number_of_object);

    for (const auto& object_entry : object_section)
    {
        const std::string& line = object_entry.second;
        std::vector<std::string> tokens = INIParser::split(line, ',');

        if (tokens.size() == 6)
        {
            id.push_back(std::stoi(tokens[0]));
            mass.push_back(std::stod(tokens[1]));
            x.push_back(std::stod(tokens[2]));
            y.push_back(std::stod(tokens[3]));
            vx.push_back(std::stod(tokens[4]));
            vy.push_back(std::stod(tokens[5]));
        }
    }

    // Domain data
    int Nx = INIParser::getInt(IniData["World"], "Nx");
    int Ny = INIParser::getInt(IniData["World"], "Ny");

    // Time data
    double sim_time = INIParser::getDouble(IniData["Time"], "sim_time");
    int sim_step = INIParser::getInt(IniData["Time"], "sim_step");
    std::string norm = INIParser::getString(IniData["World"], "norm");


    World world(Nx, Ny);
    world.sim_time = sim_time;
    world.sim_step = sim_step;
    world.DT = world.sim_time/world.sim_step;
    world.norm = norm;

    std::vector<Particle> object_list;
    object_list.reserve(number_of_object);

    for (int i = 0; i < number_of_object; i++)
    {
        object_list.emplace_back(id[i], mass[i], x[i], y[i], vx[i], vy[i], world);
    }

    // Store trajectories for each object
    std::vector<std::vector<double>> traj_x(number_of_object);
    std::vector<std::vector<double>> traj_y(number_of_object);
    std::vector<double> posx(number_of_object), posy(number_of_object);

    // Colors for different objects 
    std::vector<std::string> colors = {"r", "b", "g", "y", "m", "c", "k"};
    if (number_of_object > colors.size())
    {
        colors.resize(number_of_object);
        for (int i = colors.size(); i < number_of_object; i++)
        {
            colors[i] = colors[i % 7];
        }
    }

    // Energy storage vectors
    std::vector<double> time_vec;
    std::vector<double> ke_vec;
    std::vector<double> pe_vec;
    std::vector<double> total_energy_vec;

    // Enable interactive mode once before the loop
    plt::ion();

    //Calculate unnormalized time (in days)
    double Mass_norm;
    if(norm == "earth")
    {
        Mass_norm = M_earth;
    }
    else if(norm == "sun")
    {
        Mass_norm = M_sun;
    }
    
    double unnorm_time = 1 / (sqrt(G * Mass_norm / (L * L * L)));
    unnorm_time = (unnorm_time /(60 * 60 * 24)) * world.DT; //in days

    for (int ts = 0; ts < sim_step; ts++)
    {
        for (auto &obj : object_list)
        {
            auto [fx, fy] = world.ComputeForce(obj, object_list);
            obj.update(fx, fy);
        }

        double total_ke = 0, total_pe = 0;
        for(auto &obj : object_list)
        {
            total_ke += obj.calculateKE();
        }
        total_pe = world.ComputePE(object_list); // Fixed method name

        std::cout << "Time step: " << ts <<" KE: "<<total_ke<<" PE: "<<total_pe<<" Total Energy :"<<total_ke + total_pe <<std::endl;

        // Store energy data
        time_vec.push_back(ts * unnorm_time);
        ke_vec.push_back(total_ke);
        pe_vec.push_back(total_pe);
        total_energy_vec.push_back(total_ke + total_pe);

        // Clear current positions
        posx.clear();
        posy.clear();
        posx.resize(number_of_object);
        posy.resize(number_of_object);

        // Update positions and trajectories
        for (int i = 0; i < number_of_object; i++)
        {
            posx[i] = object_list[i].x;
            posy[i] = object_list[i].y;
            traj_x[i].push_back(object_list[i].x);
            traj_y[i].push_back(object_list[i].y);
        }

        // Plot trajectories (default figure 1)
        plt::figure(1); // Explicitly set to figure 1
        plt::clf();
        for (int i = 0; i < number_of_object; i++)
        {
            plt::plot(traj_x[i], traj_y[i], {{"color", colors[i]}, {"linewidth", "1"}});
            plt::scatter(std::vector<double>{posx[i]}, std::vector<double>{posy[i]}, 50.0, {{"color", colors[i]}});
        }
        plt::xlabel("X(A.U.)");
        plt::ylabel("Y(A.U.)");
        plt::title("Simulation - days: " + std::to_string(ts * unnorm_time));
        plt::xlim(-world.Lx, world.Lx);
        plt::ylim(-world.Ly, world.Ly);
        plt::grid(true);

        // Plot energies (figure 2)
        plt::figure(2);
        plt::clf();
        plt::named_plot("Kinetic Energy", time_vec, ke_vec, "r-");
        plt::named_plot("Potential Energy", time_vec, pe_vec, "b-");
        plt::named_plot("Total Energy", time_vec, total_energy_vec, "g-");
        plt::xlabel("Time (days)");
        plt::ylabel("Energy");
        plt::title("Energy vs Time");
        plt::legend();
        //plt::grid(true);

        plt::draw();
        plt::pause(0.01);
    }

    plt::show();

    return 0;
}