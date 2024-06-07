/*
 * Created on May 14 2023
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.04.16
*/

/*
MIT License

Copyright (c) 2023 Daniel Hajnal

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
*/

#include "Shellminator.hpp"

void Shellminator::overrideUpArrow( void( *func )( Shellminator* ) ){
    upArrowOverrideFunc = func;
}

void Shellminator::overrideDownArrow( void( *func )( Shellminator* ) ){
    downArrowOverrideFunc = func;
}

void Shellminator::overrideLeftArrow( void( *func )( Shellminator* ) ){
    leftArrowOverrideFunc = func;
}

void Shellminator::overrideRightArrow( void( *func )( Shellminator* ) ){
    rightArrowOverrideFunc = func;
}

void Shellminator::overrideAbortKey( void( *func )( Shellminator* ) ){
    abortKeyFunc = func;
}

void Shellminator::overridePageUpKey( void( *func )( Shellminator* ) ){
    pageUpKeyFunc = func;
}

void Shellminator::overridePageDownKey( void( *func )( Shellminator* ) ){
    pageDownKeyFunc = func;
}

void Shellminator::overrideHomeKey( void( *func )( Shellminator* ) ){
    homeKeyFunc = func;
}

void Shellminator::overrideEndKey( void( *func )( Shellminator* ) ){
    endKeyFunc = func;
}

void Shellminator::overrideLogoutKey( void( *func )( Shellminator* ) ){
    logoutKeyFunc = func;
}

void Shellminator::overrideSearchKey( void( *func )( Shellminator* ) ){
    searchKeyFunc = func;
}

void Shellminator::freeUpArrow(){
    upArrowOverrideFunc = NULL;
}

void Shellminator::freeDownArrow(){
    downArrowOverrideFunc = NULL;
}

void Shellminator::freeLeftArrow(){
    leftArrowOverrideFunc = NULL;
}

void Shellminator::freeRightArrow(){
    rightArrowOverrideFunc = NULL;
}

void Shellminator::freeAbortKey(){
    abortKeyFunc = NULL;
}

void Shellminator::freePageUpKey(){
    pageUpKeyFunc = NULL;
}

void Shellminator::freePageDownKey(){
    pageDownKeyFunc = NULL;
}

void Shellminator::freeHomeKey(){
    homeKeyFunc = NULL;
}

void Shellminator::freeEndKey(){
    endKeyFunc = NULL;
}

void Shellminator::freeLogoutKey(){
    logoutKeyFunc = NULL;
}

void Shellminator::freeSearchKey(){
    searchKeyFunc = NULL;
}

void Shellminator::ShellminatorUpArrowKeyState(){

    // Because we have finished the escape sequence interpretation we reset the state-machine.
    currentState = &Shellminator::ShellminatorDefaultState;

    if( screen != NULL ){
        pushEvent( ( shellEvent_t ){ SHELL_EVENT_CODED_KEY, EVENT_CODE_UP_ARROW } );
        return;
    }

    // Check if the arrow function is overriden.
    if( upArrowOverrideFunc ){
        upArrowOverrideFunc( this );
        return;
    }

    // We have to check that we can go upper in history
    if ( cmd_buff_dim < ( SHELLMINATOR_BUFF_DIM ) ) {

        // If we can, we have to check that the previous command was not empty.
        if ( cmd_buff[ cmd_buff_dim ][0] == '\0' ) {
            // If it was empty we can't do much with an empty command so we return.
            return;
        }

        // Now we have to copy the characters form the histoy to the 0th element in the buffer.
        // Remember the 0th element is always reserved for the new data. If we browse the history the
        // data in the history will overwrite the data in the 0th element so the historical data will be
        // the new data. We use strncpy to prevent overflow.
        strncpy( cmd_buff[ 0 ], cmd_buff[ cmd_buff_dim ], SHELLMINATOR_BUFF_LEN + 1 );

        // We have to calculate the historical data length to pass it to the cmd_buff_cntr variable.
        // It is important to track the end of the loaded string.
        cmd_buff_cntr = strlen( cmd_buff[ 0 ] );
        cursor = cmd_buff_cntr;

        // We print the loaded command to the terminal interface.
        //channel -> print( cmd_buff[ 0 ] );

        redrawLine();

        // We have to increment the cmd_buff_dim variable, to track the history position.
        // Greater number means older command!
        cmd_buff_dim++;

    }

}

void Shellminator::ShellminatorDownArrowKeyState(){

    // Because we have finished the escape sequence interpretation we reset the state-machine.
    currentState = &Shellminator::ShellminatorDefaultState;

    // Check if the arrow function is overriden.
    if( downArrowOverrideFunc ){
        downArrowOverrideFunc( this );
        return;
    }

    if( screen != NULL ){
        pushEvent( ( shellEvent_t ){ SHELL_EVENT_CODED_KEY, EVENT_CODE_DOWN_ARROW } );
        return;
    }

    // We have to check that we can go lover in history, and we are not in the first previous command.
    if ( cmd_buff_dim > 2 ) {

        // We have to decrement the cmd_buff_dim variable, to track the history position.
        // Lower number means newer command!
        cmd_buff_dim--;

        // Now we have to copy the characters form the histoy to the 0th element in the buffer.
        // Remember the 0th element is always reserved for the new data. If we browse the history the
        // data in the history will overwrite the data in the 0th element so the historical data will be
        // the new data. We use strncpy to prevent overflow.
        strncpy( cmd_buff[ 0 ], cmd_buff[ cmd_buff_dim - 1  ], SHELLMINATOR_BUFF_LEN + 1 );

        // We have to calculate the historical data length to pass it to the cmd_buff_cntr variable.
        // It is important to track the end of the loaded string.
        cmd_buff_cntr = strlen( cmd_buff[ 0 ] );
        cursor = cmd_buff_cntr;

        // We print the loaded command to the terminal interface.
        //channel -> print( cmd_buff[ 0 ] );
        redrawLine();

    }

    // Check that if we are in the first previous command.
    else if ( cmd_buff_dim == 2 ) {

        // To empty the incoming string we have to zero it's counter.
        cmd_buff_cntr = 0;
        cursor = 0;

        // We have to reset the cmd_buff_dim variable to the default value.
        cmd_buff_dim = 1;

        redrawLine();

    }

    // We have finished so we can break from the switch.

}

