#include "utils.h"
#include <fstream>
#include <iostream>

std::string file_to_string(const std::string& file_path)
{
    std::string temp_line;
    std::string file_string = "";
    std::ifstream file(file_path);

    if (!file.is_open())
    {
        std::cout << "ERROR: Can't open file:" + file_path << "\n";
        return "";
    }
    while (std::getline(file, temp_line))
    {
        file_string += temp_line + '\n';
    }
    file.close();

    return file_string;
}

RandomState random_init()
{
    RandomState rs = {
        std::mt19937(std::random_device{}()), std::uniform_real_distribution<float>(0.0f, 1.0f),
        std::uniform_real_distribution<float>(-1.0f, 1.0f)};
    return rs;
}

float random_01()
{
    return g_random.dist01(g_random.engine);
}

float random_m11()
{
    return g_random.dist_m11(g_random.engine);
}

RandomState g_random = random_init();
