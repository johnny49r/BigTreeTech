/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#define REVERSE_TEMP_SENSOR_RANGE

// Marlin temperature sensor type 22
//
// This was created to allow use of the E3D PT100 amplifier (using INA826) on the BigTreeTech 
// and similar boards without requiring any hardware mods (cutting traces, changing resistors, etc.).
// The BigTreeTech boards have a 3.3V ADC reference and a 4.7K pullup resistor 
// connected to the Thermistor inputs so we can't use the standard Marlin temp sensor types.
//
// HARDWARE CONFIGURATION:
//
// Since the BigTreeTech board is not 5V tolerant the E3D PT100 amplifier board needs to operate 
// at 3.3V. 
// NOTE: the PT100 amp board will operate fine at 3.3V but it won't report temperatures beyond 
// approx 360C because the output of the INA826 goes into saturation.
// 3.3V can be found on some BTT connectors (WIFI connector on the SKR V1.4)
// Connect the signal output of the PT100 amp to the standard thermistor input on the BTT board.
//
// SOFTWARE INSTALL:
//
// This header file must be compiled into the Marlin 2.0.x firmware. The current release of Marlin is
// 2.0.5.3 and the firmware is built using VSCode with the PlatformIO IDE installed.
//
// 1) Add this header file into the 'thermistor' directory (Marlin/src/module/thermistor).
//
// 2) In the same directory open the 'thermistors.h' file and add the following text:
//    #if ANY_THERMISTOR_IS(22) 
//      #include "thermistor_22.h"
//    #endif
//
// 3) In 'configuration.h' change the nozzle thermistor type:
//      #define TEMP_SENSOR_0 22       // use new thermistor table 22
//
// 4) In 'configuration.h' insure max temp is not > 360C:
//      #define HEATER_0_MAXTEMP 310   
//
// CALCULATIONS:
//
// Array values below were derived using a standard PT100 thermocouple resistance vs temperature  
// table available online.
//
// Constants:
//    Vref = 3.30V
//    Rb = 4400 (total series resistance of the two bridge resistors in the INA826 circuit)
//    Rpu = 4700 (pullup resistor on the BTT thermistor input)
//    Ramp = 100 (INA826 amp output buffer resistor)
//    RTD = Resistance @ temperature from thermocouple table. Example 111 ohms at 25C.
//      
// Calculate Vo : output of the INA826 amplifier in the E3D circuit.
//    Vo = ((Vref / (RTD + Rb)) * RTD) * 10
// Next calculate Vadc : output voltage with a 4.7K pullup to 3.3V and a 100 ohm series output resistor.
//    Vadc = (((Vref - Vo) / Rpu) * Ramp) + Vo
// Lastly calculate ADC digital value for each point in the array below (10 bit ADC, 0 - 1023).
//    ADC = (Vadc / Vref) * 1024
//
// The first value in the array below is the raw ADC value. The second value is the temperature equivalent. 
// Incremental values are calculated by Marlin using interpolation.
//
// Abby@Abbycus 2020
//
const short temptable_22[][2] PROGMEM = {
  { OV(  0),    0 },
  { OV(245),    1 },
  { OV(253),   10 },
  { OV(261),   20 },
  { OV(270),   30 },
  { OV(278),   40 },
  { OV(287),   50 },
  { OV(295),   60 },
  { OV(303),   70 },
  { OV(311),   80 },
  { OV(319),   90 },
  { OV(328),  100 },
  { OV(336),  110 },
  { OV(344),  120 },
  { OV(352),  130 },
  { OV(360),  140 },
  { OV(368),  150 },
  { OV(376),  160 },
  { OV(384),  170 },
  { OV(391),  180 },
  { OV(399),  190 },
  { OV(407),  200 },
  { OV(415),  210 },
  { OV(422),  220 },
  { OV(430),  230 },
  { OV(438),  240 },
  { OV(445),  250 },
  { OV(453),  260 },
  { OV(460),  270 },
  { OV(468),  280 },
  { OV(475),  290 },
  { OV(483),  300 },
  { OV(490),  310 },
  { OV(497),  320 },
  { OV(505),  330 },
  { OV(512),  340 },
  { OV(519),  350 },
  { OV(526),  360 },
  { OV(533),  370 },
  { OV(541),  380 },
  { OV(548),  390 },
  { OV(554),  400 },
  { OV(623),  500 },
  { OV(689),  600 },
  { OV(751),  700 },
  { OV(810),  800 },
  { OV(850),  900 },
  { OV(910), 1000 },
  { OV(960), 1100 }
};