void Shellminator::ShellminatorLeftArrowKeyState(){

    // We just simply reset the state-machine.
    currentState = &Shellminator::ShellminatorDefaultState;

    // Check if the arrow function is overriden.
    if( leftArrowOverrideFunc ){
        leftArrowOverrideFunc( this );
        return;
    }

    // Detect if the channel was configured incorrectly.
    // In this case, we have to stop command execution.
    if( channel == NULL ){
        return;
    }

    // Check if we can move to left.
    if( cursor > 0 ){

        channel -> write( 27 );   // ESC character( decimal 27 )
        channel -> print( '[' );  // VT100 Cursor command.
        channel -> print( '1' );  // 1 character movement.
        channel -> print( 'D' );  // Left.

        // Decrement the cursor variable.
        cursor--;

    }

}

void Shellminator::ShellminatorRightArrowKeyState(){

    // We just simply reset the state-machine.
    currentState = &Shellminator::ShellminatorDefaultState;

    // Check if the arrow function is overriden.
    if( rightArrowOverrideFunc ){

        rightArrowOverrideFunc( this );
        return;

    }

    // Detect if the channel was configured incorrectly.
    // In this case, we have to stop command execution.
    if( channel == NULL ){
        return;
    }

    // Check if we can move to right.
    if( cursor < cmd_buff_cntr ){

        channel -> write( 27 );   // ESC character( decimal 27 )
        channel -> print( '[' );  // VT100 Cursor command.
        channel -> print( '1' );  // 1 character movement.
        channel -> print( 'C' );  // Left.

        // Increment the cursor variavble.
        cursor++;

    }

}

void Shellminator::ShellminatorHomeKeyState(){

    currentState = &Shellminator::ShellminatorDefaultState;

    if( homeKeyFunc ){

        homeKeyFunc( this );
        return;

    }

    // send the cursor to the begining of the buffer
    cursor = 0;
    redrawLine();

}

void Shellminator::ShellminatorHomeKeyState( char new_char ){

    if( new_char == '~' ){

        ShellminatorHomeKeyState();

    }

    else{

        currentState = &Shellminator::ShellminatorDefaultState;

    }

}

void Shellminator::ShellminatorEndKeyState(){

    currentState = &Shellminator::ShellminatorDefaultState;

    if( endKeyFunc ){
        endKeyFunc( this );
        return;
    }

    // send the cursor to the end of the buffer
    cursor = cmd_buff_cntr;
    redrawLine();

}

void Shellminator::ShellminatorEndKeyState( char new_char ){

    if( new_char == '~' ){
        ShellminatorEndKeyState();
    }

    else{
        currentState = &Shellminator::ShellminatorDefaultState;
    }

}

void Shellminator::ShellminatorDelKeyState(){

    // Del key detected.
    // If we press a delet key we have to reset cmd_buff_dim to default value
    cmd_buff_dim = 1;

    // General counter variable
    uint32_t i;

    currentState = &Shellminator::ShellminatorDefaultState;

    // We have to check the number of the characters in the buffer.
    // If the buffer is full we must not do anything!
    if ( cursor != cmd_buff_cntr ) {

        for( i = cursor; i < ( cmd_buff_cntr - 1 ); i++ ){
            cmd_buff[ 0 ][ i ] = cmd_buff[ 0 ][ i + 1 ];
        }

        // If there is at least 1 character in the buffer we jus simply
        // decrement the cmd_buff_cntr. This will result that the new character
        // will be stored in the previous characters place in the buffer.
        cmd_buff_cntr--;

        redrawLine();

    }

}

void Shellminator::ShellminatorDelKeyState( char new_char ){

    if( new_char == '~' ){
        ShellminatorDelKeyState();
    }

    else{
        currentState = &Shellminator::ShellminatorDefaultState;
    }

}

void Shellminator::ShellminatorPageUpKeyState(){

    currentState = &Shellminator::ShellminatorDefaultState;

    if( pageUpKeyFunc ){
        pageUpKeyFunc( this );
        return;
    }

    historySearchBackward();

}

void Shellminator::ShellminatorPageUpKeyState( char new_char ){

    if( new_char == '~' ){
        ShellminatorPageUpKeyState();
    }

    else{
        currentState = &Shellminator::ShellminatorDefaultState;
    }

}

void Shellminator::ShellminatorPageDownKeyState(){

    currentState = &Shellminator::ShellminatorDefaultState;

    if( pageDownKeyFunc ){
        pageDownKeyFunc( this );
        return;
    }

    historySearchForward();

}

void Shellminator::ShellminatorPageDownKeyState( char new_char ){

    if( new_char == '~' ){
        ShellminatorPageDownKeyState();
    }

    else{
        currentState = &Shellminator::ShellminatorDefaultState;
    }

}