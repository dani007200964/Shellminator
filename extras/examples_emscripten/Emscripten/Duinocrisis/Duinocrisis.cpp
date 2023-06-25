
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
 *
 * To test this example, you need a terminal emulator like PuTTY or Minicom.
 * This example shows a simple setup for Shellminator. It will create an
 * interactive interface, but it does not execute any command.
 * See Shellminator_execute example for further information.
*/


// std library.
#include <stdio.h>
#include <stdlib.h>

// Core System Functions.
#include "System.h"

// Contains a modified Stream class for communication.
#include "stdioStream.hpp"

// Contains Emscripten related functions.
#include <emscripten.h>

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"

// The number of obstacles. I think 5 is more than enough.
#define NUMBER_OF_OBSTACLES 5


// Use stdio as Channel.
stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

// Graphics pattern for the upper part of the floor.
const char terrainUp[] = "\\/ ~ ~ \\/ ~  \\/ ^  ";
uint8_t terrainUpSize= sizeof( terrainUp ) - 1;

// Graphics pattern for the lower part of the floor.
const char terrainDown[] = "^ \\/ \\/  ^ \\/ ~  ";
uint8_t terrainDownSize = sizeof( terrainDown ) - 1;

// Jump animation sequence. When the player jumps, these
// values will be added to the Y coordinate as offset.
uint8_t jumpSequence[] = { 0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 6, 5, 4, 3, 2, 1, 0};
uint8_t jumpSequenceSize = sizeof( jumpSequence );

// This is a simple counter, that tracks the state of the jumping sequence.
int jumpSequenceCntr = 0;

// This shows how many life left.
int playerLife = 3;

// Streak counter. When the player jumps successfully 5 times in a row, one life will be added.
int streak = 0;

// Graphics for the player.
// It has two elements for running animation.
const char* dinoRunningNormal[2] ={
    "            __\r\n"
    "           / _)\r\n"
    "    .-^^^-/ /\r\n"
    " __/       /\r\n"
    "<__.|_|-/ˇˇ"

    ,

    "            __\r\n"
    "           / _)\r\n"
    "    .-^^^-/ /\r\n"
    " __/       /\r\n"
    "<__./ˇˇ-|_|"
};

// Game over text.
const char* gameOverText{
    "+-------------+\r\n"
    "|             |\r\n"
    "|  GAME OVER  |\r\n"
    "|             |\r\n"
    "+-------------+\r\n"
    "Score: "
};

// Difficulty select menu elements.
char* difficultyList[ 3 ] = {
    (char*)"Easy",
    (char*)"Medium",
    (char*)"Hard"
};

// Selected difficulty.
int difficulty = 0;

// The difficulty increases runtime by distance to make the game challenging.
// This variable tracks the runtime difficulty.
int runtimeDifficulty;

// The obstacle generator will use this value pairs.
// 0-th element is the easiest
// 11-th element is the hardest.
uint8_t obstacleRandomRange[][ 2 ] = {
    { 45, 60 },
    { 40, 60 },
    { 40, 55 },
    { 40, 50 },
    { 35, 50 },
    { 30, 50 },
    { 25, 45 },
    { 20, 40 },
    { 15, 35 },
    { 15, 30 },
    { 15, 25 },
    { 15, 20 }
};

// Graphics for the rock obstacle. I know, I know, it is ugly.
const char* rock{
    "/%%%\\"
};

// The width and height of the terminal will be stored here.
int width;
int height;

// This variable tracks the distance from start. Basically it is the score.
int distance = 0;


// Infinite Loop.
void loop();

// Init Section.
void setup();

// This function will be called every time, when the up arrow key is pressed on the keyboard.
void upArrowFunc();

// This function will be called every time, when the Ctrl+C key is pressed on the keyboard.
// It not works on the PC version, because Ctrl+C will terminate the application.
void abortKeyFunc();

// This function will draw all the graphics elements.
void drawTerrain();

// This function will draw the score, life and difficulty menus.
void drawScore();

// This is a class for the obstacles.
class obstacle{

public:
    // Constructor. Basically it resets all the internal variables.
    obstacle();

    // Generate a random position and type.
    // It shall be used for the first element after init.
    void generateManual();

    // Generate a random position automatically.
    // The previous obstacle must be set to use this function correctly.
    void generate();

    // Draw the obstacle to the screen.
    void draw();

    // The obstacles are stored in a circular like structure.
    // The first obstacle will 'die' first, when it runs off the screen.
    // If it happens it will be generated after the last obstacle.
    // With this function you can set the order.
    void setPrevious( obstacle* prev );

