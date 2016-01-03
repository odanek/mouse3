cmake_minimum_required(VERSION 2.6)

############################################################################
#
#  The Mouse 3
#
#  Copyright (C) 2010   Ondrej Danek <ondrej.danek@gmail.com>
# 
#############################################################################

# project name
project(Mouse3)

#########################################################################
# compilation details
#########################################################################

# turn off some warnings under msc
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")    
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
endif(MSVC)

# gnu compiler or clang
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -mfpmath=sse -msse2 -std=c++1y")
endif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")

# MinGW
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows -static-libgcc -static-libstdc++")
endif(MINGW)

# platform-independent debug macro
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DM3_DEBUG")

#########################################################################
#
# The Mouse 3
#  
#########################################################################

########################
#  Project source files
########################

# set the list of source files
set(M3_SOURCES

    source/declare.h
    source/draw.cpp
    source/end.cpp
    source/font.cpp
    source/hlavni.cpp
    source/kontrola.cpp
    source/load.cpp
    source/menu.cpp
    source/misc.cpp
    source/monsters.cpp
    source/mouse3.cpp
    source/mouse3.h
    source/move.cpp
    source/setup.cpp
    source/shots.cpp
    source/sound.cpp

    source/mylib/myerror.cpp
    source/mylib/myfile.cpp
    source/mylib/mykh3.cpp
    source/mylib/mylang.cpp
    source/mylib/mylib.h
    source/mylib/mytools.cpp
    
    source/core/co_core.h
    source/core/co_font.cpp
    source/core/co_fps.cpp
    source/core/co_main.cpp
    source/core/co_vid.cpp
)

if(WIN32)
    set(M3_SOURCES ${M3_SOURCES} source/mouse3.rc)
endif(WIN32)

########################
#  Add application
########################

set(M3_APP_NAME "mouse3" CACHE STRING "Filename of the application.")
set(M3_APP_DEBUG_NAME "mouse3d" CACHE STRING "Filename of the debug version of the application.")
add_executable(${M3_APP_NAME} ${M3_SOURCES})
set_target_properties(${M3_APP_NAME} PROPERTIES VERSION 1.0.0 DEBUG_OUTPUT_NAME ${M3_APP_DEBUG_NAME})

#########################################################################
# External dependencies
#########################################################################

# Platform
if (MINGW)
    target_link_libraries(${M3_APP_NAME} mingw32)
endif(MINGW)

# OpenGL
if(WIN32)
    target_link_libraries(${M3_APP_NAME} opengl32.lib)
elseif(APPLE)
    find_library(LIB_OPEN_GL OpenGL DOC "Path to OpenGL framework")
    target_link_libraries(${M3_APP_NAME} ${LIB_OPEN_GL})
else(WIN32) # Linux GCC
    find_library(LIB_OPEN_GL GL DOC "Path to OpenGL import library")
    target_link_libraries(${M3_APP_NAME} ${LIB_OPEN_GL})
endif(WIN32)

# SDL
if(APPLE)
    find_library(LIB_SDL2 SDL2 DOC "Path to SDL2 framework")
    target_link_libraries(${M3_APP_NAME} ${LIB_SDL2})
else(APPLE) # MSVC, GCC or MINGW
    find_path(HEADERS_SDL2 SDL2/SDL.h DOC "Path to SDL2 headers")
    include_directories(${HEADERS_SDL2})

    find_library(LIB_SDL2 SDL2 DOC "Path to SDL2 import library")
    find_library(LIB_SDL2_MAIN SDL2main DOC "Path to SDL2main import library")
    target_link_libraries(${M3_APP_NAME} ${LIB_SDL2_MAIN} ${LIB_SDL2})
endif(APPLE)

########################
#  Install application
########################

# install binary files
install(TARGETS ${M3_APP_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
