#include "resource_manager.h"

#include <fstream>
#include <string>

std::string Resource_Manager::read_file(std::string const& filepath)
{
    std::string contents;
    std::ifstream file(filepath);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            contents += line + "\n";
        }
        file.close();
    }
    return contents;
}
