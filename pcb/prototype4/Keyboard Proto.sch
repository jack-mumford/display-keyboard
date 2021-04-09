EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A 11000 8500
encoding utf-8
Sheet 1 1
Title "Keyboard Prototype Board"
Date "2020-12-27"
Rev "4"
Comp "Prototype Schematic"
Comment1 "Based on FeatherS2 and ADP5589 keyboard IC"
Comment2 ""
Comment3 "License: MIT"
Comment4 "Design: cmumford"
$EndDescr
Text GLabel 5650 5850 2    50   Input ~ 0
3V3_SDA
Text GLabel 5650 5750 2    50   Input ~ 0
3V3_SCL
Text GLabel 5650 5550 2    50   Input ~ 0
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
Text GLabel 4800 4450 0    50   Input ~ 0
3V3
Text GLabel 4800 4650 0    50   Input ~ 0
GND
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
P 9950 5550
F 0 "J1" H 9978 5526 50  0000 L CNN
F 1 "Conn_01x14_Female" H 9400 4700 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x14_P2.54mm_Vertical" H 9950 5550 50  0001 C CNN
F 3 "~" H 9950 5550 50  0001 C CNN
	1    9950 5550
	1    0    0    -1  
$EndComp
Text GLabel 9750 4950 0    50   Input ~ 0
3V3
Text GLabel 9750 5050 0    50   Input ~ 0
GND
Text GLabel 9750 5150 0    50   Input ~ 0
D_CS
Text GLabel 9750 5250 0    50   Input ~ 0
RST
Text GLabel 9750 5450 0    50   Input ~ 0
MOSI
Text GLabel 9750 5550 0    50   Input ~ 0
SCK
Text GLabel 9750 5650 0    50   Input ~ 0
3V3
Text GLabel 9750 5750 0    50   Input ~ 0
MISO
Text GLabel 9750 5850 0    50   Input ~ 0
SCK
Text GLabel 9750 5950 0    50   Input ~ 0
T_CS
Text GLabel 9750 6050 0    50   Input ~ 0
MOSI
Text GLabel 9750 6150 0    50   Input ~ 0
MISO
Text GLabel 9750 6250 0    50   Input ~ 0
T_IRQ
Text GLabel 5650 5250 2    50   Input ~ 0
D_CS
Text GLabel 5650 5150 2    50   Input ~ 0
T_CS
Text GLabel 5650 5050 2    50   Input ~ 0
T_IRQ
Text GLabel 5650 4850 2    50   Input ~ 0
EN
Text GLabel 4800 5350 0    50   Input ~ 0
SCK
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
F 1 "2.2kΩ" V 1950 5650 50  0000 C CNN
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
F 1 "2.2kΩ" V 1700 5900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1600 5900 50  0001 C CNN
F 3 "~" H 1600 5900 50  0001 C CNN
	1    1600 5900
	0    1    1    0   
$EndComp
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
	3400 550  3400 8050
Wire Notes Line
	3400 4500 500  4500
Wire Notes Line
	3400 2500 500  2500
Wire Notes Line
	7000 500  7000 6750
Wire Notes Line
	7000 4150 10500 4150
Wire Notes Line
	7000 3350 3400 3350
Text Notes 9200 4550 0    118  ~ 0
LCD Header
Text Notes 5100 3900 0    197  ~ 0
MCU
Text Notes 4500 1150 0    118  ~ 0
Keyboard Matrix
Text Notes 900  4950 0    118  ~ 0
I2C pull-up resistors
Text Notes 7850 900  0    118  ~ 0
Keyboard control IC
$Comp
L Connector:Conn_01x02_Female J2
U 1 1 5FE93246
P 4300 5750
F 0 "J2" H 4328 5726 50  0000 L CNN
F 1 "UART Debug header" H 4100 5850 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4300 5750 50  0001 C CNN
F 3 "~" H 4300 5750 50  0001 C CNN
	1    4300 5750
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x05_Female J3
U 1 1 5FEA0C78
P 7950 5500
F 0 "J3" H 7978 5526 50  0000 L CNN
F 1 "Conn_01x05_Female" H 7350 5100 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 7950 5500 50  0001 C CNN
F 3 "~" H 7950 5500 50  0001 C CNN
	1    7950 5500
	1    0    0    -1  
$EndComp
Wire Notes Line
	8700 4150 8700 6750
