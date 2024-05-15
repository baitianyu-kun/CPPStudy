cmake_minimum_required(VERSION 3.27)
project(CPPStudy)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPPStudy
        ReflectUniquePtr/include/processor.h
        ReflectUniquePtr/src/factory.cpp
        ReflectUniquePtr/include/factory.h
        ReflectUniquePtr/include/reader.h
        ReflectUniquePtr/src/reader.cpp
        ReflectUniquePtr/include/writer.h
        ReflectUniquePtr/src/writer.cpp
        ReflectUniquePtr/include/register.h
        ReflectUniquePtr/src/register.cpp
        ReflectUniquePtr/src/test.cpp
)

add_executable(test_big_short BigLittleDian/test_big_short.cpp)
add_executable(test_string_char StringChar/test_string_char.cpp)
add_executable(test_struct StructClass/test_struct.cpp)
add_executable(test_sizeof StringChar/test_sizeof.cpp)
add_executable(FatTest SimpleFactory/FatTest.cpp)
add_executable(test_while StructClass/test_while.cpp)
add_executable(test_enum StringChar/test_enum.cpp)
add_executable(test_ptr test_ptr.cpp)