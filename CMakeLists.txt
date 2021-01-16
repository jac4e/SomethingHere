cmake_minimum_required(VERSION 3.10)
project(HackED)

#add source and header files
file(GLOB_RECURSE SOURCE_FILES src/*.c src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h src/*.hpp)
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

#policy preferring GLVND over legacy GL
cmake_policy(SET CMP0072 NEW)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#openGL
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw-3.3.2)
target_link_libraries(${PROJECT} glfw)
