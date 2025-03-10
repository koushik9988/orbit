#include "iniparser.h"


std::map<std::string, std::map<std::string, std::string>> INIParser::parse(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::map<std::string, std::map<std::string, std::string>> sections;
    std::string currentSection;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == ';' || line[0] == '#')
        {
            continue;
        }

        if (line[0] == '[' && line[line.length() - 1] == ']')
        {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }

        if (currentSection == "Objects")
        {
            // Handle species section differently
            sections[currentSection][std::to_string(sections[currentSection].size())] = line;
        }
        else
        {
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=') && std::getline(iss, value))
            {
                trim(key);
                trim(value);
                sections[currentSection][key] = value;
            }
        }
    }

    return sections;
}



int INIParser::getInt(const std::map<std::string, std::string> &section, const std::string &key) 
{
    return std::stoi(section.at(key));
}

double INIParser::getDouble(const std::map<std::string, std::string> &section, const std::string &key) 
{
    return std::stod(section.at(key));
}

std::string INIParser::getString(const std::map<std::string, std::string> &section, const std::string &key) 
{
    return section.at(key);
}


void INIParser::trim(std::string &str)
{
    if (str.empty()) 
    {
        return; //Return early if string is empty
    }
    
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");

    if (first == std::string::npos) {
        str.clear(); // Entire string is whitespace; clear the string
    } 
    else 
    {
        str = str.substr(first, (last - first + 1));
    }
}

// Function to split a string by a delimiter
std::vector<std::string> INIParser::split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) 
    {
        tokens.push_back(token);
    }
    return tokens;
}
