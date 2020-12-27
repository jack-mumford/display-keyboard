EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A 11000 8500
encoding utf-8
Sheet 1 1
Title "Keyboard Prototype Board"
Date "2020-12-27"
Rev "2"
Comp "Prototype Schematic"
Comment1 "Based on FeatherS2 and LM8330 keyboard IC"
Comment2 ""
Comment3 "License: MIT"
Comment4 "Design: cmumford"
$EndDescr
$Comp
L Keyboard-Proto-rescue:Adafruit_Feather_nRF52840_Express-Adafruit_Feather_nRF52840_Express U1
U 1 1 5FCC3339
P 4800 6100
F 0 "U1" H 4800 7065 50  0000 C CNN
F 1 "Adafruit_Feather_nRF52840_Express" H 4800 6974 50  0000 C CNN
F 2 "Adafruit_Feather_nRF52840_Express:Adafruit_Feather_nRF52840_Express" H 4550 7000 50  0001 C CNN
F 3 "https://www.adafruit.com/product/4062" H 4550 7000 50  0001 C CNN
	1    4800 6100
	1    0    0    -1  
$EndComp
Text GLabel 4450 5400 0    50   Input ~ 0
3V3_SDA
Text GLabel 4450 5500 0    50   Input ~ 0
3V3_SCL
Text GLabel 8200 1650 0    50   Input ~ 0
1V8_SDA
Text GLabel 8200 1750 0    50   Input ~ 0
1V8_SCL
Text GLabel 8200 1850 0    50   Input ~ 0
1V8_KBDINT
Text GLabel 4450 5700 0    50   Input ~ 0
3V3_KBDINT
$Comp
L Switch:SW_Push Q1
U 1 1 5FCC9F75
P 4900 2000
F 0 "Q1" H 4900 2193 50  0001 C CNN
F 1 "Q" H 4900 2193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4900 2200 50  0001 C CNN
F 3 "~" H 4900 2200 50  0001 C CNN
	1    4900 2000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W1
U 1 1 5FCCA414
P 5500 2000
F 0 "W1" H 5500 2193 50  0001 C CNN
F 1 "W" H 5500 2193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 5500 2200 50  0001 C CNN
F 3 "~" H 5500 2200 50  0001 C CNN
	1    5500 2000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E1
U 1 1 5FCCAA98
P 6100 2000
F 0 "E1" H 6100 2193 50  0001 C CNN
F 1 "E" H 6100 2193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6100 2200 50  0001 C CNN
F 3 "~" H 6100 2200 50  0001 C CNN
	1    6100 2000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push A1
U 1 1 5FCCB8B3
P 4900 2600
F 0 "A1" H 4900 2793 50  0001 C CNN
F 1 "A" H 4900 2793 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4900 2800 50  0001 C CNN
F 3 "~" H 4900 2800 50  0001 C CNN
	1    4900 2600
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push S1
U 1 1 5FCCBF44
P 5500 2600
F 0 "S1" H 5500 2793 50  0001 C CNN
F 1 "S" H 5500 2793 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 5500 2800 50  0001 C CNN
F 3 "~" H 5500 2800 50  0001 C CNN
	1    5500 2600
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push D1
U 1 1 5FCCC6C4
P 6100 2600
F 0 "D1" H 6100 2793 50  0001 C CNN
F 1 "D" H 6100 2793 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6100 2800 50  0001 C CNN
F 3 "~" H 6100 2800 50  0001 C CNN
	1    6100 2600
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift1
U 1 1 5FCCCBD3
P 4300 2000
F 0 "Shift1" H 4300 2193 50  0001 C CNN
F 1 "Shift" H 4300 2193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4300 2200 50  0001 C CNN
F 3 "~" H 4300 2200 50  0001 C CNN
	1    4300 2000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Ctrl1
U 1 1 5FCCD3E9
P 4300 2600
F 0 "Ctrl1" H 4300 2793 50  0001 C CNN
F 1 "Ctrl" H 4300 2793 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4300 2800 50  0001 C CNN
F 3 "~" H 4300 2800 50  0001 C CNN
	1    4300 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 2000 4550 2000
