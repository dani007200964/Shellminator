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
import json
from jinja2 import Template
import minify_html

def convertToHeader( data, variable_name ):
    data = data.encode('utf-8')
    result = []
    result.append( '#define ' + variable_name + ' {' )
    line = [ data[i:i+12] for i in range(0, len(data), 12) ]
    for i, x in enumerate( line ):
        lineText = ', '.join( [ '0x{val:02x}'.format( val = c ) for c in x ] )
        result.append('  {line}{end_comma}'.format( line=lineText, end_comma=',' if i < len( line ) - 1 else '' ) )
    result.append( '}' )
    result = '\\\n'.join( result )
    result = result + '\n\n' + '#define ' + variable_name + '_SIZE ' + str( len( data ) ) + '\n'
    return result

# Collect input arguments
argv = sys.argv[ 1: ]

helpText =  """Arguments:
-t or --target   : Can be: clean, all, test, examples, web
-h or --help     : Help message.
-v or --version  : Prints the version info.
-c or --checkout : It has to be used before new release. It generates examples, badges and documentation data."""

versionText = 'Shellminator Build Tools Version: 0.0.1'

# Read the arguments
opts, args = getopt.getopt( argv, "t:hvc", [ 'target', 'help', 'version', 'checkout' ] )

# Default argument values
target = 'none'
checkout = False

# Process the arguments
for opt, arg in opts:

    # Check for target options.
    if opt in [ '-t' , '--target' ]:
        target = arg

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

# We have to change directory to the build directory to make CMake happy.
os.chdir( rootDirectory + "/" + buildDirectoryName )

# ---- Examples Build Section ----

# Check if we have to build the examples.
if ( 'examples' in target ) or ( 'all' in target ):

    print()
    print( '---- Generate CMake files for Examples ----' )
    print()

    # Create CMake structure
    command = 'cmake .. -G "MinGW Makefiles" -DBUILD_EXAMPLES=ON'
    terminalProcess = subprocess.Popen( command, shell=True )
    terminalProcess.wait()

    if( terminalProcess.returncode !=0 ):
        print( "CMake command failed!" )
        print( "Probably, you have to clean the project and it will work next time :)" )
        sys.exit( 1 )

    print()
    print( '---- Build all examples ----' )
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
    print( 'Examples are built. You can find the executables here: {:s}'.format( rootDirectory + "/" + buildDirectoryName ) )


# ---- WebAssembly Build Section ----

