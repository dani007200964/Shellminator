# Created on April 08 2023
# Copyright (c) 2020 - Daniel Hajnal
# hajnal.daniel96@gmail.com
# This file is part of the Shellminator project.

cmake_minimum_required( VERSION 3.24 )
project( Shellminator VERSION 1.1.3 )

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories( 
    src
    extras/simulator
    extras/simulator/Commander-API
    extras/Unity/src
)

set( SOURCES
    src/Shellminator-Browser-Response.hpp
    src/Shellminator-IO.cpp
    src/Shellminator-IO.hpp
    src/Shellminator-Settings.hpp
    src/Shellminator.hpp
    src/Shellminator.cpp
    extras/simulator/Arduino.h
    extras/simulator/Print.cpp
    extras/simulator/Print.h
    extras/simulator/Printable.h
    #extras/simulator/simulator.cpp
    extras/simulator/Stream.cpp
    extras/simulator/Stream.h
    extras/simulator/System.h
    extras/simulator/System.cpp
    extras/simulator/stdioStream.hpp
    extras/simulator/stdioStream.cpp
    #extras/simulator/Commander-API/Commander-API-Commands.cpp
    #extras/simulator/Commander-API/Commander-API-Commands.hpp
    extras/simulator/Commander-API/Commander-API.cpp
    extras/simulator/Commander-API/Commander-API.hpp
    extras/simulator/Commander-API/Commander-IO.cpp
    extras/simulator/Commander-API/Commander-IO.hpp
    extras/simulator/Commander-API/Commander-Settings.hpp
)

set( UNIT_TESTING
    extras/Unity/src/unity_internals.h
    extras/Unity/src/unity.c
    extras/Unity/src/unity.h
    extras/tests/testStream.cpp
    extras/tests/testStream.hpp
    extras/tests/unitTest.cpp
)

#add_compile_options( -std=gnu++11 )

# Shellminator settings
add_compile_definitions(
    SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
    SHELLMINATOR_ENABLE_PROGRESS_BAR_SUPPORT
    SHELLMINATOR_ENABLE_PLOT_MODULE

    #UNITY_OUTPUT_COLOR

)

# Build the simulator
add_executable( Simulator ${SOURCES} extras/simulator/simulator.cpp )

# Unit testing
add_executable( UnitTest ${SOURCES} ${UNIT_TESTING} )
