#pragma once
#include <string>
#include <random>

std::string file_to_string(const std::string& file_path);

struct RandomState
{
    std::mt19937 engine;
    std::uniform_real_distribution<float> dist01;
    std::uniform_real_distribution<float> dist_m11;
};

RandomState random_init();

float random_01();
float random_m11();

extern RandomState g_random;