# Check if we have to build the examples to the web.
if ( 'web' in target ) or ( 'all' in target ):

    print()
    print( '---- Generate CMake files for Examples ----' )
    print()

    # Create CMake structure
    command = 'emcmake cmake .. -G "MinGW Makefiles" -DBUILD_WEBASSEMBLY=ON'
    terminalProcess = subprocess.Popen( command, shell=True )
    terminalProcess.wait()

    if( terminalProcess.returncode !=0 ):
        print( "CMake command failed!" )
        print( "Probably, you have to clean the project and it will work next time :)" )
        sys.exit( 1 )

    print()
    print( '---- Build all examples ----' )
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
    print( 'Examples for Web are built. You can find the executables here: {:s}'.format( rootDirectory + "/" + buildDirectoryName ) )

    buildFiles = os.listdir( rootDirectory + '/build' )
    print( "Files in build folder:" )
    print( buildFiles )

    ShellminatorDocsFolder = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'ShellminatorDocs') )

    # Check if the ShellminatorDocs directory exists.
    if os.path.isdir( ShellminatorDocsFolder ) == False:
        os.mkdir( ShellminatorDocsFolder )
    
    print( os.path.abspath(os.path.join(os.path.dirname(__file__), 'run_server.py') ) )
    shutil.copyfile( os.path.abspath(os.path.join(os.path.dirname(__file__), 'run_server.py') ), os.path.join( ShellminatorDocsFolder, 'run_server.py' ) )

    ShellminatorDocsHtmlFolder = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'ShellminatorDocs/html') )

    # Check if the ShellminatorDocs/html directory exists.
    if os.path.isdir( ShellminatorDocsHtmlFolder ) == False:
        os.mkdir( ShellminatorDocsHtmlFolder )
    

    webExamplesFolder = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'ShellminatorDocs/html/webExamples') )

    # Check if the ShellminatorDocs/html/webExamples directory exists. In this case delete its content.
    if os.path.isdir( webExamplesFolder ) == True:
        shutil.rmtree( webExamplesFolder )
    
    os.mkdir( webExamplesFolder )

    for file in buildFiles:
        if file.endswith( ".js" ):

            fileName = file.split(".")[0]
            wasmFile = file.split(".")[0] + ".wasm"
            print( rootDirectory + "/build/" + file,      webExamplesFolder + "/" + file  )
            shutil.copyfile( rootDirectory + "/build/" + file,      webExamplesFolder + "/" + file )
            shutil.copyfile( rootDirectory + "/build/" + wasmFile,  webExamplesFolder + "/" + wasmFile )
            
            # Read the next template file line-by-line.
            templateFile = open( rootDirectory + "/extras/emscripten_template_page.html" )
            templateData = templateFile.read()
            templateFile.close()

            # Parameter fields.
            fields = {
                'EXAMPLE_NAME': fileName,
                'EXAMPLE_PATH': "./" + fileName + ".js"
            }

            # Create a template object from the template file.
            currentTemplate = Template(  templateData )
            webPageData  = currentTemplate.render( fields )

            outputFile = open( webExamplesFolder + "/" + fileName + ".html", "w" )
            outputFile.write( webPageData )
            outputFile.close()

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
    print( '---- Run Unit Tests ----' )
    print()

    # List all the template files.
    executableTests = os.listdir( rootDirectory + '/build' )
    print( "Files in build folder:" )
    print( executableTests )

    for file in executableTests:
        if file.startswith( "test_" ) and file.endswith( ".exe" ):
        
            # Run unit test code
            command = '.\\' + file
            terminalProcess = subprocess.Popen( command, shell=True )
            terminalProcess.wait()

            if( terminalProcess.returncode !=0 ):
                print( "Unit Test run failed!" )
                print( "There is something wrong with the unit test binary. It have to run all the tests without error code." )
                sys.exit( 3 )

    print()
    print( '---- Generate Coverage Report ----' )
    print()

    # Run gcovr to evaluate coverage
    command = 'gcovr -r .. --json-summary-pretty -o coverage_report.json'
    terminalProcess = subprocess.Popen( command, shell=True )
    terminalProcess.wait()

    if( terminalProcess.returncode !=0 ):
        print( "gcovr failed!" )
        print( "Probably you doesn't installed gcovr properly. You need to install gcovr with pip. You need version 6.0 of gcovr. Also it have to be added to PATH." )
        sys.exit( 4 )

    print( 'Coverage report text generated here: {:s}'.format( rootDirectory + "/" + buildDirectoryName + "/coverage_report.json" ) )



# ---- Documentation Section ----

# We have to change directory to the root directory to make gcovr happy.
#os.chdir( rootDirectory )

