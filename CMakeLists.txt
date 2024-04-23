cmake_minimum_required(VERSION 3.27)
project(CPPStudy)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPPStudy
        Reflect/include/processor.h
        Reflect/src/factory.cpp
        Reflect/include/factory.h
        Reflect/include/reader.h
        Reflect/src/reader.cpp
        Reflect/include/writer.h
        Reflect/src/writer.cpp
        Reflect/include/register.h
        Reflect/src/register.cpp
        Reflect/src/test.cpp
)
