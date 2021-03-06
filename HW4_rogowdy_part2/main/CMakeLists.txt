# Main cmake file 
# Multi Texture example MODIFIED
# Rafael Radkowski, rafael@iastate.edu 
# Iowa State University
# ME/CS/CprE 557 Computer Graphics, Fall 2015
# Updated by rogowdy

cmake_minimum_required(VERSION 2.6)

# Main project name is ARMaker
project(HW4_rogowdy_p2)


set(CFILE_PATH  "${CMAKE_SOURCE_DIR}/../SDK/cmake/")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/" ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../SDK/cmake/")

message(STATUS "Source directory ${CMAKE_SOURCE_DIR}")
message(STATUS "SDK directory ${CMAKE_SOURCE_DIR}/../SDK")


# Find packages
FIND_PACKAGE(glew REQUIRED)
FIND_PACKAGE(glm REQUIRED)
FIND_PACKAGE(glfw3 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)


# Include dirs
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLFW3_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/../gl_common/)

# Add all files to the configuration
file(GLOB HW4_rogowdy_p2_SRC
	HW4_rogowdy_main_p2.cpp
    ../gl_common/HCI557Common.cpp
    ../gl_common/HCI557Common.h
	../gl_common/controls.cpp
    ../gl_common/controls.h
	../gl_common/CoordSystem.cpp
    ../gl_common/CoordSystem.h
	../gl_common/GLObject.h
   	../gl_common/GLObject.cpp
    ../gl_common/GLSphere.cpp
    ../gl_common/GLSphere.h
    ../gl_common/Shaders.h
    ../gl_common/Shaders.cpp
    ../gl_common/Plane3d.h
    ../gl_common/Plane3d.cpp
    ../gl_common/Texture.h
    ../gl_common/Texture.cpp
    ../gl_common/HCI557Datatypes.h
    ../gl_common/GLAppearanceBase.h
    ../gl_common/GLAppearance.h
    ../gl_common/GLAppearance.cpp
    ../gl_common/camera.h
	../gl_common/camera.cpp
    ../gl_common/GLObjects3D.h
)

set(HW4_rogowdy_p2_RES
	../data/shaders/multi_texture.fs
    ../data/shaders/multi_texture.vs
)


#----
# SRC Groups
source_group(Resources FILES ${HW4_rogowdy_p2_RES})


# Create an executable
add_executable(HW4_rogowdy_p2 ${HW4_rogowdy_p2_SRC})


# Add link directories
# Note required for this project
IF(WIN32)
	link_directories(${CMAKE_SOURCE_DIR}/../SDK/lib)
ELSE(WIN32)
	link_directories(${CMAKE_SOURCE_DIR}/../SDK/lib)
	link_directories(/usr/local/lib)
	link_directories(/usr/local/Cellar/glfw3/3.0.4/lib)
	link_directories(/usr/local/Cellar/glew/1.11.0/lib)
ENDIF(WIN32)


# Add libraries
target_link_libraries(HW4_rogowdy_p2 ${GLEW_LIBRARY} ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} ${OPENGL_LIBRARIES} )