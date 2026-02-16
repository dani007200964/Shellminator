
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/


#include "Commander-API.hpp"
#include "Commander-Arguments.hpp"
#include "Shellminator.hpp"
#include "Shellminator-Commander-Interface.hpp"
#include "Shellminator-Screen.hpp"
#include "GUI/Shellminator-Progress.hpp"
#include "GUI/Shellminator-Buttons.hpp"
#include "GUI/Shellminator-List.hpp"
#include "GUI/Shellminator-List-Detailed.hpp"
#include "GUI/Shellminator-PlotModule.hpp"
#include "GUI/Shellminator-Notification.hpp"
#include "GUI/Shellminator-Level-Meter.hpp"
#include "Shellminator-Neofetch.hpp"
#include "math.h"

#ifdef ARDUINO
    #include "Wire.h"
#endif

// Simulation constants
#define R 5100.0      // Resistor (Ohm)
#define C 0.01    // Capacitance (Farad)
#define U0 5.0        // Supply Voltage (Volt)

#ifndef ARDUINO
    #ifndef INPUT
        #define INPUT 0
    #endif
    #ifndef INPUT_PULLUP
        #define INPUT_PULLUP 0
    #endif
    #ifndef OUTPUT
        #define OUTPUT 1
    #endif
    #ifndef MSBFIRST
        #define MSBFIRST 0
    #endif
    #ifndef LSBFIRST
        #define LSBFIRST 1
    #endif
    #ifndef LOW
        #define LOW 0
    #endif
    #ifndef HIGH
        #define HIGH 1
    #endif
    int digitalRead( int pin ){ return random( 0, 2 ); }
    void digitalWrite( int pin, int state ){}
    void analogWrite( int pin, int val ){}
    int analogRead( int pin ){ return random( 5,40 ); }
    void pinMode( int pin, int state ){}
    void tone( int pin, int freq ){}
    void noTone( int pin ){}
    int shiftIn( int pin, int clock, int msb ){ return random( 0, 256 ); }
    void shiftOut( int pin, int clock, int msb, int val ){}
    class _Wire{
        public:
            void beginTransmission( int ch ){}
            int endTransmission(){ return random( 0, 5 ); }
    };
    _Wire Wire;
#endif


// We have to create an object from Commander class.
Commander commander;

// Create a colorizer object.
CommanderColorizer colorizer;

// Command callbacks
bool drawSine_func( char *args, CommandCaller* caller );
bool selfDestruct_func( char *args, CommandCaller* caller );
bool beep_func( char *args, CommandCaller* caller );
bool mute_func( char *args, CommandCaller* caller );
bool unmute_func( char *args, CommandCaller* caller );
bool battery_func( char *args, CommandCaller* caller );
bool plotAdc_func( char *args, CommandCaller* caller );
bool pinConfig_func( char *args, CommandCaller* caller );
bool digitalRead_func( char *args, CommandCaller* caller );
bool digitalWrite_func( char *args, CommandCaller* caller );
bool uptime_func( char *args, CommandCaller* caller );
bool analogWrite_func( char *args, CommandCaller* caller );
bool tone_func( char *args, CommandCaller* caller );
bool noTone_func( char *args, CommandCaller* caller );
bool shiftIn_func( char *args, CommandCaller* caller );
bool shiftOut_func( char *args, CommandCaller* caller );
bool i2cScan_func( char *args, CommandCaller* caller );

void capacitorSimulator();
void adcPolotter();

