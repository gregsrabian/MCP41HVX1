
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

#ifndef MCP41HVX1_h
#define MCP41HVX1_h

#include <Arduino.h>
#include <SPI.h> 

/*
Wiring Definition:
+-----------------------------+---------------------------------
|   Arduino Uno               |   MCP41HVX1
| Pin | Description           | Pin | Description
+-----+-----------------------+-----+---------------------------
| +5v | 3.3v is also supported|  1  | Power 1.8v to 5.5v 
|  13 | SPI: SCK              |  2  | SCK
|  10 | CS (any digital pin)  |  3  | CS
|  11 | SPI: MOSI             |  4  | SDI
|  12 | SPI: MISO             |  5  | SDO
|   8 | Any Digital Pin       |  6  | WLAT
|   9 | Any Digital Pin       |  7  | SHDN
| GND |                       |  8  | NC
| GND |                       |  9  | GND
| GND |                       | 10  | V-    (Connect to external power supply max 36 volts. Common ground with Arduino
|     |                       | 11  | POB   (This is the potentiometer)
|     |                       | 12  | POW   (This is the wiper of the potentiometer)
|     |                       | 13  | POA   (This is the potentiometer)
|     |                       | 14  | V+    (Connect to external power supply max 36 volts)


Alternate Wiring Definition (uses less Arduino pins with some loss of features):
+-----------------------------+---------------------------------
|   Arduino Uno               |   MCP41HVX1
| Pin | Description           | Pin | Description
+-----+-----------------------+-----+---------------------------
| +5v | 3.3v is also supported|  1  | Power 1.8v to 5.5v 
|  13 | SPI: SCK              |  2  | SCK
|  10 | CS (any digital pin)  |  3  | CS
|  11 | SPI: MOSI             |  4  | SDI
|  12 | SPI: MISO             |  5  | SDO
| +5v | 3.3v is also supported|  6  | WLAT - wiring directly to HIGH (Arudino power) forces WLAT to always be enabled. Pass MCP41HVX1_PIN_NOT_CONFIGURED in the constructor for WLAT parameter.
| GND |                       |  7  | SHDN - wiring directly to Arduino GND forces the resistor network to always be enabled. Pass MCP41HVX1_PIN_NOT_CONFIGURED in the constructor for SHDN parameter.
| GND |                       |  8  | NC
| GND |                       |  9  | GND
| GND |                       | 10  | V-    (Connect to external power supply max 36 volts. Common ground with Arduino
|     |                       | 11  | POB   (This is the potentiometer)
|     |                       | 12  | POW   (This is the wiper of the potentiometer)
|     |                       | 13  | POA   (This is the potentiometer)
|     |                       | 14  | V+    (Connect to external power supply max 36 volts)

	The consturctor that only accepts the SS pin can be used if wiring both WLAT directly to Arudino power and wiring SHDN directly to Arduino GND.

Supported chips in the MCP41HVX1 family:

	 Chip Name	     |  Taps  | Resistance | Package Type
	---------------------+-------_+------------+------------------------
	 MCP41HV31-104E/ST   | 7 bits |	 100k ohm  | TSSOP-14
	 MCP41HV31-503E/ST   | 7 bits |   50k ohm  | TSSOP-14	
	 MCP41HV31-103E/ST   | 7 bits |   10k ohm  | TSSOP-14
	 MCP41HV31-502E/ST   | 7 bits |	   5k ohm  | TSSOP-14	
	 MCP41HV31-103E/MQ   | 7 bits |	  10k ohm  | QFN 
	 MCP41HV51-104E/ST   | 8 bits |	 100k ohm  | TSSOP-14
	 MCP41HV51-503E/ST   | 8 bits |   50k ohm  | TSSOP-14
	 MCP41HV51T-503E/ST  | 8 bits |   50k ohm  | TSSOP-14 (Cut Tape)	
	 MCP41HV51-103E/ST   | 8 bits |   10k ohm  | TSSOP-14		
	 MCP41HV51-502E/ST   | 8 bits |    5k ohm  | TSSOP-14	
*/

// SPI Command protocol
// Address  (4 bits)  0000  For the Volatile Wiper (supports Read\Write\Increment\Decrement)
// Commands (2 bits)  00 - Write direct value
//                    01 - Increment
//                    10 - Decrement
//                    11 - Read
// Error    (2 bits)  00 - Used only on read \ otherwise ignored
//
// Read & Write use a second byte which contains the wiper position

#define MCP41HVX1_PIN_NOT_CONFIGURED  -32768
#define MCP41HVX1_STATE_ENABLED       true
#define MCP41HVX1_STATE_DISABLED      false

class MCP41HVX1
{
  public:
    MCP41HVX1( int nCSPin);
    MCP41HVX1( int nCSPin,  int nSHDNPin, int nWLATPin);
    MCP41HVX1( int nCSPin,  int nSHDNPin, bool bSHDNState, int nWLATPin, bool WLATState);
 
    void  ResistorNetworkDisable();
    void  ResistorNetworkEnable();
    int   ResistorNetworkGetState();
  
    byte  WiperGetPosition();
    byte  WiperSetPosition( byte byWiper);
    byte  WiperIncrement();
    byte  WiperDecrement();

    void  WLATDisable();
    void  WLATEnable();
    int   WLATGetState();

  private:
    int   __nCSPin;
    int   __nWLATPin;
    int   __nSHDNPin;
    bool  __bResistorNetworkState;
    bool  __bWLATState;
};

#endif
