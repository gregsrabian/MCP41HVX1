# MCP41HVX1 Digital Potentiometer Driver 

**[*Scoll to the bottom if you are looking for the License.*]**

The MCP41HVX1 family of chips are digitial potentiometers (also known as digipots). They allow for full control from a device which can send and receive commands over an SPI bus. This means that they are compatible with Arduio. 

There are other digipots available that are compatible with Arduino so why use MCP41HVX1? Most digipots that work with Arudio expect the same voltage range thru the resistor network as what is used to control the digipot. This means that you are limited to 5 volts thru the resistor network.

The MCP41HVX1 family of chips have a split rail design. This means that the control voltage is different than the voltage passed thru the resistor network. MCP41HVX1 chips can be controlled with 1.8 to 5.5v which makes them compatible with Aurdio but allow for up to 36 volts thru the resistor network.

```
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
	
		(Chips in the MCP41HVX1 family)
```

The internal architecture of the MPC41HVX1 chips are actually two seperate devices inside a single package. There is the resistor network itself with higer voltage input as well as the potentiometer connections (P0A, P0W, and POB). The other device is the part that communicates with the Arduino which contains the SPI communications and registers to hold the commands and data that is sent on the SPI bus. The connection between both internal devices is WLAT.

Due to this architecture there are more pins on an MCP41HVX1 chip than you might find on other single potentiometer digipots.  There are two extra pins for a higher voltage to drive the resistor network. There are also extra pins for WLAT and SHDN which allow for greater control of the chip.

WLAT is used to control when the wiper position information sent to the chip via the SPI interface is transfered to the resistor network to update the wiper position. If you leave WLAT low then any new wiper position data that is received via the SPI interface is automaticaly passed to the resister network and the wiper position is updated. If you have WLAT set to high any incoming wiper information is held in the register but the resistor network wiper position is not updated until the point WLAT is set to low again. WLAT can be wired directly to GND to force it low all the time which causes all wiper position informatoin that is passed to the chip to automatically update the wiper position immediately on the resistor network.

SHDN allows control of disabling the resistor network. If SHDN is set to low, P0A is disconnected and P0B is connected directly to P0W. Since there is some internal resistance, measuring the resistance across P0B and P0W will show close to 0. Setting SHDN to high enables the resistor network so that when you adjust the wiper position you will see resistance change on P0W. You can directly wire SHDN to Arduio + (positive) power to force it to high which will keep the resistor network enabled at all time. 

See the alternate wiring diagram if you are wiring either WLAT or SHDN to power pins instead of control pins. While both WLAT and SHDN may be wired to power pins instead of controlable pins, SS must always be wired to a controlable (digital) pin.

### Wiring Diagram ###
```
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
```

### Alternate Wiring Definition (uses less Arduino pins with some loss of features): ###
```
+-----------------------------+---------------------------------
|   Arduino Uno               |   MCP41HVX1
| Pin | Description           | Pin | Description
+-----+-----------------------+-----+---------------------------
| +5v | 3.3v is also supported|  1  | Power 1.8v to 5.5v 
|  13 | SPI: SCK              |  2  | SCK
|  10 | CS (any digital pin)  |  3  | CS
|  11 | SPI: MOSI             |  4  | SDI
|  12 | SPI: MISO             |  5  | SDO
| +5v | 3.3v is also supported|  6  | WLAT - wiring directly to GND forces WLAT to always be enabled. Pass PIN_NOT_CONFIGURED in the constructor for WLAT parameter.
| GND |                       |  7  | SHDN - wiring directly to HIGH (Arduino power) forces the resistor network to always be enabled. Pass PIN_NOT_CONFIGURED in the constructor for SHDN parameter.
| GND |                       |  8  | NC
| GND |                       |  9  | GND
| GND |                       | 10  | V-    (Connect to external power supply max 36 volts. Common ground with Arduino
|     |                       | 11  | POB   (This is the potentiometer)
|     |                       | 12  | POW   (This is the wiper of the potentiometer)
|     |                       | 13  | POA   (This is the potentiometer)
|     |                       | 14  | V+    (Connect to external power supply max 36 volts)
```

