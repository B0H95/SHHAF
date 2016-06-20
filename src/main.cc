#include <iostream>
#include "log.hh"
#include "programcontroller.hh"
#include "heapmanager.hh"

int main(int argc, char* argv[])
{   
    std::cout << "main: Entered main." << std::endl;

    if (!SHH::HeapManager::Init())
    {
	std::cout << "ERROR: main: Could not init the heap manager." << std::endl;
	std::cout << "main: Exited main with return code 1." << std::endl;
	return 1;
    }

    if (!SHH::Log::Init())
    {
	std::cout << "ERROR: main: Could not init the log." << std::endl;
	SHH::HeapManager::Deinit();
	std::cout << "main: Exited main with return code 1." << std::endl;
	return 1;
    }

    if (!SHH::ProgramController::Init(argc, argv))
    {
	SHH::Log::Error("main: Could not init the program controller.");
	SHH::Log::Deinit();
	SHH::HeapManager::Deinit();
	std::cout << "main: Exited main with return code 1." << std::endl;
	return 1;
    }

    if (!SHH::ProgramController::Postinit())
    {
	SHH::Log::Error("main: Could not postinit the program controller.");
	SHH::ProgramController::Deinit();
	SHH::Log::Deinit();
	SHH::HeapManager::Deinit();
	std::cout << "main: Exited main with return code 1." << std::endl;
	return 1;
    }
    
    if (!SHH::ProgramController::GetParameter("-norun"))
    {
	SHH::ProgramController::Run();
    }

    SHH::ProgramController::Deinit();
    SHH::Log::Deinit();
    SHH::HeapManager::Deinit();
    
    std::cout << "main: Exited main with return code 0." << std::endl;
    return 0;
}
