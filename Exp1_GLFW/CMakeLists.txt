
cmake_minimum_required(VERSION 3.13)
project(Exp1_GLFW)
set(CMAKE_CXX_STANDARD 11)
include_directories(../include)
find_package(glfw3 REQUIRED)
link_directories(/usr/local/lib)
add_link_options(-lglfw3 -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor)
set(SOURCE_FILES main.cpp)
add_executable(Exp1_GLFW ${SOURCE_FILES})






