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
message(STATUS "operating system is ${CMAKE_SYSTEM}")
message(STATUS "operating system name is ${CMAKE_SYSTEM_NAME}")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/googletest)

include_directories(include ${CMAKE_CURRENT_SOURCE_DIR}/googletest/include ${CMAKE_CURRENT_SOURCE_DIR}/json)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/linux)
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/mac)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(LIBRARIES pthread)

aux_source_directory(${PROJECT_SOURCE_DIR}/src dir_srcs)

set(WORK ${dir_srcs})

add_executable(data_structure_homework ${WORK})

target_link_libraries(data_structure_homework jsoncpp)

# add test files here
# after adding your test file, change the configurations on the top right corner
# then go for it, here`s an example

set(testdb test/db_test.cpp src/DB.cpp src/IDgenerator.cpp src/BasicOperation.cpp)

set(register_test test/register_test.cpp src/DB.cpp src/RegisterUser.cpp src/RegisterSeller.cpp src/RegisterManager.cpp)

set(login_test test/login_test.cpp src/LoginUser.cpp src/LoginSeller.cpp src/LoginManager.cpp src/DB.cpp)

add_executable(testdb ${testdb})

add_executable(boost test/boost_test.cpp src/DB.cpp src/IDgenerator.cpp src/BasicOperation.cpp)

add_executable(question test/question_test.cpp src/DB.cpp src/QuestionSystem.cpp src/BasicOperation.cpp)

add_executable(service test/service_test.cpp src/BasicOperation.cpp src/ServiceSystem.cpp src/DB.cpp)

add_executable(register_test ${register_test})

add_executable(login_test ${login_test})

# linking google test lib to your test file
target_link_libraries(testdb jsoncpp ${Boost_LIBRARIES})

target_link_libraries(boost jsoncpp ${Boost_LIBRARIES})

target_link_libraries(question jsoncpp)

target_link_libraries(service jsoncpp)

target_link_libraries(register_test jsoncpp)

target_link_libraries(login_test jsoncpp)

add_executable(rbtree_test test/rbtree_test.cpp)