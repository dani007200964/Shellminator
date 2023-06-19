# Created on April 08 2023
# Copyright (c) 2020 - Daniel Hajnal
# hajnal.daniel96@gmail.com
# This file is part of the Shellminator project.

cmake_minimum_required( VERSION 3.24 )
project( Shellminator VERSION 1.1.3 )

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

option(RUN_TESTS "Enable Tests" OFF)
option(BUILD_SIMULATOR "Build Simulator" OFF)
option(BUILD_EXAMPLES "Build Examples" OFF)

if (RUN_TESTS)

    add_compile_options( -fprofile-arcs )
    add_compile_options( -ftest-coverage )
    add_compile_options( -O0 )
    add_link_options("--coverage")
    set( CMAKE_EXE_LINKER_FLAGS "-lgcov -fprofile-arcs -ftest-coverage" )

endif()

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
    src/Shellminator-BufferedPrinter.cpp
    src/Shellminator-BufferedPrinter.hpp
    src/Shellminator-PlotModule.cpp
    src/Shellminator-PlotModule.hpp
    src/Shellminator-DefaultSettings.hpp
    src/Shellminator.hpp
    src/Shellminator.cpp
    src/Shellminator-SpecialKeys.cpp
    src/Shellminator-Styles.cpp
    src/Shellminator-SearchModule.cpp
    src/Shellminator-Progress.hpp
    src/Shellminator-Progress.cpp

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

if (RUN_TESTS)


    set( UNIT_TESTING
        extras/Unity/src/unity_internals.h
        extras/Unity/src/unity.c
        extras/Unity/src/unity.h
        extras/tests/testStream.cpp
        extras/tests/testStream.hpp
        #extras/tests/unitTest.cpp
        #extras/tests/testCases/test_channels.cpp
    )

endif()

# Shellminator settings
add_compile_definitions(
    # SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
    SHELLMINATOR_ENABLE_PROGRESS_BAR_SUPPORT
    SHELLMINATOR_ENABLE_PLOT_MODULE

    #UNITY_OUTPUT_COLOR

)

if( BUILD_SIMULATOR )

    # Build the simulator
    add_executable( Simulator ${SOURCES} extras/simulator/simulator.cpp )

endif()

##add_subdirectory( extras )
#add_executable( Basic ${SOURCES} extras/examples_desktop/Desktop/Basic/Basic.cpp )

if(RUN_TESTS)

    # Unit testing
    add_executable( test_buffered_printer ${SOURCES} ${UNIT_TESTING} extras/tests/testCases/test_buffered_printer.cpp )
    add_executable( test_special_keys ${SOURCES} ${UNIT_TESTING} extras/tests/testCases/test_special_keys.cpp )
    add_executable( test_channels ${SOURCES} ${UNIT_TESTING} extras/tests/testCases/test_channels.cpp )
    add_executable( test_shellminator_begin ${SOURCES} ${UNIT_TESTING} extras/tests/testCases/test_shellminator_begin.cpp )
    add_executable( test_format_commands ${SOURCES} ${UNIT_TESTING} extras/tests/testCases/test_format_commands.cpp )
    add_executable( test_update ${SOURCES} ${UNIT_TESTING} extras/tests/testCases/test_update.cpp )

endif()

if( BUILD_EXAMPLES )

# Do not edit this comment and do not write anything after it!
# It will be generated with the exampleGenerator.py script.
#---- Examples Section ----#

add_executable( Basic ${SOURCES} extras/examples_desktop/Desktop/Basic/Basic.cpp )
add_executable( Buffered ${SOURCES} extras/examples_desktop/Desktop/Buffered/Buffered.cpp )
add_executable( Execute ${SOURCES} extras/examples_desktop/Desktop/Execute/Execute.cpp )
add_executable( Executeconstructoradvanced ${SOURCES} extras/examples_desktop/Desktop/Executeconstructoradvanced/Executeconstructoradvanced.cpp )
add_executable( Executeconstructorsimple ${SOURCES} extras/examples_desktop/Desktop/Executeconstructorsimple/Executeconstructorsimple.cpp )
add_executable( Mouse ${SOURCES} extras/examples_desktop/Desktop/Mouse/Mouse.cpp )
add_executable( Progressbaradvanced ${SOURCES} extras/examples_desktop/Desktop/Progressbaradvanced/Progressbaradvanced.cpp )
add_executable( Progressbarsimple ${SOURCES} extras/examples_desktop/Desktop/Progressbarsimple/Progressbarsimple.cpp )
add_executable( Runningplot ${SOURCES} extras/examples_desktop/Desktop/Runningplot/Runningplot.cpp )
add_executable( Simpleplot ${SOURCES} extras/examples_desktop/Desktop/Simpleplot/Simpleplot.cpp )
endif()