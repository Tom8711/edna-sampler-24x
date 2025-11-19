#pragma once
#include <iostream>

class SerialMock {
public:
    void begin(int) {}
    void println(const char* msg) { std::cout << msg << std::endl; }
    void print(const char* msg) { std::cout << msg; }
    void print(unsigned long v) { std::cout << v; }
    void println(unsigned long v) { std::cout << v << std::endl; }
};

static SerialMock Serial;