The full datasheet on the MCP41HVX1 family of chips can be found here (http://ww1.microchip.com/downloads/en/DeviceDoc/20005207B.pdf);


## Class Methods Documentation: ###

### Constructors

If either WLAT or SHDN are wired directly to power pins (see Alternate Wiring Diagram) pass the constant MCP41HVX1_PIN_NOT_CONFIGURED to the constructor for the pin not wired to an Arduio digital control pin. If both WLAT and SHDN are wiried directly to Arduino power pins the constructur that only accepts the SS pin can be used.

It is important to note that the SS Pin must always be specified. You cannot pass in MCP41HVX1_PIN_NOT_CONFIGURED to the library for the SS Pin value.

##### MCP41HVX1( int nCSPin)
    
**Argument:**
- int nCSPin -   This is the digital pin used to control chip select for SPI communications. A valid pin number must be specified.
    
    
**Description:**
Constructor for creating an instance of MCP41HVX1 with reduced functionaliy. Use this constructor only when both WLAT is connected directly to GND and SHDN is connected directly to HIGH (Arduino power). See alternate wiring diagram.
    

**Example:**
```
int		nCSPin = 10;
MCP41HVX1	MyDigipot( nCSPin);
```



#### MCP41HVX1( int nCSPin,  int nSHDNPin, int nWLATPin)
    
**Arguments:**
- int nCSPin -   The digital pin used to control chip select for SPI communications. A valid pin number must be specified.
- int nSHDNPin - The digital pin used to enable\disable the resistor network. Specify a value of MCP41HVX1_PIN_NOT_CONFIGURED if SHDN is connected to HIGH (Arduino VCC).
- int nWLAT -    The digital pin used to enable\disable transfering the wiper value to the resistor network. Specify a value of MCP41HVX1_PIN_NOT_CONFIGURED if WLAT is connected to GND (Arduino GND).
    
**Description:**
Constructor for creating an instance of the MCP41HVX1 class which allows for specifying both SHDN and WLAT control pins. If a pin number is specified for WLAT it is defaulted to enabled within the constructor. If a pin number is specified for SHDN the resistor network is defaulted to enabled within the constructor.
    
**Example:**
```
int	nCSPin = 10;
int	nWLATPin  = 9;
	
MCP41HVX1	MyDigipot( nCSPin, MCP41HVX1_PIN_NOT_CONFIGURED, nWLATPin);			//  SHDN is connected directly to GND
```

#### MCP41HVX1( int nCSPin,  int nSHDNPin, bool bSHDNState, int nWLATPin, bool WLATState)
    
**Arguments**
- int  nCSPin -     The digital pin used to control chip select for SPI communications. A valid pin number must be specified.
- int  nSHDNPin -   The digital pin used to enable\disable the resistor network. Specify a value of MCP41HVX1_PIN_NOT_CONFIGURED if SHDN is connected to LOW (Arduino GND).
- bool bSHDNState - Defines the initial state of the resistor network.
*MCP41HVX1_STATE_ENABLED* - enable the resistor network within the constructor. 
*MCP41HVX1_STATE_DISABLED* - disable the resistor network within the constructor.
- int nWLAT -       The digital pin used to enable\disable updating of transfering the wiper value to the resistor network. Specify a value of MCP41HVX1_PIN_NOT_CONFIGURED if WLAT is connected to HIGH (Arduino power).
- bool WLATState -  Defines the initial state if the wiper value is passed to the resistor network. 
*MCP41HVX1_STATE_ENABLED* - enable the resistor network within the constructor. 
*MCP41HVX1_STATE_DISABLED* - disable the resistor network within the constructor.
	
**Description:**
Constructor for creating an instance of the MCP41HVX1 class. This version of the constructor allows for full control to specify both the SHDN and WLAT pins and also define their initial state when the instance is constructed. 
   
    
**Example:**
```
int	nCSPin    = 10;
int	nWLATPin  = 9;
int	nSHDNPin  = 8;
	
MCP41HVX1 MyDigipot( nCSPin, nSHDNPin, MCP41HVX1_STATE_ENABLED, nWLATPin, MCP41HVX1_STATE_DISABLED);
```

### Resistor Network (SHDN) Control Methods

#### void  ResistorNetworkDisable()
    
**Description:**
Disables the resistor network. P0A is disconnected and will show as open using a multimeter across P0W or P0B. P0B is connected to P0W and will show close to 0 ohms when checked	with a multimeter. The reason it will not show 0 ohms is that there is some internal resistance within the chip. While the resistor network is disabled the wiper value can continue to be updated although the actual resistance value will not change when measured with a meter.
	
	
**Example:**
```
int	nCSPin = 10;
	
MCP41HVX1 MyDigipot( nCSPin);
	
MyDigipot.ResistorNetworkDisable();	// Resistance shows open across P0A and P0W
MyDigipot.WiperSetPosition( 127);	// Resistance does not change
```

#### void  ResistorNetworkEnable()
    
**Description:**
Enables the resistor network. If the wiper value was updated when the resistor network was disabled the updated wiper value will take effect once re-enabled. 
	
**Example:**
```	
int	nCSPin = 10;
	
MCP41HVX1 MyDigipot( nCSPin);
	
MyDigipot.ResistorNetworkDisable();	// Resistance shows open across P0A and P0W
MyDigipot.WiperSetPosition( 127);	// Resistance does not change
MyDigipot.ResistorNetworkEnable();	// Resistance shows max resistance on a 7 bit chip or half of max resistance on an 8 bit chip across P0A and P0W
```

#### int ResistorNetworkGetState()

**Return Value:**
- *MCP41HVX1_STATE_ENABLED* -  The resistor network is enabled.
- *MCP41HVX1_STATE_DISABLED* - The resistor network is disabled.

**Description:**   
Returns the state of the resistor network.
	
**Example:**
```
int 	nCSPin = 10;
MCP41HVX1 MyDigipot( nCSPin);
	
switch( MyDigipot.ResistorGetState()
{
	case MCP41HVX1_STATE_ENABLED:
		Serial.println( "The resistor network is enabled");
		break;
	case MCP41HVX1_STATE_DISABLED:
		Serial.println( "The resistor network is disabled");
		break;
}	
```
	
### Wiper Control Methods

#### byte WiperGetPosition()
    
**Return Value:**
Returns the current wiper position.
	
**Description:**
Sends an SPI request to the chip to get the current wiper position.

**Example:**
```
int 	nCSPin = 10;
int	nTap;
MCP41HVX1 MyDigipot( nCSPin);
	
MiDigipot.WiperSetPosition(50);			// Go to wiper position 50
Serial.print( "The current wiper position is ");
Serial.println( Digipot.WiperGetPosition());	// Displays 50 in the serial monitor.
```

#### byte WiperSetPosition( byte byWiper)
    
**Argument:**
- byte byWiper - The wiper value to set.
	
**Return Value:**
Returns the actual wiper position.
	
**Description:**
Sets the wiper position. The wiper position cannot exceed the max value supported by the digipot. This only affects 7 bit chips because the argument byWiper is (unsigned) 8 bits.
	
**Example:**
```
int 	nCSPin = 10;
MCP41HVX1 MyDigipot( nCSPin);
	
MiDigipot.WiperSetPosition(0);		// Go to the minimum resistance which is wiper position 0    
```

#### byte WiperIncrement()
    
**Return Value:**
Returns the actual wiper position. 
	
**Description:**
Increments the wiper position. The wiper position cannot exceed the max (either 127 for a 7 bit chip or 255 for a 8 bit chip). If an attempt is made to increment the chip while it is already at the max position it will remain at the max position.
	
**Example:**       
```
int 	nCSPin = 10;
int	nTap;
MCP41HVX1 MyDigipot( nCSPin);
	
MiDigipot.WiperSetPosition(0);		// Go to the minimum resistance
	
for( int x=0; x<20; x++)
{
	nTap = MyDigipot.WiperIncrement();	// Increment 20 times
	Serial.print( "Current tap position = ");
	Serial.println( nTap);			// Print the current tap position
}
```

#### byte WiperDecrement()
    
**Return Value:**
Returns the actual wiper position. 
	  
    
**Description:**
Decrements the wiper position. The wiper position cannot become negative. If an attempt is made to decrement the wiper when it is 0 it will remain at the 0 position.
    
**Example:**
```
int 	nCSPin = 10;
int	nTap;
MCP41HVX1 MyDigipot( nCSPin);
	
MiDigipot.WiperSetPosition(100);		// Go to position 100 in the resistor network resistance
	
for( int x=0; x<20; x++)
{
	nTap = MyDigipot.WiperDecrement();	// Decrement 20 times
	Serial.print( "Current tap position = ");
	Serial.println( nTap);			// Print the current tap position
}    
```

#### void WLATDisable()
    
**Description:**
Disables WLAT. If the wiper position is adjusted thru WiperSetPosition(), WiperDecrement(), or WiperIncrement() when WLAT is disabled the value is saved but it is not applied	to the resistor network so the resistance value will not change. If the wiper position is changed while WLAT is disabled it will be applied once WLAT is enabled. 
	
**Example:**
```	
int 	nCSPin = 10;
MCP41HVX1 MyDigipot( nCSPin);
	
MiDigipot.WiperSetPosition(0);		// Go to the minimum resistance
MyDigipot.WLATDisable();
MyDigipot.WiperSetPosition( 100);	// Resistance does not change when measured with a multimeter across P0A and P0W.
```

#### void WLATEnable()
    
**Description:**
Enables WLAT so that when the wiper value is adjusted thru WiperSetPosition(), WiperDecrement(), or WiperIncrement() the new value is applied to the resistor network so that	the resistance is changed.
	
**Example:**
```
int 	nCSPin = 10;
MCP41HVX1 MyDigipot( nCSPin);
	
MiDigipot.WiperSetPosition(0);      // to the minimum resistance
MyDigipot.WLATDisable();
MyDigipot.WiperSetPosition( 127);   // Resistance does not change when measured with a multimeter across P0A and P0W.
MyDigipot.WLATEnable();	            // Resistance goes to maximum if 7 bit chip or half of maximum if a 8 bit chip when measured with a multimeter across P0A and P0W.
```

####  int WLATGetState()
    
**Return Value:** 
- *MCP41HVX1_STATE_ENABLED* -  The wiper value will be transfered to the resistor network.
- *MCP41HVX1_STATE_DISABLED* - The wiper value will not be transfered to the resistor network.
		
**Description:**
Returns the state of WLAT.
	
**Example:**
```
int 	nCSPin = 10;
MCP41HVX1 MyDigipot( nCSPin);
	
switch( MyDigipot.WLATGetState()
{
	case MCP41HVX1_STATE_ENABLED:
		Serial.println( "WLAT is enabled");
		break;
	case MCP41HVX1_STATE_DISABLED:
		Serial.println( "WLAT is disabled");
		break;
}
```

## License ##
Copyright (c) 2020 Greg Srabian. All rights reserved.
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

To receive a copy of the GNU Lesser General Public License, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA




