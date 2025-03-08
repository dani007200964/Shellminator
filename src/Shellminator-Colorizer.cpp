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