# Check if we have to build the documentation.
if checkout:
    print()
    print( '---- Minifying self hosted pages ----' )
    print()

    browserResponseFileData = ""

    #---- Minifying web_xterm_themed.html page ----

    htmlFile = open( rootDirectory + "/extras/webpage/web_xterm_themed.html" )
    htmlData = htmlFile.read()
    htmlFile.close()

    minified = minify_html.minify( htmlData, minify_js=True, minify_css=True, keep_spaces_between_attributes=True, remove_processing_instructions=True )
    browserResponseFileData = browserResponseFileData + convertToHeader( minified, 'SHELLMINATOR_WEB_XTERM_THEMED' ) + '\n'

    outputFile = open( rootDirectory + "/extras/webpage/minified_pages/web_xterm_themed_mini.html", "w" )
    outputFile.write( minified )
    outputFile.close()

    #---- Minifying index_themed_offline.html page ----

    htmlFile = open( rootDirectory + "/extras/webpage/web_xterm_themed_offline.html" )
    htmlData = htmlFile.read()
    htmlFile.close()

    minified = minify_html.minify( htmlData, minify_js=True, minify_css=True, keep_spaces_between_attributes=True, remove_processing_instructions=True )
    browserResponseFileData = browserResponseFileData + convertToHeader( minified, 'SHELLMINATOR_WEB_XTERM_THEMED_OFFLINE' ) + '\n'

    outputFile = open( rootDirectory + "/extras/webpage/minified_pages/web_xterm_themed_offline_mini.html", "w" )
    outputFile.write( minified )
    outputFile.close()

    #---- Minifying 404.html page ----

    htmlFile = open( rootDirectory + "/extras/webpage/404_themed.html" )
    htmlData = htmlFile.read()
    htmlFile.close()

    minified = minify_html.minify( htmlData, minify_js=True, minify_css=True, keep_spaces_between_attributes=True, remove_processing_instructions=True )
    browserResponseFileData = browserResponseFileData + convertToHeader( minified, 'SHELLMINATOR_404_THEMED' ) + '\n'

    outputFile = open( rootDirectory + "/extras/webpage/minified_pages/404_themed_mini.html", "w" )
    outputFile.write( minified )
    outputFile.close()

    #---- Converting xterm.min.js ----

    htmlFile = open( rootDirectory + "/extras/webpage/xterm.min.js" )
    htmlData = htmlFile.read()
    htmlFile.close()

    #minified = minify_html.minify( htmlData, minify_js=True, minify_css=True, keep_spaces_between_attributes=True, remove_processing_instructions=True )
    browserResponseFileData = browserResponseFileData + convertToHeader( htmlData, 'XTERM_MIN_JS' ) + '\n'

    #---- Converting addon-web-links.min.js ----

    htmlFile = open( rootDirectory + "/extras/webpage/addon-web-links.min.js" )
    htmlData = htmlFile.read()
    htmlFile.close()

    #minified = minify_html.minify( htmlData, minify_js=True, minify_css=True, keep_spaces_between_attributes=True, remove_processing_instructions=True )
    browserResponseFileData = browserResponseFileData + convertToHeader( htmlData, 'XTERM_ADDON_WEB_LINKS_MIN_JS' ) + '\n'

    #---- Generating Browser Response ----

    outputFile = open( rootDirectory + "/src/Shellminator-Browser-Response.hpp", "w" )
    outputFile.write( browserResponseFileData )
    outputFile.close()

    print()
    print( '---- Generate HTML Coverage Report ----' )
    print()

    os.chdir( rootDirectory + "/" + buildDirectoryName )

    # Check if the report directory not exists. In this case create a report folder.
    if os.path.isdir( rootDirectory + "/" + buildDirectoryName + "/report" ) == False:
        os.mkdir( rootDirectory + "/" + buildDirectoryName + "/report" )

    #shutil.copyfile( rootDirectory + "/docs/Style/gcovr/style.css",  rootDirectory + "/" + buildDirectoryName + "/style.css" )

    # Run gcovr to evaluate coverage
    command = "gcovr -r .. --html-theme github.dark-blue --html-nested -o report/report.html"

    # Excluded stuff. Everything that is not part of the Shellminator library must be excluded.
    command += " --exclude .*/Unity/"
    command += " --exclude .*/test"
    command += " --exclude .*/Print"
    command += " --exclude .*/Stream"
    command += " --exclude .*/Commander"
    terminalProcess = subprocess.Popen( command, shell=True )
    terminalProcess.wait()

    if( terminalProcess.returncode !=0 ):
        print( "gcovr failed!" )
        print( "Probably you doesn't installed gcovr properly. You need to install gcovr with pip. You need version 6.0 of gcovr. Also it have to be added to PATH." )
        sys.exit( 4 )

    print( 'Coverage report data generated here: {:s}'.format( rootDirectory + "/" + buildDirectoryName + "/report/report.html" ) )

    # Copy report data to Doxygen source folder
    #reportFiles = os.listdir( rootDirectory + "/" + buildDirectoryName + "/report" )
    #for reportFile in reportFiles:
    #    shutil.copyfile( rootDirectory + "/" + buildDirectoryName + "/report/" + reportFile,  rootDirectory + "/docs/html/" + reportFile )



print()
print( 'Shellminator Build Tools Finished!' )
print()
