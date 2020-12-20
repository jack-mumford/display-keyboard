EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Keyboard Prototype Board"
Date "12 Dec 2020"
Rev "1"
Comp "Prototype Schematic"
Comment1 "Based on Adafruit Feather nRF52840 Express and TCA8418 keyboard IC"
Comment2 ""
Comment3 "License: MIT"
Comment4 "Design: cmumford"
$EndDescr
$Comp
L TCA8418:TCA8418 U2
U 1 1 5FCC0456
P 1500 2300
F 0 "U2" H 1750 3100 50  0000 C CNN
F 1 "TCA8418" H 1700 3000 50  0000 C CNN
F 2 "Package_DFN_QFN:WQFN-24-1EP_4x4mm_P0.5mm_EP2.45x2.45mm" H 1550 2800 50  0001 C CNN
F 3 "" H 1550 2800 50  0001 C CNN
	1    1500 2300
	1    0    0    -1  
$EndComp
$Comp
L Keyboard-Proto-rescue:Adafruit_Feather_nRF52840_Express-Adafruit_Feather_nRF52840_Express U1
U 1 1 5FCC3339
P 1550 5750
F 0 "U1" H 1550 6715 50  0000 C CNN
F 1 "Adafruit_Feather_nRF52840_Express" H 1550 6624 50  0000 C CNN
F 2 "Adafruit_Feather_nRF52840_Express:Adafruit_Feather_nRF52840_Express" H 1300 6650 50  0001 C CNN
F 3 "https://www.adafruit.com/product/4062" H 1300 6650 50  0001 C CNN
	1    1550 5750
	1    0    0    -1  
$EndComp
Text GLabel 1200 5050 0    50   Input ~ 0
SDA
Text GLabel 1200 5150 0    50   Input ~ 0
SCL
Text GLabel 1050 1900 0    50   Input ~ 0
SDA
Text GLabel 1050 2000 0    50   Input ~ 0
SCL
Wire Wire Line
	1500 1450 1500 1600
Text GLabel 1900 1800 2    50   Input ~ 0
KBDINT
Text GLabel 1200 5350 0    50   Input ~ 0
KBDINT
$Comp
L Switch:SW_Push Q1
U 1 1 5FCC9F75
P 4200 2050
F 0 "Q1" H 4200 2243 50  0001 C CNN
F 1 "Q" H 4200 2243 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4200 2250 50  0001 C CNN
F 3 "~" H 4200 2250 50  0001 C CNN
	1    4200 2050
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W1
U 1 1 5FCCA414
P 4800 2050
F 0 "W1" H 4800 2243 50  0001 C CNN
F 1 "W" H 4800 2243 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4800 2250 50  0001 C CNN
F 3 "~" H 4800 2250 50  0001 C CNN
	1    4800 2050
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E1
U 1 1 5FCCAA98
P 5400 2050
F 0 "E1" H 5400 2243 50  0001 C CNN
F 1 "E" H 5400 2243 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 5400 2250 50  0001 C CNN
F 3 "~" H 5400 2250 50  0001 C CNN
	1    5400 2050
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push A1
U 1 1 5FCCB8B3
P 4200 2650
F 0 "A1" H 4200 2843 50  0001 C CNN
F 1 "A" H 4200 2843 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4200 2850 50  0001 C CNN
F 3 "~" H 4200 2850 50  0001 C CNN
	1    4200 2650
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push S1
U 1 1 5FCCBF44
P 4800 2650
F 0 "S1" H 4800 2843 50  0001 C CNN
F 1 "S" H 4800 2843 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4800 2850 50  0001 C CNN
F 3 "~" H 4800 2850 50  0001 C CNN
	1    4800 2650
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push D1
U 1 1 5FCCC6C4
P 5400 2650
F 0 "D1" H 5400 2843 50  0001 C CNN
F 1 "D" H 5400 2843 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 5400 2850 50  0001 C CNN
F 3 "~" H 5400 2850 50  0001 C CNN
	1    5400 2650
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift1
U 1 1 5FCCCBD3
P 3600 2050
F 0 "Shift1" H 3600 2243 50  0001 C CNN
F 1 "Shift" H 3600 2243 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3600 2250 50  0001 C CNN
F 3 "~" H 3600 2250 50  0001 C CNN
	1    3600 2050
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Ctrl1
U 1 1 5FCCD3E9
P 3600 2650
F 0 "Ctrl1" H 3600 2843 50  0001 C CNN
F 1 "Ctrl" H 3600 2843 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3600 2850 50  0001 C CNN
F 3 "~" H 3600 2850 50  0001 C CNN
	1    3600 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 2050 3850 2050
Wire Wire Line
	3850 2050 3850 2650
Wire Wire Line
	4400 2050 4450 2050
Wire Wire Line
	4450 2050 4450 2650
Wire Wire Line
	5000 2050 5050 2050
Wire Wire Line
	5050 2050 5050 2650
Wire Wire Line
	5600 2050 5650 2050
Wire Wire Line
	5650 2050 5650 2650
Wire Wire Line
	5200 2150 5200 2050
Wire Wire Line
	4600 2050 4600 2150
Connection ~ 4600 2150
Wire Wire Line
	4600 2150 5200 2150
Wire Wire Line
	4000 2050 4000 2150
Connection ~ 4000 2150
Wire Wire Line
	4000 2150 4600 2150
Wire Wire Line
	3350 2650 3350 2750
Wire Wire Line
	3350 2750 4000 2750
Wire Wire Line
	5200 2750 5200 2650
Wire Wire Line
	3350 2650 3400 2650
