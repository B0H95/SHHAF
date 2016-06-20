#include "heapmanager.hh"

#include <iostream>

static const unsigned int ALLOCS_MAX = 1000;
static const unsigned int LISTALLOCS_MAX = 1000;

static bool inited = false;
static unsigned int allocCounter = 0;
static void** allocs = nullptr;
static void** listallocs = nullptr;
static unsigned int allocsEnd = 0;
static unsigned int listallocsEnd = 0;

static void deinitList(void** pointerlist, unsigned int maxvalue);
static void findAndNull(void** pointerlist, unsigned int maxvalue, unsigned int endindex, void* toremove);

bool SHH::HeapManager::Init()
{
    allocs = new void* [ALLOCS_MAX];
    if (allocs == nullptr)
    {
	std::cout << "ERROR: HeapManager::Init(): Could not allocate memory for allocs." << std::endl;
	return false;
    }

    listallocs = new void* [LISTALLOCS_MAX];
    if (listallocs == nullptr)
    {
	std::cout << "ERROR: HeapManager::Init(): Could not allocate memory for allocs." << std::endl;
	delete[] allocs;
	return false;
    }
    
    inited = true;
    return true;
}

void SHH::HeapManager::Deinit()
{
    if (allocCounter > 0)
    {
	std::cout << "WARNING: HeapManager::Deinit(): " << allocCounter << " allocations left unfreed." << std::endl;
    }
    if (inited)
    {
	deinitList(allocs, ALLOCS_MAX);
	deinitList(listallocs, LISTALLOCS_MAX);
	delete[] allocs;
	delete[] listallocs;
	allocCounter = 0;
	inited = false;
    }
}

void* SHH::HeapManager::Allocate(std::size_t count)
{
    if (allocsEnd >= ALLOCS_MAX || allocs == nullptr)
    {
	return nullptr;
    }
    void* results = malloc(count);
    if (results != nullptr)
    {
	++allocCounter;
	allocs[allocsEnd++] = results;
    }
    return results;
}

void* SHH::HeapManager::AllocateList(std::size_t count)
{
    if (listallocsEnd >= ALLOCS_MAX || listallocs == nullptr)
    {
	return nullptr;
    }
    void* results = malloc(count);
    if (results != nullptr)
    {
	++allocCounter;
	listallocs[listallocsEnd++] = results;
    }
    return results;
}

void SHH::HeapManager::Deallocate(void* ptr)
{
    if (allocs != nullptr && ptr != nullptr)
    {
	free(ptr);
	--allocCounter;
	--allocsEnd;
	findAndNull(allocs, ALLOCS_MAX, allocsEnd, ptr);
    }
}

void SHH::HeapManager::DeallocateList(void* ptr)
{
    if (listallocs != nullptr && ptr != nullptr)
    {
	free(ptr);
	--allocCounter;
	--listallocsEnd;
	findAndNull(listallocs, LISTALLOCS_MAX, listallocsEnd, ptr);
    }
}

unsigned int SHH::HeapManager::GetAllocationCount()
{
    return allocCounter;
}

static void deinitList(void** pointerlist, unsigned int maxvalue)
{
    if (pointerlist == nullptr)
    {
	return;
    }
    for (unsigned int i = 0; i < maxvalue; ++i)
    {
	if (pointerlist[i] == nullptr)
	{
	    break;
	}
	free(pointerlist[i]);
    }
}

static void findAndNull(void** pointerlist, unsigned int maxvalue, unsigned int endindex, void* toremove)
{
    for (unsigned int i = 0; i < maxvalue; ++i)
    {
	if (pointerlist[i] == nullptr)
	{
	    return;
	}
	else if (pointerlist[i] == toremove)
	{
	    pointerlist[i] = pointerlist[endindex];
	    pointerlist[endindex] = nullptr;
	}
    }
}
