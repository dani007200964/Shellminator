"""
Created on April 08 2023

Copyright (c) 2020 - Daniel Hajnal
hajnal.daniel96@gmail.com
This file is part of the Shellminator project.
Modified 2023.04.08



MIT License

Copyright (c) 2022 Daniel Hajnal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

"""

import subprocess
import os
import shutil
import sys
import getopt

# Collect input arguments
argv = sys.argv[ 1: ]

helpText =  """Arguments:
-t or --target   : Can be: clean, all, simulator, test, examples
-r or --rebuild  : Clear everything and start the build process from the beginning.
-i or --install  : Installs the necessary libraries for the build system.
-h or --help     : Help message.
-v or --version  : Prints the version info.
-c or --checkout : It have to be used before new release. It generates examples, badges and documentation data."""

versionText = 'Shellminator Build Tools Version: 0.0.1'

# Read the arguments
opts, args = getopt.getopt( argv, "t:rcihvc", [ 'target', 'rebuild', 'install', 'help', 'version', 'checkout' ] )

# Default argument values
target = 'all'
rebuild = False
install = False
checkout = False

# Process the arguments
for opt, arg in opts:

    # Check for target options.
    if opt in [ '-t' , '--target' ]:
        target = arg

    # Check for rebuild option.
    if opt in [ '-r' , '--rebuild' ]:
        rebuild = True
    
    # Check for install option.
    if opt in [ '-i' , '--install' ]:
        install = True

    # Check for help message.
    if opt in [ '-h' , '--help' ]:
        print( helpText )
        quit()

    # Check for version message.
    if opt in [ '-v' , '--version' ]:
        print( versionText )
        quit()

    # Check for checkout option.
    if opt in [ '-c' , '--checkout' ]:
        checkout = True

# We have to split the target string to an array of options.
target = target.split()

# The name of the build directory
buildDirectoryName = "build"

# Save the root directory path. It will be useful later.
rootDirectory = os.getcwd()

# Check if the build directory exists.
if os.path.isdir( buildDirectoryName ):

    # If we have a clean argument, we have to empty it's content.
    # We just simply delete the whole build folder and create a new one.
    if 'clean' in target:

        print()
        print( '---- Clean the build folder ----' )
        print()

        shutil.rmtree( rootDirectory + "/" + buildDirectoryName )
        os.mkdir( rootDirectory + "/" + buildDirectoryName )

        print( 'Cleaning done!' )

# ---- Install Python Modules Section ----

# Check if we have to install the python packages.
if install:

    print()
    print( '---- Install Python Modules ----' )
    print()

    # Install Python modules.
    command = 'pip install -r pythonRequirements.txt'
    terminalProcess = subprocess.Popen( command, shell=True )
    terminalProcess.wait()

    if( terminalProcess.returncode !=0 ):
        print( "pip command failed!" )
        print( "You probably need to install the necessary Python modules manually. You can find them in the pythonRequirements.txt file. It is located in the root folder." )
        sys.exit( 5 )

    quit()


# We have to change directory to the build directory to make CMake happy.
os.chdir( rootDirectory + "/" + buildDirectoryName )

# ---- Simulator Build Section ----

# Check if we have to build the simulator.
if ( 'simulator' in target ) or ( 'all' in target ):

    print()
    print( '---- Generate CMake files for Simulator ----' )
    print()

    # Create CMake structure
    command = 'cmake .. -G "MinGW Makefiles" -DBUILD_SIMULATOR=ON'
    terminalProcess = subprocess.Popen( command, shell=True )
    terminalProcess.wait()

    if( terminalProcess.returncode !=0 ):
        print( "CMake command failed!" )
        print( "Probably, you have to clean the project and it will work next time :)" )
        sys.exit( 1 )

    print()
    print( '---- Build target for Simulator ----' )
    print()

    # Build the project
    command = 'cmake --build .'
    terminalProcess = subprocess.Popen( command, shell=True )
    terminalProcess.wait()

    if( terminalProcess.returncode !=0 ):
        print( "CMake build command failed!" )
        print( "Probably, you have to clean the project and it will work next time :)" )
        sys.exit( 2 )

    print()
    print( 'Simulator building finished! You can find the executable here: {:s}'.format( rootDirectory + "/" + buildDirectoryName + "report/Simulator.exe" ) )


# ---- Unit Test Section ----

# Check if we have to build for unit test.
if ( 'test' in target ) or ( 'all' in target ):

    print()
    print( '---- Generate CMake files for Unit Test ----' )
    print()

    # Create CMake structure
    command = 'cmake .. -G "MinGW Makefiles" -DRUN_TESTS=ON'
    terminalProcess = subprocess.Popen( command, shell=True )
    terminalProcess.wait()

    if( terminalProcess.returncode !=0 ):
        print( "CMake command failed!" )
        print( "Probably, you have to clean the project and it will work next time :)" )
        sys.exit( 1 )

    print()
    print( '---- Build target for Unit Test ----' )
    print()

    # Build the project
    command = 'cmake --build .'
    terminalProcess = subprocess.Popen( command, shell=True )
    terminalProcess.wait()

    if( terminalProcess.returncode !=0 ):
        print( "CMake build command failed!" )
        print( "Probably, you have to clean the project and it will work next time :)" )
        sys.exit( 2 )

    print()
    print( '---- Run Unit Test ----' )
    print()

    # Run unit test code
    command = '.\\test_channels.exe'
    terminalProcess = subprocess.Popen( command, shell=True )
    terminalProcess.wait()

    if( terminalProcess.returncode !=0 ):
        print( "Unit Test run failed!" )
        print( "There is something wrong with the unit test binary. It have to run all the tests without error code." )
        sys.exit( 3 )

    # Check if the report directory not exists. In this case create a report folder.
    if os.path.isdir( rootDirectory + "/" + buildDirectoryName + "/report" ) == False:
        os.mkdir( rootDirectory + "/" + buildDirectoryName + "/report" )

    print()
    print( '---- Generate Coverage Report ----' )
    print()

    # Run gcovr to evaluate coverage
    command = 'gcovr -r .. --html-details -o report/report.html'
    terminalProcess = subprocess.Popen( command, shell=True )
    terminalProcess.wait()

    if( terminalProcess.returncode !=0 ):
        print( "gcovr failed!" )
        print( "Probably you doesn't installed gcovr properly. You need to install gcovr with pip. You need version 6.0 of gcovr. Also it have to be added to PATH." )
        sys.exit( 4 )

    print( 'Coverage report data generated here: {:s}'.format( rootDirectory + "/" + buildDirectoryName + "/report/report.html" ) )


print()
print( 'Shellminator Build Tools Finished!' )
print()
