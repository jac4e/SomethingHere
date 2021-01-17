cmake_minimum_required(VERSION 3.10)
project(survival_of_littest)

#add source and header files
file(GLOB_RECURSE SOURCE_FILES src/*.c src/*.cpp assets/c_srcs/*.c)
file(GLOB_RECURSE HEADER_FILES src/*.h src/*.hpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

#policy preferring GLVND over legacy GL
cmake_policy(SET CMP0072 NEW)
#something ab honouring macros
cmake_policy(SET CMP0075 NEW)

#openGL - you probably have
find_package(OpenGL REQUIRED)

#GLUT - build and static link from git submodule
set(FREEGLUT_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(FREEGLUT_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(FREEGLUT_REPLACE_GLUT OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/freeglut/freeglut/freeglut)
include_directories(lib/freeglut/freeglut/freeglut/include)

target_link_libraries(${PROJECT_NAME} OpenGL::GL freeglut_static)
