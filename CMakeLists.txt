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
option(BUILD_WEBASSEMBLY "Build WebAssembly" OFF)

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
    extras/Commander-API/src
    extras/Unity/src
    extras/nayuki-qrcodegen/QR-Code-generator/c
)

set( SOURCES
    src/Shellminator-Browser-Response.hpp
    src/Shellminator-VT100-Commands.hpp
    src/Shellminator-Screen.hpp
    src/Shellminator-BufferedPrinter.cpp
    src/Shellminator-BufferedPrinter.hpp
    src/Shellminator-GUI.hpp
    src/GUI/Shellminator-PlotModule.cpp
    src/GUI/Shellminator-PlotModule.hpp
    src/GUI/Shellminator-Buttons.cpp
    src/GUI/Shellminator-Buttons.hpp
    src/GUI/Shellminator-Progress.hpp
    src/GUI/Shellminator-Progress.cpp
    src/GUI/Shellminator-List.hpp
    src/GUI/Shellminator-List.cpp
    src/GUI/Shellminator-Checkbox.hpp
    src/GUI/Shellminator-Checkbox.cpp
    src/GUI/Shellminator-List-Detailed.hpp
    src/GUI/Shellminator-List-Detailed.cpp
    src/GUI/Shellminator-Level-Meter.hpp
    src/GUI/Shellminator-Level-Meter.cpp
    src/Shellminator-DefaultSettings.hpp
    src/Shellminator.hpp
    src/Shellminator.cpp
    src/Shellminator-SpecialKeys.cpp
    src/Shellminator-Styles.cpp
    src/Shellminator-SearchModule.cpp
    src/Shellminator-MouseInterface.cpp
    src/Shellminator-Crypto.cpp
    src/Shellminator-Input.cpp
    src/Shellminator-Events.cpp
    src/Shellminator-QR-Code-Module.hpp
    src/Shellminator-QR-Code-Module.cpp
    src/Shellminator-Colorizer.hpp
    src/Shellminator-Colorizer.cpp
    src/Shellminator-Commander-Interface.hpp
    src/Shellminator-Commander-Interface.cpp

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
    #extras/simulator/Commander-API/Commander-API.cpp
    #extras/simulator/Commander-API/Commander-API.hpp
    #extras/simulator/Commander-API/Commander-IO.cpp
    #extras/simulator/Commander-API/Commander-IO.hpp
    #extras/simulator/Commander-API/Commander-Settings.hpp
    extras/Commander-API/src/Commander-API.hpp
    extras/Commander-API/src/Commander-API.cpp
    extras/Commander-API/src/Commander-IO.hpp
    extras/Commander-API/src/Commander-IO.cpp
    extras/Commander-API/src/Commander-Utils.hpp
    extras/Commander-API/src/Commander-Utils.cpp
    extras/Commander-API/src/Commander-Settings.hpp

    extras/nayuki-qrcodegen/QR-Code-generator/c/qrcodegen.c
    extras/nayuki-qrcodegen/QR-Code-generator/c/qrcodegen.h
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

endif()

# Do not edit this comment and do not write anything after it!
# It will be generated with the exampleGenerator.py script.
#---- Examples Section ----#

if( BUILD_EXAMPLES )
	#add_executable( 001_basic ${SOURCES} extras/examples_desktop/Desktop/001_basic/001_basic.cpp )
	#add_executable( 002_commandCallback ${SOURCES} extras/examples_desktop/Desktop/002_commandCallback/002_commandCallback.cpp )
	#add_executable( 003_textFormatting ${SOURCES} extras/examples_desktop/Desktop/003_textFormatting/003_textFormatting.cpp )
	#add_executable( 004_formatMatrix ${SOURCES} extras/examples_desktop/Desktop/004_formatMatrix/004_formatMatrix.cpp )
	#add_executable( 005_logo ${SOURCES} extras/examples_desktop/Desktop/005_logo/005_logo.cpp )
	#add_executable( 006_password ${SOURCES} extras/examples_desktop/Desktop/006_password/006_password.cpp )
	#add_executable( 007_input ${SOURCES} extras/examples_desktop/Desktop/007_input/007_input.cpp )
	#add_executable( 008_input_number ${SOURCES} extras/examples_desktop/Desktop/008_input_number/008_input_number.cpp )
	#add_executable( 009_beeeeep ${SOURCES} extras/examples_desktop/Desktop/009_beeeeep/009_beeeeep.cpp )
	#add_executable( 010_bannerText ${SOURCES} extras/examples_desktop/Desktop/010_bannerText/010_bannerText.cpp )
	#add_executable( 100_keyOverride ${SOURCES} extras/examples_desktop/Desktop/100_keyOverride/100_keyOverride.cpp )
	#add_executable( 101_qrCode ${SOURCES} extras/examples_desktop/Desktop/101_qrCode/101_qrCode.cpp )
	add_executable( 200_commanderBasic ${SOURCES} extras/examples_desktop/Desktop/200_commanderBasic/200_commanderBasic.cpp )
	#add_executable( colorizer ${SOURCES} extras/examples_desktop/Desktop/colorizer/colorizer.cpp )
	#add_executable( levelMeter ${SOURCES} extras/examples_desktop/Desktop/levelMeter/levelMeter.cpp )
	#add_executable( list ${SOURCES} extras/examples_desktop/Desktop/list/list.cpp )
	#add_executable( progress ${SOURCES} extras/examples_desktop/Desktop/progress/progress.cpp )
	#add_executable( screenAdvanced ${SOURCES} extras/examples_desktop/Desktop/screenAdvanced/screenAdvanced.cpp )
endif()
if( BUILD_WEBASSEMBLY )
	add_executable( 001_basic ${SOURCES} extras/examples_emscripten/Emscripten/001_basic/001_basic.cpp )
	target_link_options( 001_basic PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 002_commandCallback ${SOURCES} extras/examples_emscripten/Emscripten/002_commandCallback/002_commandCallback.cpp )
	target_link_options( 002_commandCallback PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 003_textFormatting ${SOURCES} extras/examples_emscripten/Emscripten/003_textFormatting/003_textFormatting.cpp )
	target_link_options( 003_textFormatting PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 004_formatMatrix ${SOURCES} extras/examples_emscripten/Emscripten/004_formatMatrix/004_formatMatrix.cpp )
	target_link_options( 004_formatMatrix PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 005_logo ${SOURCES} extras/examples_emscripten/Emscripten/005_logo/005_logo.cpp )
	target_link_options( 005_logo PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 006_password ${SOURCES} extras/examples_emscripten/Emscripten/006_password/006_password.cpp )
	target_link_options( 006_password PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 007_input ${SOURCES} extras/examples_emscripten/Emscripten/007_input/007_input.cpp )
	target_link_options( 007_input PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 008_input_number ${SOURCES} extras/examples_emscripten/Emscripten/008_input_number/008_input_number.cpp )
	target_link_options( 008_input_number PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 009_beeeeep ${SOURCES} extras/examples_emscripten/Emscripten/009_beeeeep/009_beeeeep.cpp )
	target_link_options( 009_beeeeep PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 010_bannerText ${SOURCES} extras/examples_emscripten/Emscripten/010_bannerText/010_bannerText.cpp )
	target_link_options( 010_bannerText PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 100_keyOverride ${SOURCES} extras/examples_emscripten/Emscripten/100_keyOverride/100_keyOverride.cpp )
	target_link_options( 100_keyOverride PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 101_qrCode ${SOURCES} extras/examples_emscripten/Emscripten/101_qrCode/101_qrCode.cpp )
	target_link_options( 101_qrCode PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 200_commanderBasic ${SOURCES} extras/examples_emscripten/Emscripten/200_commanderBasic/200_commanderBasic.cpp )
	target_link_options( 200_commanderBasic PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( colorizer ${SOURCES} extras/examples_emscripten/Emscripten/colorizer/colorizer.cpp )
	target_link_options( colorizer PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( levelMeter ${SOURCES} extras/examples_emscripten/Emscripten/levelMeter/levelMeter.cpp )
	target_link_options( levelMeter PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( list ${SOURCES} extras/examples_emscripten/Emscripten/list/list.cpp )
	target_link_options( list PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( progress ${SOURCES} extras/examples_emscripten/Emscripten/progress/progress.cpp )
	target_link_options( progress PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( screenAdvanced ${SOURCES} extras/examples_emscripten/Emscripten/screenAdvanced/screenAdvanced.cpp )
	target_link_options( screenAdvanced PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

endif()
