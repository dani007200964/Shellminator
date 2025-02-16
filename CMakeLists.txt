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
    src/Shellminator-Neofetch.cpp
    src/Shellminator-Neofetch.hpp
    src/Shellminator-GUI.hpp
    src/GUI/Shellminator-PlotModule.cpp
    src/GUI/Shellminator-PlotModule.hpp
    src/GUI/Shellminator-Buttons.cpp
    src/GUI/Shellminator-Buttons.hpp
    src/GUI/Shellminator-Progress.hpp
    src/GUI/Shellminator-Progress.cpp
    src/GUI/Shellminator-List.hpp
    src/GUI/Shellminator-List.cpp
    src/GUI/Shellminator-ScreenGrid.hpp
    src/GUI/Shellminator-ScreenGrid.cpp
    src/GUI/Shellminator-List-Detailed.hpp
    src/GUI/Shellminator-List-Detailed.cpp
    src/GUI/Shellminator-Level-Meter.hpp
    src/GUI/Shellminator-Level-Meter.cpp
    src/GUI/Shellminator-Notification.hpp
    src/GUI/Shellminator-Notification.cpp
    src/Shellminator-DefaultSettings.hpp
    src/Shellminator.hpp
    src/Shellminator.cpp
    src/Shellminator-SpecialKeys.cpp
    src/Shellminator-Styles.cpp
    src/Shellminator-SearchModule.cpp
    src/Shellminator-MouseInterface.cpp
    src/Shellminator-Crypto.hpp
    src/Shellminator-Crypto.cpp
    src/Shellminator-Websocket.hpp
    src/Shellminator-Websocket.cpp
    src/Shellminator-WebServer.hpp
    src/Shellminator-WebServer.cpp
    src/Shellminator-Input.cpp
    src/Shellminator-Events.cpp
    src/Shellminator-QR-Code-Module.hpp
    src/Shellminator-QR-Code-Module.cpp
    src/Shellminator-Colorizer.hpp
    src/Shellminator-Colorizer.cpp
    src/Shellminator-Commander-Interface.hpp
    src/Shellminator-Commander-Interface.cpp
    src/Shellminator-TcpSocket.hpp
    src/Shellminator-TcpSocket.cpp
    src/Shellminator-Helpers.hpp
    src/Shellminator-Helpers.cpp

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
    extras/Commander-API/src/Commander-Arguments.hpp
    extras/Commander-API/src/Commander-Arguments.cpp
    extras/Commander-API/src/Commander-Database.hpp
    extras/Commander-API/src/Commander-DefaultSettings.hpp
    extras/Commander-API/src/Commander-Caller-Interface.hpp
    extras/Commander-API/src/Commander-Caller-Interface.cpp
    extras/Commander-API/src/Commander-Autocomplete.hpp
    extras/Commander-API/src/Commander-Autocomplete.cpp

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
	add_executable( 001_basic ${SOURCES} extras/examples_desktop/Desktop/001_basic/001_basic.cpp )
	add_executable( 002_commandCallback ${SOURCES} extras/examples_desktop/Desktop/002_commandCallback/002_commandCallback.cpp )
	add_executable( 003_textFormatting ${SOURCES} extras/examples_desktop/Desktop/003_textFormatting/003_textFormatting.cpp )
	add_executable( 004_formatMatrix ${SOURCES} extras/examples_desktop/Desktop/004_formatMatrix/004_formatMatrix.cpp )
	add_executable( 005_logo ${SOURCES} extras/examples_desktop/Desktop/005_logo/005_logo.cpp )
	add_executable( 006_password ${SOURCES} extras/examples_desktop/Desktop/006_password/006_password.cpp )
	add_executable( 007_input ${SOURCES} extras/examples_desktop/Desktop/007_input/007_input.cpp )
	add_executable( 008_input_number ${SOURCES} extras/examples_desktop/Desktop/008_input_number/008_input_number.cpp )
	add_executable( 009_beeeeep ${SOURCES} extras/examples_desktop/Desktop/009_beeeeep/009_beeeeep.cpp )
	add_executable( 010_bannerText ${SOURCES} extras/examples_desktop/Desktop/010_bannerText/010_bannerText.cpp )
	add_executable( 100_keyOverride ${SOURCES} extras/examples_desktop/Desktop/100_keyOverride/100_keyOverride.cpp )
	add_executable( 101_qrCode ${SOURCES} extras/examples_desktop/Desktop/101_qrCode/101_qrCode.cpp )
	add_executable( 102_buffering ${SOURCES} extras/examples_desktop/Desktop/102_buffering/102_buffering.cpp )
	add_executable( 120_neofetchBasic ${SOURCES} extras/examples_desktop/Desktop/120_neofetchBasic/120_neofetchBasic.cpp )
	add_executable( 121_neofetchAdvanced ${SOURCES} extras/examples_desktop/Desktop/121_neofetchAdvanced/121_neofetchAdvanced.cpp )
	add_executable( 200_commanderBasic ${SOURCES} extras/examples_desktop/Desktop/200_commanderBasic/200_commanderBasic.cpp )
	add_executable( 201_commanderOptimizations ${SOURCES} extras/examples_desktop/Desktop/201_commanderOptimizations/201_commanderOptimizations.cpp )
	add_executable( 202_commanderSimpleArguments ${SOURCES} extras/examples_desktop/Desktop/202_commanderSimpleArguments/202_commanderSimpleArguments.cpp )
	add_executable( 203_commanderArgumentTypes ${SOURCES} extras/examples_desktop/Desktop/203_commanderArgumentTypes/203_commanderArgumentTypes.cpp )
	add_executable( 204_commanderPositionIndependentArg ${SOURCES} extras/examples_desktop/Desktop/204_commanderPositionIndependentArg/204_commanderPositionIndependentArg.cpp )
	add_executable( 205_commanderCowsay ${SOURCES} extras/examples_desktop/Desktop/205_commanderCowsay/205_commanderCowsay.cpp )
	add_executable( 206_commanderSystemVariables ${SOURCES} extras/examples_desktop/Desktop/206_commanderSystemVariables/206_commanderSystemVariables.cpp )
	add_executable( 207_commanderSetSystemVariables ${SOURCES} extras/examples_desktop/Desktop/207_commanderSetSystemVariables/207_commanderSetSystemVariables.cpp )
	add_executable( 208_commanderColorizer ${SOURCES} extras/examples_desktop/Desktop/208_commanderColorizer/208_commanderColorizer.cpp )
	add_executable( 250_commanderAutoComplete ${SOURCES} extras/examples_desktop/Desktop/250_commanderAutoComplete/250_commanderAutoComplete.cpp )
	add_executable( 300_guiButton ${SOURCES} extras/examples_desktop/Desktop/300_guiButton/300_guiButton.cpp )
	add_executable( 301_guiProgress ${SOURCES} extras/examples_desktop/Desktop/301_guiProgress/301_guiProgress.cpp )
	add_executable( 302_guiProgressAdvanced ${SOURCES} extras/examples_desktop/Desktop/302_guiProgressAdvanced/302_guiProgressAdvanced.cpp )
	add_executable( 303_guiLevelMeter ${SOURCES} extras/examples_desktop/Desktop/303_guiLevelMeter/303_guiLevelMeter.cpp )
	add_executable( 304_guiLevelMeterAdvanced ${SOURCES} extras/examples_desktop/Desktop/304_guiLevelMeterAdvanced/304_guiLevelMeterAdvanced.cpp )
	add_executable( 305_guiNotification ${SOURCES} extras/examples_desktop/Desktop/305_guiNotification/305_guiNotification.cpp )
	add_executable( 306_guiList ${SOURCES} extras/examples_desktop/Desktop/306_guiList/306_guiList.cpp )
	add_executable( 307_guiListAdvanced ${SOURCES} extras/examples_desktop/Desktop/307_guiListAdvanced/307_guiListAdvanced.cpp )
	add_executable( 308_guiPlot ${SOURCES} extras/examples_desktop/Desktop/308_guiPlot/308_guiPlot.cpp )
	add_executable( 309_guiPlotAdvanced ${SOURCES} extras/examples_desktop/Desktop/309_guiPlotAdvanced/309_guiPlotAdvanced.cpp )
	add_executable( 400_simpleScreen ${SOURCES} extras/examples_desktop/Desktop/400_simpleScreen/400_simpleScreen.cpp )
	add_executable( 401_screenEndCallback ${SOURCES} extras/examples_desktop/Desktop/401_screenEndCallback/401_screenEndCallback.cpp )
	add_executable( 402_endlessScreen ${SOURCES} extras/examples_desktop/Desktop/402_endlessScreen/402_endlessScreen.cpp )
	#add_executable( 500_webSocketSimple ${SOURCES} extras/examples_desktop/Desktop/500_webSocketSimple/500_webSocketSimple.cpp )
	#add_executable( 501_webSocketPretty ${SOURCES} extras/examples_desktop/Desktop/501_webSocketPretty/501_webSocketPretty.cpp )
	#add_executable( 502_webSocketSelfHosted ${SOURCES} extras/examples_desktop/Desktop/502_webSocketSelfHosted/502_webSocketSelfHosted.cpp )
	#add_executable( 510_bleSimple ${SOURCES} extras/examples_desktop/Desktop/510_bleSimple/510_bleSimple.cpp )
	#add_executable( 520_tcpSimple ${SOURCES} extras/examples_desktop/Desktop/520_tcpSimple/520_tcpSimple.cpp )
	add_executable( 800_debuggersAndDevelopers ${SOURCES} extras/examples_desktop/Desktop/800_debuggersAndDevelopers/800_debuggersAndDevelopers.cpp )
	add_executable( 801_dinoCrysis ${SOURCES} extras/examples_desktop/Desktop/801_dinoCrysis/801_dinoCrysis.cpp )
	add_executable( 999_techDemo ${SOURCES} extras/examples_desktop/Desktop/999_techDemo/999_techDemo.cpp )
