
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/


#include "Shellminator.hpp"
#include "Shellminator-Screen.hpp"
#include "GUI/Shellminator-ScreenGrid.hpp"
#include "GUI/Shellminator-List.hpp"
#include "GUI/Shellminator-List-Detailed.hpp"


#define FPS         100

#define DINOSAUR_A  "               __\r\n"     \
                    "              / _)\r\n"    \
                    "     _.----._/ /\r\n"      \
                    "    /         /\r\n"       \
                    " __/ (  | (  |\r\n"        \
                    "/__.-'|_|--ˇˇ"

#define DINOSAUR_B  "               __\r\n"     \
                    "              / _)\r\n"    \
                    "     _.----._/ /\r\n"      \
                    "    /         /\r\n"       \
                    " __/ (  | (  |\r\n"        \
                    "/__.-' ˇˇ--|_|"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Create a buffer with 100 elements
uint8_t printBuffer[300];

// Calculate buffer size
int printBufferSize = sizeof(printBuffer);


class Cactus{
public:
    Cactus();
    Cactus( int position_p );
    bool draw( Stream* channel, int score, int width, int height );
    int getPosition();
    void generate();
    static int lastCactusPos;

private:
    const char* cactusRender[ 3 ] = {
        "  |  ",
        "(_|_)",
        "  |  "
    };

    int position = 0;;

};

int Cactus::lastCactusPos = 0;

class DinoCrysis : public ShellminatorScreen{

public:

    DinoCrysis();
    void init( Shellminator* parent_p, Stream* channel_p ) override;
    void draw( bool noClear = false ) override;
    void update( int width_p, int  height_p ) override;

private:
    char* ground[2] = {
        (char*)"~~vvv~~~~<>~~~~/------\\~-^~~",
        (char*)"' ' '  \" ' '' '  '' ' '' '' "
    };

    void shiftGround();

    uint32_t timerStart = 0;
    uint32_t updateRate = FPS;
    int score = 0;
    int highScore = 0;
    int health;
    int jump;
    bool cleared;

    int groundLength;

    Cactus cactus[ 5 ];

};

void execution_function( char* command, Shellminator* caller );

DinoCrysis game;


// System init section.
void setup(){

    Serial.begin(115200);

    // put your setup code here, to run once:
    shell.clear();

    // Attach the execution function to the shell object.
    shell.attachExecFunc( execution_function );

    // Enable buffering
    shell.enableBuffering(printBuffer, printBufferSize);

    Serial.println();
    Serial.println( "Dino Crysis" );
    Serial.println();
    Serial.print( __CONST_TXT__( "-- Game starts with the " ) );
    shell.format( &Serial, Shellminator::BOLD, Shellminator::GREEN );
    Serial.print( __CONST_TXT__( "start" ) );
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
    Serial.println( __CONST_TXT__( " command --" ) );
    Serial.println();

    shell.begin( "dino" );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}

void execution_function( char* command, Shellminator* caller ){
    if( strcmp( command, "start" ) == 0 ){
        shell.beginScreen( &game, FPS );
    }
    else{
        caller -> channel -> print( __CONST_TXT__( "Unknown command: " ) );
        shell.format( &Serial, Shellminator::BOLD, Shellminator::BG_RED );
        caller -> channel -> print( command );
        shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
        caller -> channel -> println();
    }
}

DinoCrysis::DinoCrysis(){
    width = 1;
    height = 1;
}

void DinoCrysis::init( Shellminator* parent_p, Stream* channel_p ){
    parent = parent_p;
    channel = channel_p;
    groundLength = strlen( ground[ 0 ] );
    cactus[ 0 ] = Cactus( 50 );
    cactus[ 1 ] = Cactus( 100 );
    cactus[ 2 ] = Cactus( 150 );
    cactus[ 3 ] = Cactus( 200 );
    cactus[ 4 ] = Cactus( 250 );
    timerStart = 0;
    updateRate = FPS;
    score = 0;
    health = 5;
    jump = 0;
    cleared = false;
}

