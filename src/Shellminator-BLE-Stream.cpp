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

#include "Shellminator-BLE-Stream.hpp"

//#include "BLEDevice.h"
//#include "BLECharacteristic.h"


#ifdef SHELLMINATOR_USE_BLE

    #ifdef ARDUINO_ARCH_NRF52840
        ShellminatorBleStream* ShellminatorBleStream::instance = NULL;
    #endif

    ShellminatorBleStream::ShellminatorBleStream(){
        dbg = NULL;

        #ifdef ESP32
            serverCallbacks.dbg = NULL;
            serverCallbacks.parent = this;

            rxCallback.dbg = NULL;
            rxCallback.parent = this;
        #elif ARDUINO_ARCH_NRF52840
            if( ShellminatorBleStream::instance == NULL ){
                ShellminatorBleStream::instance = this;
            }
        #endif

        resetVariables();
    }

    void ShellminatorBleStream::resetVariables(){
        memset( streamBuffer, 0, SHELLMINATOR_BLE_STREAM_BUFFER_SIZE );
        streamBufferWritePointer = 0;
        streamBufferReadPointer = 0;
        state = BLE_DISCONNECTED_STATE;
    }

    void ShellminatorBleStream::begin(){

        #ifdef ESP32
            // Create the BLE Device
            BLEDevice::init( SHELLMINATOR_BLE_SERVICE_NAME );

            // Create the BLE Server
            bleServer = BLEDevice::createServer();
            bleServer -> setCallbacks( &serverCallbacks );

            // Create the BLE Service
            bleService = bleServer -> createService( NUS_SERVICE_UUID );

            // Create TX characteristics.
            bleTxChar = bleService -> createCharacteristic( NUS_TX_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY );
            bleTxChar -> addDescriptor( new BLE2902() );

            // Create RX characteristics.
            bleRxChar = bleService -> createCharacteristic( NUS_RX_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE );
            bleRxChar -> setCallbacks( &rxCallback );

            // Start the service
            bleService -> start();

            // Start advertising
            bleServer -> getAdvertising() -> start();

            SHELLMINATOR_BLE_DBGLN( "NUS service started with ESP32 BLE drivers." );

        #elif ARDUINO_ARCH_NRF52840

            if( !BLE.begin() ){
                SHELLMINATOR_BLE_DBGLN( "BLE Init Error!" );
                return;
            }
            BLE.setLocalName( SHELLMINATOR_BLE_SERVICE_NAME );
            //BLE.setDeviceName( SHELLMINATOR_BLE_SERVICE_NAME );

            BLE.setAdvertisedService( bleService );
            bleService.addCharacteristic( bleRxChar );
            bleService.addCharacteristic( bleTxChar );
            bleRxChar.setEventHandler( BLEWritten, ShellminatorBleStream::bleRxCallback );
            BLE.addService( bleService );
            BLE.advertise();
            BLE.setEventHandler( BLEConnected, onConnect );
            BLE.setEventHandler( BLEDisconnected, onDisconnect );
            SHELLMINATOR_BLE_DBGLN( "NUS service started with ESP32 BLE drivers." );

        #endif

    }

    void ShellminatorBleStream::update(){
        #ifdef ARDUINO_ARCH_NRF52840
            BLE.poll();
        #endif
    }

    void ShellminatorBleStream::attachDebugChannel( Stream* dbg_p ){
        dbg = dbg_p;
        #ifdef ESP32
            serverCallbacks.dbg = dbg_p;
            rxCallback.dbg = dbg_p;
        #endif
    }

    #ifdef ESP32
        void ShellminatorBleStream::ServerCallbacks::onConnect( BLEServer* serverPtr ){
            parent -> state = BLE_CONNECTED_STATE;
            SHELLMINATOR_BLE_DBGLN( "Client connected to BLE server." );
        }

        void ShellminatorBleStream::ServerCallbacks::onDisconnect( BLEServer* serverPtr ){
            parent -> state = BLE_DISCONNECTED_STATE;
            parent -> resetVariables();
            SHELLMINATOR_BLE_DBGLN( "Client disconnected from BLE server." );
            parent -> bleServer -> startAdvertising();
            SHELLMINATOR_BLE_DBGLN( "Start advertising again." );
        }

        void ShellminatorBleStream::bleRxCallback::onWrite( BLECharacteristic *charPtr ){
            int i;
            int num;
            std::string dataStr;

            dataStr = charPtr -> getValue();
            num = dataStr.length();

            if( num > 0 ){
                SHELLMINATOR_BLE_DBGLN( "Data from BLE client:" );
                for( i = 0; i < num; i++ ){
                    SHELLMINATOR_BLE_DBG( i );
                    SHELLMINATOR_BLE_DBG( __CONST_TXT__( "\t0x" ) );
                    SHELLMINATOR_BLE_DBG( dataStr[ i ], HEX );
                    SHELLMINATOR_BLE_DBG( __CONST_TXT__( "\t" ) );
                    SHELLMINATOR_BLE_DBGLN( (int)dataStr[ i ] );
                    parent -> appendToCircularBuffer( (uint8_t)dataStr[ i ] );
                }
            }

        }

    #elif ARDUINO_ARCH_NRF52840
        void ShellminatorBleStream::bleRxCallback( BLEDevice device, BLECharacteristic characteristics ){
            int i;
            int num;
            const uint8_t* dataStr;

            Serial.println( "Hurray!" );

            num = characteristics.valueLength();
            dataStr = characteristics.value();

            if( ShellminatorBleStream::instance == NULL ){
                return;
            }

            if( num > 0 ){
                if( ShellminatorBleStream::instance-> dbg ){
                    ShellminatorBleStream::instance-> dbg -> println( "Data from BLE client:" );
                }
                for( i = 0; i < num; i++ ){
                    if( ShellminatorBleStream::instance-> dbg ){
                        ShellminatorBleStream::instance-> dbg -> print( i );
                        ShellminatorBleStream::instance-> dbg -> print( __CONST_TXT__( "\t0x" ) );
                        ShellminatorBleStream::instance-> dbg -> print( dataStr[ i ], HEX );
                        ShellminatorBleStream::instance-> dbg -> print( __CONST_TXT__( "\t" ) );
                        ShellminatorBleStream::instance-> dbg -> println( (int)dataStr[ i ] );
                    }
                    ShellminatorBleStream::instance -> appendToCircularBuffer( (uint8_t)dataStr[ i ] );
                }
            }
            //ShellminatorBleStream::instance -> appendToCircularBuffer( (uint8_t*)characteristics.value(), characteristics.valueLength() );
        }
        
        void ShellminatorBleStream::onConnect( BLEDevice device ){
            if( ShellminatorBleStream::instance == NULL ){
                return;
            }

            ShellminatorBleStream::instance -> state = BLE_CONNECTED_STATE;

            if( ( ShellminatorBleStream::instance-> dbg ) == NULL ){
                return;
            }

            ShellminatorBleStream::instance -> dbg -> println( "Client connected to BLE server." );
        }
        void ShellminatorBleStream::onDisconnect( BLEDevice device ){
            if( ShellminatorBleStream::instance == NULL ){
                return;
            }

            ShellminatorBleStream::instance -> state = BLE_DISCONNECTED_STATE;
            ShellminatorBleStream::instance -> resetVariables();
            
            if( ( ShellminatorBleStream::instance-> dbg ) == NULL ){
                return;
            }

            ShellminatorBleStream::instance -> dbg -> println( "Client disconnected from BLE server." );
            ShellminatorBleStream::instance -> dbg -> println( "Start advertising again." );
        }
    #endif

    void ShellminatorBleStream::appendToCircularBuffer( uint8_t data ){
        streamBuffer[ streamBufferWritePointer ] = data;

        // Increment the write pointer and handle wrapping.
        streamBufferWritePointer++;
        if( streamBufferWritePointer >= SHELLMINATOR_BLE_STREAM_BUFFER_SIZE ){
            streamBufferWritePointer = 0;
        }

        // Detect buffer overflow
        if( streamBufferWritePointer == streamBufferReadPointer ){
            SHELLMINATOR_BLE_DBGLN( __CONST_TXT__( "BLE Circular Buffer Overflow!" ) );
            #ifdef ESP32
                bleService -> stop();
            #endif
        }
    }

    void ShellminatorBleStream::appendToCircularBuffer( uint8_t* data, int dataSize ){
        int i;
        for( i = 0; i < dataSize; i++ ){
            appendToCircularBuffer( data[ i ] );
        }
    }

    int ShellminatorBleStream::available(){
        if( state != BLE_CONNECTED_STATE ){
            return 0;
        }

        if( streamBufferWritePointer == streamBufferReadPointer ){
            return 0;
        }
        else if( streamBufferWritePointer > streamBufferReadPointer ){
            return streamBufferWritePointer - streamBufferReadPointer;
        }
        return SHELLMINATOR_BLE_STREAM_BUFFER_SIZE - streamBufferReadPointer + streamBufferWritePointer;
    }

    int ShellminatorBleStream::read(){
        int ret;
        
        if( state != BLE_CONNECTED_STATE ){
            return -1;
        }
        
        if( streamBufferWritePointer == streamBufferReadPointer ){
            return -1;
        }
        ret = (uint8_t) streamBuffer[ streamBufferReadPointer ];
        streamBufferReadPointer++;

        if( streamBufferReadPointer >= SHELLMINATOR_BLE_STREAM_BUFFER_SIZE ){
            streamBufferReadPointer = 0;
        }

        return ret;
    }

    int ShellminatorBleStream::peek(){
        if( state != BLE_CONNECTED_STATE ){
            return -1;
        }

        if( streamBufferWritePointer == streamBufferReadPointer ){
            return -1;
        }
        return (uint8_t) streamBuffer[ streamBufferReadPointer ];
    }

    void ShellminatorBleStream::flush(){
        // Honestly I don't know what to do.
        // Arduino flush methods are weird.
    }

    size_t ShellminatorBleStream::write( uint8_t b ){
        if( state != BLE_CONNECTED_STATE ){
            return 0;
        }

        #ifdef ESP32
            bleTxChar -> setValue( &b, 1 );
            bleTxChar -> notify();
            return 1;
        #elif ARDUINO_ARCH_NRF52840
            bleTxChar.writeValue( b );
            delay( 30 );
            return 1;
        #endif
    }

    size_t ShellminatorBleStream::write(const uint8_t *data, size_t size){
        if( state != BLE_CONNECTED_STATE ){
            return 0;
        }
        #ifdef ESP32
            bleTxChar -> setValue( (uint8_t*) data, size );
            bleTxChar -> notify();
            return size;
        #elif ARDUINO_ARCH_NRF52840
            int i = 0;
            int cntr = 0;
            for( i = 0; i < size; i++ ){
                bleTxBuffer[ cntr ] = data[ i ];
                cntr++;
                if( cntr >= SHELLMINATOR_BLE_STREAM_BUFFER_SIZE ){
                    bleTxChar.writeValue( bleTxBuffer, cntr );
                    delay( 30 );
                    cntr = 0;
                }
            }
            bleTxChar.writeValue( bleTxBuffer, cntr );
            delay( 30 );
            
            return size;
        #endif
    }

#endif