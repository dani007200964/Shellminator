/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2024.06.10
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

#include "Shellminator-Commander-Interface.hpp"

//---- Commander-API support specific part ----//
#ifdef COMMANDER_API_VERSION

ShellminatorCaller::ShellminatorCaller(){

}

void ShellminatorCaller::setShell( Shellminator* shell_p ){
    shell = shell_p;
}

void ShellminatorCaller::beep(){
    if( shell != NULL ){
        shell -> beep();
    }
}

void ShellminatorCaller::setBannerText( const char* text_p ){
    if( shell != NULL ){
        shell -> setBannerText( text_p );
    }
}

void ShellminatorCaller::setPathText( const char* text_p ){
    if( shell != NULL ){
        shell -> setBannerPathText( text_p );
    }
}

void ShellminatorCaller::clear(){
    if( shell != NULL ){
        shell -> clear();
    }
}


void ShellminatorCommanderInterface::printCommandParserHelp( Stream* channel_p, bool formatting_p ){
    if( commander != NULL ){
        commander -> printHelp( channel_p, formatting_p );
    }
}

void ShellminatorCommanderInterface::attachCommander( Commander* commander_p ){
    commander = commander_p;
}

void ShellminatorCommanderInterface::checkCommandFraction(){
    // By default it is empty, because no command parser is available.
}

void ShellminatorCommanderInterface::executeWithCommandParser(){
    if( commander != NULL ){
        caller.setChannel( channel );
        enableFormatting ? commander -> enableFormatting() : commander -> disableFormatting();
        commander -> execute( cmd_buff[ 0 ], caller.getChannel(), &caller );
    }
}

void ShellminatorCommanderInterface::attachAutoCompleteBuffer( char* buffer_p, int buffer_size_p ){
    autoCompleteBuffer = buffer_p;
    autoCompleteBufferSize = buffer_size_p;
}


void ShellminatorCommanderInterface::autoCompleteWithCommandParser(){

    int num_of_hints;
    int i;
    const char* hint_text;
    int hint_text_size;

    // By default it is empty, because no command parser is available.
    if( commander == NULL ){
        return;
    }

    if( autoCompleteBuffer == NULL ){
        return;
    }

    if( autoCompleteBufferSize < 10 ){
        return;
    }

    if( cmd_buff_cntr >= SHELLMINATOR_BUFF_LEN ){
        return;
    }

    cmd_buff[ 0 ][ cmd_buff_cntr ] = '\0';
    num_of_hints = commander -> generateHint( cmd_buff[ 0 ], autoCompleteBuffer, autoCompleteBufferSize );


    if( num_of_hints < 1 ){
        return;
    }

    hint_text = commander -> getHint( 0, true );
    hint_text_size = strlen( hint_text );

    for( i = 0; i < hint_text_size; i++ ){
        cmd_buff[ 0 ][ cmd_buff_cntr ] = hint_text[ i ];
        cmd_buff_cntr++;
        cursor++;
        if( cmd_buff_cntr >= SHELLMINATOR_BUFF_LEN ){
            break;
        }
    }

    redrawLine();

}

bool ShellminatorCommanderInterface::hasCommandParser(){
    // By default it returns false, because no command parser is available.
    if( commander != NULL ){
        return true;
    }

    return false;
}





CommanderColorizer::CommanderColorizer(){
    reset( NULL );
}

void CommanderColorizer::reset( Stream* response ){
    int i;

    for( i = 0; i < ( SHELLMINATOR_BUFF_LEN + 1 ); i++ ){
        stateBuffer[ i ] = DEFAULT_STATE;
    }

    bufferCntr = 0;
    currentState = DEFAULT_STATE;

    if( response ){
        response -> print( "\033[0m" );
    }
}