    // Read the generated position.
    int getPosition();

private:

    // 0: bush
    // 1: rock
    // 2: grass
    uint8_t type;

    // The generated position will be stored here.
    int position;

    // Flag for collision event detection.
    bool collision;

    // Address of the previous obstacle in the buffer.
    obstacle* previous = NULL;

};

// An array of obstacles.
obstacle obstacles[ NUMBER_OF_OBSTACLES ];


// Main program.
int main(){

    // Call init section.
    setup();

    // Setup the infinite loop and start it.
    emscripten_set_main_loop( loop, 0, 1 );

    // If we are lucky, the code never reaches this.
    return 0;

}

void upArrowFunc(){
  
  // Start the jump sequence only, when the jumpSequenceCntr is in reset state.
  if( jumpSequenceCntr == 0 ){

    // Start the jump sequence.
    jumpSequenceCntr = 1;

  }

}

void abortKeyFunc(){

    // Quick and dirty way of killing the player. This way the game over menu will appear.
    playerLife = -10;

}

void drawTerrain(){

    // Generic counter variable.
    int i;

    // Hide the cursor every at rendering cycle.
    shell.hideCursor();

    // Set color to white for gound.
    Shellminator::setTerminalCharacterColor( &stdioChannel, Shellminator::REGULAR, Shellminator::WHITE );

    // Draw the top of the floor
    shell.setCursorPosition( 1, height - 2 );
    for( i = 0; i < width; i++ ){

        stdioChannel.print( '_' );

    }

    // Draw the top ground pattern.
    shell.setCursorPosition( 1, height - 1 );
    for( i = 0; i < width; i++ ){

        stdioChannel.print( terrainUp[ ( distance + i ) % terrainUpSize ] );

    }

    // Draw the bottom ground pattern.
    shell.setCursorPosition( 1, height );
    for( i = 0; i < width; i++ ){

        stdioChannel.print( terrainDown[ ( distance + i ) % terrainDownSize ] );

    }

    // Draw the character
    Shellminator::setTerminalCharacterColor( &stdioChannel, Shellminator::BOLD, Shellminator::GREEN );
    shell.setCursorPosition( 1, height - 6 - jumpSequence[ jumpSequenceCntr ] );
    stdioChannel.print( dinoRunningNormal[ ( distance % 2 ) == 0 ] );

        // Handle jumping state.
        if( jumpSequenceCntr > 0 ){

        jumpSequenceCntr++;

        // Detect jump sequence finish event.
        if( jumpSequenceCntr >= jumpSequenceSize ){
            jumpSequenceCntr = 0;
        }

    }

    // Print difficulty level.
    shell.setCursorPosition( 1, 1 );

    for( i = 0; i < 12; i++ ){

        if( i > runtimeDifficulty ){
            stdioChannel.print( "-" );
        }

        else{
            stdioChannel.print( "#" );
        }

    }

    stdioChannel.println();

    // Print score.
    stdioChannel.print( "Score: " );
    stdioChannel.println( distance );

    // Print life.
    Shellminator::setTerminalCharacterColor( &stdioChannel, Shellminator::BOLD, Shellminator::RED );
    stdioChannel.print( " \u2665 " );
    stdioChannel.println( playerLife );

    // Print obstacles.
    for( i = 0; i < NUMBER_OF_OBSTACLES; i++ ){

        obstacles[ i ].draw();

    }

    // Detect game over.
    if( playerLife <= 0 ){

        // Game Over
        drawScore();

        // Create a circular parrenting list.
        for( i = 1; i < NUMBER_OF_OBSTACLES; i++ ){

            obstacles[ i ].setPrevious( &obstacles[ i - 1 ] );

        }

        // The parrent of the first obstacle is the last obstacle, to make it circular.
        obstacles[ 0 ].setPrevious( &obstacles[ NUMBER_OF_OBSTACLES - 1 ] );

        // Generate the first obstacle manually.
        obstacles[ 0 ].generateManual();

        // Generate the rest with the regular method.
        for( i = 1; i < NUMBER_OF_OBSTACLES; i++ ){
            obstacles[ i ].generate();
        }

    }


}

obstacle::obstacle(){
}

void obstacle::generateManual(){

    collision = false;

	// In case of manual generation, whe new position will be generated 'off-screen'.
	// It would be ugly it it just appears on the middle of the screen.
    position = distance + random( 	width + difficultyList[ runtimeDifficulty ][ 0 ],
									width + difficultyList[ runtimeDifficulty ][ 1 ] );

	// Generate a random type between 0 and 2.
    type = random( 3 );

}

