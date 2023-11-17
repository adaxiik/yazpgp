cmake_minimum_required(VERSION 3.0.0)
project(yazpgp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE IMGUI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/imgui/*.cpp)
set(IMGUI_INCLUDE_DIR_PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/imgui)
add_library(imgui STATIC ${IMGUI_FILES})
target_include_directories(imgui PRIVATE ${IMGUI_INCLUDE_DIR_PRIVATE})

set(3RDPARTY_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/)


file(GLOB YAZPGP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM YAZPGP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
set(YAZPGP_INCLUDE_DIRS_PREFIXED ${CMAKE_CURRENT_SOURCE_DIR}/src/include/)
set(YAZPGP_INCLUDE_DIRS ${YAZPGP_INCLUDE_DIRS_PREFIXED}/yazpgp/)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address -Wall -Wextra -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -fsanitize=address")
add_library(yazpgp-lib STATIC ${YAZPGP_FILES})
target_include_directories(yazpgp-lib PRIVATE 
    ${YAZPGP_INCLUDE_DIRS} 
    ${YAZPGP_INCLUDE_DIRS_PREFIXED}
    ${3RDPARTY_INCLUDE_DIRS}
    ${IMGUI_INCLUDE_DIR_PRIVATE})
target_compile_features(yazpgp-lib PUBLIC cxx_std_20)

add_executable(yazpgp src/main.cpp)
target_include_directories(yazpgp PUBLIC ${YAZPGP_INCLUDE_DIRS_PREFIXED} ${YAZPGP_INCLUDE_DIRS})
target_link_libraries(yazpgp yazpgp-lib imgui)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(glm REQUIRED)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

target_link_libraries(yazpgp
    ${SDL2_LIBRARIES}
    ${SDL2IMAGE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    ${GLM_LIBRARIES}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})