#include "string.h"

String::String() : size(0), data(nullptr) {};

String::String(const String& other) {
   size = other.size + 1;
   data = new char[size];
   memcpy(data, other.data, size); 
}

String& String::operator=(const String& other) {
    if (this != &other) {
        delete[] data;
        data = new char[other.size];
        size = other.size;
        memcpy(data, other.data, size);
    }
    return *this;
}

String& String::operator=(const char* other) {
    int otherSize = strlen(other) + 1;
    if ((memcmp(data, other, size) == 0) && (size == otherSize)) {
        return *this;
    }
    delete[] data;
    data = new char[otherSize];
    memcpy(data, other, otherSize - 1);
    data[otherSize - 1] = '\0';
    size = otherSize;

    return *this;
}

String::~String() {
    delete[] data;
}

String::String(const char* string) {
    size = strlen(string) + 1;
    data = new char[size];
    memcpy(data, string, size);
}

String String::operator+(const String& other) const {
    int newSize = size + other.size - 1;
    char* newData = new char[newSize];
    memcpy(newData, data, size - 1);
    for (size_t i = 0; i < other.size; i++){
        newData[size - 1 + i] = other.data[i];
    }
    return String(newData);
}

String String::operator+(const char other) const {
    int newSize = size + 1;
    char* newData = new char[newSize];
    for (size_t i = 0; i < size - 1; i++){
        newData[i] = data[i];
    }
    newData[size - 1] = other;
    newData[size] = '\0';
    return String(newData);
}

bool String::operator==(const String& other) const{
    if (this == &other) {
        return true;
    }
    if (size != other.size) {
        return false;
    }
    return memcmp(data, other.data, size) == 0;
}

bool String::operator!=(const String& other) const {
    if (this == &other) {
        return false;
    }
    if (size != other.size) {
        return true;
    }
    return memcmp(data, other.data, size) != 0;
}

bool String::operator>(const String& other) const {
    return size > other.size;
}

bool String::operator<(const String& other) const {
    return size < other.size;
}

bool String::operator>=(const String& other) const {
    return size >= other.size;
}

bool String::operator<=(const String& other) const {
    return size <= other.size;
}

char String::operator[](const int index) const {
    if (index > size - 2) {
        return ' ';
    }
    return data[index];
}

std::ostream& operator<<(std::ostream& output, const String& string) {
    output << string.data;
    return output;
}

std::istream& operator>>(std::istream& input, String& string) {
    delete[] string.data;

    const int bufferSize = 1024;
    char tempString[bufferSize];
    
    input >> tempString;
    
    int tempSize = strlen(tempString) + 1;
    tempString[tempSize - 1] = '\0';
    string.data = new char[tempSize];
    memcpy(string.data, tempString, tempSize);
    string.size = tempSize;
    
    return input;
}

void String::printString() {
    if (data == nullptr) {
        std::cerr << "Error in printing stirng." << std::endl;
        return;
    }
    for (size_t i = 0; i < size; i++){
        std::cout << data[i];
    }
    std::cout << std::endl;
}