#include "log.hh"

#include <iostream>

bool SHH::Log::Init()
{
    return true;
}

void SHH::Log::Deinit()
{
}

void SHH::Log::Log(std::string str)
{
    std::cout << str << std::endl;
}

void SHH::Log::Warning(std::string str)
{
    std::cout << "WARNING: " << str << std::endl;
}

void SHH::Log::Error(std::string str)
{
    std::cout << "ERROR: " << str << std::endl;
}
