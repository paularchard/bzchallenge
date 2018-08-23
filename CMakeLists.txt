cmake_minimum_required(VERSION 2.8.8)
set(PROJECT_NAME_STR bluzelle-challenge)
project(${PROJECT_NAME_STR} CXX C)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
get_filename_component(DEPS_ROOT "${PROJECT_BINARY_DIR}/deps" ABSOLUTE)

include(ExtProjectUtils)

find_package(Threads REQUIRED)

set(REQUIRED_BOOST "1.67.0")
set(Boost_USE_STATIC_LIBS on)

find_package(
        Boost ${REQUIRED_BOOST}
        REQUIRED COMPONENTS
        date_time
        system
)

if(Boost_FOUND)
    message(STATUS "Boost: ${Boost_INCLUDE_DIRS}")
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O0 -ggdb -Wall -Wno-deprecated")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-deprecated")
endif()

if(MSVC)
    #vc 2012 fix for vararg templates
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_VARIADIC_MAX=10")
endif()

if(WIN32)
    set(_OPT_CMAKE_ARGS "-Dgtest_force_shared_crt=ON;-DCMAKE_SH=CMAKE_SH-NOTFOUND")
else()
    set(_OPT_CMAKE_ARGS "")
endif()

# Will download external CMakeable project from git repo, branch "master" and install it in $DEPS_ROOT
# This also will create "googletest.git" target, which we'll use as dependency for our test project
ExtProjectGit("https://github.com/google/googletest.git" "master" ${DEPS_ROOT} CMAKE_ARGS "${_OPT_CMAKE_ARGS}")

include_directories("${DEPS_ROOT}/include")
link_directories("${DEPS_ROOT}/lib")
link_directories("${DEPS_ROOT}/lib64")

#-------------------
# set common include folder for module
#-------------------
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)

#-------------------
# Test
#-------------------
enable_testing()
include_directories(${COMMON_INCLUDES})

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME_STR}_objects OBJECT ${SRC_FILES})
add_executable(${PROJECT_NAME_STR} $<TARGET_OBJECTS:${PROJECT_NAME_STR}_objects> ${PROJECT_SOURCE_DIR}/main/main.cpp)
target_link_libraries(${PROJECT_NAME_STR} boost_date_time boost_system ${CMAKE_THREAD_LIBS_INIT} ssl crypto)

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

# from list of files we'll create tests test_name.cpp -> test_name
foreach(_test_file ${TEST_SRC_FILES})
    get_filename_component(_test_name ${_test_file} NAME_WE)
    add_executable(${_test_name} ${_test_file} $<TARGET_OBJECTS:${PROJECT_NAME_STR}_objects>)
    add_dependencies(${_test_name} "googletest.git")
    target_link_libraries(${_test_name} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT} boost_date_time boost_system ssl crypto)
    add_test(${_test_name} ${_test_name})
    set_tests_properties(${_test_name} PROPERTIES TIMEOUT 5)
endforeach()

