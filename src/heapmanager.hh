#pragma once

#include <cstring>

namespace SHH
{
    namespace HeapManager
    {
	bool Init();
	void Deinit();
	void* Allocate(std::size_t count);
	void Deallocate(void* ptr);
	unsigned int GetAllocationCount();
    }
}