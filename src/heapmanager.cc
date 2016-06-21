#include "heapmanager.hh"

#include <string>
#include "log.hh"

static const unsigned int ALLOCS_MAX = 1000;

static bool inited = false;
static unsigned int allocCounter = 0;
static void** allocs = nullptr;
static unsigned int allocsEnd = 0;

static void deinitAllocs();
static void findAndNull(void* toremove);

bool SHH::HeapManager::Init()
{
    allocs = new void* [ALLOCS_MAX];
    if (allocs == nullptr)
    {
	SHH::Log::Error("HeapManager::Init(): Could not allocate memory for list.");
	return false;
    }
    
    for (unsigned int i = 0; i < ALLOCS_MAX; ++i)
    {
	allocs[i] = nullptr;
    }

    inited = true;
    return true;
}

void SHH::HeapManager::Deinit()
{
    if (allocCounter > 0)
    {
	SHH::Log::Warning("HeapManager::Deinit(): " + std::to_string(allocCounter) + " allocations left unfreed.");
    }
    if (inited)
    {
	deinitAllocs();
	delete[] allocs;
	allocs = nullptr;
	allocCounter = 0;
	inited = false;
    }
}

void* SHH::HeapManager::Allocate(std::size_t count)
{
    void* results = malloc(count);
    if (results == nullptr)
    {
	return nullptr;
    }
    if (allocs != nullptr)
    {
	if (allocsEnd >= ALLOCS_MAX)
	{
	    SHH::Log::Warning("HeapManager::Allocate(): No room to keep track of allocation.");
	    return results;
	}
	++allocCounter;
	allocs[allocsEnd++] = results;
    }
    return results;
}

void SHH::HeapManager::Deallocate(void* ptr)
{
    if (ptr == nullptr)
    {
	return;
    }
    free(ptr);
    if (allocs != nullptr)
    {
	findAndNull(ptr);
    }
}

unsigned int SHH::HeapManager::GetAllocationCount()
{
    return allocCounter;
}

static void deinitAllocs()
{
    if (allocs == nullptr)
    {
	return;
    }
    for (unsigned int i = 0; i < ALLOCS_MAX; ++i)
    {
	if (allocs[i] == nullptr)
	{
	    break;
	}
	free(allocs[i]);
    }
}

static void findAndNull(void* toremove)
{
    if (allocs == nullptr)
    {
	return;
    }
    for (unsigned int i = 0; i < ALLOCS_MAX; ++i)
    {
	if (allocs[i] == nullptr)
	{
	    return;
	}
	else if (allocs[i] == toremove)
	{
	    --allocCounter;
	    --allocsEnd;
	    allocs[i] = allocs[allocsEnd];
	    allocs[allocsEnd] = nullptr;
	}
    }
}
