cmake_minimum_required(VERSION 3.0)
project(General-Linear-Least-Squares)

find_package(Boost COMPONENTS unit_test_framework OPTIONAL)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3  -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

aux_source_directory(src SRC_LIST)
add_executable(glls ${SRC_LIST})

add_definitions(-DBOOST_TEST_DYN_LINK)

########################################
add_executable(test_all
    src/test/test_all.cc
    src/test/test_gllsparser.cc
    src/test/test_symbollist.cc
    src/gllsparser.cc
    src/gllsparser.h
    src/symbollist.cc
    src/symbollist.h
    )
target_link_libraries(test_all ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(test test_all)

########################################
add_executable(test_gllsparser
    src/test/test_gllsparser.cc
    src/gllsparser.cc
    src/gllsparser.h
    )
set_target_properties(test_gllsparser PROPERTIES COMPILE_DEFINITIONS "BOOST_TEST_MAIN")
target_link_libraries(test_gllsparser ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

########################################
add_executable(test_symbollist
    src/test/test_symbollist.cc
    src/symbollist.cc
    src/symbollist.h
    )
set_target_properties(test_symbollist PROPERTIES COMPILE_DEFINITIONS  "BOOST_TEST_MAIN")
target_link_libraries(test_symbollist ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

