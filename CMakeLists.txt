cmake_minimum_required(VERSION 2.8)
if (UNIX OR CYGWIN)
set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo Coverage CACHE TYPE INTERNAL FORCE )
else()
set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo CACHE TYPE INTERNAL FORCE )
endif()
project(General-Linear-Least-Squares)
enable_testing()

find_program(GCOV gcov)
find_program(LCOV lcov)
find_program(GENHTML genhtml)

find_package(Boost COMPONENTS unit_test_framework OPTIONAL)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3  -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
else(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /W4 /EHsc")
endif()

aux_source_directory(src SRC_LIST)
add_executable(glls ${SRC_LIST})

add_definitions(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

########################################
set(all_tests
    test_condparser
    test_condtree
    test_gllsparser
    test_parsercommon
    test_symbollist
)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${all_tests})

add_custom_target(coverage
    COMMAND ${LCOV} -z -d .
    COMMAND ${CMAKE_CTEST_COMMAND}
    COMMAND ${LCOV} -d . -c -o coverage_all.txt
    COMMAND ${LCOV} -r coverage_all.txt 'test/*' '/usr/*' -o coverage_src.txt
    COMMAND ${GENHTML} --demangle-cpp -o coverage_report coverage_src.txt
    COMMAND ${CMAKE_COMMAND} -E remove coverage_all.txt coverage_src.txt
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${all_tests}
)

########################################
add_test(gllsparser test_gllsparser)
add_executable(test_gllsparser
    test/gllsparser.cc
    src/gllsparser.cc
    src/gllsparser.h
    src/symbollist.cc
    src/symbollist.h
    src/parsercommon.cc
    src/parsercommon.h
    src/condtree.cc
    src/condtree.h
    src/condparser.cc
    src/condparser.h
    )
target_link_libraries(test_gllsparser ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

########################################
add_test(symbollist test_symbollist)
add_executable(test_symbollist
    test/symbollist.cc
    src/symbollist.cc
    src/symbollist.h
    )
target_link_libraries(test_symbollist ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

########################################
add_test(parsercommon test_parsercommon)
add_executable(test_parsercommon
    test/parsercommon.cc
    src/parsercommon.cc
    src/parsercommon.h
    )
target_link_libraries(test_parsercommon ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

########################################
add_test(condparser test_condparser)
add_executable(test_condparser
    test/condparser.cc
    src/condparser.cc
    src/condparser.h
    src/parsercommon.cc
    src/parsercommon.h
    src/symbollist.cc
    src/symbollist.h
    src/condtree.cc
    src/condtree.h
    )
target_link_libraries(test_condparser ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

########################################
add_test(condtree test_condtree)
add_executable(test_condtree
    test/condtree.cc
    src/condtree.cc
    src/condtree.h
    src/condparser.cc
    src/condparser.h
    src/parsercommon.cc
    src/parsercommon.h
    src/symbollist.cc
    src/symbollist.h
    )
target_link_libraries(test_condtree ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