endif()
if( BUILD_WEBASSEMBLY )
	# add_executable( 001_basic ${SOURCES} extras/examples_emscripten/Emscripten/001_basic/001_basic.cpp )
	# target_link_options( 001_basic PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 002_commandCallback ${SOURCES} extras/examples_emscripten/Emscripten/002_commandCallback/002_commandCallback.cpp )
	# target_link_options( 002_commandCallback PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 003_textFormatting ${SOURCES} extras/examples_emscripten/Emscripten/003_textFormatting/003_textFormatting.cpp )
	# target_link_options( 003_textFormatting PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 004_formatMatrix ${SOURCES} extras/examples_emscripten/Emscripten/004_formatMatrix/004_formatMatrix.cpp )
	# target_link_options( 004_formatMatrix PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 005_logo ${SOURCES} extras/examples_emscripten/Emscripten/005_logo/005_logo.cpp )
	# target_link_options( 005_logo PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 006_password ${SOURCES} extras/examples_emscripten/Emscripten/006_password/006_password.cpp )
	# target_link_options( 006_password PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 007_input ${SOURCES} extras/examples_emscripten/Emscripten/007_input/007_input.cpp )
	# target_link_options( 007_input PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 008_input_number ${SOURCES} extras/examples_emscripten/Emscripten/008_input_number/008_input_number.cpp )
	# target_link_options( 008_input_number PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 009_beeeeep ${SOURCES} extras/examples_emscripten/Emscripten/009_beeeeep/009_beeeeep.cpp )
	# target_link_options( 009_beeeeep PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 010_bannerText ${SOURCES} extras/examples_emscripten/Emscripten/010_bannerText/010_bannerText.cpp )
	# target_link_options( 010_bannerText PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 100_keyOverride ${SOURCES} extras/examples_emscripten/Emscripten/100_keyOverride/100_keyOverride.cpp )
	# target_link_options( 100_keyOverride PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 101_qrCode ${SOURCES} extras/examples_emscripten/Emscripten/101_qrCode/101_qrCode.cpp )
	# target_link_options( 101_qrCode PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 102_buffering ${SOURCES} extras/examples_emscripten/Emscripten/102_buffering/102_buffering.cpp )
	# target_link_options( 102_buffering PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 120_neofetchBasic ${SOURCES} extras/examples_emscripten/Emscripten/120_neofetchBasic/120_neofetchBasic.cpp )
	# target_link_options( 120_neofetchBasic PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 121_neofetchAdvanced ${SOURCES} extras/examples_emscripten/Emscripten/121_neofetchAdvanced/121_neofetchAdvanced.cpp )
	# target_link_options( 121_neofetchAdvanced PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 200_commanderBasic ${SOURCES} extras/examples_emscripten/Emscripten/200_commanderBasic/200_commanderBasic.cpp )
	# target_link_options( 200_commanderBasic PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 201_commanderOptimizations ${SOURCES} extras/examples_emscripten/Emscripten/201_commanderOptimizations/201_commanderOptimizations.cpp )
	# target_link_options( 201_commanderOptimizations PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 202_commanderSimpleArguments ${SOURCES} extras/examples_emscripten/Emscripten/202_commanderSimpleArguments/202_commanderSimpleArguments.cpp )
	# target_link_options( 202_commanderSimpleArguments PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 203_commanderArgumentTypes ${SOURCES} extras/examples_emscripten/Emscripten/203_commanderArgumentTypes/203_commanderArgumentTypes.cpp )
	# target_link_options( 203_commanderArgumentTypes PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 204_commanderPositionIndependentArg ${SOURCES} extras/examples_emscripten/Emscripten/204_commanderPositionIndependentArg/204_commanderPositionIndependentArg.cpp )
	# target_link_options( 204_commanderPositionIndependentArg PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 205_commanderCowsay ${SOURCES} extras/examples_emscripten/Emscripten/205_commanderCowsay/205_commanderCowsay.cpp )
	# target_link_options( 205_commanderCowsay PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 206_commanderSystemVariables ${SOURCES} extras/examples_emscripten/Emscripten/206_commanderSystemVariables/206_commanderSystemVariables.cpp )
	# target_link_options( 206_commanderSystemVariables PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 207_commanderSetSystemVariables ${SOURCES} extras/examples_emscripten/Emscripten/207_commanderSetSystemVariables/207_commanderSetSystemVariables.cpp )
	# target_link_options( 207_commanderSetSystemVariables PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 208_commanderColorizer ${SOURCES} extras/examples_emscripten/Emscripten/208_commanderColorizer/208_commanderColorizer.cpp )
	# target_link_options( 208_commanderColorizer PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 250_commanderAutoComplete ${SOURCES} extras/examples_emscripten/Emscripten/250_commanderAutoComplete/250_commanderAutoComplete.cpp )
	# target_link_options( 250_commanderAutoComplete PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 300_guiButton ${SOURCES} extras/examples_emscripten/Emscripten/300_guiButton/300_guiButton.cpp )
	# target_link_options( 300_guiButton PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 301_guiProgress ${SOURCES} extras/examples_emscripten/Emscripten/301_guiProgress/301_guiProgress.cpp )
	# target_link_options( 301_guiProgress PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 302_guiProgressAdvanced ${SOURCES} extras/examples_emscripten/Emscripten/302_guiProgressAdvanced/302_guiProgressAdvanced.cpp )
	# target_link_options( 302_guiProgressAdvanced PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 303_guiLevelMeter ${SOURCES} extras/examples_emscripten/Emscripten/303_guiLevelMeter/303_guiLevelMeter.cpp )
	# target_link_options( 303_guiLevelMeter PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 304_guiLevelMeterAdvanced ${SOURCES} extras/examples_emscripten/Emscripten/304_guiLevelMeterAdvanced/304_guiLevelMeterAdvanced.cpp )
	# target_link_options( 304_guiLevelMeterAdvanced PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 305_guiNotification ${SOURCES} extras/examples_emscripten/Emscripten/305_guiNotification/305_guiNotification.cpp )
	# target_link_options( 305_guiNotification PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 306_guiList ${SOURCES} extras/examples_emscripten/Emscripten/306_guiList/306_guiList.cpp )
	# target_link_options( 306_guiList PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 307_guiListAdvanced ${SOURCES} extras/examples_emscripten/Emscripten/307_guiListAdvanced/307_guiListAdvanced.cpp )
	# target_link_options( 307_guiListAdvanced PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 308_guiPlot ${SOURCES} extras/examples_emscripten/Emscripten/308_guiPlot/308_guiPlot.cpp )
	# target_link_options( 308_guiPlot PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 309_guiPlotAdvanced ${SOURCES} extras/examples_emscripten/Emscripten/309_guiPlotAdvanced/309_guiPlotAdvanced.cpp )
	# target_link_options( 309_guiPlotAdvanced PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 400_simpleScreen ${SOURCES} extras/examples_emscripten/Emscripten/400_simpleScreen/400_simpleScreen.cpp )
	# target_link_options( 400_simpleScreen PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 401_screenEndCallback ${SOURCES} extras/examples_emscripten/Emscripten/401_screenEndCallback/401_screenEndCallback.cpp )
	# target_link_options( 401_screenEndCallback PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 402_endlessScreen ${SOURCES} extras/examples_emscripten/Emscripten/402_endlessScreen/402_endlessScreen.cpp )
	# target_link_options( 402_endlessScreen PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# #add_executable( 500_webSocketSimple ${SOURCES} extras/examples_emscripten/Emscripten/500_webSocketSimple/500_webSocketSimple.cpp )
	# #target_link_options( 500_webSocketSimple PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# #add_executable( 501_webSocketPretty ${SOURCES} extras/examples_emscripten/Emscripten/501_webSocketPretty/501_webSocketPretty.cpp )
	# #target_link_options( 501_webSocketPretty PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# #add_executable( 502_webSocketSelfHosted ${SOURCES} extras/examples_emscripten/Emscripten/502_webSocketSelfHosted/502_webSocketSelfHosted.cpp )
	# #target_link_options( 502_webSocketSelfHosted PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# #add_executable( 510_bleSimple ${SOURCES} extras/examples_emscripten/Emscripten/510_bleSimple/510_bleSimple.cpp )
	# #target_link_options( 510_bleSimple PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# #add_executable( 520_tcpSimple ${SOURCES} extras/examples_emscripten/Emscripten/520_tcpSimple/520_tcpSimple.cpp )
	# #target_link_options( 520_tcpSimple PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	# add_executable( 800_debuggersAndDevelopers ${SOURCES} extras/examples_emscripten/Emscripten/800_debuggersAndDevelopers/800_debuggersAndDevelopers.cpp )
	# target_link_options( 800_debuggersAndDevelopers PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 801_dinoCrysis ${SOURCES} extras/examples_emscripten/Emscripten/801_dinoCrysis/801_dinoCrysis.cpp )
	target_link_options( 801_dinoCrysis PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

	add_executable( 999_techDemo ${SOURCES} extras/examples_emscripten/Emscripten/999_techDemo/999_techDemo.cpp )
	target_link_options( 999_techDemo PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )

endif()
