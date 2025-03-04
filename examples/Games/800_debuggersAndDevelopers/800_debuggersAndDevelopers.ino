
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


#define START_TEXT_0    "The world was once free. Code was open-source. Developers lived\r\n"       \
                        "in harmony, committing their changes and sharing knowledge\r\n"            \
                        "through pull requests.\r\n"                                                \
                        "\r\n"                                                                      \
                        "But then, Lord Obfuscator, the dark lord of proprietary code,\r\n"         \
                        "emerged. One day, he deployed a mysterious exploit and vanished\r\nthe "   

#define START_TEXT_1    "Prime Repository,\r\n"

#define START_TEXT_2    "the sacred source of all open knowledge. Without it,\r\n"                  \
                        "documentation crumbled, dependencies broke, and mysterious bugs\r\n"       \
                        "spread like ancient curses.\r\n"                                           \
                        "\r\n"                                                                      \
                        "You, a legendary software engineer, are the last hope. Your mission:\r\n"

#define START_TEXT_3    "Recover the Lost Repository and restore open-source freedom\r\n"           \
                        "before the world descends into chaos!\r\n"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

//-- The First Decision --
const char* firstDecisionListText =  "The logs suggest that the repository\nwas hidden in one of two locations:";

const char* firstDecisionListOptions[] = {
    "The Abandoned Data Center",
    "The Deep Stack"
};
 
const char* firstDecisionListDetails[] = {
    "A forgotten server farm, now haunted by\nzombie processes.",
    "A dark chasm in the digital realm,\nrumored to contain recursion\nand infinite loops."
};
 
//-- Abandoned Data Center --
const char* dataCenterListText =  "The servers are running ghost processes\nthat refuse to terminate. The only working prompt asks:\n\nWhat is the most efficient way to free up memory?";

const char* dataCenterListOptions[] = {
    "Use garbage collection",
    "Manually deallocate",
    "Sacrifice a junior developer"
};

//-- Deep Stack --
const char* deepStackListText =  "You're trapped in an infinite loop! \nYou must refactor the following code:\n\nwhile(true) {\n\tstd::cout << \"Help!\" << std::endl;\n}";

const char* deepStackListOptions[] = {
    "Add a break condition",
    "Increase recursion",
    "Sacrifice your GPU"
};

//-- Finale --
const char* finaleListText =  "You reach the Lost Repository, Lord Obfuscator is waiting!\nHe wields the License of Eternal Lockdown and\nthreatens to turn all code proprietary forever.";

const char* finaleListOptions[] = {
    "Deploy an open-source counterattack",
    "Offer him free stickers",
    "Use AI code to confuse him"
};

// Create a detailed list.
ShellminatorListDetailed questionListDetailed( firstDecisionListOptions, firstDecisionListDetails, 2, firstDecisionListText );

// Create a simple list.
ShellminatorList questionList;


void execution_function( char* command, Shellminator* caller );
void endCallback( Shellminator* parent );
void firstDecisionCallback(const char* optionsList[], int listSize, int selected, ShellminatorScreen*);
void challengeCallback(const char* optionsList[], int listSize, int selected, ShellminatorScreen*);
void finaleCallback(const char* optionsList[], int listSize, int selected, ShellminatorScreen*);

void printStartInstructions();
void animatingCrash();


// System init section.
void setup(){

    Serial.begin(115200);

    shell.clear();

    printStartInstructions();

    shell.attachExecFunc( execution_function );
    shell.begin( "slave" );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}

void execution_function( char* command, Shellminator* caller ){
    if( strcmp( command, "start" ) == 0 ){
        questionListDetailed.attachCallback( firstDecisionCallback );
        shell.beginScreen( &questionListDetailed );
    }
    else{
        caller -> channel -> print( __CONST_TXT__( "Unknown command: " ) );
        shell.format( &Serial, Shellminator::BOLD, Shellminator::BG_RED );
        caller -> channel -> print( command );
        shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
        caller -> channel -> println();
    }
}

