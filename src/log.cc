#include "log.hh"

#include <iostream>
#include <string>

static const int LOGSIZE = 100;
static std::string* log = nullptr;
static int logpos = 0;

static void LogWrite(std::string str);

bool SHH::Log::Init()
{
    log = new std::string [LOGSIZE];
    if (log == nullptr)
    {
	return false;
    }
    for (int i = 0; i < LOGSIZE; ++i)
    {
	log[i] = "";
    }
    return true;
}

void SHH::Log::Deinit()
{
    if (log != nullptr)
    {
	delete[] log;
    }
}

void SHH::Log::Log(std::string str)
{
    LogWrite(str);
}

void SHH::Log::Warning(std::string str)
{
    LogWrite("WARNING: " + str);
    std::cout << "WARNING: " << str << std::endl;
}

void SHH::Log::Error(std::string str)
{
    LogWrite("ERROR: " + str);
    std::cout << "ERROR: " << str << std::endl;
}

std::string SHH::Log::GetLogEntry(int logentry)
{
    if (logentry >= LOGSIZE)
    {
	return "";
    }
    int entrypos = (logpos - logentry - 1 + LOGSIZE) % LOGSIZE;
    return log[entrypos];
}

static void LogWrite(std::string str)
{
    log[logpos] = str;
    logpos = (logpos + 1) % LOGSIZE;
}
