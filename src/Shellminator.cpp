/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.05.08
*/

/*
MIT License

Copyright (c) 2020 Daniel Hajnal

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

const char *Shellminator::version = SHELLMINATOR_VERSION;

Shellminator::Shellminator( Stream *stream_p ){

    // Save channel to internal variable.
    channel = stream_p;

    // It has to be zero. We dont want to process any garbage.
    cmd_buff_cntr = 0;

    // This has to be 1 minimum, because the 0th element is used for the incoming data.
    // The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
    cmd_buff_dim = 1;

    // Just in case terminate the begining of the buffer
    cmd_buff[ 0 ][ 0 ] = '\0';

    // Because we did not specified the execution function, we have to make it a NULL
    // pointer to make it detectable.
    execution_fn = NULL;

    // Reset all the event pointers.
    eventBufferWritePtr = 0;
    eventBufferReadPtr = 0;


}

bool Shellminator::enableBuffering( uint8_t* buffer, int bufferSize ){

    // Check if the buffer or its size is defined incorrectly.
    // In this case stop execution.
    if( buffer == NULL ){
        return false;
    }

    if( bufferSize < 1 ){
        return false;
    }

    // Initialise the buffered printer object.
    bufferedPrinter = ShellminatorBufferedPrinter( channel, buffer, bufferSize );

    // Save the corresponding flag.    
    bufferMemoryAllocated = true;
    return bufferMemoryAllocated;

}

ShellminatorBufferedPrinter* Shellminator::getBufferedPrinter(){
    
    // Check if buffering is disabled. In this case we have
    // to return with NULL. This indicates that the buffering
    // is not enabled.
    if( bufferMemoryAllocated == false ){
        return NULL;
    }

    // If buffering is enabled, return with the pointer
    // of the buffered printer.
    return &bufferedPrinter;

}

void Shellminator::setBannerText( const char* banner_p ){

    // Copy the content from banner_p to banner. Because strncpy we can be sure that it wont overflow.
    strncpy( banner, banner_p, SHELLMINATOR_BANNER_LEN );

    // Just in case terminate the string
    banner[ SHELLMINATOR_BANNER_LEN - 1 ] = '\0';

    redrawLine();

}

void Shellminator::setBannerPathText( const char* bannerPath_p ){

    // Copy the content from bannerPath_p to bannerPath. Because strncpy we can be sure that it wont overflow.
    strncpy( bannerPath, bannerPath_p, SHELLMINATOR_BANNER_PATH_LEN );

    // Just in case close the string
    bannerPath[ SHELLMINATOR_BANNER_PATH_LEN - 1 ] = '\0';

}

#ifdef __AVR__
void Shellminator::attachLogo( __FlashStringHelper * progmemLogo_p ){

    // Save the PROGMEM based logo address to internal variable.
    progmemLogo = progmemLogo_p;

}
#endif

void Shellminator::attachLogo( const char* logo_p ){

    // Save the logo address to internal variable.
    logo = (char*)logo_p;

}

void Shellminator::attachExecFunc( void( *execution_fn_p )( char*, Shellminator* ) ){

    // Save the function pointer to internal variable.
    execution_fn = execution_fn_p;

}

void Shellminator::printBanner(){

    // Reset this counter for the beginning.
    lastBannerSize = 0;

    // Detect if the channel was configured incorrectly.
    // In this case, we have to stop command execution.
    if( channel == NULL ){
        return;
    }

    // This will be redirected to the channel by default.
    Stream* selectedChannel = channel;

    // If buffering is enabled, overwrite the
    // selectedChannel to the buffered printer.
    if( bufferMemoryAllocated ){
        selectedChannel = &bufferedPrinter;
    }

    // Sets the terminal style to bold and the color to green.
    format( selectedChannel, BOLD, GREEN );

    // Print the banner text and save it's size.
    lastBannerSize += selectedChannel -> print( banner );

    // Sets the terminal style to regular and the color to white.
    format( selectedChannel, WHITE );

    // Print the separator.
    lastBannerSize += selectedChannel -> print( ':' );

    // Set color to blue.
    format( selectedChannel, BLUE );

    // Print banner path text.
    lastBannerSize += selectedChannel -> print( bannerPath );

    // Set formatting to default.
    format( selectedChannel, REGULAR, WHITE );

    // Print one space at the end to separate the banner from the user text.
    lastBannerSize += selectedChannel -> print( ' ' );

    // Check if buffering is enabled.
    // If so, we have to flush the buffer to the output stream.
    if( bufferMemoryAllocated ){
        selectedChannel -> flush();
    }

}

void Shellminator::printHistory(){

    // Generic counter.
    uint32_t i;

    // The index of the first valid command
    // in the history.
    uint32_t firstBuffDim;

    // This variable will hold the actual
    // history element order.
    // 1 is the oldest.
    uint32_t index;

    // The first history element usually located in the
    // 1-st index.
    firstBuffDim = 1;

    // Detect if the channel was configured incorrectly.
    // In this case, we have to stop command execution.
    if( channel == NULL ){
        return;
    }

    // This will be redirected to the channel by default.
    Stream* selectedChannel = channel;

    // If buffering is enabled, overwrite the
    // selectedChannel to the buffered printer.
    if( bufferMemoryAllocated ){
        selectedChannel = &bufferedPrinter;
    }

    // Check the first valid command in the buffer
    for( i = 1; i < SHELLMINATOR_BUFF_DIM; i++ ){

        // If it's found, store it's index to firstBuffDim
        if( cmd_buff[ i ][ 0 ] == '\0' ){
            firstBuffDim = i - 1;
            break;
        }

    }

    // If the history is empty, we can return.
    if( firstBuffDim == 0 ){
        return;
    }

    // If the history is full, we have to protect
    // firstBuffDim variable.
    if( i >= SHELLMINATOR_BUFF_DIM ){
        firstBuffDim = SHELLMINATOR_BUFF_DIM - 1;
    }

    // Print the history.
    for( i = firstBuffDim; i > 0; i-- ){

        index = firstBuffDim - i + 1;

        selectedChannel -> print( ' ' );
        selectedChannel -> print( ' ' );

        // It is used to ident digits.
        if( index < 10 ){

            selectedChannel -> print( ' ' );
            selectedChannel -> print( ' ' );

        }

        // It is used to ident digits.
        else if( index < 100 ){

            selectedChannel -> print( ' ' );

        }

        // Print the index and the command.
        format( selectedChannel, BOLD, MAGENTA );
        selectedChannel -> print( index );
        format( selectedChannel, REGULAR, WHITE );
        selectedChannel -> print( ' ' );
        selectedChannel -> print( ' ' );
        selectedChannel -> println( cmd_buff[ i ] );

    }

    // Check if buffering is enabled.
    // If so, we have to flush the buffer to the output stream.
    if( bufferMemoryAllocated ){
        selectedChannel -> flush();
    }

}

void Shellminator::printCommandParserHelp( Stream* channel_p, bool formatting_p ){
    // By default it is empty, because no command parser is available.
}

void Shellminator::checkCommandFraction(){
    // By default it is empty, because no command parser is available.
}

void Shellminator::executeWithCommandParser(){
    // By default it is empty, because no command parser is available.
}

void Shellminator::autoCompleteWithCommandParser(){
    // By default it is empty, because no command parser is available.    
}

bool Shellminator::hasCommandParser(){
    // By default it returns false, because no command parser is available.
    return false;
}

void Shellminator::printHelp(){

    // Detect if the channel was configured incorrectly.
    // In this case, we have to stop command execution.
    if( channel == NULL ){
        return;
    }

    // This will be redirected to the channel by default.
    Stream* selectedChannel = channel;

    // If buffering is enabled, overwrite the
    // selectedChannel to the buffered printer.
    if( bufferMemoryAllocated ){
        selectedChannel = &bufferedPrinter;
    }

    // Header section
    selectedChannel -> println();
    format( selectedChannel, BOLD, RED );
    selectedChannel -> print( __CONST_TXT__( "----" ) );
    format( selectedChannel, WHITE );
    selectedChannel -> print( __CONST_TXT__( " Shortcut Keys " ) );
    format( selectedChannel, RED );
    selectedChannel -> print( __CONST_TXT__( "----" ) );
    format( selectedChannel, YELLOW );
    selectedChannel -> println();

    // Ctrl-A / Home
    selectedChannel -> print( __CONST_TXT__( "Ctrl-A / Home " ) );
    format( selectedChannel, GREEN );
    selectedChannel -> print( __CONST_TXT__( "\u2196  " ) );
    format( selectedChannel, WHITE );
    selectedChannel -> println( __CONST_TXT__( "Jumps the cursor to the beginning of the line." ) );
    format( selectedChannel, YELLOW );

    // Ctrl-E / End
    selectedChannel -> print( __CONST_TXT__( "Ctrl-E / End  " ) );
    format( selectedChannel, GREEN );
    selectedChannel -> print( __CONST_TXT__( "\u2198  " ) );
    format( selectedChannel, WHITE );
    selectedChannel -> println( __CONST_TXT__( "Jumps the cursor to the end of the line." ) );
    format( selectedChannel, YELLOW );

    // Ctrl-D
    selectedChannel -> print( __CONST_TXT__( "Ctrl-D        " ) );
    format( selectedChannel, GREEN );
    selectedChannel -> print( __CONST_TXT__( "\u233D  " ) );
    format( selectedChannel, WHITE );
    selectedChannel -> println( __CONST_TXT__( "Log out." ) );
    format( selectedChannel, YELLOW );

    // Ctrl-R
    selectedChannel -> print( __CONST_TXT__( "Ctrl-R        " ) );
    format( selectedChannel, GREEN );
    selectedChannel -> print( __CONST_TXT__( "\u26D5  " ) );
    format( selectedChannel, WHITE );
    selectedChannel -> println( __CONST_TXT__( "Reverse-i-search." ) );
    format( selectedChannel, YELLOW );

    // Page Up
    selectedChannel -> print( __CONST_TXT__( "Page Up       " ) );
    format( selectedChannel, GREEN );
    selectedChannel -> print( __CONST_TXT__( "\u21DE  " ) );
    format( selectedChannel, WHITE );
    selectedChannel -> println( __CONST_TXT__( "History search backwards and auto completion." ) );
    format( selectedChannel, YELLOW );

    // Page Down
    selectedChannel -> print( __CONST_TXT__( "Page Down     " ) );
    format( selectedChannel, GREEN );
    selectedChannel -> print( __CONST_TXT__( "\u21DF  " ) );
    format( selectedChannel, WHITE );
    selectedChannel -> println( __CONST_TXT__( "History search forward and auto completion." ) );
    format( selectedChannel, YELLOW );

    // Arrow Up
    selectedChannel -> print( __CONST_TXT__( "Arrow Up      " ) );
    format( selectedChannel, GREEN );
    selectedChannel -> print( __CONST_TXT__( "\u2191  " ) );
    format( selectedChannel, WHITE );
    selectedChannel -> println( __CONST_TXT__( "Browse history backward." ) );
    format( selectedChannel, YELLOW );

    // Arrow Down
    selectedChannel -> print( __CONST_TXT__( "Arrow Down    " ) );
    format( selectedChannel, GREEN );
    selectedChannel -> print( __CONST_TXT__( "\u2193  " ) );
    format( selectedChannel, WHITE );
    selectedChannel -> println( __CONST_TXT__( "Browse history forward." ) );
    format( selectedChannel, YELLOW );

    // Return / Enter
    selectedChannel -> print( __CONST_TXT__( "Return        " ) );
    format( selectedChannel, GREEN );
    selectedChannel -> print( __CONST_TXT__( "\u21A9  " ) );
    format( selectedChannel, WHITE );
    selectedChannel -> println( __CONST_TXT__( "Execute command, or exit from screen." ) );

    printCommandParserHelp( selectedChannel, enableFormatting );

    format( selectedChannel, REGULAR, WHITE );

    // Check if buffering is enabled.
    // If so, we have to flush the buffer to the output stream.
    if( bufferMemoryAllocated ){
        selectedChannel -> flush();
    }

}

void Shellminator::begin( const char* banner_p ) {

    // If channel is not specified, we have to stop here.
    if( channel == NULL ){
        return;
    }

    // Copy the content from banner_p to banner. Because strncpy we can be sure that it wont overflow.
    strncpy( banner, banner_p, SHELLMINATOR_BANNER_LEN );

    // Just in case close the string
    banner[ SHELLMINATOR_BANNER_LEN - 1 ] = '\0';

    // Set the terminal color and style to the defined settings for the logo
    format( channel, SHELLMINATOR_LOGO_FONT_STYLE, SHELLMINATOR_LOGO_COLOR );

    // Draw the startup logo.
    drawLogo();

    // By default set the login status to true.
    // It means password is not required.
    loggedIn = true;

    // Check if the password hash buffer is not NULL.
    // It means, that a password hash is attached and
    // login is required to use the terminal.
    if( passwordHash != NULL ){

        // Clear the loggedIn flag to force the user
        // to enter the correct password.
        loggedIn = false;

        // Create a special input prompt for internal use.
        // Because it is used internally, we did not specify
        // an output buffer and did nos specify a callback.
        // The ShellminatorEnterKeyState function will identify
        // this prompt by the loggedIn flag.
        input( NULL, SHELLMINATOR_BUFF_LEN, "Password:", NULL, true );

    }

    else{

        // If we have an empty string as banner, it probably means that we need
        // an initialised shell, but we still configuring it. It thats the case,
        // we skip the banner printing here.
        if( banner[ 0 ] == '\0' ){
            return;
        }

        // Print the banner message.
        printBanner();

    }

}

void Shellminator::sendBackspace() {

    // If channel is not specified, we have to stop here.
    if( channel == NULL ){
        return;
    }

    // Send a simple backspace combo to the serial port
    channel -> print( __CONST_TXT__( "\b \b" ) );

}

void Shellminator::redrawLine(){

    // -- Note --
    //
    // Even if formatting is disabled, this function requires VT100 commands.
    // Please not use it if you dont't want any formatting.

    // Generic counter.
    uint32_t i;

    // General counter variable
    int j = -1;

    // Detect if the channel was configured incorrectly.
    // In this case, we have to stop command execution.
    if( channel == NULL ){
        return;
    }

    // This will be redirected to the channel by default.
    Stream* selectedChannel= channel;

    // If buffering is enabled, overwrite the
    // selectedChannel to the buffered printer.
    if( bufferMemoryAllocated ){
        selectedChannel = &bufferedPrinter;
    }

    colorizer -> reset( selectedChannel );

    // If reverse search mode is active,
    // we have to call redrawHistorySearch
    // function and than stop there.
    if( inSearch ){

        redrawHistorySearch();

        // Check if buffering is enabled.
        // If so, we have to flush the buffer to the output stream.
        if( bufferMemoryAllocated ){
            selectedChannel -> flush();
        }

        return;

    }

    // Check if it is out of boundaries and
    // limit the variable to a maximum allowed value
    // to prevent buffer overflow.
    if( cmd_buff_cntr > SHELLMINATOR_BUFF_LEN ){

        cmd_buff_cntr = SHELLMINATOR_BUFF_LEN;

    }

    // Terminate the command at the cmd_buff_cntr
    // to not print out the previous command's data.
    cmd_buff[ 0 ][ cmd_buff_cntr ] = '\0';

    // Return to the beginning of the line and print the banner
    // then the command buffer will be printed (with colors)
    selectedChannel -> print( '\r' );

    // We have to print the banner only, when no input
    // prompt is active.
    if( !inputActive ){
        printBanner();
    }

    // If an input prompt is active, we have to set the cursor
    // to the end of the input prompts instruction text.
    else{
        selectedChannel -> write( 27 );    // ESC character( decimal 27 )
        selectedChannel -> print( '[' );  // VT100 Cursor command.
        selectedChannel -> print( inputInstuctionSize );  // Step cursor
        selectedChannel -> print( 'C' );  // Left.
    }

    // If an input is not active, we print the user text according to
    // the colorizers output.
    if( !inputActive ){
        colorizer -> reset( selectedChannel );
        for( i = 0; i < cmd_buff_cntr; i++ ){
            colorizer -> printChar( selectedChannel, cmd_buff[ 0 ][ i ] );
        }
    }

    // If we an active input, we just print the characters without a colorizer.
    // If the input is in secret mode, we replace the characters with a dot character.
    else{
        for( i = 0; i < cmd_buff_cntr; i++ ){
            if( inputSecretMode ){
                selectedChannel -> print( "\u2022" );
            }
            else{
                selectedChannel -> print( cmd_buff[ i ] );
            }
        }
    }

    // Clear the rest of the line to right.
    selectedChannel -> print( __CONST_TXT__( "\033[0m\033[0K" ) );

    // It is also a tricky part. We have to check if the cursor
    // is not at the end of the command. In this case, we have
    // to set back the cursor to its intended position.
    if( cmd_buff_cntr > cursor ){

        selectedChannel -> write( 27 );                                 // ESC character( decimal 27 )
        selectedChannel -> print( '[' );                                // VT100 Cursor command.
        selectedChannel -> print( uint8_t( cmd_buff_cntr - cursor ) );  // Step cursor
        selectedChannel -> print( 'D' );                                // Left.

    }

    // Restore the color and style for user text.
    format( selectedChannel, REGULAR, WHITE );

    // Check if buffering is enabled.
    // If so, we have to flush the buffer to the output stream.
    if( bufferMemoryAllocated ){
        selectedChannel -> flush();
    }

}

void Shellminator::process( char new_char ) {

    // This is the enty point of the main state machine.
    ((*this).*currentState)(new_char);

}

void Shellminator::update() {

    // This variable will hold the character that was read from the channel buffer.
    char c;

    // In case of screen drawing, the terminal width
    // will be queried directly to this variable. 
    int w;

    // In case of screen drawing, the terminal width
    // will be queried directly to this variable. 
    int h;

    // If channel is not specified, we have to stop here.
    if( channel == NULL ){
        return;
    }

    // We have to check the channel buffer. If it is not empty we can read as many characters as possible.
    while ( channel -> available() ) {

        // Read one character from channel Buffer.
        c = (char)channel -> read();

        // Process the new character.
        process( c );

        // Save the system time when the last keypress was executed.
        lastKeyPressTime = millis();

    }

    // Check if a Screen object is attached.
    if( screen != NULL ){

        // If a Screen is attached, we have to request the terminal
        // size periodically. The intervall for this is specidied
        // in the screenUpdatePeriod variable.
        if( ( millis() - sizeTimerStart ) > 2000 ){

            // Save system time to detect the next timing event.
            sizeTimerStart = millis();

            // Try to get the screen size.
            getTerminalSize( &w, &h );

            // Check if the width or the height has been changed since the
            // last iteration.
            if( ( terminalWidth != w ) || ( terminalHeight != h ) ){

                // If the terminal size has been changed, we have to signal the
                // attached Screen. Maybe the Screen object has to redraw itself
                // to adopt for the new screen size.
                pushEvent( ( shellEvent_t ){ SHELL_EVENT_RESIZE } );

                clear();

                // Call the update function of the screen with the new
                // dimensions to make it possible to adopt to the new
                // screen size.
                screen -> update( w, h );

                // Set the redraw flag to true.
                screenRedraw = true;

            }

            // Save the dimensions to internal variables.
            terminalWidth = w;
            terminalHeight = h;

            // Check if we have a redraw request.
            if( screenRedraw ){

            }

        }

        if( screenRedraw && ( ( millis() - screenTimerStart ) > screenUpdatePeriod ) ){
            // In this case we have to clear the flag first.
            // The order is important, because it is possible
            // that the Screen object will generate a redraw
            // request in the draw function.
            screenRedraw = false;

            // Call the Screen objects draw function.
            screen -> draw();

            // Check if buffering is enabled.
            // If so, we have to flush the buffer to the output stream.
            if( bufferMemoryAllocated ){
                bufferedPrinter.flush();
            }
        }

        // Call the Screen objects update function periodically.
        screen -> update( terminalWidth, terminalHeight );

        // Pop the current element from the event buffer.
        popEvent();

    }

    checkCommandFraction();

}

bool Shellminator::getCursorPosition( int* x, int* y, uint32_t timeout ){

    // Screen coordinate report looks like this:
    // ESC[yCoord;xCoordR
    // ESC[10;3R

    // It will hold the current character.
    char c;

    // Stores the current state of the state-machine.
    // Default state is 0.
    uint8_t charState = 0;

    // Buffer for the X-coordinate text.
    char xBuff[ 4 ];

    // This variable tracks the index of the
    // next free slot in the xBuff array.
    uint8_t xBuffCntr = 0;

    // Buffer for the Y-coordinate text.
    char yBuff[ 4 ];

    // This variable tracks the index of the
    // next free slot in the yBuff array.
    uint8_t yBuffCntr = 0;

    // To save system time for timeout detection.
    unsigned long timerStart;

    // If channel is not specified, we have to stop here.
    if( channel == NULL ){
        return false;
    }

    // Flush the input buffer.
    while( channel -> available() ){
        channel -> read();
    }

    // Send the request.
    channel -> print( __CONST_TXT__( "\033[6n" ) );

    // Save system time.
    timerStart = millis();

    // If no timeout event, or no timeout specified, check for new data.
    while( ( ( millis() - timerStart ) < timeout ) || timeout == 0 ){

        // Process all data if available.
        while( channel -> available() ){

            // Read character
            c = channel -> read();

            switch( charState ){

                // Inital state. We expect ESC character.
                case 0:

                    // If ESC character is received, go to the next state.
                    if( c == 27 ){
                        charState = 1;
                    }
                    
                    // Otherwise, something wrong.
                    else{
                        //channel -> println( "\r\nERROR: CASE 0\r\n" );
                        return false;
                    }

                    break;

                // Second character state. We expect '[' character.
                case 1:

                    // If '[' character is received, go to the next state.
                    if( c == '[' ){
                        charState = 2;
                    }
                    
                    // Otherwise, something wrong.
                    else{
                        //channel -> println( "\r\nERROR: CASE 1\r\n" );
                        return false;
                    }
                    
                    break;

                // We wait for ';' character. Until this character we put the data in
                // the yBuff;
                case 2:

                    // If ';' character is received, go to the next state.
                    if( c == ';' ){
                        charState = 3;
                        yBuff[ yBuffCntr ] = '\0';
                    }

                    else{

                        yBuff[ yBuffCntr ] = c;
                        yBuffCntr++;

                        // Protection against buffer overflow.
                        if( yBuffCntr >= 4 ){
                            //channel -> println( "\r\nERROR: CASE 2\r\n" );
                            return false;
                        }

                    }
                    
                    break;

                // We wait for 'R' character. Until this character we put the data in
                // the yBuff;
                case 3:

                    // If 'R' character is received, we finished with data collecting.
                    if( c == 'R' ){
                        xBuff[ xBuffCntr ] = '\0';

                        // We have to parse the numbers.
                        charState  = sscanf( (const char*)xBuff, "%d", x );
                        charState += sscanf( (const char*)yBuff, "%d", y );

                        // Check if all data is number.
                        if( charState == 2 ){
                            return true;
                        }

                        // Conversion went wrong.
                        else{
                        //channel -> println( "\r\nERROR: CASE 3 R\r\n" );
                            return false;
                        }

                    }

                    else{

                        xBuff[ xBuffCntr ] = c;
                        xBuffCntr++;

                        // Protection against buffer overflow.
                        if( xBuffCntr >= 4 ){
                            //channel -> println( "\r\nERROR: CASE 3\r\n" );
                            return false;
                        }

                    }
                    
                    break;

            }

        }

        // If emscripten is teh build target, a small sleep is necessary.
        // The xterm js frontend can do its job, while the 'c++' code sleeps.
        #ifdef EMSCRIPTEN
            emscripten_sleep( 20 );
        #endif

    }

    //channel -> println( "\r\nTIMEOUT!\r\n" );

    // We should not be here.
    return false;

}

void Shellminator::setCursorPosition( int x, int y ){

    setCursorPosition( channel, x, y );

}

void Shellminator::setCursorPosition( Stream* channel_p, int x, int y ){

    // If channel is not specified, we have to stop here.
    if( channel_p == NULL ){
        return;
    }

    // Boundary protection.
    if( x < 1 ) x = 1;
    if( y < 1 ) y = 1;

    channel_p -> print( __CONST_TXT__( "\033[" ) );
    channel_p -> print( y );
    channel_p -> print( ';' );
    channel_p -> print( x );
    channel_p -> print( 'H' );

}

bool Shellminator::getTerminalSize( int* width, int* height ){

    int currentX;
    int currentY;

    // Save the current position.
    // Later we can restore it.
    if( !getCursorPosition( &currentX, &currentY ) ){
        return false;
    }

    // This is a tricky solution.
    // Firstly, we send the cursor to a very large bottom right coordinate.
    // The terminal emulator will push the cursor as far as it can.
    // Than, we read the actual position. This will be equal with the terminal size.
    setCursorPosition( 999, 999 );

    if( !getCursorPosition( width, height ) ){
        return false;
    }

    // Restore the original position.
    setCursorPosition( currentX, currentY );

    // If we are here, that means everything went well.
    return true;

}

void Shellminator::drawLogo() {

    // If channel is not specified, we have to stop here.
    if( channel == NULL ){
        return;
    }

    if( logo ){

        // Set the terminal color and style to the defined settings for the logo
        format( channel, SHELLMINATOR_LOGO_FONT_STYLE, SHELLMINATOR_LOGO_COLOR );

        // Draws the startup logo to the terminal interface.
        channel -> print( logo );

        // Set the terminal style to normal.
        format( channel, REGULAR, WHITE );

    }

    #ifdef __AVR__

    else if( progmemLogo ){

        // Set the terminal color and style to the defined settings for the logo
        format( channel, SHELLMINATOR_LOGO_FONT_STYLE, SHELLMINATOR_LOGO_COLOR );

        // Draws the startup logo to the terminal interface.
        channel -> print( progmemLogo );

        // Set the terminal style to normal.
        format( channel, REGULAR, WHITE );

    }

    #endif

}

void Shellminator::beep(){

    // If channel is not specified, we have to stop here.
    if( channel == NULL ){
        return;
    }

    if( !mute ){

        // Bell character.
        channel -> print( '\a' );

    }

}

void Shellminator::ShellminatorDefaultState( char new_char ){

    shellEvent_t event;

    switch( new_char ){

        case '\0':
            break;

        case '\n':
            break;

        case '\b':
            ShellminatorBackspaceState();
            break;

        case 127:
            ShellminatorBackspaceState();
            break;

        case '\r':
            // Check is a screen object is drawing.
            if( screen != NULL ){

                // Push the return key event to the event buffer.
                pushEvent( ( shellEvent_t ){ SHELL_EVENT_CODED_KEY, EVENT_CODE_RETURN } );

                // Call the Screens update function.
                // This way it can react to the return event.
                screen -> update( terminalWidth, terminalHeight );

                // We have to pop the EVENT_CODE_RETURN event from the event buffer.
                popEvent();

                // If a screen swap occured, we doesn't end the screen.
                // Otherwise we stop rendering and return to regular terminal mode.
                if( readEvent().type != SHELL_EVENT_SCREEN_SWAP ){
                    // In this case we have to end the drawing process.
                    endScreen();
                    // And set the state machine back to default state.
                    currentState = &Shellminator::ShellminatorDefaultState;
                }

                break;
            }
            ShellminatorEnterKeyState();
            break;

        case 0x01:
            ShellminatorBeginningOfLineState();
            break;

        case 0x05:
            ShellminatorEndOfLineState();
            break;

        case 0x04:
            ShellminatorLogoutState();
            break;

        case 0x12:
            if( inputActive || ( screen != NULL ) ){
                break;
            }
            ShellminatorReverseSearchState();
            break;

        case 0x0C:
            if( inputActive || ( screen != NULL ) ){
                break;
            }
            ShellminatorClearScreenState();
            break;

        case '\t':
            if( inputActive || ( screen != NULL ) ){
                break;
            }
            ShellminatorAutoCompleteState();
            break;

        case 0x03:
            ShellminatorAbortState();
            break;

        case 27:
            currentState = &Shellminator::ShellminatorEscapeCharacterState;
            break;

        default:
            currentState = &Shellminator::ShellminatorDefaultState;
            if( screen != NULL ){
                event.type = SHELL_EVENT_KEY;
                event.data = (uint8_t)new_char;
                pushEvent( event );
                break;
            }
            ShellminatorProcessRegularCharacter( new_char );
            break;

    }

}

void Shellminator::ShellminatorBackspaceState(){

    // General counter variable
    uint32_t i;

    // If we press a backspace we have to reset cmd_buff_dim to default value
    cmd_buff_dim = 1;

    // We have to check the number of the characters in the buffer.
    // If the buffer is empty we must not do anything!
    if ( cursor > 0 ) {

        // decrease the cmd buffer counter and the cursor position
        cmd_buff_cntr--;
        cursor--;

        // if we are at the end of the command buffer
        if ( cursor == cmd_buff_cntr ) {

            if( inSearch || ( colorizer != &defaultColorizer ) ){

                cmd_buff[ 0 ][ cursor + 1 ] = '\0'; // and from the cmd buffer
                redrawLine();

            }

            else{

                channel -> print( __CONST_TXT__( "\b \b" ) ); // just delete the last character from the terminal
                cmd_buff[ 0 ][ cursor + 1 ] = '\0'; // and from the cmd buffer
                

            }

        }

        else {

            // if the cursor is somewhere in the middle of the cmd buffer
            // rework the buffer and redraw the whole line
            for( i = cursor; i < cmd_buff_cntr; i++ ) {

                cmd_buff[ 0 ][ i ] = cmd_buff[ 0 ][ i + 1 ];

            }

            redrawLine();

        }

    }

}

void Shellminator::ShellminatorEnterKeyState(){

    // If the enter key is pressed cmd_buff_dim has to be reset to the default value
    cmd_buff_dim = 1;

    // General counter variable
    int i;

    // If channel is not specified, we have to stop here.
    if( channel == NULL ){
        return;
    }

    colorizer -> reset( channel );

    if( inSearch  ){

        if( searchMatch > 0 ){

            inSearch = false;
            strncpy( cmd_buff[ 0 ], cmd_buff[ searchMatch ], SHELLMINATOR_BUFF_LEN + 1 );
            cmd_buff_cntr = strlen( cmd_buff[ 0 ] );
            redrawLine();

        }

        else{
        cmd_buff_cntr = 0;
        }

    }

    // Because a command is sent we have to close it. Basically we replace the arrived
    // '\r' character with a '\0' string terminator character. Now we have our command
    // in a C/C++ like standard string format.
    cmd_buff[ 0 ][ cmd_buff_cntr ] = '\0';

    // Check if we have an active input prompt.
    if( inputActive ){

        // Check if we are waiting for user password.
        if( !loggedIn ){

            // To empty the incoming string we have to zero it's counter.
            cmd_buff_cntr = 0;
            cursor = 0;

            // Check if the user typed the correct password.
            if( checkPassword( cmd_buff[ 0 ] ) ){
                loggedIn = true;
                inputActive = false;
                channel -> println();
                printBanner();
                return;
            }

            // Otherwise generate a beep sound and start over the
            // password section.
            beep();
            redrawLine();
            return;

        }

        // If we are waiting for regular text input.
        else{

            inputActive = false;

            // Copy the data from the input to the destination.
            strncpy( inputDestinationBuffer, cmd_buff[ 0 ], inputDestinationBufferSize );

            // Just in case terminate the string.
            inputDestinationBuffer[ inputDestinationBufferSize - 1 ] = '\0';

            // To empty the incoming string we have to zero it's counter.
            cmd_buff_cntr = 0;
            cursor = 0;

            if( inputCallback != NULL ){
                inputCallback( inputDestinationBuffer, inputDestinationBufferSize, this );
            }

            // If the callback created a new input, we need to return here.
            if( inputActive ){
                return;
            }

            // If we doesn't has any more input prompts, close the prompt,
            // and return to regular operation.
            channel -> println();
            printBanner();
            return;

        }

    }

    // We send a line break to the terminal to put the next data in new line
    channel -> println();

    inSearch = false;

    // If the arrived data is not just a single enter we have to process the command.
    if ( cmd_buff_cntr > 0 ) {

        // Flush out everything before command execution.
        // It is required for the prompts, because sometimes garbage
        // can be found in the channel stream. It has to be removed before
        // prompt reading.
        while( channel -> available() ){
            channel -> read();
        }

        if( ( strcmp( cmd_buff[ 0 ], "help" ) == 0 ) || ( strcmp( cmd_buff[ 0 ], "?" ) == 0 ) ){
            printHelp();
        }

        else if( strcmp( cmd_buff[ 0 ], "history" ) == 0 ){
            printHistory();
        }

        // We haveto check that execution_fn is not NULL.
        else if( execution_fn != NULL ){
            // If it is a valid, then call it's function.
            execution_fn( cmd_buff[ 0 ], this );
        }

        else if( hasCommandParser() ){
            executeWithCommandParser();
        }

        // If not, then just print it with Serial.
        else{
            channel -> print( __CONST_TXT__( "cmd: " ) );
            channel -> print( cmd_buff[ 0 ] );
            channel -> println();
        }

        // After we processed the command we have to shift the history upwards.
        // To protect the copy against buffer overflow we use strncpy
        for ( i = ( SHELLMINATOR_BUFF_DIM - 1 ); i > 0; i-- ) {
            strncpy( cmd_buff[ i ], cmd_buff[ i - 1 ], SHELLMINATOR_BUFF_LEN + 1 );
        }

    }

    // After the command processing finished we print a new banner to the terminal.
    // This means that the device is finished and waits the new command.
    printBanner();

    // To empty the incoming string we have to zero it's counter.
    cmd_buff_cntr = 0;
    cursor = 0;

}

void Shellminator::ShellminatorBeginningOfLineState(){
    cursor = 0;
    redrawLine();
}

void Shellminator::ShellminatorEndOfLineState(){
    cursor = cmd_buff_cntr;
    redrawLine();
}

void Shellminator::ShellminatorLogoutState(){

    if( !loggedIn ){
        return;
    }

    if( logoutKeyFunc ){
        logoutKeyFunc( this );
        return;
    }

    if( passwordHash != NULL ){
        loggedIn = false;
        input( NULL, SHELLMINATOR_BUFF_LEN, "Password:", NULL, true );
    }

}

void Shellminator::ShellminatorReverseSearchState(){

    if( !loggedIn ){
        return;
    }

    if( searchKeyFunc ){
        searchKeyFunc( this );
        return;
    }

    inSearch = !inSearch;
    redrawLine();

}

void Shellminator::ShellminatorClearScreenState(){

    clear();
    redrawLine();
}

void Shellminator::ShellminatorAutoCompleteState(){
    // Auto complete section.
    autoCompleteWithCommandParser();
}

void Shellminator::ShellminatorAbortState(){

    if( !loggedIn ){
        return;
    }

    if( abortKeyFunc ){
        abortKeyFunc( this );
    }


    inSearch = false;

    // If the abort key is pressed cmd_buff_dim has to be reset to the default value
    cmd_buff_dim = 1;

    // We send a line break to the terminal to put the next data in new line
    channel -> println();

    printBanner();

    cursor = 0;
    cmd_buff_cntr = 0;

}

void Shellminator::ShellminatorEscapeCharacterState( char new_char ){

    if( new_char == '[' ){
        currentState = &Shellminator::ShellminatorEscapeBracketState;
    }

    else{
        currentState = &Shellminator::ShellminatorDefaultState;
    }

}

void Shellminator::ShellminatorEscapeBracketState( char new_char ){

    switch( new_char ){

        case 'A':
            ShellminatorUpArrowKeyState();
            break;

        case 'B':
            ShellminatorDownArrowKeyState();
            break;

        case 'D':
            ShellminatorLeftArrowKeyState();
            break;

        case 'C':
            ShellminatorRightArrowKeyState();
            break;

        case 'H':
            ShellminatorHomeKeyState();
            break;

        case '1':
            currentState = &Shellminator::ShellminatorHomeKeyState;
            break;

        case 'F':
            ShellminatorEndKeyState();
            break;

        case '4':
            currentState = &Shellminator::ShellminatorEndKeyState;
            break;

        case '3':
            currentState = &Shellminator::ShellminatorDelKeyState;
            break;

        case '5':
            currentState = &Shellminator::ShellminatorPageUpKeyState;
            break;

        case '6':
            currentState = &Shellminator::ShellminatorPageDownKeyState;
            break;

        case '<':
            currentState = &Shellminator::ShellminatorMouseEventParserState;
            break;

        default:
            currentState = &Shellminator::ShellminatorDefaultState;
            break;

    }

}

void Shellminator::ShellminatorProcessRegularCharacter( char new_char ){

    // General counter variable
    uint32_t i;

    // If channel is not specified, we have to stop here.
    if( channel == NULL ){
        return;
    }

    // If the cursor is at the end of the command,
    // we simply store the new character.
    if( cursor == cmd_buff_cntr ){
        cmd_buff[ 0 ][ cmd_buff_cntr ] = new_char;
    }

    // If the cursor is somewhere in the middle, we have to shift the
    // end of the command by one character end insert the new character
    // to the cursor position.
    else{

        for( i = cmd_buff_cntr; i > cursor; i-- ){
            cmd_buff[ 0 ][ i ] = cmd_buff[ 0 ][ i - 1 ];
        }

        // Add the new character after the cursor in the buffer
        cmd_buff[ 0 ][ cursor ] = new_char;

    }

    // In this case we have to reset the cmd_buff_dim variable to the default value.
    cmd_buff_dim = 1;

    // If the cursor was at the end we have to print the
    // new character if the cmd_buff had free space at
    // the end.
    if ( cursor == cmd_buff_cntr ) {

        if ( cmd_buff_cntr < SHELLMINATOR_BUFF_LEN ) {

            if( inSearch || ( colorizer != &defaultColorizer ) ){

                // Increment counters.
                cmd_buff_cntr++;
                cursor++;

                redrawLine();

                // Decrement counters.
                cmd_buff_cntr--;
                cursor--;

            }

            else if( inputActive && inputSecretMode ){
                channel -> print( "\u2022" );
            }

            else{
                channel -> print( new_char );
                //colorizer -> printChar( channel, new_char );
            }

        }

    }

    // Increment counters.
    cmd_buff_cntr++;
    cursor++;

    if ( cursor != cmd_buff_cntr ) {
        // Redraw the command line.
        redrawLine();
    }

    // Check if the counters are overloaded.
    // the buffer storage is SHELLMINATOR_BUFF_LEN + 2,
    // so it is safe to make the counters equeal to SHELLMINATOR_BUFF_LEN
    if( cmd_buff_cntr > SHELLMINATOR_BUFF_LEN ) {
        cmd_buff_cntr = SHELLMINATOR_BUFF_LEN;
    }

    if( cursor > SHELLMINATOR_BUFF_LEN ) {
        cursor = SHELLMINATOR_BUFF_LEN;
    }

}

bool Shellminator::waitForKey( Stream* source, char key, uint32_t timeout ){

    // Save system time
    unsigned long timerStart = millis();

    while( source -> available() < 1 ){
        // Wait for key input.

        // Check if timeout is specified.
        if( timeout == 0 ){
            continue;
        }

        // Timeout detection.
        if( ( millis() - timerStart ) > timeout ){
            return false;
        }

    }

    // Read the pressed key
    char c = source -> read();

    // Compare with the expected key
    if( c == key ){
        return true;
    }

    return false;

}

bool Shellminator::waitForKey( Stream* source, char* keys, uint32_t timeout ){

    // Generic counter variable.
    uint32_t i;

    // Save system time.
    unsigned long timerStart = millis();

    while( source -> available() < 1 ){
        // Wait for key input.

        // Check if timeout is specified.
        if( timeout == 0 ){
            continue;
        }

        // Timeout detection.
        if( ( millis() - timerStart ) > timeout ){
            return false;
        }

    }

    // Read the pressed key
    char c = source -> read();

    // Check all characters in keys array.
    for( i = 0; i < strlen( keys ); i++ ){

        // If there is a match, return with true.
        if( keys[ i ] == c ){
            return true;
        }

    }

    // No match, return false;
    return false;

}

Shellminator* Shellminator::castVoidToShellminator( void* ptr ){
      return (Shellminator*)ptr;
}

void Shellminator::autoDetectTerminal(){


    // !!! Not working yet !!!
    char buff[ 20 ] = "";
    char magic[] = { 5, 0 };

    return;

    //Shellminator::input( channel, sizeof( buff ), buff, magic, 500 );

    if( strcmp( buff, "PuTTY" ) == 0 ){

    }

    else{

        enableFormatting = false;
        channel -> println( __CONST_TXT__( "unknown terminal..." ) );
        
    }

}

void Shellminator::beginScreen( ShellminatorScreen* screen_p, int updatePeriod ){

    // Generic counter.
    int i;

    // If the Screen object pointer is invalid, we have to stop here.
    if( screen_p == NULL ){
        return;
    }

    // If channel is not specified, we have to stop here.
    if( channel == NULL ){
        return;
    }

    // We need the terminal size for the initial run.
    getTerminalSize( &terminalWidth, &terminalHeight );
    /*
    else{
        // Basically, we clear the screen in a funky way.
        // Instead of remove the previous data, we shift it upwards
        // to make it avaiable after the Screen finished.
        for( i = 0; i < terminalHeight; i++ ){
            channel -> println();
        }
    }
    */
   clear();

    // Save Screen object pointer to internal variable.
    screen = screen_p;

    // Turn off cursor. It would look ugly when drawing.
    hideCursor();

    // Request mouse reports form host.
    mouseBegin();

    // Set the redraw flag to true, to force the redraw request.
    screenRedraw = true;

    // Save the redraw intervall to internal variable.
    screenUpdatePeriod = updatePeriod;

    // Save system time to the timers.
    screenTimerStart = millis();
    sizeTimerStart = millis();

    // Call the Screens init function.
    screen -> init( this, bufferMemoryAllocated ? &bufferedPrinter : channel );

    // Call the Screens update function.
    screen -> update( terminalWidth, terminalHeight );

}

