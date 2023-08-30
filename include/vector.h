#include <iostream>
#include <initializer_list>

#ifndef _VECTOR_HEADER_INCLUDED_
#define _VECTOR_HEADER_INCLUDED_

class Vector{
private:
    int* data;
    size_t size;
    size_t capacity;

private:
    void resize();
    void copyFrom(const Vector& other);
    void free();
    void swap(int& first, int& second);
public:
    Vector();
    Vector(const Vector&);
    Vector& operator=(const Vector&);
    ~Vector();

    Vector(int* newData, size_t newSize, size_t newCapacity);

    //(pushes an element to the back of the vector)
    void pushBack(const int number);
    //(only changes Vector[index])
    // Vector& operator=(int* array);
    Vector(std::initializer_list<int> initList);

    int operator[](const size_t index) const;
    //(adds new vector data to our vector)
    Vector operator+(const Vector&);
    //(adds new vector data to our vector and returns our vector reference)
    Vector& operator+=(const Vector&);
    //(checks if vectors are same)
    bool operator==(const Vector&) const;
    //(removes last element)
    void pop();
    //(pushes an integer in front)    
    void pushFront(const int number);
    //(removes the first element of the vector)
    void popFront();
    //(sorts the vector in ascending order)
    void sort();
    //(returns the max element in the vector)
    const int getMaxElement();
    //(returns the number of elements in the vector)
    const int getSize() const;

    const int getSum() const;

    friend std::ostream& operator<<(std::ostream& output, const Vector& vector);
};


#endif