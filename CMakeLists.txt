project(StoneRook)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(Boost COMPONENTS unit_test_framework date_time REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(HDRS
    SiteGrammar.hpp
    DateGrammar.hpp
    Common.hpp
    MoveValidation.hpp
)

set(SRCS
    unit_tests.cpp
)

add_executable(unit_tests ${HDRS} ${SRCS})
target_link_libraries(unit_tests ${Boost_LIBRARIES})
