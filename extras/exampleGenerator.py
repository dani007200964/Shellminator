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

arduinoExampleFolder = "/arduinoOutput"
desktopExampleFolder = "/desktopOutput"

# Clear the examples folders.
if os.path.isdir(  rootDirectory + arduinoExampleFolder ):
    shutil.rmtree( rootDirectory + arduinoExampleFolder )

if os.path.isdir(  rootDirectory + desktopExampleFolder ):
    shutil.rmtree( rootDirectory + desktopExampleFolder )

# Generate the example folders.
os.mkdir( rootDirectory + arduinoExampleFolder )
os.mkdir( rootDirectory + desktopExampleFolder )


# List all the template files.
templateFiles = os.listdir( rootDirectory + '/example_templates' )

# List all the example files.
exampleFiles  = os.listdir( rootDirectory + '/example_database' )

# This is the outer loop. We go through every template file.
for template in templateFiles:

    # Check for extension. Only .template files are processed.
    if template.endswith( '.template' ) == False:
        continue

    print( "Generating template: {:s}".format( template ) )

    # Read the next template file line-by-line.
    templateFile = open( rootDirectory + "/example_templates/" + template )
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
    if environmentInfo == 'Arduino':
        os.mkdir( rootDirectory + arduinoExampleFolder + "/" + boardInfo )

    if environmentInfo == 'Desktop':
        os.mkdir( rootDirectory + desktopExampleFolder + "/" + boardInfo )

    # Inner loop. We go through every example file.
    for example in exampleFiles:

        # Check for extension. Only .example files are processed.
        if example.endswith( '.example' ) == False:
            continue

        print( "\tGenerating example: {:s}".format( example ) )

        # Read the next example file line-by-line.
        exampleFile = open( rootDirectory + "/example_database/" + example )
        exampleData = exampleFile.readlines()

        # Store additional example parameters.
        excludeData = exampleData[ 0 ]

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
        exampleFolderName = example.split( '.' )[ 0 ].replace( '_', ' ' ).capitalize()

        # Generate folder for the example and write the output.
        if environmentInfo == 'Arduino':
            os.mkdir( rootDirectory + arduinoExampleFolder + "/" + boardInfo + "/" + exampleFolderName )
            outputFile = open( rootDirectory + arduinoExampleFolder + "/" + boardInfo + "/" + exampleFolderName + "/" + exampleFolderName + ".ino", "w" )
            outputFile.write( secondRunData )
            outputFile.close()

        if environmentInfo == 'Desktop':
            os.mkdir( rootDirectory + desktopExampleFolder + "/" + boardInfo + "/" + exampleFolderName )
            outputFile = open( rootDirectory + desktopExampleFolder + "/" + boardInfo + "/" + exampleFolderName + "/" + exampleFolderName + ".cpp", "w" )
            outputFile.write( secondRunData )
            outputFile.close()


        #print( secondRunData )