Wire Wire Line
	4550 2000 4550 2600
Wire Wire Line
	5100 2000 5150 2000
Wire Wire Line
	5150 2000 5150 2600
Wire Wire Line
	5700 2000 5750 2000
Wire Wire Line
	5750 2000 5750 2600
Wire Wire Line
	6300 2000 6350 2000
Wire Wire Line
	6350 2000 6350 2600
Wire Wire Line
	5900 2100 5900 2000
Wire Wire Line
	5300 2000 5300 2100
Connection ~ 5300 2100
Wire Wire Line
	5300 2100 5900 2100
Wire Wire Line
	4700 2000 4700 2100
Connection ~ 4700 2100
Wire Wire Line
	4700 2100 5300 2100
Wire Wire Line
	4050 2600 4050 2700
Wire Wire Line
	4050 2700 4700 2700
Wire Wire Line
	5900 2700 5900 2600
Wire Wire Line
	4050 2600 4100 2600
Wire Wire Line
	5300 2600 5300 2700
Connection ~ 5300 2700
Wire Wire Line
	5300 2700 5900 2700
Wire Wire Line
	4700 2600 4700 2700
Connection ~ 4700 2700
Wire Wire Line
	4700 2700 5300 2700
Wire Wire Line
	4500 2600 4550 2600
Connection ~ 4550 2600
Wire Wire Line
	5100 2600 5150 2600
Wire Wire Line
	5700 2600 5750 2600
Wire Wire Line
	6300 2600 6350 2600
Wire Wire Line
	4550 2600 4550 2850
Text GLabel 8200 1950 0    50   Input ~ 0
1V8_RST
Text GLabel 5150 6900 2    50   Input ~ 0
3V3_RST
NoConn ~ 4450 5600
NoConn ~ 4450 5900
NoConn ~ 4450 6000
NoConn ~ 4450 6100
NoConn ~ 4450 6200
NoConn ~ 4450 6300
NoConn ~ 4450 6400
NoConn ~ 4450 6500
NoConn ~ 4450 6700
NoConn ~ 4450 6800
NoConn ~ 4450 6900
NoConn ~ 5150 5400
NoConn ~ 5150 6200
NoConn ~ 5150 6300
NoConn ~ 5150 6400
NoConn ~ 5150 6500
NoConn ~ 5150 6700
Text GLabel 5150 6800 2    50   Input ~ 0
3V3
Text GLabel 5150 6600 2    50   Input ~ 0
Gnd
Text GLabel 8750 3650 3    50   Input ~ 0
Gnd
Text GLabel 8700 1450 1    50   Input ~ 0
1V8
Text GLabel 9250 1950 2    50   Input ~ 0
ROW0
Text GLabel 9250 1850 2    50   Input ~ 0
ROW1
Text GLabel 9250 2200 2    50   Input ~ 0
COL0
Text GLabel 9250 2300 2    50   Input ~ 0
COL1
Text GLabel 9250 2400 2    50   Input ~ 0
COL2
Text GLabel 9250 2500 2    50   Input ~ 0
COL3
Text GLabel 6350 2850 3    50   Input ~ 0
COL0
Text GLabel 4550 2850 3    50   Input ~ 0
COL3
Text GLabel 5150 2850 3    50   Input ~ 0
COL2
Text GLabel 5750 2850 3    50   Input ~ 0
COL1
Wire Wire Line
	5150 2850 5150 2600
Connection ~ 5150 2600
Wire Wire Line
	5750 2600 5750 2850
Connection ~ 5750 2600
Wire Wire Line
	6350 2600 6350 2850
Connection ~ 6350 2600
Text GLabel 3950 2100 0    50   Input ~ 0
ROW0
Text GLabel 3950 2700 0    50   Input ~ 0
ROW1
Wire Wire Line
	3950 2100 4050 2100
Wire Wire Line
	3950 2700 4050 2700
Connection ~ 4050 2700
Wire Wire Line
	4100 2000 4050 2000