void printStartInstructions(){
    Serial.print( __CONST_TXT__( START_TEXT_0 ) );

    shell.format( &Serial, Shellminator::BOLD, Shellminator::BG_RED );
    Serial.print( __CONST_TXT__( START_TEXT_1 ) );
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    Serial.print( __CONST_TXT__( START_TEXT_2 ) );

    shell.format( &Serial, Shellminator::BOLD, Shellminator::YELLOW );
    Serial.print( __CONST_TXT__( START_TEXT_3 ) );
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    Serial.println();
    Serial.print( __CONST_TXT__( "-- Journey starts with the " ) );
    shell.format( &Serial, Shellminator::BOLD, Shellminator::GREEN );
    Serial.print( __CONST_TXT__( "start" ) );
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
    Serial.println( __CONST_TXT__( " command --" ) );
    Serial.println();
}

void animatingCrash(){
    int i;
    shell.endScreen();
    shell.clear();
    Serial.println( __CONST_TXT__( "--- Critical System Failure ---" ) );
    delay( 1000 );
    Serial.println();
    Serial.print( __CONST_TXT__( "Collecting data for crash dump" ) );
    delay( 250 );
    Serial.print( '.' );
    delay( 250 );
    Serial.print( '.' );
    delay( 250 );
    Serial.println( '.' );
    delay( 1000 );
    Serial.println( __CONST_TXT__( "FAILED" ) );
    Serial.print( __CONST_TXT__( "Resolving runtime conflict" ) );
    delay( 250 );
    Serial.print( '.' );
    delay( 250 );
    Serial.print( '.' );
    delay( 250 );
    Serial.println( '.' );
    delay( 1000 );
    Serial.println( __CONST_TXT__( "FAILED" ) );
    Serial.print( __CONST_TXT__( "Checking memory integrity" ) );
    delay( 250 );
    Serial.print( '.' );
    delay( 250 );
    Serial.print( '.' );
    delay( 250 );
    Serial.println( '.' );
    delay( 1000 );
    Serial.println( __CONST_TXT__( "Memory corruption detected!" ) );
    Serial.println( __CONST_TXT__( "Invoking user space recov" ) );
    for( i = 'a'; i < '$'; i++ ){
        Serial.print( (char)i );
        delay( 25 );
    }
    shell.clear();
    delay( 2000 );
    shell.beep();
    delay( 1000 );
    Serial.println( __CONST_TXT__( "Core reset detected." ) );
    delay( 1000 );
    Serial.println( __CONST_TXT__( "Booting in legacy mode" ) );
    delay( 250 );
    Serial.print( '.' );
    delay( 250 );
    Serial.print( '.' );
    delay( 250 );
    Serial.println( '.' );
    delay( 1000 );
    printStartInstructions();
    shell.printBanner();
}

void firstDecisionCallback(const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen) {
    Shellminator* parent = screen -> getParent();
    if( parent == NULL ) {
        return;
    }

    switch( selected ){
        case 0:
            questionList = ShellminatorList( dataCenterListOptions, 3, dataCenterListText );
            questionList.attachCallback( challengeCallback );
            parent -> swapScreenAndClear( &questionList );
            break;
        case 1:
            questionList = ShellminatorList( deepStackListOptions, 3, deepStackListText );
            questionList.attachCallback( challengeCallback );
            parent -> swapScreenAndClear( &questionList );
            break;
    }

}

void challengeCallback(const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen) {
    Shellminator* parent = screen -> getParent();
    if( parent == NULL ) {
        return;
    }

    if( selected == 0 ){
        questionList = ShellminatorList( finaleListOptions, 3, finaleListText );
        questionList.attachCallback( finaleCallback );
        parent -> swapScreenAndClear( &questionList );
        return;
    }

    animatingCrash();
}

void finaleCallback(const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen){
    Shellminator* parent = screen -> getParent();
    if( parent == NULL ) {
        return;
    }

    if( selected == 0 ){
        parent -> endScreen();
        parent -> setBannerText( "sudo" );
        parent -> clear();
        parent -> printHelp();
        delay( 1000 );
        parent -> printBanner();
        return;
    }

    animatingCrash();
}