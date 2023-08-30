#include "vector.h"

void Vector::resize() {
    if (size == capacity) {
        size_t newCapacity = capacity * 2;
        int* newData = new int[newCapacity];
        for (size_t i = 0; i < size; i++){
            newData[i] = data[i];
        }
        capacity = newCapacity;
        delete[] data;
        data = newData;
    }
}

void Vector::copyFrom(const Vector& other) {
    data = new int[other.capacity];
    for (size_t i = 0; i < other.size; i++){
        data[i] = other.data[i];
    }
    size = other.size;
    capacity = other.capacity;
}

void Vector::free() {
    delete[] data;
}

void Vector::swap(int& first, int& second) {
    int temp = first;
    first = second;
    second = temp;
}

Vector::Vector() : size(0), capacity(1) {
    data = new int[1];
}

Vector::Vector(const Vector& other) {
    copyFrom(other);
}

Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

Vector::~Vector() {
    free();
}

Vector::Vector(int* newData, size_t newSize, size_t newCapacity) {
    size = newSize;
    capacity = newCapacity;
    data = new int[capacity];
    for (size_t i = 0; i < size; i++){
        data[i] = newData[i];
    }
}

void Vector::pushBack(const int number) {
    resize();
    data[size++] = number;
}

Vector::Vector(std::initializer_list<int> initList) : size(initList.size()), capacity(initList.size() * 2) {
    data = new int[capacity];
    const int* arr = initList.begin();
    size_t i = 0;
    for (size_t i = 0; i < initList.size(); ++i) {
        data[i] = arr[i];
    }
}

int Vector::operator[](const size_t index) const {
    if (index >= size || index < 0) {
        throw std::out_of_range("Index out of bounds!");
    }
    return data[index];
}

Vector Vector::operator+(const Vector& other) {
    int* newData = new int[size + other.size];
    for (size_t i = 0; i < size ; i++) {
        newData[i] = data[i];
    }
    for (size_t i = 0; i < other.size; i++) {
        newData[i + size] = other.data[i];
    }
    return Vector(newData, size + other.size, (size + other.size) * 2);
}

Vector& Vector::operator+=(const Vector& other) {
    size_t newSize = size + other.size;
    int* newData = new int[newSize];
    for (size_t i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    for (size_t i = 0; i < other.size; ++i) {
        newData[size + i] = other.data[i];
    }
    size = newSize;
    delete[] data;
    capacity = newSize * 2;
    data = newData;
    return *this;
}


bool Vector::operator==(const Vector& other) const{
    if (this == &other) {
        return true;
    }
    if (size != other.size) {
        return false;
    }
    for (size_t i = 0; i < other.size; i++) {
        if (data[i] != other.data[i]) {
            return false;
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& output, const Vector& vector) {
    for (size_t i = 0; i < vector.size; ++i) {
        output << vector.data[i] << ' ';
    }
    return output;
}

void Vector::pop() {
    if (size > 0) {
        size -= 1;
    }
}

void Vector::pushFront(const int number) {
    resize();
    for (size_t i = 0; i < size; i++){
        data[size - i] = data[size - 1 - i];
    }
    data[0] = number;
    size += 1;
    
}

void Vector::popFront() {
    if (size > 0) {
        for (size_t i = 0; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        size -= 1;
    }
}

void Vector::sort() {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size - 1; j++) {
            if (data[j + 1] < data[j]) {
                swap(data[j + 1], data[j]);
            }
        }
    }
}

const int Vector::getMaxElement() {
    int maxInt = data[0];
    for (size_t i = 1; i < size; i++) {
        if (maxInt < data[i]) {
            maxInt = data[i];
        }
    }
    return maxInt;
}

const int Vector::getSize() const {
    return size;
}

const int Vector::getSum() const {
    int sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += data[i];
    }
    return sum;
}