Wire Wire Line
	4050 2000 4050 2100
Connection ~ 4050 2100
Wire Wire Line
	4050 2100 4700 2100
$Comp
L Connector:Conn_01x14_Female J1
U 1 1 5FCD18E2
P 8850 5700
F 0 "J1" H 8878 5676 50  0000 L CNN
F 1 "Conn_01x14_Female" H 8878 5585 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x14_P2.54mm_Vertical" H 8850 5700 50  0001 C CNN
F 3 "~" H 8850 5700 50  0001 C CNN
	1    8850 5700
	1    0    0    -1  
$EndComp
Text GLabel 8650 5100 0    50   Input ~ 0
3V3
Text GLabel 8650 5200 0    50   Input ~ 0
Gnd
Text GLabel 8650 5300 0    50   Input ~ 0
D_CS
Text GLabel 8650 5400 0    50   Input ~ 0
3V3_RST
Text GLabel 8650 5600 0    50   Input ~ 0
MOSI
Text GLabel 8650 5700 0    50   Input ~ 0
SCK
Text GLabel 8650 5800 0    50   Input ~ 0
3V3
Text GLabel 8650 5900 0    50   Input ~ 0
MISO
Text GLabel 8650 6000 0    50   Input ~ 0
SCK
Text GLabel 8650 6100 0    50   Input ~ 0
T_CS
Text GLabel 8650 6200 0    50   Input ~ 0
MOSI
Text GLabel 8650 6300 0    50   Input ~ 0
MISO
Text GLabel 8650 6400 0    50   Input ~ 0
T_IRQ
Text GLabel 5150 6000 2    50   Input ~ 0
D_CS
Text GLabel 5150 5800 2    50   Input ~ 0
MOSI
Text GLabel 5150 5700 2    50   Input ~ 0
MISO
Text GLabel 5150 6100 2    50   Input ~ 0
T_CS
Text GLabel 4450 5800 0    50   Input ~ 0
T_IRQ
NoConn ~ 8650 5500
$Comp
L LM8330:LM8330 U4
U 1 1 5FE7EC0A
P 9250 2150
F 0 "U4" H 8250 3100 50  0000 C CNN
F 1 "LM8330" H 8350 3000 50  0000 C CNN
F 2 "LM8330:BGA-25_5x5_2.0x2.0mm" H 9250 2150 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/lm8330.pdf" H 9250 2150 50  0001 C CNN
F 4 "Texas Instruments" H 8150 2900 50  0000 C CNN "Manufacturer"
	1    9250 2150
	1    0    0    -1  
$EndComp
NoConn ~ 9250 1650
NoConn ~ 9250 1750
NoConn ~ 9250 2600
NoConn ~ 9250 2700
NoConn ~ 9250 2800
NoConn ~ 9250 2900
NoConn ~ 9250 3050
NoConn ~ 9250 3150
NoConn ~ 9250 3250
NoConn ~ 8200 2350
NoConn ~ 8200 2450
NoConn ~ 8200 2550
NoConn ~ 8200 2650
$Comp
L Regulator_Linear:MIC5504-1.8YM5 U3
U 1 1 5FE8CB62
P 1900 1600
F 0 "U3" H 1900 1967 50  0000 C CNN
F 1 "MIC5504-1.8YM5" H 1900 1876 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 1900 1200 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/MIC550X.pdf" H 1650 1850 50  0001 C CNN
	1    1900 1600
	1    0    0    -1  
$EndComp
$Comp
L LSF0204DPWR:LSF0204DPWR U2
U 1 1 5FE8D187
P 1900 3700
F 0 "U2" H 2200 4250 50  0000 C CNN
F 1 "LSF0204DPWR" H 2350 4150 50  0000 C CNN
F 2 "Package_SO:TSSOP-14_4.4x5mm_P0.65mm" H 2850 3250 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/lsf0204d.pdf" H 1350 4150 50  0001 C CNN
F 4 "Texas Instruments" H 2400 4350 50  0001 C CNN "Manufacturer"
	1    1900 3700
	1    0    0    -1  
