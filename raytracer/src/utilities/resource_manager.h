#pragma once

#include <string>

class resource_manager
{
  public:
    static resource_manager& get_instance()
    {
        static resource_manager global_resource_manager{};
        return global_resource_manager;
    }

    /**
     * @brief Reads the contents of the file at the given path.
     * @param[in] filepath. Path at which to find the file (including extension).
     * @return The contents of the file, as a string.
     */
    std::string read_file(std::string const& filepath);
};