Wire Wire Line
	4600 2650 4600 2750
Connection ~ 4600 2750
Wire Wire Line
	4600 2750 5200 2750
Wire Wire Line
	4000 2650 4000 2750
Connection ~ 4000 2750
Wire Wire Line
	4000 2750 4600 2750
Wire Wire Line
	3800 2650 3850 2650
Connection ~ 3850 2650
Wire Wire Line
	4400 2650 4450 2650
Wire Wire Line
	5000 2650 5050 2650
Wire Wire Line
	5600 2650 5650 2650
Wire Wire Line
	3850 2650 3850 2900
NoConn ~ 1900 2100
NoConn ~ 1900 2200
NoConn ~ 1900 2300
NoConn ~ 1900 2400
NoConn ~ 1900 2500
NoConn ~ 1900 2600
NoConn ~ 1900 3100
NoConn ~ 1900 3200
NoConn ~ 1900 3300
NoConn ~ 1900 3400
NoConn ~ 1900 3500
NoConn ~ 1900 3600
Text GLabel 1050 1800 0    50   Input ~ 0
RST
Text GLabel 1900 6550 2    50   Input ~ 0
RST
NoConn ~ 1200 5250
NoConn ~ 1200 5550
NoConn ~ 1200 5650
NoConn ~ 1200 5750
NoConn ~ 1200 5850
NoConn ~ 1200 5950
NoConn ~ 1200 6050
NoConn ~ 1200 6150
NoConn ~ 1200 6250
NoConn ~ 1200 6350
NoConn ~ 1200 6450
NoConn ~ 1200 6550
NoConn ~ 1900 5050
NoConn ~ 1900 5150
NoConn ~ 1900 5250
NoConn ~ 1900 5850
NoConn ~ 1900 5950
NoConn ~ 1900 6050
NoConn ~ 1900 6150
NoConn ~ 1900 6350
Text GLabel 1900 6450 2    50   Input ~ 0
3.3V
Text GLabel 1900 6250 2    50   Input ~ 0
Gnd
Text GLabel 1500 3800 3    50   Input ~ 0
Gnd
Text GLabel 1500 1450 1    50   Input ~ 0
3.3V
Text GLabel 1900 1900 2    50   Input ~ 0
ROW0
Text GLabel 1900 2000 2    50   Input ~ 0
ROW1
Text GLabel 1900 2700 2    50   Input ~ 0
COL0
Text GLabel 1900 2800 2    50   Input ~ 0
COL1
Text GLabel 1900 2900 2    50   Input ~ 0
COL2
Text GLabel 1900 3000 2    50   Input ~ 0
COL3
Text GLabel 5650 2900 3    50   Input ~ 0
COL0
Text GLabel 3850 2900 3    50   Input ~ 0
COL3
Text GLabel 4450 2900 3    50   Input ~ 0
COL2
Text GLabel 5050 2900 3    50   Input ~ 0
COL1
Wire Wire Line
	4450 2900 4450 2650
Connection ~ 4450 2650
Wire Wire Line
	5050 2650 5050 2900
Connection ~ 5050 2650
Wire Wire Line
	5650 2650 5650 2900
Connection ~ 5650 2650
Text GLabel 3250 2150 0    50   Input ~ 0
ROW0
Text GLabel 3250 2750 0    50   Input ~ 0
ROW1
Wire Wire Line
	3250 2150 3350 2150
Wire Wire Line
	3250 2750 3350 2750
Connection ~ 3350 2750
Wire Wire Line
	3400 2050 3350 2050
Wire Wire Line
	3350 2050 3350 2150
Connection ~ 3350 2150
Wire Wire Line
	3350 2150 4000 2150
$Comp
L Connector:Conn_01x14_Female J1
U 1 1 5FCD18E2
P 4600 5300
F 0 "J1" H 4628 5276 50  0000 L CNN
F 1 "Conn_01x14_Female" H 4628 5185 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x14_P2.54mm_Vertical" H 4600 5300 50  0001 C CNN
F 3 "~" H 4600 5300 50  0001 C CNN
	1    4600 5300
	1    0    0    -1  
$EndComp
Text GLabel 4400 4700 0    50   Input ~ 0
3.3V
Text GLabel 4400 4800 0    50   Input ~ 0
Gnd
Text GLabel 4400 4900 0    50   Input ~ 0
D_CS
Text GLabel 4400 5000 0    50   Input ~ 0
RST
Text GLabel 4400 5200 0    50   Input ~ 0
MOSI
Text GLabel 4400 5300 0    50   Input ~ 0
SCK
Text GLabel 4400 5400 0    50   Input ~ 0
3.3V
Text GLabel 4400 5500 0    50   Input ~ 0
MISO
Text GLabel 4400 5600 0    50   Input ~ 0
SCK
Text GLabel 4400 5700 0    50   Input ~ 0
T_CS
Text GLabel 4400 5800 0    50   Input ~ 0
MOSI
Text GLabel 4400 5900 0    50   Input ~ 0
MISO
Text GLabel 4400 6000 0    50   Input ~ 0
T_IRQ
Text GLabel 1900 5650 2    50   Input ~ 0
D_CS
Text GLabel 1900 5450 2    50   Input ~ 0
MOSI
Text GLabel 1900 5350 2    50   Input ~ 0
MISO
Text GLabel 1900 5550 2    50   Input ~ 0
SCK
Text GLabel 1900 5750 2    50   Input ~ 0
T_CS
Text GLabel 1200 5450 0    50   Input ~ 0
T_IRQ
NoConn ~ 4400 5100
$EndSCHEMATC