$EndComp
Text GLabel 1000 1500 0    50   Input ~ 0
3V3
Text GLabel 4450 6600 0    50   Input ~ 0
EN
Text GLabel 1500 1700 0    50   Input ~ 0
EN
Text GLabel 1900 2050 3    50   Input ~ 0
Gnd
Wire Wire Line
	1900 1900 1900 1950
$Comp
L Device:C_Small C1
U 1 1 5FE9973B
P 1200 1750
F 0 "C1" H 1000 1800 50  0000 L CNN
F 1 "1µF/10V" H 750 1700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1200 1750 50  0001 C CNN
F 3 "~" H 1200 1750 50  0001 C CNN
	1    1200 1750
	1    0    0    -1  
$EndComp
Text GLabel 2600 1500 2    50   Input ~ 0
1V8
Wire Wire Line
	2300 1500 2450 1500
$Comp
L Device:C_Small C2
U 1 1 5FE9BA17
P 2450 1750
F 0 "C2" H 2542 1796 50  0000 L CNN
F 1 "1µF/10V" H 2542 1705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2450 1750 50  0001 C CNN
F 3 "~" H 2450 1750 50  0001 C CNN
	1    2450 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 1950 2450 1950
Wire Wire Line
	2450 1950 2450 1850
Connection ~ 1900 1950
Wire Wire Line
	2450 1650 2450 1500
Connection ~ 2450 1500
Wire Wire Line
	2450 1500 2600 1500
Wire Wire Line
	1000 1500 1200 1500
Wire Wire Line
	1200 1850 1200 1950
Wire Wire Line
	1200 1650 1200 1500
Connection ~ 1200 1500
Wire Wire Line
	1200 1500 1500 1500
Wire Wire Line
	1900 1950 1200 1950
Wire Wire Line
	1900 1950 1900 2050
Text GLabel 1850 3250 1    50   Input ~ 0
3V3
Text GLabel 1950 3250 1    50   Input ~ 0
1V8
Text GLabel 1900 4150 3    50   Input ~ 0
Gnd
Text GLabel 1500 3500 0    50   Input ~ 0
EN
Text GLabel 5150 5900 2    50   Input ~ 0
SCK
Text GLabel 1500 3600 0    50   Input ~ 0
3V3_SDA
Text GLabel 1500 3700 0    50   Input ~ 0
3V3_SCL
Text GLabel 2300 3600 2    50   Input ~ 0
1V8_SDA
Text GLabel 2300 3700 2    50   Input ~ 0
1V8_SCL
Text GLabel 1500 3800 0    50   Input ~ 0
3V3_KBDINT
Text GLabel 2300 3800 2    50   Input ~ 0
1V8_KBDINT
Text GLabel 1500 3900 0    50   Input ~ 0
3V3_RST
Text GLabel 2300 3900 2    50   Input ~ 0
1V8_RST
Text GLabel 2350 6850 2    50   Input ~ 0
1V8_SDA
Text GLabel 2350 7050 2    50   Input ~ 0
1V8_SCL
Text GLabel 2350 7250 2    50   Input ~ 0
1V8_KBDINT
Text GLabel 2350 7450 2    50   Input ~ 0
1V8_RST
Text GLabel 1100 6650 1    50   Input ~ 0
1V8
$Comp
L Device:R_Small R6
U 1 1 5FEB396C
P 2150 6850
F 0 "R6" V 2050 6850 50  0000 C CNN
F 1 "106Ω" V 2250 6850 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 2150 6850 50  0001 C CNN
F 3 "~" H 2150 6850 50  0001 C CNN
	1    2150 6850
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R5
U 1 1 5FEB4643
P 1900 7050
F 0 "R5" V 1800 7050 50  0000 C CNN
F 1 "106Ω" V 2000 7050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1900 7050 50  0001 C CNN
F 3 "~" H 1900 7050 50  0001 C CNN
	1    1900 7050
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R3
U 1 1 5FEB5280
P 1650 7250
F 0 "R3" V 1550 7250 50  0000 C CNN
F 1 "106Ω" V 1750 7250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1650 7250 50  0001 C CNN
F 3 "~" H 1650 7250 50  0001 C CNN
	1    1650 7250
	0    1    1    0   
