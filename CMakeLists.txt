cmake_minimum_required(VERSION 3.20)
project(OgreExample C CXX)

add_executable(ogre_Example ogre_Ex.cpp )

target_include_directories(ogre_Example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ogre)
 
# this also sets the includes and pulls third party dependencies
target_link_libraries(ogre_Example OgreMain OgreBites OgreRTShaderSystem)

add_subdirectory(ogre)