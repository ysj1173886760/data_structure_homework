cmake_minimum_required(VERSION 3.17)
project(data_structure_homework)

set(CMAKE_CXX_STANDARD 20)

#small evil test
#set(google_src ${CMAKE_CURRENT_SOURCE_DIR}/googletest)
#add_library(gtest_static STATIC ${google_src})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
#set_target_properties( gtest_static PROPERTIES OUTPUT_NAME "gtest_static" )
#obviously failed, bu kui shi wo

#do not change it pls
# boost stuff
set(BOOST_MIN_VERSION "1.67.0")

FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost error.\n")
endif()
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${BOOST_LIBRARY_DIRS}")

include_directories(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
#
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/googletest)

include_directories(include ${CMAKE_CURRENT_SOURCE_DIR}/googletest/include ${CMAKE_CURRENT_SOURCE_DIR}/json)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib ${CMAKE_CURRENT_SOURCE_DIR}/googletest)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(LIBRARIES pthread)

aux_source_directory(${PROJECT_SOURCE_DIR}/src dir_srcs)

set(WORK ${dir_srcs})

add_executable(data_structure_homework ${WORK})

target_link_libraries(data_structure_homework jsoncpp)

# add test files here
# after adding your test file, change the configurations on the top right corner
# then go for it, here`s an example

set(testdb test/db_test.cpp src/DB.cpp)

add_executable(testdb ${testdb})

add_executable(boost test/boost_test.cpp src/IDgenerator.cpp src/BasicOperation.cpp)

# linking google test lib to your test file
target_link_libraries(testdb jsoncpp)

target_link_libraries(boost ${Boost_LIBRARIES})