void CommanderColorizer::printChar( Stream* response, char c ){

    if( bufferCntr > SHELLMINATOR_BUFF_LEN ){
        return; 
    }

    switch( currentState ){

        case DEFAULT_STATE:
            defaultStateFuncion( response, c );
            break;

        case FIRST_DASH_STATE:
            firstDashStateFuncion( response, c );
            break;

        case WAIT_FOR_WHITESPACE:
            waitForWhitespaceStateFuncion( response, c );
            break;

        case SECOND_DASH_STATE:
            secondDashStateFuncion( response, c );
            break;

        case WAIT_TOKEN_END:
            waitTokenEndStateFunction( response, c );
            break;

        case WAIT_STRING_END:
            waitStringEndStateFunction( response, c );
            break;
        
        case ENV_VAR_START:
            envVarStartStateFunction( response, c );
            break;


    }

    bufferCntr++;
    stateBuffer[ bufferCntr ] = currentState;

}

void CommanderColorizer::defaultStateFuncion( Stream* response, char c ){

    /*
    if( ( ( c >= 'A' ) && ( c <= 'Z' ) ) || ( ( c >= 'a' ) && ( c <= 'z' ) ) || ( ( c >= '0' ) && ( c <= '9' ) ) ){
        response -> print( c );
    }
    */

    if( ( ( c == ' ' ) || ( c == '\t' ) ) && bufferCntr == 0 ){
        response -> print( "\033[1;37;41m" );
        response -> print( "\u00b7" );
        response -> print( "\033[0m" );
        return;
    }

    if( c == '-' ){
        response -> print( "\033[1;35m" );
        response -> print( c );
        currentState = FIRST_DASH_STATE;
        return;
    }

    if( c == '\"' ){
        response -> print( "\033[1;36m" );
        response -> print( c );
        currentState = WAIT_STRING_END;
        return;
    }

    if( c == '$' ){
        response -> print( "\033[1;33m" );
        response -> print( c );
        currentState = ENV_VAR_START;
        return;
    }

    response -> print( c );

}

void CommanderColorizer::firstDashStateFuncion( Stream* response, char c ){
    if( c == '-' ){
        response -> print( c );
        currentState = SECOND_DASH_STATE;
        return;
    }

    if( isCharacter( c ) || isNumber( c ) ){
        response -> print( c );
        currentState = WAIT_FOR_WHITESPACE;
        return;
    }

    response -> print( "\033[1;37;41m" );
    response -> print( c );
    response -> print( "\033[0m" );
    currentState = DEFAULT_STATE;

}

void CommanderColorizer::secondDashStateFuncion( Stream* response, char c ){
    if( isCharacter( c ) || isNumber( c ) ){
        response -> print( c );
        currentState = WAIT_TOKEN_END;
        return;
    }

    response -> print( "\033[1;37;41m" );
    response -> print( c );

    currentState = WAIT_TOKEN_END;

}

void CommanderColorizer::waitTokenEndStateFunction( Stream* response, char c ){
    if( ( c == ' ' ) || ( c == '\t' ) ){
        response -> print( "\033[0m" );
        response -> print( c );
        currentState = DEFAULT_STATE;
        return;
    }

    response -> print( c );

}

void CommanderColorizer::waitForWhitespaceStateFuncion( Stream* response, char c ){
    if( ( c == ' ' ) || ( c == '\t' ) ){
        response -> print( "\033[0m" );
        response -> print( c );
        currentState = DEFAULT_STATE;
        return;
    }

    response -> print( "\033[1;37;41m" );
    response -> print( c );

}

void CommanderColorizer::waitStringEndStateFunction( Stream* response, char c ){
    if( c == '\"' ){
        response -> print( c );
        response -> print( "\033[0m" );
        currentState = DEFAULT_STATE;
        return;
    }

    response -> print( c );

}

void CommanderColorizer::envVarStartStateFunction( Stream* response, char c ){
    if( isCharacter( c ) || isNumber( c ) ){
        response -> print( c );
        currentState = WAIT_TOKEN_END;
        return;
    }

    if( ( c == ' ' ) || ( c == '\t' ) ){
        response -> print( "\033[1;37;41m" );
        response -> print( "\u00b7" );
        response -> print( "\033[0m" );
        currentState = DEFAULT_STATE;
        return;
    }

    response -> print( "\033[1;37;41m" );
    response -> print( c );
    currentState = WAIT_TOKEN_END;
}

void CommanderColorizer::printBackwardError( Stream* response ){
    response -> print( "\033[1;37;41m" );
    response -> print( "\u219c" );
    response -> print( "\033[0m" );
}

#endif