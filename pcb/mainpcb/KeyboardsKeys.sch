EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Switch:SW_Push Q?
U 1 1 60C9DBFA
P 1900 1150
F 0 "Q?" H 1900 1343 50  0001 C CNN
F 1 "Q" H 1900 1343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 1900 1350 50  0001 C CNN
F 3 "~" H 1900 1350 50  0001 C CNN
	1    1900 1150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 60C9DC00
P 2500 1150
F 0 "W?" H 2500 1343 50  0001 C CNN
F 1 "W" H 2500 1343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 2500 1350 50  0001 C CNN
F 3 "~" H 2500 1350 50  0001 C CNN
	1    2500 1150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 60C9DC06
P 3100 1150
F 0 "E?" H 3100 1343 50  0001 C CNN
F 1 "E" H 3100 1343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3100 1350 50  0001 C CNN
F 3 "~" H 3100 1350 50  0001 C CNN
	1    3100 1150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push A?
U 1 1 60C9DC0C
P 1900 1750
F 0 "A?" H 1900 1943 50  0001 C CNN
F 1 "A" H 1900 1943 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 1900 1950 50  0001 C CNN
F 3 "~" H 1900 1950 50  0001 C CNN
	1    1900 1750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push S?
U 1 1 60C9DC12
P 2500 1750
F 0 "S?" H 2500 1943 50  0001 C CNN
F 1 "S" H 2500 1943 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 2500 1950 50  0001 C CNN
F 3 "~" H 2500 1950 50  0001 C CNN
	1    2500 1750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push D?
U 1 1 60C9DC18
P 3100 1750
F 0 "D?" H 3100 1943 50  0001 C CNN
F 1 "D" H 3100 1943 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3100 1950 50  0001 C CNN
F 3 "~" H 3100 1950 50  0001 C CNN
	1    3100 1750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 60C9DC1E
P 1300 1150
F 0 "Shift?" H 1300 1343 50  0001 C CNN
F 1 "Shift" H 1300 1343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 1300 1350 50  0001 C CNN
F 3 "~" H 1300 1350 50  0001 C CNN
	1    1300 1150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Ctrl?
U 1 1 60C9DC24
P 1300 1750
F 0 "Ctrl?" H 1300 1943 50  0001 C CNN
F 1 "Ctrl" H 1300 1943 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 1300 1950 50  0001 C CNN
F 3 "~" H 1300 1950 50  0001 C CNN
	1    1300 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 1150 1550 1150
Wire Wire Line
	1550 1150 1550 1750
Wire Wire Line
	2100 1150 2150 1150
Wire Wire Line
	2150 1150 2150 1750
Wire Wire Line
	2700 1150 2750 1150
Wire Wire Line
	2750 1150 2750 1750
Wire Wire Line
	3300 1150 3350 1150
Wire Wire Line
	3350 1150 3350 1750
Wire Wire Line
	2900 1250 2900 1150
Wire Wire Line
	2300 1150 2300 1250
Connection ~ 2300 1250
Wire Wire Line
	2300 1250 2900 1250
Wire Wire Line
	1700 1150 1700 1250
Connection ~ 1700 1250
Wire Wire Line
	1700 1250 2300 1250
Wire Wire Line
	1050 1750 1050 1850
Wire Wire Line
	1050 1850 1700 1850
Wire Wire Line
	2900 1850 2900 1750
Wire Wire Line
	1050 1750 1100 1750
Wire Wire Line
	2300 1750 2300 1850
Connection ~ 2300 1850
Wire Wire Line
	2300 1850 2900 1850
Wire Wire Line
	1700 1750 1700 1850
Connection ~ 1700 1850
Wire Wire Line
	1700 1850 2300 1850
Wire Wire Line
	1500 1750 1550 1750
Connection ~ 1550 1750
Wire Wire Line
	2100 1750 2150 1750
Wire Wire Line
	2700 1750 2750 1750
Wire Wire Line
	3300 1750 3350 1750
Wire Wire Line
	1550 1750 1550 2000
Text GLabel 3350 2000 3    50   Input ~ 0
COL0
Text GLabel 1550 2000 3    50   Input ~ 0
COL3
Text GLabel 2150 2000 3    50   Input ~ 0
COL2
Text GLabel 2750 2000 3    50   Input ~ 0
COL1
Wire Wire Line
	2150 2000 2150 1750
Connection ~ 2150 1750
Wire Wire Line
	2750 1750 2750 2000
Connection ~ 2750 1750
Wire Wire Line
	3350 1750 3350 2000
Connection ~ 3350 1750
Text GLabel 950  1250 0    50   Input ~ 0
ROW0
Text GLabel 950  1850 0    50   Input ~ 0
ROW1
Wire Wire Line
	950  1250 1050 1250
Wire Wire Line
	950  1850 1050 1850
Connection ~ 1050 1850
Wire Wire Line
	1100 1150 1050 1150
Wire Wire Line
	1050 1150 1050 1250
Connection ~ 1050 1250
Wire Wire Line
	1050 1250 1700 1250
$EndSCHEMATC
