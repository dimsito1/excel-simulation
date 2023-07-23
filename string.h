#include <iostream>
#include <cstring>

#ifndef _STRING_HEADER_INCLUDED_
#define _STRING_HEADER_INCLUDED_

class String{
private:
    char* data;
    int size;

public:
    String();
    String(const String& other);
    String& operator=(const String& other);
    String& operator=(const char* other);
    ~String();

    String(const char* string);

    String operator+(const String& other) const;
    String operator+(const char other) const;

    bool operator==(const String& other) const;
    bool operator!=(const String& other) const;
    bool operator>(const String& other) const;
    bool operator>=(const String& other) const;
    bool operator<=(const String& other) const;
    bool operator<(const String& other) const;
    char operator[](const int index) const;

    friend std::ostream& operator<<(std::ostream& output, const String& string);
    friend std::istream& operator>>(std::istream& input, String& string);

    void printString();
};


#endif