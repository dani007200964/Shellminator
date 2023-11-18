#include "Shellminator-Colorizer.hpp"

ShellminatorColorizer::ShellminatorColorizer(){
    reset();
}

void ShellminatorColorizer::reset(){
    int i;

    for( i = 0; i < ( SHELLMINATOR_BUFF_LEN + 1 ); i++ ){
        buffer[ i ].c = '\0';
        buffer[ i ].state = (uint8_t)DEFAULT_STATE;
    }

    bufferCntr = 0;
    currentState = DEFAULT_STATE;
}

void ShellminatorColorizer::printChar( Stream* response, char c ){

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

    buffer[ bufferCntr ].state = currentState;
    bufferCntr++;


}

void ShellminatorColorizer::defaultStateFuncion( Stream* response, char c ){

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

void ShellminatorColorizer::firstDashStateFuncion( Stream* response, char c ){
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

void ShellminatorColorizer::secondDashStateFuncion( Stream* response, char c ){
    if( isCharacter( c ) || isNumber( c ) ){
        response -> print( c );
        currentState = WAIT_TOKEN_END;
        return;
    }

    response -> print( "\033[1;37;41m" );
    response -> print( c );

    currentState = WAIT_TOKEN_END;

}

void ShellminatorColorizer::waitTokenEndStateFunction( Stream* response, char c ){
    if( ( c == ' ' ) || ( c == '\t' ) ){
        response -> print( "\033[0m" );
        response -> print( c );
        currentState = DEFAULT_STATE;
        return;
    }

    response -> print( c );

}

void ShellminatorColorizer::waitForWhitespaceStateFuncion( Stream* response, char c ){
    if( ( c == ' ' ) || ( c == '\t' ) ){
        response -> print( "\033[0m" );
        response -> print( c );
        currentState = DEFAULT_STATE;
        return;
    }

    response -> print( "\033[1;37;41m" );
    response -> print( c );

}

void ShellminatorColorizer::waitStringEndStateFunction( Stream* response, char c ){
    if( c == '\"' ){
        response -> print( c );
        response -> print( "\033[0m" );
        currentState = DEFAULT_STATE;
        return;
    }

    response -> print( c );

}

void ShellminatorColorizer::envVarStartStateFunction( Stream* response, char c ){
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

void ShellminatorColorizer::printBackwardError( Stream* response ){
    response -> print( "\033[1;37;41m" );
    response -> print( "\u219c" );
    response -> print( "\033[0m" );
}

bool ShellminatorColorizer::isCharacter( char c ){
    if( ( c >= 'a' ) && ( c <= 'z' ) ){
        return true;
    }
    if( ( c >= 'A' ) && ( c <= 'Z' ) ){
        return true;
    }
    return false;
}

bool ShellminatorColorizer::isNumber( char c ){
    if( ( c >= '0' ) && ( c <= '9' ) ){
        return true;
    }
    return false;
}


void ShellminatorColorizer::backspace(){

}