void obstacle::generate(){

	collision = false;

	// Position of the previous obstacle.
	int prevPosition;

	// Screen position of the previous obstacle.
	int prevScreenPos;

	// Check if the previous element is set.
	// We don't want to invoke a call fom NULL ptr.
	// Mama would be sad :(
	if( previous ){

		// Get the position of the previous obstacle.
		prevPosition = previous -> getPosition();

		// Calculate the screen position of the previous obstacle.
		prevScreenPos = prevPosition - distance ;

		// If the screen position is off screen, we have to generate the manual way.
		if( prevScreenPos < 1 ){
			generateManual();
		}

		// Other case we just generate according to the current difficulty.
		else{
			position = random( 	prevPosition + difficultyList[ runtimeDifficulty ][ 0 ],
								prevPosition + difficultyList[ runtimeDifficulty ][ 1 ] );
		}

	}

	// Generate a random type between 0 and 2.
	type = random( 3 );

}

int obstacle::getPosition(){

	// Return the current position.
  	return position;

}

void obstacle::draw(){

	// Generic counter.
	int i;

	// Screen position of the obstacle.
	int screenPos;

	// If the drawing still in negative state, this flag will be true.
	// If negative flag is false, that means we can draw because we are on screen.
	bool negative;
	
	// If this flag is true, regeneration is needed.
	bool regenerate = true;

	// Calculate screen position.
	screenPos = position - distance ;
	
	// Set negative flag to true by default.
	negative = true;

	// Every obstacle is 5 characters long.
	for( i = screenPos; i < ( screenPos + 5 ); i++ ){

		// Detect if the current character is off screen.
		if( i < 1 ){

			// This case we can not draw it.
			continue;
		}

		// Detect negative to positive state transition event.
		if( negative ){

			// This case set the cursor position and clear the negative flag.
			shell.setCursorPosition( i, height - 2 );
			negative = false;

		}

		// Draw if we are on screen.
		if( i <= width ){

			// grass
			if( type == 2 ){
				stdioChannel.print( '|' );
			}

			// rock
			else if( type == 1 ){
				stdioChannel.print( rock[ i - screenPos ] );
			}

			// Bush
			else{
				stdioChannel.print( '^' );
			}

		}

		// Reset the regenerate flag.
		regenerate = false;

	}

	// Detect if the player is hit the obstacle.
	if( ( screenPos < 12 ) && ( screenPos > 2 ) && ( jumpSequence[ jumpSequenceCntr ] < 1 ) ){

		// Player hit event detection.
		if( collision == false ){

			// This case we have to decrease the life of the player, and reset the streak counter.
			playerLife--;
			streak = 0;

			// Set the flags.
			collision = true;
			regenerate = true;
		}
	}

	// If regenerate flag is set, we have to generate a new position for the obstacle.
	if( regenerate ){

		generate();
		streak++;

		// Detect succesful streak.
		if( streak >= 5 ){

			// Increase players life and reset the streak counter.
			playerLife++;
			streak = 0;

		}
	}

}

void obstacle::setPrevious( obstacle* prev ){
  	previous = prev;
}

void drawScore(){

	// Print game over text.
	shell.setCursorPosition( 1, 1 );
	Shellminator::setTerminalCharacterColor( &stdioChannel, Shellminator::BOLD, Shellminator::RED );
	stdioChannel.print( gameOverText );

	// Print score.
	Shellminator::setTerminalCharacterColor( &stdioChannel, Shellminator::BOLD, Shellminator::GREEN );
	stdioChannel.println( distance );

	// Create an option to change difficulty.
	Shellminator::setTerminalCharacterColor( &stdioChannel, Shellminator::BOLD, Shellminator::WHITE );
	stdioChannel.println( "Press 'D' to change difficulty," );
	stdioChannel.println( "or any other key to restart..." );
	if( Shellminator::waitForKey( &stdioChannel, (char*)"dD", 0 ) ){

	// Create a select menu for difficulty change.
		difficulty = Shellminator::selectList( &stdioChannel, (char*)"Please select difficulty:", 3, difficultyList, 0 );

	}

	// Reset the game variables.
	distance = 0;
	playerLife = 3 + difficulty;
	streak = 0;
	randomSeed( millis() );

}


