/*
this header contain function prototypes that helps to parse a .ini file using STL map container and template functions 
*/
#ifndef INIPARSER_H
#define INIPARSER_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "world.h"

class World;

class INIParser 
{
    public:

    //INIParser(World &world):world(world){};
    static std::map<std::string, std::map<std::string, std::string>> parse(const std::string &filename);
    //static std::map<std::string, std::vector<std::string>> parse(const std::string& filename);
    static int getInt(const std::map<std::string, std::string> &section, const std::string &key);
    static double getDouble(const std::map<std::string, std::string> &section, const std::string &key);
    static std::string getString(const std::map<std::string, std::string> &section, const std::string &key);
    static std::vector<std::string> split(const std::string &str, char delimiter);
    //static std::pair<std::string, int> loadtypeextract(const std::string &position_init);
    
    private:
    static void trim(std::string& str);
    //World &world;
    
};

#endif // INIPARSER_H
