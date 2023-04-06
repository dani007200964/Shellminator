cmake_minimum_required( VERSION 3.24 )
project( Shellminator VERSION 1.1.3 )


link_directories( src )
link_directories( extras/ArduinoSim )

add_library(
    Stream SHARED
    extras/ArduinoSim/Arduino.h
    extras/ArduinoSim/Print.cpp
    extras/ArduinoSim/Print.h
    extras/ArduinoSim/Printable.h
    extras/ArduinoSim/Stream.cpp
    extras/ArduinoSim/Stream.h
    extras/ArduinoSim/System.h
)

add_library(
    Shellminator SHARED
    src/Shellminator.cpp
    src/Shellminator.hpp
    src/Shellminator-IO.cpp
    src/Shellminator-IO.hpp
    src/Shellminator-Settings.hpp
)

target_include_directories( Shellminator PUBLIC
    extras/ArduinoSim/
    src
)


target_link_libraries( Shellminator PUBLIC Stream )

add_subdirectory( extras )