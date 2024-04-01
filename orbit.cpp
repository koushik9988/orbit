#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>

//namespace fs = std::filesystem;

class Object {
public:
    std::string name;
    double mass;
    double x;
    double y;
    double vx;
    double vy;

    double fx;
    double fy;
   
    Object(std::string name, double mass, double x, double y, double vx, double vy)
    {
        this->name = name;
        this->mass = mass;
        this->x = x;
        this->y = y;
        this->vx = vx;
        this->vy = vy;
    }

    void Compute_force(Object &obj, std::vector<Object> &object_list)
    {
        for (Object &i : object_list)
        {
            if (obj.name != i.name)
            {
                double dx = obj.x - i.x;
                double dy = obj.y - i.y;
                double r = sqrt(dx * dx + dy * dy);
                double f = (- obj.mass * i.mass) / (r * r);
                obj.fx += f * dx / r;
                obj.fy += f * dy / r;
            }
        }
    }

    void move(double dt)
    {
        vx += (fx / mass) * dt;
        vy += (fy / mass) * dt;

        x += vx * dt;
        y += vy * dt;
    }
};

class Output {
public:
    Output(const std::filesystem::path &outputfolder, const std::vector<Object>& object_list) : outputfolder(outputfolder)
    {
        std::filesystem::remove_all(outputfolder);  // Clear previous output
        std::filesystem::create_directories(outputfolder);
        std::filesystem::create_directories(outputfolder / "files");

        // Open files for each object
        for (const Object& obj : object_list) 
        {
            std::string filename = outputfolder / "files" / (obj.name + ".txt");
            files.emplace(obj.name, std::ofstream(filename));

            if (!files[obj.name].is_open())
            {
                throw std::runtime_error("Error opening file for " + obj.name);
            }
        }
    }

    void write_data(int ts, const Object &obj)
    {
        // Write data to the appropriate file for the object
        files[obj.name] << obj.x << "\t" << obj.y << std::endl;
    }

private:
    std::filesystem::path outputfolder;
    // Use a map to store file streams for each object
    std::map<std::string, std::ofstream> files;
};

const double G = 6.67e-11;
//characteristic lengths
const double L = 1.5e11; //1 a.u
//characteristic mass
const double M = 5.97e24; //mass of earth in kg
const double M_sun = 1.989e30;

int main()
{
    std::string outputfolder = "data";

    std::vector<Object> object_list;

    // --------------------------------------------------------------
    object_list.emplace_back("sun1", 1, -1, 0, 0*0.347113, 0*0.532727);
    object_list.emplace_back("sun2", 1, 1, 0, 0*0.347113, 0*0.532727);
    object_list.emplace_back("sun3", 1, 0, 0, 0*-0.694226,  0*-1.065454);
    object_list.emplace_back("ea", 100*M/M_sun, 0.2, 0, 0, 1);
    object_list.emplace_back("ea1", 100*M/M_sun, 0.8, 0, 0, -1);
    //------------------------------------------------------------------
    
    // --------------------solar system----------------
    //object_list.emplace_back("sun", 333000, 0, 0, 0, 0);
    //object_list.emplace_back("mercury", 0.05553, .39, 0, 0, sqrt(333000/0.39));
    //object_list.emplace_back("venus",0.815, .72, 0, 0, sqrt(333000/0.72));
    //object_list.emplace_back("earth",1, 1, 0, 0, sqrt(333000/1));
    //object_list.emplace_back("moon",0.0123, 1, 0.00256953, -sqrt(1/0.00256953) , sqrt(333000/1) );



    //normalized sim time
    double sim_time = 30;
    int n_step = 50000;
    double dt = sim_time / n_step;

    //unnormalized time in seconds
    double dt_un = 1 / (sqrt(G * M / (L * L)));

    dt_un = dt_un / (60 * 60 * 24) * dt;

    Output output(outputfolder, object_list);

    for (int ts = 0; ts < n_step; ++ts)
    {
        for (auto &obj : object_list)
        {
            obj.fx = 0.0;
            obj.fy = 0.0;
        }

        for (auto &obj : object_list)
        {
            obj.Compute_force(obj, object_list);
            obj.move(dt);
        }


        for (auto &obj : object_list) 
        {
            output.write_data(ts, obj);
        }
       
    
    }

    return 0;
}
