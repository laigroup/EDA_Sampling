cmake_minimum_required(VERSION 3.16)
project(Project)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_CXX_FLAGS "-g -Ofast -static")

#禁用优化，便于DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

#PATH
SET(lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib)
#SET(lib_tar ${lib_dir}.tar)
SET(cudd_dir ${lib_dir}/cudd-3.0.0)

ADD_CUSTOM_COMMAND(
        OUTPUT ${lib_dir}
        DEPENDS ${lib_tar}
        COMMAND tar -xf ${lib_tar} && touch ${lib_dir} && cd ${cudd_dir} && ./configure --enable-silent-rules --enable-obj && make
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

LINK_DIRECTORIES(${cudd_dir}/cudd/.libs)

#CUDD
INCLUDE_DIRECTORIES(${cudd_dir})
INCLUDE_DIRECTORIES(${cudd_dir}/cudd)
INCLUDE_DIRECTORIES(${cudd_dir}/epd)
INCLUDE_DIRECTORIES(${cudd_dir}/mtr)
INCLUDE_DIRECTORIES(${cudd_dir}/st)
INCLUDE_DIRECTORIES(${cudd_dir}/util)

INCLUDE_DIRECTORIES(${lib_dir})
#nlohmann
include_directories( CMAKE_CURRENT_SOURCE_DIR/nlohmann)
INCLUDE_DIRECTORIES(src/interface)

#gmp
include_directories("/usr/local/include")

FILE(GLOB cpp_files "src/implementation/*.cpp" "src/test/*.cpp")
ADD_EXECUTABLE(Project ${cpp_files} ${lib_dir})

TARGET_LINK_LIBRARIES(Project cudd)
target_link_libraries(Project /usr/local/lib/libgmpxx.a /usr/local/lib/libgmp.a)