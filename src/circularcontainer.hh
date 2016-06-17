#pragma once

#include "log.hh"

template <class T> class CircularContainer
{
public:
    CircularContainer(unsigned int newsize);
    ~CircularContainer();

    bool Push(T const& element);
    T Pop();
    bool IsValid();
    bool Reinit(unsigned int newsize);
    void Deinit();
    int Size();

private:
    unsigned int maxsize;
    unsigned int size;
    unsigned int start;
    unsigned int end;
    T* list;
};

template <class T> CircularContainer<T>::CircularContainer(unsigned int newsize) :
    maxsize(newsize),
    size(0),
    start(0),
    end(0),
    list(nullptr)
{
    list = new T [newsize];
    if (list == nullptr)
    {
	SHH::Log::Error("CircularContainer::CircularContainer(): Could not allocate memory for list.");
    }
}

template <class T> CircularContainer<T>::~CircularContainer()
{
    Deinit();
}

template <class T> bool CircularContainer<T>::Push(T const& element)
{
    if (size >= maxsize)
    {
	SHH::Log::Warning("CircularContainer::Push(): List is filled.");
	return false;
    }
    
    list[end] = element;
    end = (end + 1) % maxsize;
    size++;
    return true;
}

template <class T> T CircularContainer<T>::Pop()
{
    int temp = start;
    if (size > 0)
    {
	start = (start + 1) % maxsize;
	size--;
    }
    return list[temp];
}

template <class T> bool CircularContainer<T>::IsValid()
{
    return list != nullptr;
}

template <class T> bool CircularContainer<T>::Reinit(unsigned int newsize)
{
    if (list != nullptr)
    {
	delete[] list;
    }
    maxsize = newsize;
    size = 0;
    start = 0;
    end = 0;
    list = new T [newsize];
    if (list == nullptr)
    {
	SHH::Log::Error("CircularContainer::Reinit(): Could not allocate memory for list.");
	return false;
    }
    return true;
}

template <class T> void CircularContainer<T>::Deinit()
{
    if (list != nullptr)
    {
	delete[] list;
	list = nullptr;
    }
}

template <class T> int CircularContainer<T>::Size()
{
    return size;
}
