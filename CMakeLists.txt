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

add_executable(BaseController WinterFrame/Controller/BaseController.cpp)
add_executable(stdfuntion_test StdSomething/stdfuntion_test.cpp)
add_executable(test_pathfunction WinterFrame/Controller/test_pathfunction.cpp)
add_executable(BaseController_gt WinterFrame/Controller/BaseController_gt.cpp)
add_executable(test_static_vector VectorTest/test_static_vector.cpp)