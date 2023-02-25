cmake_minimum_required(VERSION 3.5)
project(cse390-hw1 CXX)

########### Do not change these lines #########
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
###############################################


set(PROJECT_NAME cse390-hw1)
set(no_group_source_files
    "inputs/basic.txt"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "include/controller.h"
    "include/directions.h"
    "include/robot.h"
    "include/symbols.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "src/controller.cpp"
    "src/main.cpp"
    "src/robot.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Source_Files}
)

include_directories(include)
add_executable(${PROJECT_NAME} ${ALL_FILES})