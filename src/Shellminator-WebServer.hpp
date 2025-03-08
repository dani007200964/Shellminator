/*
 * Created on July 07 2024
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified July 07 2024
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

#ifndef SHELLMINATOR_WEBSERVER_HPP_
#define SHELLMINATOR_WEBSERVER_HPP_

    #include "Stream.h"
    #include "Shellminator-DefaultSettings.hpp"
    #include "Shellminator-Helpers.hpp"
    #include "Shellminator-Browser-Response.hpp"
    #include <string.h>

    // Todo add hooks to connection and disconnection events!
    // Todo add SHELLMINATOR_WEBS_DBG_ENABLE to defaultSettings.

    #ifdef ARDUINO
        #include "Arduino.h"
    #endif

    #ifdef SHELLMINATOR_USE_WIFI_CLIENT

        // Macro magic for macro overloading.
        #define SHELLMINATOR_WEBS_DBG_OVERLOAD( _1, _2, NAME, ... ) NAME

        #define SHELLMINATOR_WEBS_DBG_ENABLE

        #ifdef SHELLMINATOR_WEBS_DBG_ENABLE
            #define SHELLMINATOR_WEBS_DBG_1(   x ) if( dbg ){ dbg -> print( x );   }
            #define SHELLMINATOR_WEBS_DBGLN_1( x ) if( dbg ){ dbg -> println( x ); }

            #define SHELLMINATOR_WEBS_DBG_2(   x, y ) if( dbg ){ dbg -> print( x, y );   }
            #define SHELLMINATOR_WEBS_DBGLN_2( x, y ) if( dbg ){ dbg -> println( x, y ); }
        #elif
            #define SHELLMINATOR_WEBS_DBG_1(   x )
            #define SHELLMINATOR_WEBS_DBGLN_1( x )

            #define SHELLMINATOR_WEBS_DBG_2(   x, y )
            #define SHELLMINATOR_WEBS_DBGLN_2( x, y )
        #endif

        /// Macro to print debug message.
        ///
        /// The syntax is the same as a regular Stream.print() function.
        /// It will check if a debug channel is attached and only then
        /// will print the debug message.
        #define SHELLMINATOR_WEBS_DBG( ... )   SHELLMINATOR_WEBS_DBG_OVERLOAD( __VA_ARGS__, SHELLMINATOR_WEBS_DBG_2,   SHELLMINATOR_WEBS_DBG_1   )( __VA_ARGS__ )
        #define SHELLMINATOR_WEBS_DBGLN( ... ) SHELLMINATOR_WEBS_DBG_OVERLOAD( __VA_ARGS__, SHELLMINATOR_WEBS_DBGLN_2, SHELLMINATOR_WEBS_DBGLN_1 )( __VA_ARGS__ )

        #define SHELLMINATOR_WEBSERVER_URL_BUFFER_SIZE      30
        #define SHELLMINATOR_WEBSERVER_CLIENT_TIMEOUT_MS    10000

        class ShellminatorWebServer{
            public:
                ShellminatorWebServer();
                ShellminatorWebServer( int port_p );

                void begin();
                void update();
                void attachDebugChannel( Stream* dbg_p );
            
            protected:

                virtual void processURL( const char* url_p );
                virtual void processNotFound( const char* url_p );

                char clientBuffer[ SHELLMINATOR_WEBSERVER_URL_BUFFER_SIZE + 1 ];
                int clientBufferCounter;

                int port = 80;
                Stream* dbg = NULL;

                uint32_t timerStart;

                WiFiServer* server = NULL;
                WiFiClient client;
        };

        class ShellminatorWebServerThemed : public ShellminatorWebServer{
        public:
            ShellminatorWebServerThemed() : ShellminatorWebServer{}{}
            ShellminatorWebServerThemed( int port_p ) : ShellminatorWebServer{ port_p }{}

        protected:
            void processRoot();
            void processIndex();
            void processURL( const char* url_p ) override;
            void processNotFound( const char* url_p ) override;
        };

        class ShellminatorWebServerThemedOffline : public ShellminatorWebServer{
        public:
            ShellminatorWebServerThemedOffline() : ShellminatorWebServer{}{}
            ShellminatorWebServerThemedOffline( int port_p ) : ShellminatorWebServer{ port_p }{}

        protected:
            void processRoot();
            void processIndex();
            void processXtermJs();
            void processXtermWeblinks();
            void processURL( const char* url_p ) override;
            void processNotFound( const char* url_p ) override;
        };



    #endif

#endif