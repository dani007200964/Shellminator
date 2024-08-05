#include "BLEProperty.h"
#include "BLECharacteristic.h"
#include "BLEService.h"
/*
 * Created on July 20 2024
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified July 20 2024
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

#ifndef SHELLMINATOR_BLE_STREAM_HPP_
#define SHELLMINATOR_BLE_STREAM_HPP_

    #include "Stream.h"
    #include "Shellminator-DefaultSettings.hpp"
    #include "Shellminator-Helpers.hpp"
    #include <string.h>

    // Todo add hooks to connection and disconnection events!
    // Todo add SHELLMINATOR_BLE_DBG_ENABLE to defaultSettings.

    #ifdef ARDUINO
        #include "Arduino.h"
    #endif

    #ifdef ESP32
        #include <BLEDevice.h>
        #include <BLEServer.h>
        #include <BLEUtils.h>
        #include <BLE2902.h>
    #endif

    #ifdef ARDUINO_ARCH_NRF52840
        #include <ArduinoBLE.h>
    #endif

    #define SHELLMINATOR_USE_BLE

    #ifdef SHELLMINATOR_USE_BLE
        #define SHELLMINATOR_BLE_SERVICE_NAME "Shellminator BLE UART"

        // Using NUS - Nordic Uart Service
        #define NUS_SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
        #define NUS_RX_CHAR_UUID        "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
        #define NUS_TX_CHAR_UUID        "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

        // Macro magic for macro overloading.
        #define SHELLMINATOR_BLE_DBG_OVERLOAD( _1, _2, NAME, ... ) NAME

        #define SHELLMINATOR_BLE_DBG_ENABLE

        #ifdef SHELLMINATOR_BLE_DBG_ENABLE
            #define SHELLMINATOR_BLE_DBG_1(   x ) if( dbg ){ dbg -> print( x );   }
            #define SHELLMINATOR_BLE_DBGLN_1( x ) if( dbg ){ dbg -> println( x ); }

            #define SHELLMINATOR_BLE_DBG_2(   x, y ) if( dbg ){ dbg -> print( x, y );   }
            #define SHELLMINATOR_BLE_DBGLN_2( x, y ) if( dbg ){ dbg -> println( x, y ); }
        #elif
            #define SHELLMINATOR_BLE_DBG_1(   x )
            #define SHELLMINATOR_BLE_DBGLN_1( x )

            #define SHELLMINATOR_BLE_DBG_2(   x, y )
            #define SHELLMINATOR_BLE_DBGLN_2( x, y )
        #endif

        /// Macro to print debug message.
        ///
        /// The syntax is the same as a regular Stream.print() function.
        /// It will check if a debug channel is attached and only then
        /// will print the debug message.
        #define SHELLMINATOR_BLE_DBG( ... )   SHELLMINATOR_BLE_DBG_OVERLOAD( __VA_ARGS__, SHELLMINATOR_BLE_DBG_2,   SHELLMINATOR_BLE_DBG_1   )( __VA_ARGS__ )
        #define SHELLMINATOR_BLE_DBGLN( ... ) SHELLMINATOR_BLE_DBG_OVERLOAD( __VA_ARGS__, SHELLMINATOR_BLE_DBGLN_2, SHELLMINATOR_BLE_DBGLN_1 )( __VA_ARGS__ )

        #define SHELLMINATOR_BLE_STREAM_BUFFER_SIZE 200

        class ShellminatorBleStream : public Stream{
            public:

                #ifdef ARDUINO_ARCH_NRF52840
                    static ShellminatorBleStream* instance;
                    static void bleRxCallback( BLEDevice device, BLECharacteristic characteristics );
                    static void onConnect( BLEDevice device );
                    static void onDisconnect( BLEDevice device );
                #endif

                typedef enum{
                    BLE_DISCONNECTED_STATE,
                    BLE_CONNECTED_STATE
                } bleState_t;

                ShellminatorBleStream();

                /// Available bytes in the channel.
                ///
                /// @returns The available bytes in the channel.
                int    available() override;

                /// Read one byte form the channel.
                ///
                /// @returns Read and return one byte form the channel. The byte will be removed from the channel.
                int    read() override;

                /// Peek the firtst byte from the channel.
                ///
                /// @returns Read and return one byte form the channel. The byte will NOT be removed from the channel.
                int    peek() override;

                /// Flush the channel.
                void   flush() override;

                /// Write one byte to the channel.
                ///
                /// @param b The value that has to be written to the channel.
                /// @returns The number of bytes that has been successfully written to the channel. Because it is the base class, it returns 0.
                size_t write( uint8_t b ) override;

                size_t write(const uint8_t *data, size_t size) override;

                void begin();
                void update();
                void attachDebugChannel( Stream* dbg_p );

                bleState_t state;
            
            private:

                uint8_t streamBuffer[ SHELLMINATOR_BLE_STREAM_BUFFER_SIZE ];
                uint32_t streamBufferWritePointer;
                uint32_t streamBufferReadPointer;

                Stream* dbg;

                void resetVariables();
                void appendToCircularBuffer( uint8_t data );
                void appendToCircularBuffer( uint8_t* data, int dataSize );

                #ifdef ESP32
                    BLEServer *bleServer = NULL;
                    BLEService *bleService = NULL;
                    BLECharacteristic *bleTxChar;
                    BLECharacteristic *bleRxChar;

                    
                    class ServerCallbacks: public BLEServerCallbacks {
                        public:
                            void onConnect( BLEServer* serverPtr ) override;
                            void onDisconnect( BLEServer* serverPtr ) override;
                            void attachDebugChannel( Stream* dbg_p );
                            ShellminatorBleStream* parent = NULL;
                            Stream* dbg = NULL;
                    };

                    class bleRxCallback: public BLECharacteristicCallbacks {
                        public:
                            void onWrite( BLECharacteristic *charPtr ) override;
                            void attachDebugChannel( Stream* dbg_p );
                            ShellminatorBleStream* parent = NULL;
                            Stream* dbg = NULL;
                    };

                    ServerCallbacks serverCallbacks;
                    bleRxCallback rxCallback;

                #elif ARDUINO_ARCH_NRF52840
                    uint8_t bleTxBuffer[ SHELLMINATOR_BLE_STREAM_BUFFER_SIZE ];
                    BLEService bleService = BLEService( NUS_SERVICE_UUID );
                    BLECharacteristic bleTxChar = BLECharacteristic( NUS_TX_CHAR_UUID, BLENotify, SHELLMINATOR_BLE_STREAM_BUFFER_SIZE );
                    BLECharacteristic bleRxChar = BLECharacteristic( NUS_RX_CHAR_UUID, BLEWrite, SHELLMINATOR_BLE_STREAM_BUFFER_SIZE );
                #endif
        };

    #endif

#endif