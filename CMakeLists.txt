cmake_minimum_required(VERSION 3.27)
project(CPPStudy)

set(CMAKE_CXX_STANDARD 17)

#add_executable(CPPStudy
#        ReflectUniquePtr/include/processor.h
#        ReflectUniquePtr/src/factory.cpp
#        ReflectUniquePtr/include/factory.h
#        ReflectUniquePtr/include/reader.h
#        ReflectUniquePtr/src/reader.cpp
#        ReflectUniquePtr/include/writer.h
#        ReflectUniquePtr/src/writer.cpp
#        ReflectUniquePtr/include/register.h
#        ReflectUniquePtr/src/register.cpp
#        ReflectUniquePtr/src/test.cpp
#)


add_executable(BaseController WinterFrame/Controller/BaseController.cpp)
add_executable(stdfuntion_test StdSomething/stdfuntion_test.cpp)
add_executable(test_pathfunction WinterFrame/Controller/test_pathfunction.cpp)
add_executable(BaseController_gt WinterFrame/Controller/BaseController_gt.cpp)
add_executable(test_static_vector VectorTest/test_static_vector.cpp)
add_executable(test_null_class StructClass/test_null_class.cpp)
add_executable(test_sizeof StringChar/test_sizeof.cpp)
add_executable(shuzu_bytes_test VectorTest/shuzu_bytes_test.cpp)
add_executable(test_strcmp StringChar/test_strcmp.cpp)
add_executable(MakeTrie Trie/MakeTrie.cpp)
add_executable(unorderedmap_test Trie/unorderedmap_test.cpp)
add_executable(move_test CMU/move_test.cpp)
add_executable(move_test2 CMU/move_test2.cpp)
add_executable(test_lowerupper StringChar/test_lowerupper.cpp)
add_executable(LRU_146 LRU/LRU_146.cpp)
add_executable(unorderedmaps_test LRU/unorderedmaps_test.cpp)
add_executable(LRU_1462 LRU/LRU_1462.cpp)
add_executable(LRU3 LRU/LRU3.cpp)
add_executable(LRU_1462_2 LRU/LRU_1462_2.cpp)