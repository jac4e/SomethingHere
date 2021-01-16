cmake_minimum_required(VERSION 3.10)
project(HackED)

#add source and header files
file(GLOB_RECURSE SOURCE_FILES src/*.c src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h src/*.hpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