void DinoCrysis::draw( bool noClear ){
    int i;
    int tmp;
    bool cactusState;
    int lastCactus;

    if( parent == NULL ){
        return;
    }

    if( channel == NULL){
        return;
    }

    if( !cleared ){
        parent -> clear();
        cleared = true;
    }

    score += 1;

    Shellminator::setCursorPosition( channel, 1, 2 );

    channel -> print( __CONST_TXT__( "Score:  " ) );
    channel -> println( score );

    channel -> print( __CONST_TXT__( "Health: " ) );
    for( i = 0; i < health; i++ ){
        channel -> print( "\u2665" );
    }
    for( i = 0; i < ( 5 - health ); i++ ){
        channel -> print( ' ' );
    }

    Shellminator::setCursorPosition( channel, 1, height - 1 );

    // Render top layer of ground
    tmp = width / groundLength;
    for( i = 0; i < tmp; i++ ){
        channel -> print( ground[ 0 ] );
    }

    tmp = width - ( tmp * groundLength );
    for( i = 0; i < tmp; i++ ){
        channel -> print( ground[ 0 ][ i ] );
    }

    // Render bottom layer of ground
    tmp = width / groundLength;
    for( i = 0; i < tmp; i++ ){
        channel -> print( ground[ 1 ] );
    }

    tmp = width - ( tmp * groundLength );
    for( i = 0; i < tmp; i++ ){
        channel -> print( ground[ 1 ][ i ] );
    }

    for( i = 0; i < 16; i++){
        Shellminator::setCursorPosition( channel, 1, height - 2 - i );
        channel -> print( "\033[0K" );
    }

    if( jump > 10 ){
        Shellminator::setCursorPosition( channel, 1, height - 7 - ( 20 - jump ) );
    }
    else{
        Shellminator::setCursorPosition( channel, 1, height - 7 - ( jump ) );
    }

    jump--;
    if( jump < 0 ){
        jump = 0;
    }

    if( ( score % 2 ) == 0 ){
        channel -> print( __CONST_TXT__( DINOSAUR_A ) );
    }
    else{
        channel -> print( __CONST_TXT__( DINOSAUR_B ) );
    }

    lastCactus = 0;
    for( i = 0; i < 5; i++ ){
        tmp = cactus[ i ].getPosition();
        if( tmp > lastCactus ){
            lastCactus = tmp;
        }
    }

    Cactus::lastCactusPos = lastCactus;

    for( i = 0; i < 5; i++ ){
        cactusState = false;
        cactusState = cactus[ i ].draw( channel, score, width, height );
        if( cactusState ){
            if( ( jump > 17 ) || ( jump < 3 ) ){
                health -= 1;
                //parent -> beep();
                cactus[ i ].generate();
                if( health < 0 ){
                    if( score > highScore ){
                        highScore = score;
                    }
                    parent -> endScreen();
                    parent -> clear();
                    parent -> channel -> print( __CONST_TXT__( "High Score:    " ) );
                    parent -> channel -> println( highScore );
                    parent -> channel -> print( __CONST_TXT__( "Current Score: " ) );
                    parent -> channel -> println( score );
                    parent -> channel -> println();
                    parent -> printBanner();
                    return;
                }
            }
        }
    }


    shiftGround();

}

void DinoCrysis::update( int width_p, int  height_p ){
    Shellminator::shellEvent_t newEvent;

    if( parent == NULL ){
        return;
    }

    width = width_p;
    height = height_p;

    newEvent = parent -> readEvent();

    if( ( millis() - timerStart ) > updateRate ){
        timerStart = millis();
        parent -> requestRedraw();
    }

    // In case of empty event, we can't do anything.
    if( newEvent.type == Shellminator::SHELL_EVENT_EMPTY ){
        return;
    }

    if( newEvent.type == Shellminator::SHELL_EVENT_CODED_KEY ){

        if( newEvent.eventCode == Shellminator::EVENT_CODE_UP_ARROW ){
            if( jump == 0 ){
                jump = 20;
            }
        }
    }

    if( newEvent.type == Shellminator::SHELL_EVENT_KEY ){
        if( newEvent.data == (uint8_t)' ' ){
            if( jump == 0 ){
                jump = 20;
            }
        }
    }

}

void DinoCrysis::shiftGround(){
    int i;
    char tmp_0 = ground[ 0 ][ 0 ];
    char tmp_1 = ground[ 1 ][ 0 ];
    for( i = 1; i < groundLength; i++ ){
        ground[ 0 ][ i - 1 ] = ground[ 0 ][ i ];
        ground[ 1 ][ i - 1 ] = ground[ 1 ][ i ];
    }
     ground[ 0 ][ groundLength - 1 ] = tmp_0;
     ground[ 1 ][ groundLength - 1 ] = tmp_1;
}

Cactus::Cactus( int position_p ){
    position = position_p;
}

Cactus::Cactus(){
    position = 0;
}

void Cactus::generate(){
    position = lastCactusPos + random( 50, 70 );
}

bool Cactus::draw( Stream* channel, int score, int width, int height ){
    int i;
    int j;
    int startPos;

    startPos = position - score;

    if( startPos <= 4 ){
        generate();
    }

    for( i = 0; i < 3; i++ ){
        Shellminator::setCursorPosition( channel, startPos, height - 2 - i );
        for( j = 0; j < 5; j++ ){
            if( width - startPos < j ){
                break;
            }
            channel -> print( cactusRender[ i ][ j ] );
        }
    }

    if( startPos <= 15 ){
        return true;
    }
    return false;
}

int Cactus::getPosition(){
    return position;
}