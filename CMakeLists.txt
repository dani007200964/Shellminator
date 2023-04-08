# Created on April 08 2023
# Copyright (c) 2020 - Daniel Hajnal
# hajnal.daniel96@gmail.com
# This file is part of the Shellminator project.

cmake_minimum_required( VERSION 3.24 )
project( Shellminator VERSION 1.1.3 )

include_directories( 
    src
    extras/simulator
 )

set( SOURCES
    extras/simulator/Arduino.h
    extras/simulator/Print.cpp
    extras/simulator/Print.h
    extras/simulator/Printable.h
    extras/simulator/simulator.cpp
    extras/simulator/Stream.cpp
    extras/simulator/Stream.h
    extras/simulator/System.h
    extras/simulator/System.cpp
    extras/simulator/stdioStream.hpp
    extras/simulator/stdioStream.cpp
    src/Shellminator-Browser-Response.hpp
    src/Shellminator-IO.cpp
    src/Shellminator-IO.hpp
    src/Shellminator-Settings.hpp
    src/Shellminator.hpp
    src/Shellminator.cpp
)

add_compile_options( -std=gnu++11 )

# Shellminator settings
add_compile_definitions(
    SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
)

add_executable( ${PROJECT_NAME} ${SOURCES} )