$EndComp
Wire Wire Line
	1100 6650 1100 6850
Wire Wire Line
	1100 7450 1250 7450
Wire Wire Line
	2250 6850 2350 6850
Wire Wire Line
	2050 6850 1100 6850
Connection ~ 1100 6850
Wire Wire Line
	1100 6850 1100 7050
Connection ~ 1100 7050
Wire Wire Line
	1100 7050 1100 7250
Connection ~ 1100 7250
Wire Wire Line
	1100 7250 1100 7450
Text GLabel 2000 5650 2    50   Input ~ 0
3V3_SDA
Text GLabel 2000 5900 2    50   Input ~ 0
3V3_SCL
Text GLabel 1450 5400 1    50   Input ~ 0
3V3
$Comp
L Device:R_Small R4
U 1 1 5FEC5323
P 1850 5650
F 0 "R4" V 1750 5650 50  0000 C CNN
F 1 "5kΩ" V 1950 5650 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1850 5650 50  0001 C CNN
F 3 "~" H 1850 5650 50  0001 C CNN
	1    1850 5650
	0    1    1    0   
$EndComp
Wire Wire Line
	2000 5650 1950 5650
$Comp
L Device:R_Small R2
U 1 1 5FEC6AE7
P 1600 5900
F 0 "R2" V 1500 5900 50  0000 C CNN
F 1 "5kΩ" V 1700 5900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1600 5900 50  0001 C CNN
F 3 "~" H 1600 5900 50  0001 C CNN
	1    1600 5900
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R1
U 1 1 5FEB569E
P 1350 7450
F 0 "R1" V 1250 7450 50  0000 C CNN
F 1 "106Ω" V 1450 7450 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1350 7450 50  0001 C CNN
F 3 "~" H 1350 7450 50  0001 C CNN
	1    1350 7450
	0    1    1    0   
$EndComp
Wire Wire Line
	1450 7450 2350 7450
Wire Wire Line
	1550 7250 1100 7250
Wire Wire Line
	1750 7250 2350 7250
Wire Wire Line
	1800 7050 1100 7050
Wire Wire Line
	2000 7050 2350 7050
Wire Wire Line
	1700 5900 2000 5900
Wire Wire Line
	1450 5400 1450 5650
Wire Wire Line
	1450 5900 1500 5900
Wire Wire Line
	1750 5650 1450 5650
Connection ~ 1450 5650
Wire Wire Line
	1450 5650 1450 5900
Wire Notes Line
	3400 500  3400 8000
Wire Notes Line
	3400 4500 500  4500
Wire Notes Line
	3400 2500 500  2500
Wire Notes Line
	7000 500  7000 6750
Wire Notes Line
	7000 4150 10500 4150
Wire Notes Line
	7000 3950 3400 3950
Text Notes 8300 4650 0    118  ~ 0
LCD Header
Text Notes 4750 4600 0    197  ~ 0
MCU
Text Notes 4500 1150 0    118  ~ 0
Keyboard Matrix
Text Notes 1250 900  0    118  ~ 0
1.8V Regulator
Text Notes 750  2900 0    118  ~ 0
3.3V ↔ 1.8V level shifter
Text Notes 1150 4950 0    118  ~ 0
Pull-up resistors
Text Notes 7850 900  0    118  ~ 0
Keyboard control IC
$Comp
L Connector:Conn_01x02_Female J2
U 1 1 5FE93246
P 6150 5500
F 0 "J2" H 6178 5476 50  0000 L CNN
F 1 "UART Debug header" H 5750 5300 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6150 5500 50  0001 C CNN
F 3 "~" H 6150 5500 50  0001 C CNN
	1    6150 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 5500 5950 5500
Wire Wire Line
	5150 5600 5950 5600
$EndSCHEMATC
