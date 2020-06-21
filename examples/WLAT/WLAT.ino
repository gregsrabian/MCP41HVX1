/*
  WLAT.ino - Example for controlling the MCP41HVX1 family of digital potentiometers. This examples demonstraits how to control the WLAT functionality. 


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
Example for driving the mixed signal MCP41HVX1 family of digital potentiometers (digipots)

This sketch demonstrates creating an instance of the MCP41HVX1 class and using the WiperSetPosition(), WiperGetPosition(), WLATEnable(), and WLATDisable() methods.

To view the resistance change you will need to use a multimeter set to read ohms. Connect the probes to the wiper P0W (pin 12) and either P0A (pin 13) or P0B (pin 11) of 
the digipot. If you connect the probes to P0A and P0B instead of connecting to the wiper you will always see a reading of max resistance.

Make sure to modify the #define MAX_WIPER_VALUE in this sketch to set it to the max number of taps for the digipot that you are using. Look in the table below for your 
chip. In the Taps column it will show either 7 or 8 bits. If it is 7 bits set MAX_WIPER_VALUE to 127. If it is 8 bits set MAX_WIPER_VALUE to 255.


	 Chip Name	     |  Taps  | Resistance | Package Type
	---------------------+--------+------------+------------------------
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

Additional information on the digipot can be found in the data sheet which is available at http://ww1.microchip.com/downloads/en/devicedoc/20005207b.pdf.

To wire this up using an Arduino UNO you will need to make the following connections (see Wiring Definition below). For boards other than the Uno determine what pins are used 
for MOSI, MISO, and CLK which are for the SPI interface. For SS, WLAT, and SHDN pins you can use any digital pin available.


Wiring Definition:
+----------------------------+---------------------------------
|   Arduino Uno              |   MCP41HVX1
| Pin | Description          | Pin | Description
+-----+----------------------+-----+---------------------------
| +5v | 3.3v also supported  |  1  | Power 1.8v to 5.5v 
|  13 | SPI: SCK             |  2  | SCK
|  10 | CS (any digital pin) |  3  | CS
|  11 | SPI: MOSI            |  4  | SDI
|  12 | SPI: MISO            |  5  | SDO
|   8 | Any Digital Pin      |  6  | WLAT
|   9 | Any Digital Pin      |  7  | SHDN
| GND |                      |  8  | NC
| GND |                      |  9  | GND
| GND |                      | 10  | V-    (Connect to external power supply max 36 volts. Common ground with Arduino
|     |                      | 11  | POB   (This is the potentiometer)
|     |                      | 12  | POW   (This is the wiper of the potentiometer)
|     |                      | 13  | POA   (This is the potentiometer)
|     |                      | 14  | V+    (Connect to external power supply max 36 volts)
*/

#include "MCP41HVX1.h"


// Define the pins used on the Arduino
#define WLAT_PIN  8    // If set to Low "transfer and use" is treated as a single operation. If set to high data is transfered but not used until pin goes low.
#define SHDN_PIN  9    // Set high to enable the resistor network
#define CS_PIN   10    // Set to low to select chip for SPI communications and high to deselect the chip. This allows the SPI interface to be shared among multiple devices.


// Define some values used for the test app
#define FORWARD true
#define REVERSE false
#define MAX_WIPER_VALUE 255     // This is the maximum wiper value which will be either 127 or 255. Check the data sheet for the digipot being used to determine how many taps (either 7 bit or 8 bit) are supported 


MCP41HVX1 Digipot( CS_PIN, SHDN_PIN, WLAT_PIN);     // Create an instance of the MCP41HVX1 class for controlling the digipot
 
void setup ()
{
    Serial.begin( 9600);

    Serial.print( "Starting Position = ");
    Serial.println( Digipot.WiperGetPosition());    // Display the initial value at power start up of the digipot. This will most likely be different every time the Arduino is restarted.

    Serial.print( "Set Wiper Position = ");
    Serial.println( Digipot.WiperSetPosition( 0));  // Set the wiper position to 0
}


void loop ()
{
  Serial.println( "-----------------------------");
  Digipot.WiperSetPosition( 0);
  Serial.println( "Wiper Position 0 - multimeter shows close to 0 ohms");

  Serial.println("");
  Digipot.WLATDisable();
  Serial.println( "Disable WLAT - multimeter reading does not change - Sleep for 4 seconds");
  delay( 4000);

  Serial.println( "");
  Digipot.WiperSetPosition( MAX_WIPER_VALUE / 2);
  Serial.print( "Wiper position ");
  Serial.print( MAX_WIPER_VALUE / 2);
  Serial.println( "- multimeter reading does not change - Sleep for 4 seconds");
  delay(4000);
  
  Serial.println( "");
  Digipot.WLATEnable();
  Serial.println( "Enable WLAT- multimeter reading shows close to 50% of the max resistance");
  Serial.println( "Sleep for 10 seconds - multimeter reading does not change");
  delay( 10000);

  Serial.println( "Done");
}
