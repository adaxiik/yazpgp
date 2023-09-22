cmake_minimum_required(VERSION 3.0.0)
project(yazpgp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE YAZPGP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM YAZPGP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
set(YAZPGP_INCLUDE_DIRS_PREFIXED ${CMAKE_CURRENT_SOURCE_DIR}/src/include/)
set(YAZPGP_INCLUDE_DIRS ${YAZPGP_INCLUDE_DIRS_PREFIXED}/yazpgp/)

add_library(yazpgp-lib ${YAZPGP_FILES})
set_target_properties(yazpgp-lib PROPERTIES COMPILE_FLAGS "-Wall -Wextra -pedantic" )
target_include_directories(yazpgp-lib PRIVATE ${YAZPGP_INCLUDE_DIRS})
target_compile_features(yazpgp-lib PUBLIC cxx_std_17)

add_executable(yazpgp src/main.cpp)
target_include_directories(yazpgp PUBLIC ${YAZPGP_INCLUDE_DIRS_PREFIXED} ${YAZPGP_INCLUDE_DIRS})
target_link_libraries(yazpgp yazpgp-lib)
target_link_libraries(yazpgp SDL2 GL X11)