Commander::systemCommand_t API_tree[] = {
    systemCommand( "battery", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tShows battery charge.\r\n\r\n\tEXAMPLE\r\n\t\tbattery\033[0;37m", battery_func ),
    systemCommand( "drawSine", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tDraw a sine wave.\r\n\r\n\tEXAMPLE\r\n\t\tdrawSine\033[0;37m", drawSine_func ),
    systemCommand( "selfDestruct", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tInitiates the self-destruct protocol.\r\n\r\n\tEXAMPLE\r\n\t\tselfDestruct\033[0;37m", selfDestruct_func ),
    systemCommand( "beep", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tGenerates a beep sound.\r\n\r\n\tEXAMPLE\r\n\t\tbeep\033[0;37m", beep_func ),
    systemCommand( "mute", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tDisables beep functionality.\r\n\r\n\tEXAMPLE\r\n\t\tmute\033[0;37m", mute_func ),
    systemCommand( "unmute", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tEnables beep functionality.\r\n\r\n\tEXAMPLE\r\n\t\tunmute\033[0;37m", unmute_func ),
    systemCommand( "plotAdc", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tPlots an analog channel by time.\r\n\r\n\tSYNOPSIS\r\n\t\tplotAdc <adc pin>\r\n\r\n\tEXAMPLE\r\n\t\tplotAdc 1\r\n\t\tMonitor A1 pin.\033[0;37m", plotAdc_func ),
    systemCommand( "pinConfig", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tConfigure a pin easily.\r\n\r\n\tSYNOPSIS\r\n\t\tpinConfig <pin>\r\n\r\n\tEXAMPLE\r\n\t\tpinConfig 13\r\n\t\tConfigure pin 13.\033[0;37m", pinConfig_func ),
    systemCommand( "digitalRead", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tReads the state of a digital pin.\r\n\r\n\tSYNOPSIS\r\n\t\tdigitalRead <pin>\r\n\r\n\tEXAMPLE\r\n\t\tdigitalRead 12\r\n\t\tRead the state of pin 12.\033[0;37m", digitalRead_func ),
    systemCommand( "digitalWrite", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tWrites a state to a digital pin.\r\n\r\n\tSYNOPSIS\r\n\t\tdigitalWrite [OPTIONS]\r\n\r\n\tOPTIONS\r\n\t\t-p, --pin\tthe Arduino pin number\r\n\t\t-s, --state\t0 means LOW, 1 means HIGH\r\n\r\n\tEXAMPLE\r\n\t\tdigitalWrite -p 13 -s 1\r\n\t\tSet pin 13 to output high state.\033[0;37m", digitalWrite_func ),
    systemCommand( "uptime", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tReturns the time since program start.\r\n\r\n\tEXAMPLE\r\n\t\tuptime\033[0;37m", uptime_func ),
    systemCommand( "analogWrite", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tWrites a value to a PWM supported pin in a 8-bit resolution\r\n\r\n\tSYNOPSIS\r\n\t\tanalogWrite [OPTIONS]\r\n\r\n\tOPTIONS\r\n\t\t-p, --pin\tthe Arduino pin number\r\n\t\t-v, --value\t0 - 255\r\n\r\n\tEXAMPLE\r\n\t\tanalogWrite -p 3 -v 128\r\n\t\tGenerate an 50% duty-cycle PWM signal on pin 3\033[0;37m", analogWrite_func ),
    systemCommand( "tone", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tGenerates a square wave on specified pin, with 50% duty cycle.\r\n\r\n\tSYNOPSIS\r\n\t\ttone [OPTIONS]\r\n\r\n\tOPTIONS\r\n\t\t-p, --pin\tthe Arduino pin number\r\n\t\t-f, --frequency\ttone frequency\r\n\r\n\tEXAMPLE\r\n\t\ttone -p 3 -f 440\r\n\t\tGenerate a 440Hz tone on pin 3\033[0;37m", tone_func ),
    systemCommand( "noTone", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tStops generation of square wave on the specified pin.\r\n\r\n\tSYNOPSIS\r\n\t\noTone <pin>\r\n\r\n\tEXAMPLE\r\n\t\tnoTone 3\r\n\t\tStops tone generation on pin 3\033[0;37m", noTone_func ),
    systemCommand( "shiftIn", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tShifts in a byte of data one bit at a time, and returns\r\n\t\tthe value of the bit read.\r\n\r\n\tSYNOPSIS\r\n\t\tshiftIn [OPTIONS]\r\n\r\n\tOPTIONS\r\n\t\t-p, --pin\tthe Arduino pin number\r\n\t\t-c, --clock\tthe Arduino pin number\r\n\t\t-m, --msb\tFlag that indicates MSB first.\r\n\r\n\tEXAMPLE\r\n\t\tshiftIn -p 4 -c 5 -m\r\n\t\tShift data in with MSB format.\r\n\t\tpin 4 : data\r\n\t\tpin 5 : clock\033[0;37m", shiftIn_func ),
    systemCommand( "shiftOut", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tShifts out a byte of data one bit at a time.\r\n\r\n\tSYNOPSIS\r\n\t\tshiftOut [OPTIONS]\r\n\r\n\tOPTIONS\r\n\t\t-p, --pin\tthe Arduino pin number\r\n\t\t-c, --clock\tthe Arduino pin number\r\n\t\t-m, --msb\tFlag that indicates MSB first\r\n\t\t-v, --value\tThis value will be shifted out.\r\n\r\n\tEXAMPLE\r\n\t\tshiftOut -p 4 -c 5 -m -v 138\r\n\t\tShift value 138 out with MSB format.\r\n\t\tpin 4 : data\r\n\t\tpin 5 : clock\033[0;37m", shiftOut_func ),
    systemCommand( "i2cScan", "\r\n\t\033[1;31mDESCRIPTION\r\n\t\tScans all the 127 avaiable addresses\r\n\t\ton the I2C bus and generates a table.\r\n\t\tEach slave that responds will be marked.\r\n\r\n\tEXAMPLE\r\n\t\tuptime\033[0;37m", i2cScan_func )
};

const char* destructListOptions[] = {
    "No",
    "Noooo",
    "I'm confused",
    "Hell yeah!"
};


const char* destructListText =  "Are you really want to initiate the self destruct protocol?";

ShellminatorList destructList( destructListOptions, 4, destructListText );

const char* pinModeListOptions[] = {
    "Input",
    "Output"
};

const char* pinModeListDetails[] = {
    "Pin can receive signals\nfrom outside( hi-Z )",
    "Pin can send signals to\noutside( push-pull )"
};

const char* pinModeListText =  "Pin direction:";

ShellminatorListDetailed pinModeList( pinModeListOptions, pinModeListDetails, 2, pinModeListText );

const char* pinStateListOptions[] = {
    "Low",
    "High"
};

const char* pinStateListDetails[] = {
    "Pin will output 0V",
    "Pin will output I/O Voltage"
};

const char* pinStateListText =  "Pin state:";

ShellminatorListDetailed pinStateList( pinStateListOptions, pinStateListDetails, 2, pinStateListText );

const char* pinPullUpListOptions[] = {
    "Disable",
    "Pull-up"
};

const char* pinPullUpListDetails[] = {
    "Internal pull-up resistor\ndisabled( hi-z )"
    "Internal pull-up resistor\nwill be activated.",
};

const char* pinPullUpListText =  "Pull Up Resistor:";

ShellminatorListDetailed pinPullUpList( pinPullUpListOptions, pinPullUpListDetails, 2, pinPullUpListText );

ShellminatorNotification notification;

// Create a plotter object.
ShellminatorButton destructButton( "Initiate Self Destruct" );

Shellminator::shellEvent_t destructButtonEvent;

// Create a progress bar object and connect it to the shell.
ShellminatorProgress progress;

float plotData[ 100 ];
int plotDataSize = sizeof( plotData ) / sizeof( plotData[ 0 ] );

uint32_t timerStart = 0;
uint32_t timerPeriod = 1000;
uint32_t chargeTimerStart = 0;

typedef enum{
    PLOT_BATTERY,
    PLOT_SINE,
    PLOT_ADC
} plotState_t;

int adcPlotterPin = 0;

plotState_t plotState;

ShellminatorPlot plot( plotData, plotDataSize, "Plot" );

// Create a Shellminator object, and initialize it to use Serial
ShellminatorCommanderInterface shell( &Serial );

bool diagEnabled = false;

// We need a buffer to store the characters from the input.
char nameBuffer[ 30 ];


uint8_t passwordHash[] = { 0xCC, 0xb4, 0x24, 0x83 };

// Create a pretty logo for the terminal.
const char logo[] =

    "   _____ __         ____          _             __            \r\n"
    "  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
    "  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
    " ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
    "/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
    "\r\n"
    "Visit on GitHub: https://www.shellminator.org/html/index.html\r\n\r\n"

;


// This function will be called, when the button is pressed.
void destructButtonClick( ShellminatorScreen* screen );

void destructListCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* );


int pinConfigPin;
int pinConfigMode;
int pinConfigState;
void pinModeListCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* );
void pinPullUpListCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* );
void pinStateListCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* );




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();
    shell.enableFormatting = false;

    Serial.println( "Program Start!" );


    // Initialize shell object just enough to create a promt.
    // Setting an empty string as argument to skip banner printing.
    shell.begin( "" );

    // Attach the colorizer to the shell.
    shell.attachColorizer( &colorizer );

    shell.input( nameBuffer, sizeof( nameBuffer ), "Would you like to enable formatting? [y/n] ", formattingCallback );


}

// Infinite loop.
void loop(){

    // In the timerPeriod defined intervals, recalculate the current
    // state of the simulated capacitor.
    if( ( millis() - timerStart ) > timerPeriod ){
        timerStart = millis();

        switch( plotState ){
            case PLOT_BATTERY:
                capacitorSimulator();
                // After calculation, we have to request the shell
                // to redraw the plot.
                shell.requestRedraw();
                break;
            case PLOT_ADC:
                adcPolotter();
                shell.requestRedraw();
                break;
            default:
                break;
        }

    }

    // Process the new data.
    shell.update();


}

void nameCallback( char* buffer, int bufferSize, Shellminator* parent ){

    parent -> channel -> println();
    if( diagEnabled ){
        Serial.println( "Initialising Commander..." );
    }
    commander.attachTree( API_tree );
    commander.init();

    if( diagEnabled ){
        Serial.print( "[ " );
        shell.format( &Serial, Shellminator::BOLD, Shellminator::GREEN );
        Serial.print( "OK" );
        shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
        Serial.println( " ]" );
    }

    shell.attachCommander( &commander );

    if( diagEnabled ){
        Serial.println( "Initialising System Components..." );
    }
    destructButtonEvent.type = Shellminator::SHELL_EVENT_KEY;
    destructButtonEvent.data = (uint8_t)'y';

    destructButton.attachEvent( destructButtonEvent );
    destructButton.attachTriggerFunction( destructButtonClick );
    destructButton.setColor( Shellminator::RED );

    destructList.attachCallback( destructListCallback );
    pinModeList.attachCallback( pinModeListCallback );
    pinPullUpList.attachCallback( pinPullUpListCallback );
    pinStateList.attachCallback( pinStateListCallback );

    if( diagEnabled ){
        Serial.print( "[ " );
        shell.format( &Serial, Shellminator::BOLD, Shellminator::GREEN );
        Serial.print( "OK" );
        shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
        Serial.println( " ]" );
    }

    shell.format( &Serial, Shellminator::BOLD, Shellminator::GREEN );
    Serial.println( "System init finished!" );
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    Serial.print( "To list available commands use the " );
    shell.format( &Serial, Shellminator::BACKGROUND, Shellminator::YELLOW );
    Serial.print( " help " );
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
    Serial.println( " command." );


    // The input does not generate a new line before the callback,
    // if it is required, it needs to be done manually.
    parent -> channel -> println();

    // Echo back the name.
    parent -> channel -> print( "Welcome " );
    parent -> channel -> print( buffer );
    parent -> channel -> println( '!' );

    parent -> channel -> print( "Note - The default password is " );
    shell.format( parent -> channel, Shellminator::BOLD, Shellminator::YELLOW );
    parent -> channel -> print( "Password");
    shell.format( parent -> channel, Shellminator::REGULAR, Shellminator::WHITE );
    parent -> channel -> println();

    parent -> channel -> print( "Please log out after you finished with" );
    shell.format( parent -> channel, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
    parent -> channel -> print( " Ctrl-D ");
    shell.format( parent -> channel, Shellminator::REGULAR, Shellminator::WHITE );
    parent -> channel -> println( "key!" );

    // Attach the logo.
    shell.attachLogo( logo );

    shell.attachNeofetchFunc( defaultShellminatorNeofetch );

    // Enable password protection.
    shell.setPassword( passwordHash, sizeof( passwordHash ) );

    shell.begin( nameBuffer );

}

void loggingCallback( char* buffer, int bufferSize, Shellminator* parent ){
    bool elabled = false;
    parent -> channel -> println();
    if( strcmp( buffer, "y" ) == 0 ){
        elabled = true;
    }
    else if( strcmp( buffer, "yes" ) == 0 ){
        elabled = true;
    }
    else if( strcmp( buffer, "Yes" ) == 0 ){
        elabled = true;
    }
    else{
        parent -> channel -> println( "Your answer is treated as No." );
    }
    if( elabled ){
        parent -> channel -> println( "Extended logging ENABLED" );
        diagEnabled = true;
    }
    else{
        parent -> channel -> println( "Extended logging DISABLED" );
        diagEnabled = true;
    }
    parent -> input( nameBuffer, sizeof( nameBuffer ), "Please enter your name: ", nameCallback );
}

void formattingCallback( char* buffer, int bufferSize, Shellminator* parent ){
    bool elabled = false;
    parent -> channel -> println();
    if( strcmp( buffer, "y" ) == 0 ){
        elabled = true;
    }
    else if( strcmp( buffer, "yes" ) == 0 ){
        elabled = true;
    }
    else if( strcmp( buffer, "Yes" ) == 0 ){
        elabled = true;
    }
    else{
        parent -> channel -> println( "Your answer is treated as No." );
    }

    if( elabled ){
        parent -> channel -> println( "Formatting ENABLED" );
        parent -> enableFormatting = true;
    }
    else{
        parent -> channel -> println( "Formatting DISABLED" );
        parent -> enableFormatting = true;
    }
    parent -> input( nameBuffer, sizeof( nameBuffer ), "Would you like to enable boot messages? [y/n] ", loggingCallback );
}



bool i2cScan_func( char *args, CommandCaller* caller ){
    int i;
    int ret;
    int collumn_cntr;
    int row_cntr;

    caller -> println( "     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f" );

    collumn_cntr = 0;
    row_cntr = 0;
    caller -> print( "00:" );
    for( i = 0; i < 127; i++ ){
        Wire.beginTransmission( i );
        ret = Wire.endTransmission();
        if( ret == 0 ){
            Shellminator::setFormat( caller, Shellminator::GREEN, Shellminator::BOLD );
            caller -> print( ' ' );
            if( i < 16 ){
                caller -> print( '0' );
            }
            caller -> print( i, HEX );
        }
        else{
            Shellminator::setFormat( caller, Shellminator::RED, Shellminator::BOLD );
            caller -> print( " --" );
        }
        Shellminator::setFormat( caller, Shellminator::REGULAR, Shellminator::WHITE );
        collumn_cntr++;
        if( collumn_cntr >= 16 ){
            collumn_cntr = 0;
            row_cntr += 10;
            caller -> println();
            caller -> print( row_cntr );
            caller -> print( ':' );
        }
    }
    caller -> println();
    return true;
}

bool shiftIn_func( char *args, CommandCaller* caller ){
    Argument pin( args, 'p', "pin" );
    Argument clock( args, 'c', "clock" );
    Argument msbfirst_flag( args, 'm', "msb" );
    Argument hex_flag( args, 'h', "hex" );
    Argument bin_flag( args, 'b', "bin" );

    int data;

    if( !pin.parseInt() || !clock.parseInt() ){
        caller -> print( "Wrong arguments! help:" );
        shell.format( caller, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
        caller -> print( " shiftIn? " );
        shell.format( caller, Shellminator::REGULAR, Shellminator::WHITE );
        caller -> println();
        return false;
    }

    msbfirst_flag.find();
    hex_flag.find();
    bin_flag.find();

    pinMode( (int)clock, OUTPUT );
    data = shiftIn( (int)pin, (int)clock, msbfirst_flag.isFound() ? MSBFIRST: LSBFIRST );

    if( hex_flag.isFound() ){
        caller -> print( "0x" );
        caller -> println( data, HEX );
    }
    else if( bin_flag.isFound() ){
        caller -> print( "0b" );
        caller -> println( data, BIN );
    }
    else{
        caller -> println( data );
    }
    
    return true;
}

bool shiftOut_func( char *args, CommandCaller* caller ){
    Argument pin( args, 'p', "pin" );
    Argument clock( args, 'c', "clock" );
    Argument msbfirst_flag( args, 'm', "msb" );
    Argument value( args, 'v', "value" );

    if( !pin.parseInt() || !clock.parseInt() || !value.parseInt() ){
        caller -> print( "Wrong arguments! help:" );
        shell.format( caller, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
        caller -> print( " shiftOut? " );
        shell.format( caller, Shellminator::REGULAR, Shellminator::WHITE );
        caller -> println();
        return false;
    }

    msbfirst_flag.find();

    pinMode( (int)pin, OUTPUT );
    pinMode( (int)clock, OUTPUT );

    shiftOut( (int)pin, (int)clock, msbfirst_flag.isFound() ? MSBFIRST: LSBFIRST, (int)value );    
    return true;
}

bool tone_func( char *args, CommandCaller* caller ){
    Argument pin( args, 'p', "pin" );
    Argument frequency( args, 'f', "frequency" );

    if( !pin.parseInt() || !frequency.parseInt() ){
        caller -> print( "Wrong arguments! help:" );
        shell.format( caller, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
        caller -> print( " tone? " );
        shell.format( caller, Shellminator::REGULAR, Shellminator::WHITE );
        caller -> println();
        return false;
    }

    tone( (int)pin, (int)frequency );
    
    return true;
}

bool noTone_func( char *args, CommandCaller* caller ){
    Argument pin( args, 0 );

    if( !pin.parseInt() ){
        caller -> print( "Wrong arguments! help:" );
        shell.format( caller, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
        caller -> print( " noTone? " );
        shell.format( caller, Shellminator::REGULAR, Shellminator::WHITE );
        caller -> println();
        return false;
    }

    noTone( (int)pin );
    
    return true;
}

bool analogWrite_func( char *args, CommandCaller* caller ){
    Argument pin( args, 'p', "pin" );
    Argument value( args, 'v', "value" );

    if( !pin.parseInt() || !value.parseInt() ){
        caller -> print( "Wrong arguments! help:" );
        shell.format( caller, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
        caller -> print( " analogWrite? " );
        shell.format( caller, Shellminator::REGULAR, Shellminator::WHITE );
        caller -> println();
        return false;
    }

    analogWrite( (int)pin, (int)value );
    
    return true;
}

bool uptime_func( char *args, CommandCaller* caller ){
    char buffer[ 30 ];
    uint32_t seconds = millis() / 1000;
    uint32_t days = seconds / 86400;
    seconds %= 86400;
    uint32_t hours = seconds / 3600;
    seconds %= 3600;
    uint32_t minutes = seconds / 60;
    seconds %= 60;

    snprintf( buffer, sizeof( buffer ), "%d:%02d:%02d:%02d", days, hours, minutes, (int)seconds );
    caller -> println( buffer );

    return true;
}

bool digitalRead_func( char *args, CommandCaller* caller ){
    Argument pin( args, 0 );

    if( !pin.parseInt() ){
        caller -> print( "Wrong arguments! help:" );
        shell.format( caller, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
        caller -> print( " digitalRead? " );
        shell.format( caller, Shellminator::REGULAR, Shellminator::WHITE );
        caller -> println();
        return false;
    }

   caller -> println( digitalRead( (int)pin ) );
    
    return true;
}

bool digitalWrite_func( char *args, CommandCaller* caller ){
    Argument pin( args, 'p', "pin" );
    Argument state( args, 's', "state" );

    if( !pin.parseInt() || !state.parseInt() ){
        caller -> print( "Wrong arguments! help:" );
        shell.format( caller, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
        caller -> print( " digitalWrite? " );
        shell.format( caller, Shellminator::REGULAR, Shellminator::WHITE );
        caller -> println();
        return false;
    }

    pinMode( (int)pin, OUTPUT );
    digitalWrite( (int)pin, (int)state );
    
    return true;
}

void pinStateListCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen ){
    
    pinMode( pinConfigPin, OUTPUT );
    if( selected == 0 ){
        digitalWrite( pinConfigPin, LOW );
        return;
    }
    digitalWrite( pinConfigPin, HIGH );

}

void pinPullUpListCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen ){
    // Input
    if( selected == 0 ){
        pinMode( pinConfigPin, INPUT );
        return;
    }
    pinMode( pinConfigPin, INPUT_PULLUP );
}

void pinModeListCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }
    
    // Input
    if( selected == 0 ){
        parent -> swapScreenAndClear( &pinPullUpList );
        return;
    }

    // Output
    parent -> swapScreenAndClear( &pinStateList );
}

bool pinConfig_func( char *args, CommandCaller* caller ){
    Argument pin( args, 0 );

    if( !pin.parseInt() ){
        caller -> print( "Wrong arguments! help:" );
        shell.format( caller, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
        caller -> print( " pinConfig? " );
        shell.format( caller, Shellminator::REGULAR, Shellminator::WHITE );
        caller -> println();
        return false;
    }

    pinConfigPin = (int)pin;

    shell.beginScreen( &pinModeList );

    return true;
}

/// This is an example function for the cat command
bool battery_func(char *args, CommandCaller* caller ){
    int i;

    plotState = PLOT_BATTERY;
    chargeTimerStart = millis();


    for( i = 0; i < plotDataSize; i++ ){
        plotData[ i ] = 0;
    }

    plot.setName( "Battery Voltage[ V ]" );
    shell.beginScreen( &plot );
    return true;
}

bool plotAdc_func( char *args, CommandCaller* caller ){
    int i;
    Argument pin( args, 0 );
    
    if( !pin.parseInt() ){
        caller -> print( "Wrong arguments! help:" );
        shell.format( caller, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
        caller -> print( " plotAdc? " );
        shell.format( caller, Shellminator::REGULAR, Shellminator::WHITE );
        caller -> println();
        return false;
    }

    plotState = PLOT_ADC;
    adcPlotterPin = (int)pin;

    for( i = 0; i < plotDataSize; i++ ){
        plotData[ i ] = 0;
    }

    plot.setName( "ADC Value [ LSB ]" );
    shell.beginScreen( &plot );
    return true;
}

/// This is an example function for the dog command
bool drawSine_func(char *args, CommandCaller* caller ){
    int i;

    for( i = 0; i < plotDataSize; i++ ){
        plotData[ i ] = sin( (float)i / (float)plotDataSize * M_PI * 2.0 );
    }

    plotState = PLOT_SINE;
    plot.setName( "Sine Function" );
    shell.beginScreen( &plot );
    return true;

}

void adcPolotter(){
    int i;
    for( i = 0; i < plotDataSize; i++ ){
        plotData[ i ] = plotData[ i + 1 ];
    }

    plotData[ plotDataSize - 1 ] = analogRead( adcPlotterPin );
}

/// This is an example function for the sum command
bool selfDestruct_func(char *args, CommandCaller* caller ){
    shell.clear();
    shell.beginScreen( &destructButton );
    return true;
}

void destructButtonClick( ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }
    parent -> swapScreenAndClear( &destructList );
}

void destructListCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }

    switch( selected ){
        case 0:
            notification.setText( "Self destruct sequence stopped!" );
            break;
        case 1:
            notification.setText( "Self destruct sequence stoooopped!" );
            break;
        case 2:
            notification.setText( "Confusion detected!\nSelf destruct sequence stopped." );
            break;
        case 3:
            notification.setText( "You doesn't have permission to\ninitiate the self destruct sequence!" );
            break;
        default:
            notification.setText( "Something went wrong with the list!" );
            break;
    }

    parent -> swapScreenAndClear( &notification );

}

void capacitorSimulator(){
    int i;
 
    // With the for loop, we shift every element in the plot to the left
    for( i = 0; i < plotDataSize - 1; i++ ){
        plotData[ i ] = plotData[ i + 1 ];
    }
 
    // To the last element, we calculate the new value based on system time.
    plotData[ plotDataSize - 1 ] = U0 * ( 1.0 - exp( -( ( millis() - chargeTimerStart ) / 1000.0 ) *  ( 1.0 / ( R * C ) ) ) );
 
}

bool beep_func(char *args, CommandCaller* caller ){
    if( shell.mute ){
        caller -> println( "Terminal is muted. Please unmute it!" );
        return true;
    }
    caller -> println( "Beeep!" );
    shell.beep();
    return true;
}

bool mute_func(char *args, CommandCaller* caller ){
    shell.mute = true;
    return true;
}

bool unmute_func(char *args, CommandCaller* caller ){
    shell.mute = false;
    return true;
}