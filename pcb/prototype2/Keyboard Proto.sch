EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A 11000 8500
encoding utf-8
Sheet 1 1
Title "Keyboard Prototype Board"
Date "26 Dec 2020"
Rev "2"
Comp "Prototype Schematic"
Comment1 "Based on Adafruit Feather S2 and LM8330 keyboard IC"
Comment2 ""
Comment3 "License: MIT"
Comment4 "Design: cmumford"
$EndDescr
$Comp
L Keyboard-Proto-rescue:Adafruit_Feather_nRF52840_Express-Adafruit_Feather_nRF52840_Express U1
U 1 1 5FCC3339
P 3500 5750
F 0 "U1" H 3500 6715 50  0000 C CNN
F 1 "Adafruit_Feather_nRF52840_Express" H 3500 6624 50  0000 C CNN
F 2 "Adafruit_Feather_nRF52840_Express:Adafruit_Feather_nRF52840_Express" H 3250 6650 50  0001 C CNN
F 3 "https://www.adafruit.com/product/4062" H 3250 6650 50  0001 C CNN
	1    3500 5750
	1    0    0    -1  
$EndComp
Text GLabel 3150 5050 0    50   Input ~ 0
SDA
Text GLabel 3150 5150 0    50   Input ~ 0
SCL
Text GLabel 3150 1650 0    50   Input ~ 0
SDA
Text GLabel 3150 1750 0    50   Input ~ 0
SCL
Text GLabel 3150 1850 0    50   Input ~ 0
KBDINT
Text GLabel 3150 5350 0    50   Input ~ 0
KBDINT
$Comp
L Switch:SW_Push Q1
U 1 1 5FCC9F75
P 6150 2050
F 0 "Q1" H 6150 2243 50  0001 C CNN
F 1 "Q" H 6150 2243 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6150 2250 50  0001 C CNN
F 3 "~" H 6150 2250 50  0001 C CNN
	1    6150 2050
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W1
U 1 1 5FCCA414
P 6750 2050
F 0 "W1" H 6750 2243 50  0001 C CNN
F 1 "W" H 6750 2243 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6750 2250 50  0001 C CNN
F 3 "~" H 6750 2250 50  0001 C CNN
	1    6750 2050
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E1
U 1 1 5FCCAA98
P 7350 2050
F 0 "E1" H 7350 2243 50  0001 C CNN
F 1 "E" H 7350 2243 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 7350 2250 50  0001 C CNN
F 3 "~" H 7350 2250 50  0001 C CNN
	1    7350 2050
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push A1
U 1 1 5FCCB8B3
P 6150 2650
F 0 "A1" H 6150 2843 50  0001 C CNN
F 1 "A" H 6150 2843 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6150 2850 50  0001 C CNN
F 3 "~" H 6150 2850 50  0001 C CNN
	1    6150 2650
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push S1
U 1 1 5FCCBF44
P 6750 2650
F 0 "S1" H 6750 2843 50  0001 C CNN
F 1 "S" H 6750 2843 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6750 2850 50  0001 C CNN
F 3 "~" H 6750 2850 50  0001 C CNN
	1    6750 2650
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push D1
U 1 1 5FCCC6C4
P 7350 2650
F 0 "D1" H 7350 2843 50  0001 C CNN
F 1 "D" H 7350 2843 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 7350 2850 50  0001 C CNN
F 3 "~" H 7350 2850 50  0001 C CNN
	1    7350 2650
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift1
U 1 1 5FCCCBD3
P 5550 2050
F 0 "Shift1" H 5550 2243 50  0001 C CNN
F 1 "Shift" H 5550 2243 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 5550 2250 50  0001 C CNN
F 3 "~" H 5550 2250 50  0001 C CNN
	1    5550 2050
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Ctrl1
U 1 1 5FCCD3E9
P 5550 2650
F 0 "Ctrl1" H 5550 2843 50  0001 C CNN
F 1 "Ctrl" H 5550 2843 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 5550 2850 50  0001 C CNN
F 3 "~" H 5550 2850 50  0001 C CNN
	1    5550 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 2050 5800 2050
Wire Wire Line
	5800 2050 5800 2650
Wire Wire Line
	6350 2050 6400 2050
Wire Wire Line
	6400 2050 6400 2650
Wire Wire Line
	6950 2050 7000 2050
Wire Wire Line
	7000 2050 7000 2650
Wire Wire Line
	7550 2050 7600 2050
Wire Wire Line
	7600 2050 7600 2650
Wire Wire Line
	7150 2150 7150 2050
Wire Wire Line
	6550 2050 6550 2150
Connection ~ 6550 2150
Wire Wire Line
	6550 2150 7150 2150
Wire Wire Line
	5950 2050 5950 2150
Connection ~ 5950 2150
Wire Wire Line
	5950 2150 6550 2150
Wire Wire Line
	5300 2650 5300 2750
Wire Wire Line
	5300 2750 5950 2750
Wire Wire Line
	7150 2750 7150 2650
Wire Wire Line
	5300 2650 5350 2650
Wire Wire Line
	6550 2650 6550 2750
Connection ~ 6550 2750
Wire Wire Line
	6550 2750 7150 2750
Wire Wire Line
	5950 2650 5950 2750
Connection ~ 5950 2750
Wire Wire Line
	5950 2750 6550 2750
Wire Wire Line
	5750 2650 5800 2650
Connection ~ 5800 2650
Wire Wire Line
	6350 2650 6400 2650
Wire Wire Line
	6950 2650 7000 2650
Wire Wire Line
	7550 2650 7600 2650
Wire Wire Line
	5800 2650 5800 2900
