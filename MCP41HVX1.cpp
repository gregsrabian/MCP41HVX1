

/*
  This file is part of the MCP41HVX1 library.
  Copyright (c) 2020 Greg Srabian. All rights reserved.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*
 * Class to control the MCP41HVX1 family of digital potentiometers
 * 
 * Datasheet: http://ww1.microchip.com/downloads/en/devicedoc/20005207b.pdf
 */

#include "MCP41HVX1.h"

// Contructor to use if WLAT and SHDN pins are connected directly to low (for SHDN) and high (for WLAT)
MCP41HVX1::MCP41HVX1( int nCSPin) : MCP41HVX1( nCSPin, MCP41HVX1_PIN_NOT_CONFIGURED, MCP41HVX1_PIN_NOT_CONFIGURED)
{
}

// Constructor to use if all 3 pins are connected (or caller passes in MCP41HVX1_PIN_NOT_CONFIGURED) and the default behavior of enabling WLAT and enabling the resistor network are required
MCP41HVX1::MCP41HVX1( int nCSPin, int nSHDNPin, int nWLATPin) : MCP41HVX1( nCSPin, nSHDNPin, MCP41HVX1_STATE_ENABLED, nWLATPin, MCP41HVX1_STATE_ENABLED)
{   
}

// Constructor to use if full control is needed over initialization. Caller can decide if either WLAT or SHDN are to be enabled or disabled in the constructor
MCP41HVX1::MCP41HVX1( int nCSPin, int nSHDNPin, bool bSHDNState, int nWLATPin, bool bWLATState)
{
    // Save the pin and initialization state arguments into the class variables
    __nCSPin   = nCSPin;
    __nSHDNPin = nSHDNPin;            // Set to MCP41HVX1_PIN_NOT_CONFIGURED if SHDN is not connected
    __nWLATPin = nWLATPin;            // Set to MCP41HVX1_PIN_NOT_CONFIGURED if WLAT is not connected

    __bResistorNetworkState = bSHDNState;
    __bWLATState = bWLATState;

    SPI.begin();                      // Initialize SPI which is used to communicate to the MCP41HVX1 chip

    pinMode( __nCSPin, OUTPUT );     // Set the SS pin to OUTPUT

    if( MCP41HVX1_PIN_NOT_CONFIGURED != __nSHDNPin)             // If SHDN Pin is connected then configure it
    {
        pinMode( __nSHDNPin, OUTPUT); 							// Set SHDN Pin to output 
        if( MCP41HVX1_STATE_ENABLED == bSHDNState)
        {
            ResistorNetworkEnable(); 
        }
        else
        {
            ResistorNetworkDisable(); 
        }
    }

    if( MCP41HVX1_PIN_NOT_CONFIGURED != __nWLATPin)             // If WLAT Pin is connected then configure it
    {
        pinMode( __nWLATPin, OUTPUT); 							// Set the WLAT Pin to output
        if( MCP41HVX1_STATE_ENABLED == bWLATState)				
        {
            WLATEnable();
        }
        else
        {
            WLATDisable(); 
        }
    }
}

void MCP41HVX1::WLATEnable()
{
    if( MCP41HVX1_PIN_NOT_CONFIGURED != __nWLATPin)
    {
        digitalWrite( __nSHDNPin, HIGH); // Set the SHDN Pin to HIGH to enable WLAT
        __bWLATState = MCP41HVX1_STATE_ENABLED;
    }
}


void MCP41HVX1::WLATDisable()
{
    if( MCP41HVX1_PIN_NOT_CONFIGURED != __nWLATPin)
    {
        digitalWrite( __nSHDNPin, LOW); // Set the SHDN Pin to LOW to disble WLAT
        __bWLATState = MCP41HVX1_STATE_DISABLED;
    }
}

int   MCP41HVX1::WLATGetState()
{
  return( __bWLATState);
}


void MCP41HVX1::ResistorNetworkDisable()
{
    if( MCP41HVX1_PIN_NOT_CONFIGURED != __nSHDNPin)
    {
        digitalWrite( __nSHDNPin, LOW); // Set the SHDN Pin to LOW to disable the resistor network
        __bResistorNetworkState = MCP41HVX1_STATE_DISABLED;
    } 
}

void MCP41HVX1::ResistorNetworkEnable()
{
    if( MCP41HVX1_PIN_NOT_CONFIGURED != __nSHDNPin)
    {
        digitalWrite( __nSHDNPin, HIGH); // Set the SHDN Pin to HIGH to enable the resistor network
        __bResistorNetworkState = MCP41HVX1_STATE_ENABLED;
    }
}

int MCP41HVX1::ResistorNetworkGetState()
{
  return( __bResistorNetworkState);
}

byte MCP41HVX1::WiperSetPosition( byte byWiper)
{
    digitalWrite( __nCSPin, LOW );
    SPI.transfer( 0x00);          // The command for wiper set position
    SPI.transfer( byWiper);       // The write command expects a second byte which is the value for the wiper
    digitalWrite( __nCSPin, HIGH);

    return( WiperGetPosition());
}

byte MCP41HVX1::WiperIncrement()
{
    digitalWrite( __nCSPin, LOW );
    SPI.transfer( 0x04);          // The command for wiper up (00000100 = 0x04)
    digitalWrite( __nCSPin, HIGH);

    return( WiperGetPosition());
}

byte MCP41HVX1::WiperDecrement()
{
    digitalWrite( __nCSPin, LOW );
    SPI.transfer( 0x08);          // The command for wiper up (00001000 = 0x08)
    digitalWrite( __nCSPin, HIGH);

    return( WiperGetPosition());
}

byte MCP41HVX1::WiperGetPosition()
{
    int rc = 0;
    digitalWrite( __nCSPin, LOW );
    rc = SPI.transfer16( 0x0C00); 	// The command for requesting the wiper position. The second byte sent has no useful info but it request 2 bytes sent and responds with 2 bytes  
    digitalWrite( __nCSPin, HIGH); 

    return( rc & 0xff);				// The wiper position is in the low order byte so & with 0xff to remove the extra info.
}