void setup(){

    // Init code.

    // Generic counter variable.
    int i;

    // Logo. Putting a lot of constants into an AVR is tricky.
    // I think all other platforms are storing const data in the FLASH,
    // but to make it happen with AVRs it is tricky. This is a quick and dirty
    // macro trick to save some RAM.
    #ifdef __AVR__
    __FlashStringHelper * logo = F(
    #else
    const char* logo =
    #endif
        "    _____        _                _____      _     _     \r\n"
        "   |  __ \\      (_)              / ____|    (_)   (_)    \r\n"
        "   | |  | |_   _ _ _ __   ___   | |     _ __ _ ___ _ ___ \r\n"
        "   | |  | | | | | | '_ \\ / _ \\  | |    | '__| / __| / __|\r\n"
        "   | |__| | |_| | | | | | (_) | | |____| |  | \\__ \\ \\__ \\\r\n"
        "   |_____/ \\__,_|_|_| |_|\\___/   \\_____|_|  |_|___/_|___/\r\n"
    #ifdef __AVR__
    );
    #else
    ;
    #endif

    // Logo Text.
    #ifdef __AVR__
    __FlashStringHelper * gameText = F(
    #else
    const char* gameText =
    #endif
        "Created by: Daniel Hajnal 2023\r\n"
        "\r\n"
        "This is a small game about a dinousaur, called Tibi.\r\nTibi is running fom a huge meteorite.\r\n"
    	"Help him to run as far as possible.\r\n"
        "This game is made to demonstrate an advanced use of Shellminator library.\r\n"
        "\r\n"
        "Controls:\r\n"
        "  Jump    - Up Arrow\r\n"
        "  Restart - Ctrl + C\r\n"  
        "If you avoid 5 obstacles in a row, yo will be rewarded with an extra life.\r\n"
    #ifdef __AVR__
    );
    #else
    ;
    #endif

    // Clear the terminal
    shell.clear();

    // Try to allocate memory for 100 characters.
    if( !shell.enableBuffering( 50 ) ){

        // If it fails, print the problem.
        // The error is handled internally, and it will still work, but without buffering.
        stdioChannel.println( "Can not allocate memory for buffering!" );

    }

    // Override the default behaviour of the abort and the up arrow key.
    shell.overrideUpArrow( upArrowFunc );
    shell.overrideAbortKey( abortKeyFunc );

    // Read the size of the terminal window in characters.
    shell.getTerminalSize( &width, &height );

    // Set cursor to top left
    shell.setCursorPosition( 1, 1 );

    // Print logo.
    Shellminator::setTerminalCharacterColor( &stdioChannel, Shellminator::BOLD, Shellminator::GREEN );
    stdioChannel.println( logo );

    // Print logo text.
    Shellminator::setTerminalCharacterColor( &stdioChannel, Shellminator::BOLD, Shellminator::WHITE );
    stdioChannel.println( gameText );

    // Generate a select menu for difficulty selection.
    Shellminator::setTerminalCharacterColor( &stdioChannel, Shellminator::BOLD, Shellminator::YELLOW );
    difficulty = Shellminator::selectList( &stdioChannel, (char*)"Please select difficulty:", 3, difficultyList, 0 );

    // Create some randomness based on the elapsed time.
    randomSeed( millis() );

    // Calculate the runtime difficulty.
    runtimeDifficulty = difficulty * 4;

    // Calculate player life based on difficulty.
    playerLife = 3 + difficulty;

    // Create a circular parrenting list.
    for( i = 1; i < NUMBER_OF_OBSTACLES; i++ ){

        obstacles[ i ].setPrevious( &obstacles[ i - 1 ] );

    }

    // The parrent of the first obstacle is the last obstacle, to make it circular.
    obstacles[ 0 ].setPrevious( &obstacles[ NUMBER_OF_OBSTACLES - 1 ] );

    // Generate the first obstacle manually.
    obstacles[ 0 ].generateManual();

    // Generate the rest with the regular method.
    for( i = 1; i < NUMBER_OF_OBSTACLES; i++ ){
        obstacles[ i ].generate();
    }



}

void loop(){

    // Infinite loop.

        // Increase the distance every iteration.
        distance++;

        // Update the difficulty.
        runtimeDifficulty = difficulty * 4 + distance / 250;

        // Overflow protection.
        if( runtimeDifficulty > 11 ){
            runtimeDifficulty = 11;
        }

        // Update the terminal interface to parse key events.
        shell.update();

        // Clear the terminal interface. It is necessary, to 'hide' terminal messages.
        shell.clear();

        // Draw the graphics.
        drawTerrain();

        // Wait 75ms. If it is faster, it will flicker. It is around 13 FPS.
        // It runs better, than some games from 2022...
        delay( 75 );



}