"""
Created on May 13 2023

Copyright (c) 2020 - Daniel Hajnal
hajnal.daniel96@gmail.com
This file is part of the Shellminator project.
Modified 2023.05.13



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

import os
import shutil
from jinja2 import Template

# Convert a string list to a single string.
def listToString( lst ):
 
    # initialize an empty string
    out = ""
 
    # traverse in the string
    for current in lst:
        out += current
 
    # return string
    return out

def findLine( lst, data ):

    cntr = 0
    for current in lst:
        if current == data:
            return cntr
        cntr += 1
    
    return 'not found'

# Save the root directory path. It will be useful later.
rootDirectory = os.getcwd()

arduinoExampleFolder = "/examples"
desktopExampleFolder = "/extras/examples_desktop"
emscriptenExampleFolder = "/extras/examples_emscripten"

# Clear the examples folders.
if os.path.isdir(  rootDirectory + arduinoExampleFolder ):
    shutil.rmtree( rootDirectory + arduinoExampleFolder )

if os.path.isdir(  rootDirectory + desktopExampleFolder ):
    shutil.rmtree( rootDirectory + desktopExampleFolder )

if os.path.isdir(  rootDirectory + emscriptenExampleFolder ):
    shutil.rmtree( rootDirectory + emscriptenExampleFolder )

# Generate the example folders.
os.mkdir( rootDirectory + arduinoExampleFolder )
os.mkdir( rootDirectory + desktopExampleFolder )
os.mkdir( rootDirectory + emscriptenExampleFolder )

# Load the content of the project CMakeLists file.
with open( rootDirectory + "/CMakeLists.txt" ) as file:
  CMakeContent = file.readlines()

# Find the end of the CMakeLists file.
indexOfCMakeContentEnd = findLine( CMakeContent, "#---- Examples Section ----#\n" )

# Remove the content from the end of CMakeLists file.
CMakeContent = CMakeContent[ 0:( indexOfCMakeContentEnd + 1 ) ]
CMakeContent.append( "\n" )

# List all the template files.
templateFiles = os.listdir( rootDirectory + '/extras/example_templates' )

# List all the example files.
exampleFiles  = os.listdir( rootDirectory + '/extras/example_database' )

# This is the outer loop. We go through every template file.
for template in templateFiles:

    # Check for extension. Only .template files are processed.
    if template.endswith( '.template' ) == False:
        continue

    print( "Generating template: {:s}".format( template ) )

    # Read the next template file line-by-line.
    templateFile = open( rootDirectory + "/extras/example_templates/" + template )
    templateData = templateFile.readlines()

    # Store additional template parameters.
    boardInfo       = templateData[ 0 ]
    channelInfo     = templateData[ 1 ]
    environmentInfo = templateData[ 2 ]

    # Process additional template parameters.
    boardInfo       = boardInfo.split(       '=' )[1].replace( '\n', '' )
    channelInfo     = channelInfo.split(     '=' )[1].replace( '\n', '' )
    environmentInfo = environmentInfo.split( '=' )[1].replace( '\n', '' )

    print( "\tBoard Info   : {:s}".format( boardInfo ) )
    print( "\tChannel Info : {:s}".format( channelInfo ) )
    print( "\tEnvironment  : {:s}".format( environmentInfo ) )

    templateData = templateData[ 3: ]

    # Generate folder for the template.
    #if environmentInfo == 'Arduino':
        #os.mkdir( rootDirectory + arduinoExampleFolder + "/" + boardInfo )
        #os.mkdir( rootDirectory + doxygenExampleFolder + "/" + boardInfo )

    if environmentInfo == 'Desktop':
        os.mkdir( rootDirectory + desktopExampleFolder + "/" + boardInfo )
        CMakeContent.append( "if( BUILD_EXAMPLES )\n" )

    if environmentInfo == 'Emscripten':
        os.mkdir( rootDirectory + emscriptenExampleFolder + "/" + boardInfo )
        CMakeContent.append( "if( BUILD_WEBASSEMBLY )\n" )

    # Inner loop. We go through every example file.
    for example in exampleFiles:

        # Check for extension. Only .example files are processed.
        if example.endswith( '.example' ) == False:
            continue

        print( "\tGenerating example: {:s}".format( example ) )

        # Read the next example file line-by-line.
        exampleFile = open( rootDirectory + "/extras/example_database/" + example )
        exampleData = exampleFile.readlines()

        # Store additional example parameters.
        excludeData = exampleData[ 0 ]
        exampleCategory = exampleData[ 1 ].split( '=' )[ 1 ].replace( '\n', '' )

        # Find the index of every section.
        headerStartIndex                  = findLine( exampleData, "++--HEADER--++\n" )
        includesStartIndex                = findLine( exampleData, "++--INCLUDES--++\n" )
        globalVariablesStartIndex         = findLine( exampleData, "++--GLOBAL_VARIABLES--++\n" )
        functionPrototypesStartIndex      = findLine( exampleData, "++--FUNCTION_PROTOTYPES--++\n" )
        setupStartIndex                   = findLine( exampleData, "++--SETUP--++\n" )
        loopStartIndex                    = findLine( exampleData, "++--LOOP--++\n" )
        functionImplementationsStartIndex = findLine( exampleData, "++--FUNCTION_IMPLEMENTATIONS--++\n" )

        # Create a string from every section.
        headerData                  = listToString( exampleData[ ( headerStartIndex                  + 2 ):( includesStartIndex                - 1 ) ] )
        includesData                = listToString( exampleData[ ( includesStartIndex                + 2 ):( globalVariablesStartIndex         - 1 ) ] )
        globalVariablesData         = listToString( exampleData[ ( globalVariablesStartIndex         + 2 ):( functionPrototypesStartIndex      - 1 ) ] )
        functionPrototypesData      = listToString( exampleData[ ( functionPrototypesStartIndex      + 2 ):( setupStartIndex                   - 1 ) ] )
        setupData                   = listToString( exampleData[ ( setupStartIndex                   + 2 ):( loopStartIndex                    - 1 ) ] )
        loopData                    = listToString( exampleData[ ( loopStartIndex                    + 2 ):( functionImplementationsStartIndex - 1 ) ] )
        functionImplementationsData = listToString( exampleData[ ( functionImplementationsStartIndex + 2 ): ] )

        # Parameter fields.
        fields = {
            'channel': channelInfo,
            'YEAR': '2023',
            'MONTH': '05',
            'DAY': '13',
            'HEADER': headerData,
            'INCLUDES': includesData,
            'GLOBAL_VARIABLES': globalVariablesData,
            'FUNCTION_PROTOTYPES': functionPrototypesData,
            'SETUP': setupData,
            'LOOP': loopData,
            'FUNCTION_IMPLEMENTATIONS': functionImplementationsData,
        }

        # Firstly, we put together the example code. It will contain some empty fields like Date.
        currentTemplate = Template( listToString( templateData ) )
        firstRunData  = currentTemplate.render( fields )

        # Secondly, we finish the leftover fields
        currentTemplate = Template( firstRunData )
        secondRunData = currentTemplate.render( fields )

        # Generate a folder name from the example file name.
        #exampleFolderName = example.split( '.' )[ 0 ].replace( '_', ' ' ).capitalize()
        exampleFolderName = example.split( '.' )[ 0 ]

        # Generate folder for the example and write the output.
        if environmentInfo == 'Arduino':
            
            # File for Arduino IDE
            if not os.path.isdir( rootDirectory + arduinoExampleFolder + "\\" + exampleCategory ):
                os.mkdir( rootDirectory + arduinoExampleFolder + "\\" + exampleCategory )
            os.mkdir( rootDirectory + arduinoExampleFolder + "\\" + exampleCategory + "\\" + exampleFolderName )
            outputFile = open( rootDirectory + arduinoExampleFolder + "\\" + exampleCategory + "\\" + exampleFolderName + "/" + exampleFolderName + ".ino", "w" )
            outputFile.write( secondRunData )
            outputFile.close()

        if environmentInfo == 'Desktop':
            os.mkdir( rootDirectory + desktopExampleFolder + "/" + boardInfo + "/" + exampleFolderName )
            outputFile = open( rootDirectory + desktopExampleFolder + "/" + boardInfo + "/" + exampleFolderName + "/" + exampleFolderName + ".cpp", "w" )
            outputFile.write( secondRunData )
            outputFile.close()
            if exampleCategory == 'Wireless':
                 CMakeContent.append( "\t#" )
            else:
                 CMakeContent.append( "\t" )

            CMakeContent.append( "add_executable( " + exampleFolderName + " ${SOURCES} " + desktopExampleFolder[ 1: ] + "/" + boardInfo + "/" + exampleFolderName + "/" + exampleFolderName + ".cpp )\n" )

        if environmentInfo == 'Emscripten':
            os.mkdir( rootDirectory + emscriptenExampleFolder + "/" + boardInfo + "/" + exampleFolderName )
            outputFile = open( rootDirectory + emscriptenExampleFolder + "/" + boardInfo + "/" + exampleFolderName + "/" + exampleFolderName + ".cpp", "w" )
            outputFile.write( secondRunData )
            outputFile.close()

            if exampleCategory == 'Wireless':
                 CMakeContent.append( "\t#" )
            else:
                 CMakeContent.append( "\t" )

            CMakeContent.append( "add_executable( " + exampleFolderName + " ${SOURCES} " + emscriptenExampleFolder[ 1: ] + "/" + boardInfo + "/" + exampleFolderName + "/" + exampleFolderName + ".cpp )\n" )

            if exampleCategory == 'Wireless':
                 CMakeContent.append( "\t#" )
            else:
                 CMakeContent.append( "\t" )

            CMakeContent.append( "target_link_options( " + exampleFolderName + " PUBLIC -sNO_EXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1 -sRETAIN_COMPILER_SETTINGS -sASYNCIFY )\n\n" )
        #print( secondRunData )

    # We have to close an if statement at the end.
    if environmentInfo == 'Desktop':
        CMakeContent.append( "endif()\n" )

    if environmentInfo == 'Emscripten':
        CMakeContent.append( "endif()\n" )

CMakeFile = open( rootDirectory + "/CMakeLists.txt", "w" )
CMakeFile.write( listToString( CMakeContent ) )
CMakeFile.close()
#print( CMakeContent )

