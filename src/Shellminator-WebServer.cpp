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

#include "Shellminator-WebServer.hpp"

#ifdef SHELLMINATOR_USE_WIFI_CLIENT

    #ifdef ESP8266
        static const char xtermMinJsResponse[] PROGMEM = XTERM_MIN_JS;
        static const char xtermWeblinksJsResponse[] PROGMEM = XTERM_ADDON_WEB_LINKS_MIN_JS;
        static const char indexThemedResponse[] PROGMEM = SHELLMINATOR_WEB_XTERM_THEMED;
        static const char indexThemedOfflineResponse[] PROGMEM = SHELLMINATOR_WEB_XTERM_THEMED_OFFLINE;
        static const char notFoundThemedResponse[] PROGMEM = SHELLMINATOR_404_THEMED;
    #else
        // Tested on ESP32, Arduino UNO R4 and Pi Pico
        static const char xtermMinJsResponse[] = XTERM_MIN_JS;
        static const char xtermWeblinksJsResponse[] = XTERM_ADDON_WEB_LINKS_MIN_JS;
        static const char indexThemedResponse[] = SHELLMINATOR_WEB_XTERM_THEMED;
        static const char indexThemedOfflineResponse[] = SHELLMINATOR_WEB_XTERM_THEMED_OFFLINE;
        static const char notFoundThemedResponse[] = SHELLMINATOR_404_THEMED;
    #endif


    ShellminatorWebServer::ShellminatorWebServer(){
        dbg = NULL;
        port = 443;
        server = new WiFiServer( port );
    }

    ShellminatorWebServer::ShellminatorWebServer( int port_p ){
        dbg = NULL;
        port = port_p;
        server = new WiFiServer( port );
    }

    void ShellminatorWebServer::attachDebugChannel( Stream* dbg_p ){
        dbg = dbg_p;
    }

    void ShellminatorWebServer::begin(){
        // todo check network module firmware on Uno R4 and Pico W
        if( server == NULL ){
            return;
        }
        server -> begin();
        SHELLMINATOR_WEBS_DBGLN( __CONST_TXT__( "Web Server started." ) );
    }

    void ShellminatorWebServer::processNotFound( const char* url_p ){
        SHELLMINATOR_WEBS_DBG( __CONST_TXT__( "Not Found Page: " ) );
        SHELLMINATOR_WEBS_DBGLN( url_p );
        client.println( __CONST_TXT__( "HTTP/1.1 404 OK" ) );
        client.println( __CONST_TXT__( "Content-type: text/plain" ) );
        client.println();

        // the content of the HTTP response follows the header:
        client.print( __CONST_TXT__( "Page not found!" ) );

    }

    void ShellminatorWebServer::processURL( const char* url_p ){
        SHELLMINATOR_WEBS_DBG( __CONST_TXT__( "URL: " ) );
        SHELLMINATOR_WEBS_DBGLN( url_p );
        client.println( __CONST_TXT__( "HTTP/1.1 200 OK" ) );
        client.println( __CONST_TXT__( "Content-type: text/html" ) );
        client.println();

        // the content of the HTTP response follows the header:
        client.print( __CONST_TXT__( "<p style=\"font-size:7vw;\">I'll be back!</p>" ) );

    }

    void ShellminatorWebServer::update(){
        char newChar;
        bool urlProcessed = false;
        int endOfURL;

        if( server == NULL ){
            return;
        }

        client = server -> accept();

        if( client ){
            SHELLMINATOR_WEBS_DBGLN( __CONST_TXT__( "New Web Server Client." ) );
            clientBuffer[ 0 ] = '\0';
            clientBufferCounter = 0;
            timerStart = millis();

            while( CLIENT_STATE ){
                if( ( millis() - timerStart ) > SHELLMINATOR_WEBSERVER_CLIENT_TIMEOUT_MS ){
                    SHELLMINATOR_WEBS_DBGLN( __CONST_TXT__( "Timeout!" ) );
                    break;
                }

                if( client.available() ){
                    newChar = client.read();

                    //SHELLMINATOR_WEBS_DBG( (char)newChar );
                    //SHELLMINATOR_WEBS_DBG( __CONST_TXT__( "\t0x" ) );
                    //SHELLMINATOR_WEBS_DBG( __CONST_TXT__( "\t" ) );
                    //SHELLMINATOR_WEBS_DBGLN( (int)newChar );

                    if( newChar == '\n' ){
                        clientBuffer[ clientBufferCounter ] = '\0';
                        clientBufferCounter = 0;

                        if( clientBuffer[ 0 ] == '\0' ){
                            SHELLMINATOR_WEBS_DBGLN( __CONST_TXT__( "Header end detected." ) );
                            break;
                        }

                        else if( urlProcessed ){
                            SHELLMINATOR_WEBS_DBG( __CONST_TXT__( "Client Data: " ) );
                            SHELLMINATOR_WEBS_DBGLN( __CONST_TXT__( clientBuffer ) );
                            continue;
                        }

                        else if( startsWith( clientBuffer, "GET " ) ){
                            endOfURL = indexOf( clientBuffer + 4, ' ' );
                            if( endOfURL >= 0 ){
                                clientBuffer[ endOfURL + 4 ] = '\0';

                                // We have to check for a '?' character as well.
                                // This character indicates that we have arguments in the
                                // URL and we do not want to pass them to the callbacks.
                                endOfURL = indexOf( clientBuffer, '?' );
                                if( endOfURL >= 0 ){
                                    clientBuffer[ endOfURL ] = '\0';
                                }

                                shiftStringLeft( clientBuffer );
                                shiftStringLeft( clientBuffer );
                                shiftStringLeft( clientBuffer );
                                shiftStringLeft( clientBuffer );
                                
                                processURL( clientBuffer );
                                urlProcessed = true;
                            }                        
                        }

                    }

                    else if( newChar == '\r' ){
                        continue;
                    }

                    else{
                        clientBuffer[ clientBufferCounter ] = newChar;
                        clientBufferCounter++;
                        if( clientBufferCounter >= SHELLMINATOR_WEBSERVER_URL_BUFFER_SIZE ){
                            clientBufferCounter = SHELLMINATOR_WEBSERVER_URL_BUFFER_SIZE;
                        }
                    }
                }
            }
            client.stop();
            SHELLMINATOR_WEBS_DBGLN( __CONST_TXT__( "Client Disconnected!" ) );
        }
        
    }

    /*-------- Themed Webserver --------*/

    void ShellminatorWebServerThemed::processRoot(){
        SHELLMINATOR_WEBS_DBGLN( __CONST_TXT__( "Redirect Root To Index Page" ) );
        client.println( __CONST_TXT__( "HTTP/1.1 302 OK" ) );
        client.println( __CONST_TXT__( "Content-type: text/plain" ) );
        client.println( __CONST_TXT__( "Location: /index.html?selfHosted" ) );
        client.println();
    }

    void ShellminatorWebServerThemed::processIndex(){
        SHELLMINATOR_WEBS_DBGLN( __CONST_TXT__( "Processing Index Page" ) );
        client.println( __CONST_TXT__( "HTTP/1.1 200 OK" ) );
        client.println( __CONST_TXT__( "Content-type: text/html" ) );
        client.println();

        // the content of the HTTP response follows the header:
        #ifdef ESP8266
            client.write_P( indexThemedResponse, SHELLMINATOR_WEB_XTERM_THEMED_SIZE );
        #else
            client.write( indexThemedResponse, SHELLMINATOR_WEB_XTERM_THEMED_SIZE );
        #endif
    }

    void ShellminatorWebServerThemed::processURL( const char* url_p ){
        SHELLMINATOR_WEBS_DBG( __CONST_TXT__( "URL: " ) );
        SHELLMINATOR_WEBS_DBGLN( url_p );
        if( strcmp( url_p, "/" ) == 0 ){
            processRoot();
        }
        else if( strcmp( url_p, "/index.html" ) == 0 ){
            processIndex();
        }
        else{
            processNotFound( url_p );
        }
    }

    void ShellminatorWebServerThemed::processNotFound( const char* url_p ){
        SHELLMINATOR_WEBS_DBG( __CONST_TXT__( "Not Found Page: " ) );
        SHELLMINATOR_WEBS_DBGLN( url_p );
        client.println( __CONST_TXT__( "HTTP/1.1 404 OK" ) );
        client.println( __CONST_TXT__( "Content-type: text/html" ) );
        client.println();

        // the content of the HTTP response follows the header:
        #ifdef ESP8266
            client.write_P( notFoundThemedResponse, SHELLMINATOR_404_THEMED_SIZE );
        #else
            client.write( notFoundThemedResponse, SHELLMINATOR_404_THEMED_SIZE );
        #endif
    }

    /*-------- Themed Offline Webserver --------*/

    void ShellminatorWebServerThemedOffline::processRoot(){
        SHELLMINATOR_WEBS_DBGLN( __CONST_TXT__( "Redirect Root To Index Page" ) );
        client.println( __CONST_TXT__( "HTTP/1.1 302 OK" ) );
        client.println( __CONST_TXT__( "Content-type: text/plain" ) );
        client.println( __CONST_TXT__( "Location: /index.html?selfHosted" ) );
        client.println();
    }

    void ShellminatorWebServerThemedOffline::processIndex(){
        SHELLMINATOR_WEBS_DBGLN( __CONST_TXT__( "Processing Index Page" ) );
        client.println( __CONST_TXT__( "HTTP/1.1 200 OK" ) );
        client.println( __CONST_TXT__( "Content-type: text/html" ) );
        client.println();

        // the content of the HTTP response follows the header:
        #ifdef ESP8266
            client.write_P( indexThemedOfflineResponse, SHELLMINATOR_WEB_XTERM_THEMED_OFFLINE_SIZE );
        #else
            client.write( indexThemedOfflineResponse, SHELLMINATOR_WEB_XTERM_THEMED_OFFLINE_SIZE );
        #endif
    }

    void ShellminatorWebServerThemedOffline::processXtermJs(){
        SHELLMINATOR_WEBS_DBGLN( __CONST_TXT__( "Processing Xterm JS file" ) );
        client.println( __CONST_TXT__( "HTTP/1.1 200 OK" ) );
        client.println( __CONST_TXT__( "Content-type: text/javascript" ) );
        client.println();

        // the content of the HTTP response follows the header:
        #ifdef ESP8266
            client.write_P( xtermMinJsResponse, XTERM_MIN_JS_SIZE );
        #else
            client.write( xtermMinJsResponse, XTERM_MIN_JS_SIZE );
        #endif
    }

    void ShellminatorWebServerThemedOffline::processXtermWeblinks(){
        SHELLMINATOR_WEBS_DBGLN( __CONST_TXT__( "Processing Xterm Weblinks file" ) );
        client.println( __CONST_TXT__( "HTTP/1.1 200 OK" ) );
        client.println( __CONST_TXT__( "Content-type: text/javascript" ) );
        client.println();

        // the content of the HTTP response follows the header:
        #ifdef ESP8266
            client.write_P( xtermWeblinksJsResponse, XTERM_ADDON_WEB_LINKS_MIN_JS_SIZE );
        #else
            client.write( xtermWeblinksJsResponse, XTERM_ADDON_WEB_LINKS_MIN_JS_SIZE );
        #endif
    }

    void ShellminatorWebServerThemedOffline::processURL( const char* url_p ){
        SHELLMINATOR_WEBS_DBG( __CONST_TXT__( "URL: " ) );
        SHELLMINATOR_WEBS_DBGLN( url_p );
        if( strcmp( url_p, "/" ) == 0 ){
            processRoot();
        }
        else if( strcmp( url_p, "/index.html" ) == 0 ){
            processIndex();
        }
        else if( strcmp( url_p, "/xterm.min.js" ) == 0 ){
            processXtermJs();
        }
        else if( strcmp( url_p, "/addon-web-links.min.js" ) == 0 ){
            processXtermWeblinks();
        }
        else{
            processNotFound( url_p );
        }
    }

    void ShellminatorWebServerThemedOffline::processNotFound( const char* url_p ){
        SHELLMINATOR_WEBS_DBG( __CONST_TXT__( "Not Found Page: " ) );
        SHELLMINATOR_WEBS_DBGLN( url_p );
        client.println( __CONST_TXT__( "HTTP/1.1 404 OK" ) );
        client.println( __CONST_TXT__( "Content-type: text/html" ) );
        client.println();

        // the content of the HTTP response follows the header:
        #ifdef ESP8266
            client.write_P( notFoundThemedResponse, SHELLMINATOR_404_THEMED_SIZE );
        #else
            client.write( notFoundThemedResponse, SHELLMINATOR_404_THEMED_SIZE );
        #endif
    }

#endif