Text GLabel 3150 1950 0    50   Input ~ 0
RST
Text GLabel 3850 6550 2    50   Input ~ 0
RST
NoConn ~ 3150 5250
NoConn ~ 3150 5550
NoConn ~ 3150 5650
NoConn ~ 3150 5750
NoConn ~ 3150 5850
NoConn ~ 3150 5950
NoConn ~ 3150 6050
NoConn ~ 3150 6150
NoConn ~ 3150 6250
NoConn ~ 3150 6350
NoConn ~ 3150 6450
NoConn ~ 3150 6550
NoConn ~ 3850 5050
NoConn ~ 3850 5150
NoConn ~ 3850 5250
NoConn ~ 3850 5850
NoConn ~ 3850 5950
NoConn ~ 3850 6050
NoConn ~ 3850 6150
NoConn ~ 3850 6350
Text GLabel 3850 6450 2    50   Input ~ 0
3.3V
Text GLabel 3850 6250 2    50   Input ~ 0
Gnd
Text GLabel 3700 3650 3    50   Input ~ 0
Gnd
Text GLabel 3650 1450 1    50   Input ~ 0
3.3V
Text GLabel 4200 1950 2    50   Input ~ 0
ROW0
Text GLabel 4200 1850 2    50   Input ~ 0
ROW1
Text GLabel 4200 2200 2    50   Input ~ 0
COL0
Text GLabel 4200 2300 2    50   Input ~ 0
COL1
Text GLabel 4200 2400 2    50   Input ~ 0
COL2
Text GLabel 4200 2500 2    50   Input ~ 0
COL3
Text GLabel 7600 2900 3    50   Input ~ 0
COL0
Text GLabel 5800 2900 3    50   Input ~ 0
COL3
Text GLabel 6400 2900 3    50   Input ~ 0
COL2
Text GLabel 7000 2900 3    50   Input ~ 0
COL1
Wire Wire Line
	6400 2900 6400 2650
Connection ~ 6400 2650
Wire Wire Line
	7000 2650 7000 2900
Connection ~ 7000 2650
Wire Wire Line
	7600 2650 7600 2900
Connection ~ 7600 2650
Text GLabel 5200 2150 0    50   Input ~ 0
ROW0
Text GLabel 5200 2750 0    50   Input ~ 0
ROW1
Wire Wire Line
	5200 2150 5300 2150
Wire Wire Line
	5200 2750 5300 2750
Connection ~ 5300 2750
Wire Wire Line
	5350 2050 5300 2050
Wire Wire Line
	5300 2050 5300 2150
Connection ~ 5300 2150
Wire Wire Line
	5300 2150 5950 2150
$Comp
L Connector:Conn_01x14_Female J1
U 1 1 5FCD18E2
P 6550 5300
F 0 "J1" H 6578 5276 50  0000 L CNN
F 1 "Conn_01x14_Female" H 6578 5185 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x14_P2.54mm_Vertical" H 6550 5300 50  0001 C CNN
F 3 "~" H 6550 5300 50  0001 C CNN
	1    6550 5300
	1    0    0    -1  
$EndComp
Text GLabel 6350 4700 0    50   Input ~ 0
3.3V
Text GLabel 6350 4800 0    50   Input ~ 0
Gnd
Text GLabel 6350 4900 0    50   Input ~ 0
D_CS
Text GLabel 6350 5000 0    50   Input ~ 0
RST
Text GLabel 6350 5200 0    50   Input ~ 0
MOSI
Text GLabel 6350 5300 0    50   Input ~ 0
SCK
Text GLabel 6350 5400 0    50   Input ~ 0
3.3V
Text GLabel 6350 5500 0    50   Input ~ 0
MISO
Text GLabel 6350 5600 0    50   Input ~ 0
SCK
Text GLabel 6350 5700 0    50   Input ~ 0
T_CS
Text GLabel 6350 5800 0    50   Input ~ 0
MOSI
Text GLabel 6350 5900 0    50   Input ~ 0
MISO
Text GLabel 6350 6000 0    50   Input ~ 0
T_IRQ
Text GLabel 3850 5650 2    50   Input ~ 0
D_CS
Text GLabel 3850 5450 2    50   Input ~ 0
MOSI
Text GLabel 3850 5350 2    50   Input ~ 0
MISO
Text GLabel 3850 5550 2    50   Input ~ 0
SCK
Text GLabel 3850 5750 2    50   Input ~ 0
T_CS
Text GLabel 3150 5450 0    50   Input ~ 0
T_IRQ
NoConn ~ 6350 5100
$Comp
L LM8330:LM8330 U?
U 1 1 5FE7EC0A
P 4200 2150
F 0 "U?" H 3200 3100 50  0000 C CNN
F 1 "LM8330" H 3300 3000 50  0000 C CNN
F 2 "LM8330:BGA-25_5x5_2.0x2.0mm" H 4200 2150 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/lm8330.pdf" H 4200 2150 50  0001 C CNN
F 4 "Texas Instruments" H 3100 2900 50  0000 C CNN "Manufacturer"
	1    4200 2150
	1    0    0    -1  
$EndComp
NoConn ~ 4200 1650
NoConn ~ 4200 1750
NoConn ~ 4200 2600
NoConn ~ 4200 2700
NoConn ~ 4200 2800
NoConn ~ 4200 2900
NoConn ~ 4200 3050
NoConn ~ 4200 3150
NoConn ~ 4200 3250
NoConn ~ 3150 2350
NoConn ~ 3150 2450
NoConn ~ 3150 2550
NoConn ~ 3150 2650
$EndSCHEMATC
