# BigTreeTech

## Use a PT100 temperature sensor with the BigTreeTech V1.4 / Turbo 3D printer controller.

This method allows the use of the E3D PT100 amplifier (using INA826) with the BigTreeTech V1.4
and similar boards without requiring any hardware mods (cutting traces, changing resistors, etc.).

### HARDWARE INSTALL

1) Since the BigTreeTech board is not 5V tolerant the E3D PT100 amplifier board needs to operate 
at 3.3V. Connect the VCC (5V) pin of the PT100 amplifier to 3.3V on the BTT board (available on the
WIFI connector on the SKR V1.4).

2) Connect the PT100 signal output to an available thermistor input on the BTT board.

**_Limitation:_** the PT100 amplifier board will operate fine at 3.3V but it won't report temperatures 
beyond approx 360C because the output voltage swing of the INA826 is limited to 3.3V.

### SOFTWARE INSTALL:

The header file **_'thermistor_22.h'_** must be compiled into the Marlin 2.0.x firmware. The 
current release of Marlin (as of this writing) is 2.0.5.3 and the firmware was compiled using 
VSCode with the PlatformIO IDE installed.

1) Copy the header file **_'thermistor_22.h'_** to the Marlin 'thermistor' directory 
(Marlin/src/module/thermistor).

2) In the same directory edit the **_'thermistors.h'_** file and add the following text:

    > #if ANY_THERMISTOR_IS(22)
    
    >   #include "thermistor_22.h"
    
    > #endif

3) In the Marlin /src directory edit **_'configuration.h'_** and change the nozzle thermistor type:
    
    > _#define TEMP_SENSOR_0 22_       // use new thermistor table 22

4) In the **_'configuration.h'_** file, insure max temp is not > 360C:
     
    > _#define HEATER_0_MAXTEMP 350_

### CALCULATIONS:

Conditions of this specific hardware environment:
1) The BTT board thermistor input has a 4.7K pullup resistor.
2) The E3D PT100 amplifier has a 100 ohm output series termination resistor.

The above are taken into account using the calculations below.

The thermistor header file in Marlin is a lookup table used to derive temperature from the raw
analog values read from the PT100 amplifier. The tables contains translation values for every 10 
degrees C and finer values are resolved through interpolation in Marlin.

The thermistor header file is an array of two elements: the first is the raw ADC value. The second 
is the temperature associated with that raw value.

The values in the table were derived using a standard PT100 thermocouple resistance vs temperature  
table available online.

Constants:
   Vref = 3.30V
   Rb = 4400 (total series resistance of the two bridge resistors in the INA826 circuit)
   Rpu = 4700 (pullup resistor on the BTT thermistor input)
   Ramp = 100 (INA826 amp output buffer resistor)
   RTD = Resistance @ temperature from a PT100 thermocouple table. Example 111 ohms at 25C.
      
a) Calculate Vo : output of the INA826 amplifier in the E3D circuit.
   Vo = ((Vref / (RTD + Rb)) * RTD) * 10
b) Next calculate Vadc : output voltage with a 4.7K pullup to 3.3V and a 100 ohm series output resistor.
   Vadc = (((Vref - Vo) / Rpu) * Ramp) + Vo
c) Lastly calculate ADC digital value for each point in the array table (10 bit ADC, 0 - 1023).
   ADC = (Vadc / Vref) * 1024

NOTE: These calculations can be easily modified to accomodate other hardware combinations.

John Hoeppner @ Abbycus Consulting 2020
