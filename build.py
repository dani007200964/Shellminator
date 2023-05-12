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

buildDirectoryName = "build"
rootDirectory = os.getcwd()

ignoreFromDelete = [ "README.md" ]

#os.rmdir( "build" )
if os.path.isdir( buildDirectoryName ):
    shutil.rmtree( rootDirectory + "/" + buildDirectoryName )

os.mkdir( rootDirectory + "/" + buildDirectoryName )

os.chdir( rootDirectory + "/" + buildDirectoryName )

print( os.getcwd() )

# Create CMake structure
command = 'cmake .. -G "MinGW Makefiles" -DRUN_TESTS=ON'
terminalProcess = subprocess.Popen( command, shell=True )
terminalProcess.wait()

if( terminalProcess.returncode !=0 ):
    print( "We have problem!" )

# Build the project
command = 'cmake --build .'
terminalProcess = subprocess.Popen( command, shell=True )
terminalProcess.wait()

if( terminalProcess.returncode !=0 ):
    print( "We have problem!" )

# Run unit test code
command = '.\\test_channels.exe'
terminalProcess = subprocess.Popen( command, shell=True )
terminalProcess.wait()

if( terminalProcess.returncode !=0 ):
    print( "We have problem!" )

# Run gcovr to evaluate coverage
os.mkdir( rootDirectory + "/" + buildDirectoryName + "/report" )
command = 'gcovr -r .. --html-details -o report/report.html'
terminalProcess = subprocess.Popen( command, shell=True )
terminalProcess.wait()

if( terminalProcess.returncode !=0 ):
    print( "We have problem!" )