Text Notes 7150 4550 0    118  ~ 0
Rotary Encoder
Text GLabel 7750 5600 0    50   Input ~ 0
3V3
Text GLabel 7750 5700 0    50   Input ~ 0
GND
Text GLabel 7750 5300 0    50   Input ~ 0
Enc_CLK
Text GLabel 7750 5400 0    50   Input ~ 0
Enc_DT
Text GLabel 7750 5500 0    50   Input ~ 0
Enc_SW
Text GLabel 4800 5150 0    50   Input ~ 0
Enc_CLK
Text GLabel 4800 5050 0    50   Input ~ 0
Enc_DT
Text GLabel 4800 4950 0    50   Input ~ 0
Enc_SW
Wire Wire Line
	4800 5650 4500 5650
Wire Wire Line
	4500 5750 4800 5750
NoConn ~ 5650 4450
NoConn ~ 5650 4550
NoConn ~ 5650 4750
NoConn ~ 5650 4950
NoConn ~ 4800 4850
NoConn ~ 4800 5850
NoConn ~ 4800 4750
NoConn ~ 4800 4550
$Comp
L 128x32_OLED_LCD_LED:128x32_OLED_LCD_LED U1
U 1 1 5FE96299
P 5100 7250
F 0 "U1" H 5278 7296 50  0000 L CNN
F 1 "128x32_OLED_LCD_LED" H 4700 6850 50  0000 L CNN
F 2 "128x32_OLED_LCD_LED:128x32_OLED_LCD_LED" H 5200 6800 50  0001 C CNN
F 3 "" H 5100 7250 50  0001 C CNN
F 4 "https://www.aliexpress.com/item/32672229793.html" H 5200 6900 50  0001 C CNN "URL"
	1    5100 7250
	1    0    0    -1  
$EndComp
Text GLabel 4900 7400 0    50   Input ~ 0
GND
Text GLabel 4900 7300 0    50   Input ~ 0
3V3
Text GLabel 4000 7100 0    50   Input ~ 0
SDA
Text GLabel 5650 5450 2    50   Input ~ 0
SDA
Text GLabel 5650 5350 2    50   Input ~ 0
SCL
Text GLabel 4000 7200 0    50   Input ~ 0
SCL
Wire Notes Line
	3400 6250 7000 6250
Text Notes 3950 6550 0    118  ~ 0
Volume Display
Text GLabel 4300 6750 0    50   Input ~ 0
3V3
Wire Wire Line
	4000 7100 4400 7100
Wire Wire Line
	4900 7200 4650 7200
$Comp
L Device:R_Small R7
U 1 1 5FEB56DF
P 4400 6950
F 0 "R7" H 4500 7000 50  0000 C CNN
F 1 "2.2kΩ" H 4550 6900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 4400 6950 50  0001 C CNN
F 3 "~" H 4400 6950 50  0001 C CNN
	1    4400 6950
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R8
U 1 1 5FEB5F07
P 4650 6950
F 0 "R8" H 4550 7000 50  0000 C CNN
F 1 "2.2kΩ" H 4500 6900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 4650 6950 50  0001 C CNN
F 3 "~" H 4650 6950 50  0001 C CNN
	1    4650 6950
	-1   0    0    1   
$EndComp
Wire Wire Line
	4300 6750 4400 6750
Wire Wire Line
	4650 6750 4650 6850
Wire Wire Line
	4650 7050 4650 7200
Connection ~ 4650 7200
Wire Wire Line
	4650 7200 4000 7200
Wire Wire Line
	4400 6750 4400 6850
Connection ~ 4400 6750
Wire Wire Line
	4400 6750 4650 6750
Wire Wire Line
	4400 7050 4400 7100
Connection ~ 4400 7100
Wire Wire Line
	4400 7100 4900 7100
Text Notes 5350 6550 0    79   ~ 0
(separate I2C)
Text GLabel 9750 5350 0    50   Input ~ 0
D_DC
Text GLabel 4800 5250 0    50   Input ~ 0
D_DC
Text GLabel 4800 5450 0    50   Input ~ 0
MOSI
Text GLabel 4800 5550 0    50   Input ~ 0
MISO
Text GLabel 5650 5650 2    50   Input ~ 0
3V3_RST
$Comp
L FeatherS2:FeatherS2 U5
U 1 1 60105E93
P 5250 5050
F 0 "U5" H 5225 6015 50  0000 C CNN
F 1 "FeatherS2" H 5225 5924 50  0000 C CNN
F 2 "FeatherS2:FeatherS2" H 5300 4050 50  0001 C CNN
F 3 "https://www.adafruit.com/product/4062" H 4900 5950 50  0001 C CNN
	1    5250 5050
	1    0    0    -1  
$EndComp
Text GLabel 4800 4350 0    50   Input ~ 0
RST
$EndSCHEMATC
