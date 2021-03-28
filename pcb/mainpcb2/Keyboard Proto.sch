EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A 11000 8500
encoding utf-8
Sheet 1 2
Title "Keyboard Prototype Board"
Date "2020-12-27"
Rev "2"
Comp "Prototype Schematic"
Comment1 "Based on FeatherS2 and LM8330 keyboard IC"
Comment2 ""
Comment3 "License: MIT"
Comment4 "Design: cmumford"
$EndDescr
Text GLabel 5650 5850 2    50   Input ~ 0
3V3_SDA
Text GLabel 5650 5750 2    50   Input ~ 0
3V3_SCL
Text GLabel 8200 1650 0    50   Input ~ 0
1V8_SDA
Text GLabel 8200 1750 0    50   Input ~ 0
1V8_SCL
Text GLabel 8200 1850 0    50   Input ~ 0
1V8_KBDINT
Text GLabel 5650 5550 2    50   Input ~ 0
3V3_KBDINT
Text GLabel 8200 1950 0    50   Input ~ 0
1V8_RST
Text GLabel 4800 4450 0    50   Input ~ 0
3V3
Text GLabel 4800 4650 0    50   Input ~ 0
GND
Text GLabel 8750 3650 3    50   Input ~ 0
GND
Text GLabel 8700 1200 1    50   Input ~ 0
1V8
Text GLabel 9250 1950 2    50   Input ~ 0
ROW0
Text GLabel 9250 1850 2    50   Input ~ 0
ROW1
Text GLabel 9250 2300 2    50   Input ~ 0
COL0
Text GLabel 9250 2200 2    50   Input ~ 0
COL1
Text GLabel 9250 2400 2    50   Input ~ 0
COL2
Text GLabel 9250 2500 2    50   Input ~ 0
COL3
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
$Comp
L LM8330:LM8330 U4
U 1 1 5FE7EC0A
P 9250 2150
F 0 "U4" H 8250 3100 50  0000 C CNN
F 1 "LM8330" H 8350 3000 50  0000 C CNN
F 2 "BGA0016:BGA0016" H 9250 2150 50  0001 C CNN
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
Text GLabel 5650 4850 2    50   Input ~ 0
EN
Text GLabel 1500 1700 0    50   Input ~ 0
EN
Text GLabel 1900 2050 3    50   Input ~ 0
GND
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
Text GLabel 1950 3250 1    50   Input ~ 0
3V3
Text GLabel 1850 3250 1    50   Input ~ 0
1V8
Text GLabel 1900 4150 3    50   Input ~ 0
GND
Text GLabel 1500 3500 0    50   Input ~ 0
GND
Text GLabel 4800 5350 0    50   Input ~ 0
SCK
Text GLabel 2300 3700 2    50   Input ~ 0
3V3_SDA
Text GLabel 2300 3600 2    50   Input ~ 0
3V3_SCL
Text GLabel 1500 3700 0    50   Input ~ 0
1V8_SDA
Text GLabel 1500 3600 0    50   Input ~ 0
1V8_SCL
Text GLabel 2300 3900 2    50   Input ~ 0
3V3_KBDINT
Text GLabel 1500 3900 0    50   Input ~ 0
1V8_KBDINT
Text GLabel 2300 3800 2    50   Input ~ 0
3V3_RST
Text GLabel 1500 3800 0    50   Input ~ 0
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
F 1 "226Ω" V 2250 6850 50  0000 C CNN
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
F 1 "226Ω" V 2000 7050 50  0000 C CNN
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
F 1 "226Ω" V 1750 7250 50  0000 C CNN
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
$Comp
L Device:R_Small R1
U 1 1 5FEB569E
P 1350 7450
F 0 "R1" V 1250 7450 50  0000 C CNN
F 1 "226Ω" V 1450 7450 50  0000 C CNN
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
Text Notes 4500 700  0    118  ~ 0
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
Wire Wire Line
	8700 1200 8700 1300
$Comp
L Device:C_Small C3
U 1 1 60362264
P 9100 1300
F 0 "C3" V 8871 1300 50  0000 C CNN
F 1 "0.1μF" V 8962 1300 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9100 1300 50  0001 C CNN
F 3 "~" H 9100 1300 50  0001 C CNN
	1    9100 1300
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 603638A2
P 9450 1350
F 0 "#PWR0101" H 9450 1100 50  0001 C CNN
F 1 "GND" H 9455 1177 50  0000 C CNN
F 2 "" H 9450 1350 50  0001 C CNN
F 3 "" H 9450 1350 50  0001 C CNN
	1    9450 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 1300 8700 1300
Connection ~ 8700 1300
Wire Wire Line
	8700 1300 8700 1450
Wire Wire Line
	9200 1300 9450 1300
Wire Wire Line
	9450 1300 9450 1350
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
$Sheet
S 3500 800  3400 2400
U 60611BA9
F0 "NewKeyboardMatrix" 50
F1 "NewKeyboardMatrix.sch" 50
F2 "ROW0" I L 3500 1100 50 
F3 "ROW1" I L 3500 1250 50 
F4 "ROW3" I L 3500 1400 50 
F5 "ROW4" I L 3500 1450 50 
F6 "ROW5" I L 3500 1600 50 
F7 "ROW2" I L 3500 1700 50 
F8 "COL0" I L 3500 1750 50 
F9 "COL1" I L 3500 1850 50 
F10 "COL2" I L 3500 1950 50 
F11 "COL3" I L 3500 2050 50 
F12 "COL4" I L 3500 2150 50 
F13 "COL5" I L 3500 2250 50 
F14 "COL6" I L 3500 2350 50 
F15 "COL7" I L 3500 2400 50 
F16 "COL8" I L 3500 2500 50 
F17 "COL9" I L 3500 2600 50 
F18 "COL10" I L 3500 2700 50 
F19 "COL11" I L 3500 2800 50 
F20 "COL12" I L 3500 2850 50 
F21 "COL13" I L 3500 2900 50 
F22 "COL14" I L 3500 2950 50 
F23 "COL15" I L 3500 3050 50 
F24 "COL16" I L 3500 3150 50 
F25 "COL17" I L 3500 1050 50 
$EndSheet
$EndSCHEMATC