void Shellminator::swapScreenAndClear( ShellminatorScreen* screen_p, int updatePeriod ){
    
    clear();
    swapScreen( screen_p, updatePeriod );
    
}

void Shellminator::swapScreen( ShellminatorScreen* screen_p, int updatePeriod ){

    // If the Screen object pointer is invalid, we have to stop here.
    if( screen_p == NULL ){
        return;
    }

    // If channel is not specified, we have to stop here.
    if( channel == NULL ){
        return;
    }

    // Save Screen object pointer to internal variable.
    screen = screen_p;

    // Set the redraw flag to true, to force the redraw request.
    screenRedraw = true;

    // Save the redraw intervall to internal variable.
    screenUpdatePeriod = updatePeriod;

    // Save system time to the timers.
    screenTimerStart = millis();
    sizeTimerStart = millis();

    // Call the Screens init function.
    screen -> init( this, bufferMemoryAllocated ? &bufferedPrinter : channel );

    // Call the Screens update function.
    screen -> update( terminalWidth, terminalHeight );

    pushEvent( ( shellEvent_t ){ SHELL_EVENT_SCREEN_SWAP, EVENT_CODE_EMPTY } );

}


void Shellminator::endScreen(){
    
    // These variables will hold the terminal size.
    int width;
    int height;

    // This variable will hold the vertical location of
    // the new banner.
    int bannerPos;

    // Stop mouse reports on host.
    mouseEnd();

    // Get the terminal site.
    getTerminalSize( &width, &height );

    // Get the bottom coordinate of the screen.
    bannerPos = screen -> down();

    // Set the cursor below the last drawn element.
    setCursorPosition( 1, bannerPos );

    // If the screen was as high as the height of the terminal
    // we have to print a new line to not overwrite the last
    // line of the screen.
    if( bannerPos > height ){
        channel -> println();
    }

    // Clear the pointer to the screen object.
    screen = NULL;

    // Set the cursor to the last line.
    // setCursorPosition( 1, height );

    // Enable the cursor again.
    showCursor();

    // Create a new line for the prompt.
    //channel -> println();

    // Print the banner.
    printBanner();

    // Set the cursor and the counter to zero.
    cursor = 0;
    cmd_buff_cntr = 0;
}

void Shellminator::requestRedraw(){
    // Set the redraw flag.
    screenRedraw = true;
}

void Shellminator::attachColorizer( DefaultColorizer *colorizer_p ){
    colorizer = colorizer_p;
}
