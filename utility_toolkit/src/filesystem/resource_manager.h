#pragma once

#include <dll_defines.h>

#include <string>

class Resource_Manager
{
  public:
    DECLSPECIFIER static Resource_Manager& get_instance()
    {
        static Resource_Manager global_resource_manager{};
        return global_resource_manager;
    }

    /**
     * @brief Reads the contents of the file at the given path.
     * @param[in] filepath. Path at which to find the file (including extension).
     * @return The contents of the file, as a string.
     */
    DECLSPECIFIER std::string read_file(std::string const& filepath);
};
