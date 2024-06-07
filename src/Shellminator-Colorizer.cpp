#include "Shellminator-Colorizer.hpp"

DefaultColorizer::DefaultColorizer(){

}


bool DefaultColorizer::isCharacter( char c ){
    if( ( c >= 'a' ) && ( c <= 'z' ) ){
        return true;
    }
    if( ( c >= 'A' ) && ( c <= 'Z' ) ){
        return true;
    }
    return false;
}

bool DefaultColorizer::isNumber( char c ){
    if( ( c >= '0' ) && ( c <= '9' ) ){
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
