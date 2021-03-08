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
U 1 1 6047B311
P 3000 4500
AR Path="/6047B311" Ref="Q?"  Part="1" 
AR Path="/60458A5B/6047B311" Ref="Q?"  Part="1" 
F 0 "Q?" H 3000 4693 50  0001 C CNN
F 1 "A" H 3000 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3000 4700 50  0001 C CNN
F 3 "~" H 3000 4700 50  0001 C CNN
	1    3000 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 6047B317
P 3600 4500
AR Path="/6047B317" Ref="W?"  Part="1" 
AR Path="/60458A5B/6047B317" Ref="W?"  Part="1" 
F 0 "W?" H 3600 4693 50  0001 C CNN
F 1 "S" H 3600 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3600 4700 50  0001 C CNN
F 3 "~" H 3600 4700 50  0001 C CNN
	1    3600 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 6047B31D
P 4200 4500
AR Path="/6047B31D" Ref="E?"  Part="1" 
AR Path="/60458A5B/6047B31D" Ref="E?"  Part="1" 
F 0 "E?" H 4200 4693 50  0001 C CNN
F 1 "D" H 4200 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4200 4700 50  0001 C CNN
F 3 "~" H 4200 4700 50  0001 C CNN
	1    4200 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push A?
U 1 1 6047B323
P 3000 5100
AR Path="/6047B323" Ref="A?"  Part="1" 
AR Path="/60458A5B/6047B323" Ref="A?"  Part="1" 
F 0 "A?" H 3000 5293 50  0001 C CNN
F 1 "Z" H 3000 5293 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3000 5300 50  0001 C CNN
F 3 "~" H 3000 5300 50  0001 C CNN
	1    3000 5100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push S?
U 1 1 6047B329
P 3600 5100
AR Path="/6047B329" Ref="S?"  Part="1" 
AR Path="/60458A5B/6047B329" Ref="S?"  Part="1" 
F 0 "S?" H 3600 5293 50  0001 C CNN
F 1 "X" H 3600 5293 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3600 5300 50  0001 C CNN
F 3 "~" H 3600 5300 50  0001 C CNN
	1    3600 5100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push D?
U 1 1 6047B32F
P 4200 5100
AR Path="/6047B32F" Ref="D?"  Part="1" 
AR Path="/60458A5B/6047B32F" Ref="D?"  Part="1" 
F 0 "D?" H 4200 5293 50  0001 C CNN
F 1 "C" H 4200 5293 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4200 5300 50  0001 C CNN
F 3 "~" H 4200 5300 50  0001 C CNN
	1    4200 5100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 6047B335
P 2400 4500
AR Path="/6047B335" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/6047B335" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 2400 4693 50  0001 C CNN
F 1 "CAPS" H 2400 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 2400 4700 50  0001 C CNN
F 3 "~" H 2400 4700 50  0001 C CNN
	1    2400 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Ctrl?
U 1 1 6047B33B
P 2400 5100
AR Path="/6047B33B" Ref="Ctrl?"  Part="1" 
AR Path="/60458A5B/6047B33B" Ref="Ctrl?"  Part="1" 
F 0 "Ctrl?" H 2400 5293 50  0001 C CNN
F 1 "Shift" H 2400 5293 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 2400 5300 50  0001 C CNN
F 3 "~" H 2400 5300 50  0001 C CNN
	1    2400 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 4500 2650 4500
Wire Wire Line
	2650 4500 2650 5100
Wire Wire Line
	3200 4500 3250 4500
Wire Wire Line
	3250 4500 3250 5100
Wire Wire Line
	3800 4500 3850 4500
Wire Wire Line
	4400 4500 4450 4500
Wire Wire Line
	4000 4600 4000 4500
Wire Wire Line
	3400 4500 3400 4600
Connection ~ 3400 4600
Wire Wire Line
	3400 4600 4000 4600
Wire Wire Line
	2800 4500 2800 4600
Connection ~ 2800 4600
Wire Wire Line
	2800 4600 3400 4600
Wire Wire Line
	2150 5100 2150 5200
Wire Wire Line
	2150 5200 2800 5200
Wire Wire Line
	4000 5200 4000 5100
Wire Wire Line
	2150 5100 2200 5100
Wire Wire Line
	3400 5100 3400 5200
Connection ~ 3400 5200
Wire Wire Line
	2800 5100 2800 5200
Connection ~ 2800 5200
Wire Wire Line
	2800 5200 3400 5200
Wire Wire Line
	2600 5100 2650 5100
Connection ~ 2650 5100
Wire Wire Line
	3200 5100 3250 5100
Wire Wire Line
	3800 5100 3850 5100
Wire Wire Line
	4400 5100 4450 5100
Connection ~ 3250 5100
Connection ~ 3850 5100
Connection ~ 4450 5100
Wire Wire Line
	2050 4600 2150 4600
Wire Wire Line
	2050 5200 2150 5200
Connection ~ 2150 5200
Wire Wire Line
	2200 4500 2150 4500
Wire Wire Line
	2150 4500 2150 4600
Connection ~ 2150 4600
Wire Wire Line
	2150 4600 2800 4600
Wire Wire Line
	4450 4500 4450 5100
$Comp
L Switch:SW_Push Q?
U 1 1 604C619E
P 5450 4500
AR Path="/604C619E" Ref="Q?"  Part="1" 
AR Path="/60458A5B/604C619E" Ref="Q?"  Part="1" 
F 0 "Q?" H 5450 4693 50  0001 C CNN
F 1 "G" H 5450 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 5450 4700 50  0001 C CNN
F 3 "~" H 5450 4700 50  0001 C CNN
	1    5450 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 604C61A4
P 6050 4500
AR Path="/604C61A4" Ref="W?"  Part="1" 
AR Path="/60458A5B/604C61A4" Ref="W?"  Part="1" 
F 0 "W?" H 6050 4693 50  0001 C CNN
F 1 "H" H 6050 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6050 4700 50  0001 C CNN
F 3 "~" H 6050 4700 50  0001 C CNN
	1    6050 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 604C61AA
P 6650 4500
AR Path="/604C61AA" Ref="E?"  Part="1" 
AR Path="/60458A5B/604C61AA" Ref="E?"  Part="1" 
F 0 "E?" H 6650 4693 50  0001 C CNN
F 1 "J" H 6650 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6650 4700 50  0001 C CNN
F 3 "~" H 6650 4700 50  0001 C CNN
	1    6650 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push A?
U 1 1 604C61B0
P 5450 5100
AR Path="/604C61B0" Ref="A?"  Part="1" 
AR Path="/60458A5B/604C61B0" Ref="A?"  Part="1" 
F 0 "A?" H 5450 5293 50  0001 C CNN
F 1 "B" H 5450 5293 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 5450 5300 50  0001 C CNN
F 3 "~" H 5450 5300 50  0001 C CNN
	1    5450 5100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push S?
U 1 1 604C61B6
P 6050 5100
AR Path="/604C61B6" Ref="S?"  Part="1" 
AR Path="/60458A5B/604C61B6" Ref="S?"  Part="1" 
F 0 "S?" H 6050 5293 50  0001 C CNN
F 1 "N" H 6050 5293 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6050 5300 50  0001 C CNN
F 3 "~" H 6050 5300 50  0001 C CNN
	1    6050 5100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push D?
U 1 1 604C61BC
P 6650 5100
AR Path="/604C61BC" Ref="D?"  Part="1" 
AR Path="/60458A5B/604C61BC" Ref="D?"  Part="1" 
F 0 "D?" H 6650 5293 50  0001 C CNN
F 1 "M" H 6650 5293 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6650 5300 50  0001 C CNN
F 3 "~" H 6650 5300 50  0001 C CNN
	1    6650 5100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 604C61C2
P 4850 4500
AR Path="/604C61C2" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/604C61C2" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 4850 4693 50  0001 C CNN
F 1 "F" H 4850 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4850 4700 50  0001 C CNN
F 3 "~" H 4850 4700 50  0001 C CNN
	1    4850 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Ctrl?
U 1 1 604C61C8
P 4850 5100
AR Path="/604C61C8" Ref="Ctrl?"  Part="1" 
AR Path="/60458A5B/604C61C8" Ref="Ctrl?"  Part="1" 
F 0 "Ctrl?" H 4850 5293 50  0001 C CNN
F 1 "V" H 4850 5293 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4850 5300 50  0001 C CNN
F 3 "~" H 4850 5300 50  0001 C CNN
	1    4850 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 4500 5100 4500
Wire Wire Line
	5100 4500 5100 5100
Wire Wire Line
	5650 4500 5700 4500
Wire Wire Line
	5700 4500 5700 5100
Wire Wire Line
	6250 4500 6300 4500
Wire Wire Line
	6850 4500 6900 4500
Wire Wire Line
	6450 4600 6450 4500
Wire Wire Line
	5850 4500 5850 4600
Connection ~ 5850 4600
Wire Wire Line
	5850 4600 6450 4600
Wire Wire Line
	5250 4500 5250 4600
Connection ~ 5250 4600
Wire Wire Line
	5250 4600 5850 4600
Wire Wire Line
	4600 5100 4600 5200
Wire Wire Line
	4600 5200 5250 5200
Wire Wire Line
	6450 5200 6450 5100
Wire Wire Line
	4600 5100 4650 5100
Wire Wire Line
	5850 5100 5850 5200
Connection ~ 5850 5200
Wire Wire Line
	5850 5200 6450 5200
Wire Wire Line
	5250 5100 5250 5200
Connection ~ 5250 5200
Wire Wire Line
	5250 5200 5850 5200
Wire Wire Line
	5050 5100 5100 5100
Connection ~ 5100 5100
Wire Wire Line
	5650 5100 5700 5100
Wire Wire Line
	6250 5100 6300 5100
Wire Wire Line
	6850 5100 6900 5100
Connection ~ 5700 5100
Connection ~ 6300 5100
Connection ~ 6900 5100
Connection ~ 4600 5200
Wire Wire Line
	4650 4500 4600 4500
Wire Wire Line
	4600 4500 4600 4600
Connection ~ 4600 4600
Wire Wire Line
	4600 4600 5250 4600
Wire Wire Line
	6900 4500 6900 5100
$Comp
L Switch:SW_Push Q?
U 1 1 604CA3C4
P 7900 4500
AR Path="/604CA3C4" Ref="Q?"  Part="1" 
AR Path="/60458A5B/604CA3C4" Ref="Q?"  Part="1" 
F 0 "Q?" H 7900 4693 50  0001 C CNN
F 1 "L" H 7900 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 7900 4700 50  0001 C CNN
F 3 "~" H 7900 4700 50  0001 C CNN
	1    7900 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 604CA3CA
P 8500 4500
AR Path="/604CA3CA" Ref="W?"  Part="1" 
AR Path="/60458A5B/604CA3CA" Ref="W?"  Part="1" 
F 0 "W?" H 8500 4693 50  0001 C CNN
F 1 ";" H 8500 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 8500 4700 50  0001 C CNN
F 3 "~" H 8500 4700 50  0001 C CNN
	1    8500 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 604CA3D0
P 9100 4500
AR Path="/604CA3D0" Ref="E?"  Part="1" 
AR Path="/60458A5B/604CA3D0" Ref="E?"  Part="1" 
F 0 "E?" H 9100 4693 50  0001 C CNN
F 1 "'" H 9100 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 9100 4700 50  0001 C CNN
F 3 "~" H 9100 4700 50  0001 C CNN
	1    9100 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push A?
U 1 1 604CA3D6
P 7900 5100
AR Path="/604CA3D6" Ref="A?"  Part="1" 
AR Path="/60458A5B/604CA3D6" Ref="A?"  Part="1" 
F 0 "A?" H 7900 5293 50  0001 C CNN
F 1 "." H 7900 5293 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 7900 5300 50  0001 C CNN
F 3 "~" H 7900 5300 50  0001 C CNN
	1    7900 5100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push S?
U 1 1 604CA3DC
P 8500 5100
AR Path="/604CA3DC" Ref="S?"  Part="1" 
AR Path="/60458A5B/604CA3DC" Ref="S?"  Part="1" 
F 0 "S?" H 8500 5293 50  0001 C CNN
F 1 "/" H 8500 5293 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 8500 5300 50  0001 C CNN
F 3 "~" H 8500 5300 50  0001 C CNN
	1    8500 5100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push D?
U 1 1 604CA3E2
P 9100 5100
AR Path="/604CA3E2" Ref="D?"  Part="1" 
AR Path="/60458A5B/604CA3E2" Ref="D?"  Part="1" 
F 0 "D?" H 9100 5293 50  0001 C CNN
F 1 "Right Shift" H 9100 5293 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 9100 5300 50  0001 C CNN
F 3 "~" H 9100 5300 50  0001 C CNN
	1    9100 5100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 604CA3E8
P 7300 4500
AR Path="/604CA3E8" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/604CA3E8" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 7300 4693 50  0001 C CNN
F 1 "K" H 7300 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 7300 4700 50  0001 C CNN
F 3 "~" H 7300 4700 50  0001 C CNN
	1    7300 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Ctrl?
U 1 1 604CA3EE
P 7300 5100
AR Path="/604CA3EE" Ref="Ctrl?"  Part="1" 
AR Path="/60458A5B/604CA3EE" Ref="Ctrl?"  Part="1" 
F 0 "Ctrl?" H 7300 5293 50  0001 C CNN
F 1 "," H 7300 5293 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 7300 5300 50  0001 C CNN
F 3 "~" H 7300 5300 50  0001 C CNN
	1    7300 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 4500 7550 4500
Wire Wire Line
	7550 4500 7550 5100
Wire Wire Line
	8100 4500 8150 4500
Wire Wire Line
	8150 4500 8150 5100
Wire Wire Line
	8700 4500 8750 4500
Wire Wire Line
	8750 4500 8750 5100
Wire Wire Line
	9300 4500 9350 4500
Wire Wire Line
	8900 4600 8900 4500
Wire Wire Line
	8300 4500 8300 4600
Connection ~ 8300 4600
Wire Wire Line
	8300 4600 8900 4600
Wire Wire Line
	7700 4500 7700 4600
Connection ~ 7700 4600
Wire Wire Line
	7700 4600 8300 4600
Wire Wire Line
	7050 5100 7050 5200
Wire Wire Line
	7050 5200 7700 5200
Wire Wire Line
	8900 5200 8900 5100
Wire Wire Line
	7050 5100 7100 5100
Wire Wire Line
	8300 5100 8300 5200
Connection ~ 8300 5200
Wire Wire Line
	8300 5200 8900 5200
Wire Wire Line
	7700 5100 7700 5200
Connection ~ 7700 5200
Wire Wire Line
	7700 5200 8300 5200
Wire Wire Line
	7500 5100 7550 5100
Connection ~ 7550 5100
Wire Wire Line
	8100 5100 8150 5100
Wire Wire Line
	8700 5100 8750 5100
Wire Wire Line
	9300 5100 9350 5100
Wire Wire Line
	7550 5100 7550 5350
Wire Wire Line
	8150 5350 8150 5100
Connection ~ 8150 5100
Wire Wire Line
	8750 5100 8750 5350
Connection ~ 8750 5100
Wire Wire Line
	9350 5100 9350 5350
Connection ~ 9350 5100
Connection ~ 7050 5200
Wire Wire Line
	7100 4500 7050 4500
Wire Wire Line
	7050 4500 7050 4600
Wire Wire Line
	7050 4600 7700 4600
Wire Wire Line
	9350 4500 9350 5100
Connection ~ 4000 4600
Wire Wire Line
	4000 4600 4600 4600
Wire Wire Line
	3400 5200 4000 5200
Connection ~ 4000 5200
Connection ~ 6450 5200
Wire Wire Line
	9900 4500 9950 4500
Wire Wire Line
	9500 4600 9500 4500
Wire Wire Line
	9950 4500 9950 5100
Connection ~ 8900 4600
$Comp
L Switch:SW_Push E?
U 1 1 60538CC7
P 9700 4500
AR Path="/60538CC7" Ref="E?"  Part="1" 
AR Path="/60458A5B/60538CC7" Ref="E?"  Part="1" 
F 0 "E?" H 9700 4693 50  0001 C CNN
F 1 "ENTER" H 9700 4693 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 9700 4700 50  0001 C CNN
F 3 "~" H 9700 4700 50  0001 C CNN
	1    9700 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Q?
U 1 1 60553C00
P 3000 3800
AR Path="/60553C00" Ref="Q?"  Part="1" 
AR Path="/60458A5B/60553C00" Ref="Q?"  Part="1" 
F 0 "Q?" H 3000 3993 50  0001 C CNN
F 1 "Q" H 3000 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3000 4000 50  0001 C CNN
F 3 "~" H 3000 4000 50  0001 C CNN
	1    3000 3800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 60553C06
P 3600 3800
AR Path="/60553C06" Ref="W?"  Part="1" 
AR Path="/60458A5B/60553C06" Ref="W?"  Part="1" 
F 0 "W?" H 3600 3993 50  0001 C CNN
F 1 "W" H 3600 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3600 4000 50  0001 C CNN
F 3 "~" H 3600 4000 50  0001 C CNN
	1    3600 3800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 60553C0C
P 4200 3800
AR Path="/60553C0C" Ref="E?"  Part="1" 
AR Path="/60458A5B/60553C0C" Ref="E?"  Part="1" 
F 0 "E?" H 4200 3993 50  0001 C CNN
F 1 "E" H 4200 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4200 4000 50  0001 C CNN
F 3 "~" H 4200 4000 50  0001 C CNN
	1    4200 3800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 60553C12
P 2400 3800
AR Path="/60553C12" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/60553C12" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 2400 3993 50  0001 C CNN
F 1 "TAB" H 2400 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 2400 4000 50  0001 C CNN
F 3 "~" H 2400 4000 50  0001 C CNN
	1    2400 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3800 2650 3800
Wire Wire Line
	3200 3800 3250 3800
Wire Wire Line
	3800 3800 3850 3800
Wire Wire Line
	4400 3800 4450 3800
Wire Wire Line
	4000 3900 4000 3800
Wire Wire Line
	3400 3800 3400 3900
Connection ~ 3400 3900
Wire Wire Line
	3400 3900 4000 3900
Wire Wire Line
	2800 3800 2800 3900
Connection ~ 2800 3900
Wire Wire Line
	2800 3900 3400 3900
Wire Wire Line
	2200 3800 2150 3800
Wire Wire Line
	2150 3800 2150 3900
Connection ~ 2150 3900
Wire Wire Line
	2150 3900 2800 3900
$Comp
L Switch:SW_Push Q?
U 1 1 60553C2C
P 5450 3800
AR Path="/60553C2C" Ref="Q?"  Part="1" 
AR Path="/60458A5B/60553C2C" Ref="Q?"  Part="1" 
F 0 "Q?" H 5450 3993 50  0001 C CNN
F 1 "T" H 5450 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 5450 4000 50  0001 C CNN
F 3 "~" H 5450 4000 50  0001 C CNN
	1    5450 3800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 60553C32
P 6050 3800
AR Path="/60553C32" Ref="W?"  Part="1" 
AR Path="/60458A5B/60553C32" Ref="W?"  Part="1" 
F 0 "W?" H 6050 3993 50  0001 C CNN
F 1 "Y" H 6050 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6050 4000 50  0001 C CNN
F 3 "~" H 6050 4000 50  0001 C CNN
	1    6050 3800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 60553C38
P 6650 3800
AR Path="/60553C38" Ref="E?"  Part="1" 
AR Path="/60458A5B/60553C38" Ref="E?"  Part="1" 
F 0 "E?" H 6650 3993 50  0001 C CNN
F 1 "U" H 6650 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6650 4000 50  0001 C CNN
F 3 "~" H 6650 4000 50  0001 C CNN
	1    6650 3800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 60553C3E
P 4850 3800
AR Path="/60553C3E" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/60553C3E" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 4850 3993 50  0001 C CNN
F 1 "R" H 4850 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4850 4000 50  0001 C CNN
F 3 "~" H 4850 4000 50  0001 C CNN
	1    4850 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 3800 5100 3800
Wire Wire Line
	5650 3800 5700 3800
Wire Wire Line
	6250 3800 6300 3800
Wire Wire Line
	6850 3800 6900 3800
Wire Wire Line
	6450 3900 6450 3800
Wire Wire Line
	5850 3800 5850 3900
Connection ~ 5850 3900
Wire Wire Line
	5850 3900 6450 3900
Wire Wire Line
	5250 3800 5250 3900
Connection ~ 5250 3900
Wire Wire Line
	5250 3900 5850 3900
Wire Wire Line
	4650 3800 4600 3800
Wire Wire Line
	4600 3800 4600 3900
Connection ~ 4600 3900
Wire Wire Line
	4600 3900 5250 3900
$Comp
L Switch:SW_Push Q?
U 1 1 60553C57
P 7900 3800
AR Path="/60553C57" Ref="Q?"  Part="1" 
AR Path="/60458A5B/60553C57" Ref="Q?"  Part="1" 
F 0 "Q?" H 7900 3993 50  0001 C CNN
F 1 "O" H 7900 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 7900 4000 50  0001 C CNN
F 3 "~" H 7900 4000 50  0001 C CNN
	1    7900 3800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 60553C5D
P 8500 3800
AR Path="/60553C5D" Ref="W?"  Part="1" 
AR Path="/60458A5B/60553C5D" Ref="W?"  Part="1" 
F 0 "W?" H 8500 3993 50  0001 C CNN
F 1 "P" H 8500 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 8500 4000 50  0001 C CNN
F 3 "~" H 8500 4000 50  0001 C CNN
	1    8500 3800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 60553C63
P 9100 3800
AR Path="/60553C63" Ref="E?"  Part="1" 
AR Path="/60458A5B/60553C63" Ref="E?"  Part="1" 
F 0 "E?" H 9100 3993 50  0001 C CNN
F 1 "[" H 9100 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 9100 4000 50  0001 C CNN
F 3 "~" H 9100 4000 50  0001 C CNN
	1    9100 3800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 60553C69
P 7300 3800
AR Path="/60553C69" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/60553C69" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 7300 3993 50  0001 C CNN
F 1 "I" H 7300 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 7300 4000 50  0001 C CNN
F 3 "~" H 7300 4000 50  0001 C CNN
	1    7300 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 3800 7550 3800
Wire Wire Line
	8100 3800 8150 3800
Wire Wire Line
	8700 3800 8750 3800
Wire Wire Line
	9300 3800 9350 3800
Wire Wire Line
	8900 3900 8900 3800
Wire Wire Line
	8300 3800 8300 3900
Connection ~ 8300 3900
Wire Wire Line
	8300 3900 8900 3900
Wire Wire Line
	7700 3800 7700 3900
Connection ~ 7700 3900
Wire Wire Line
	7700 3900 8300 3900
Wire Wire Line
	7100 3800 7050 3800
Wire Wire Line
	7050 3800 7050 3900
Wire Wire Line
	7050 3900 7700 3900
Connection ~ 4000 3900
Wire Wire Line
	4000 3900 4600 3900
Connection ~ 8900 3900
Wire Wire Line
	2650 3800 2650 4500
Connection ~ 2650 4500
Wire Wire Line
	3250 3800 3250 4500
Connection ~ 3250 4500
Wire Wire Line
	3850 3800 3850 4500
Connection ~ 3850 4500
Wire Wire Line
	3850 4500 3850 5100
Wire Wire Line
	4450 3800 4450 4500
Connection ~ 4450 4500
Wire Wire Line
	5100 3800 5100 4500
Connection ~ 5100 4500
Wire Wire Line
	5700 3800 5700 4500
Connection ~ 5700 4500
Wire Wire Line
	6300 3800 6300 4500
Connection ~ 6300 4500
Wire Wire Line
	6300 4500 6300 5100
Wire Wire Line
	6900 3800 6900 4500
Connection ~ 6900 4500
Wire Wire Line
	7550 3800 7550 4500
Connection ~ 7550 4500
Wire Wire Line
	8150 3800 8150 4500
Connection ~ 8150 4500
Wire Wire Line
	8750 3800 8750 4500
Connection ~ 8750 4500
Wire Wire Line
	9350 3800 9350 4500
Connection ~ 9350 4500
$Comp
L Switch:SW_Push E?
U 1 1 605BFF6F
P 9700 3800
AR Path="/605BFF6F" Ref="E?"  Part="1" 
AR Path="/60458A5B/605BFF6F" Ref="E?"  Part="1" 
F 0 "E?" H 9700 3993 50  0001 C CNN
F 1 "]" H 9700 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 9700 4000 50  0001 C CNN
F 3 "~" H 9700 4000 50  0001 C CNN
	1    9700 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 3800 9950 3800
Wire Wire Line
	9500 3900 9500 3800
Wire Wire Line
	8900 3900 9500 3900
Wire Wire Line
	9500 3900 10100 3900
Connection ~ 9500 3900
Wire Wire Line
	9950 3800 9950 4500
Connection ~ 9950 4500
Wire Wire Line
	8900 4600 9500 4600
$Comp
L Switch:SW_Push E?
U 1 1 605D1F58
P 10300 3800
AR Path="/605D1F58" Ref="E?"  Part="1" 
AR Path="/60458A5B/605D1F58" Ref="E?"  Part="1" 
F 0 "E?" H 10300 3993 50  0001 C CNN
F 1 "\\" H 10300 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 10300 4000 50  0001 C CNN
F 3 "~" H 10300 4000 50  0001 C CNN
	1    10300 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	10500 3800 10550 3800
Wire Wire Line
	10100 3900 10100 3800
Wire Wire Line
	10550 3800 10550 4500
$Comp
L Switch:SW_Push Q?
U 1 1 60632C09
P 3000 3000
AR Path="/60632C09" Ref="Q?"  Part="1" 
AR Path="/60458A5B/60632C09" Ref="Q?"  Part="1" 
F 0 "Q?" H 3000 3193 50  0001 C CNN
F 1 "1" H 3000 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3000 3200 50  0001 C CNN
F 3 "~" H 3000 3200 50  0001 C CNN
	1    3000 3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 60632C0F
P 3600 3000
AR Path="/60632C0F" Ref="W?"  Part="1" 
AR Path="/60458A5B/60632C0F" Ref="W?"  Part="1" 
F 0 "W?" H 3600 3193 50  0001 C CNN
F 1 "2" H 3600 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3600 3200 50  0001 C CNN
F 3 "~" H 3600 3200 50  0001 C CNN
	1    3600 3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 60632C15
P 4200 3000
AR Path="/60632C15" Ref="E?"  Part="1" 
AR Path="/60458A5B/60632C15" Ref="E?"  Part="1" 
F 0 "E?" H 4200 3193 50  0001 C CNN
F 1 "3" H 4200 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4200 3200 50  0001 C CNN
F 3 "~" H 4200 3200 50  0001 C CNN
	1    4200 3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 60632C1B
P 2400 3000
AR Path="/60632C1B" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/60632C1B" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 2400 3193 50  0001 C CNN
F 1 "ESCAPE" H 2400 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 2400 3200 50  0001 C CNN
F 3 "~" H 2400 3200 50  0001 C CNN
	1    2400 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3000 2650 3000
Wire Wire Line
	3200 3000 3250 3000
Wire Wire Line
	3800 3000 3850 3000
Wire Wire Line
	4400 3000 4450 3000
Wire Wire Line
	4000 3100 4000 3000
Wire Wire Line
	3400 3000 3400 3100
Connection ~ 3400 3100
Wire Wire Line
	3400 3100 4000 3100
Wire Wire Line
	2800 3000 2800 3100
Connection ~ 2800 3100
Wire Wire Line
	2800 3100 3400 3100
Wire Wire Line
	2200 3000 2150 3000
Wire Wire Line
	2150 3000 2150 3100
Connection ~ 2150 3100
Wire Wire Line
	2150 3100 2800 3100
$Comp
L Switch:SW_Push Q?
U 1 1 60632C31
P 5450 3000
AR Path="/60632C31" Ref="Q?"  Part="1" 
AR Path="/60458A5B/60632C31" Ref="Q?"  Part="1" 
F 0 "Q?" H 5450 3193 50  0001 C CNN
F 1 "5" H 5450 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 5450 3200 50  0001 C CNN
F 3 "~" H 5450 3200 50  0001 C CNN
	1    5450 3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 60632C37
P 6050 3000
AR Path="/60632C37" Ref="W?"  Part="1" 
AR Path="/60458A5B/60632C37" Ref="W?"  Part="1" 
F 0 "W?" H 6050 3193 50  0001 C CNN
F 1 "6" H 6050 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6050 3200 50  0001 C CNN
F 3 "~" H 6050 3200 50  0001 C CNN
	1    6050 3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 60632C3D
P 6650 3000
AR Path="/60632C3D" Ref="E?"  Part="1" 
AR Path="/60458A5B/60632C3D" Ref="E?"  Part="1" 
F 0 "E?" H 6650 3193 50  0001 C CNN
F 1 "7" H 6650 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6650 3200 50  0001 C CNN
F 3 "~" H 6650 3200 50  0001 C CNN
	1    6650 3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 60632C43
P 4850 3000
AR Path="/60632C43" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/60632C43" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 4850 3193 50  0001 C CNN
F 1 "4" H 4850 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4850 3200 50  0001 C CNN
F 3 "~" H 4850 3200 50  0001 C CNN
	1    4850 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 3000 5100 3000
Wire Wire Line
	5650 3000 5700 3000
Wire Wire Line
	6250 3000 6300 3000
Wire Wire Line
	6850 3000 6900 3000
Wire Wire Line
	6450 3100 6450 3000
Wire Wire Line
	5850 3000 5850 3100
Connection ~ 5850 3100
Wire Wire Line
	5850 3100 6450 3100
Wire Wire Line
	5250 3000 5250 3100
Connection ~ 5250 3100
Wire Wire Line
	5250 3100 5850 3100
Wire Wire Line
	4650 3000 4600 3000
Wire Wire Line
	4600 3000 4600 3100
Connection ~ 4600 3100
Wire Wire Line
	4600 3100 5250 3100
$Comp
L Switch:SW_Push Q?
U 1 1 60632C58
P 7900 3000
AR Path="/60632C58" Ref="Q?"  Part="1" 
AR Path="/60458A5B/60632C58" Ref="Q?"  Part="1" 
F 0 "Q?" H 7900 3193 50  0001 C CNN
F 1 "9" H 7900 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 7900 3200 50  0001 C CNN
F 3 "~" H 7900 3200 50  0001 C CNN
	1    7900 3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 60632C5E
P 8500 3000
AR Path="/60632C5E" Ref="W?"  Part="1" 
AR Path="/60458A5B/60632C5E" Ref="W?"  Part="1" 
F 0 "W?" H 8500 3193 50  0001 C CNN
F 1 "0" H 8500 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 8500 3200 50  0001 C CNN
F 3 "~" H 8500 3200 50  0001 C CNN
	1    8500 3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 60632C64
P 9100 3000
AR Path="/60632C64" Ref="E?"  Part="1" 
AR Path="/60458A5B/60632C64" Ref="E?"  Part="1" 
F 0 "E?" H 9100 3193 50  0001 C CNN
F 1 "-" H 9100 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 9100 3200 50  0001 C CNN
F 3 "~" H 9100 3200 50  0001 C CNN
	1    9100 3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 60632C6A
P 7300 3000
AR Path="/60632C6A" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/60632C6A" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 7300 3193 50  0001 C CNN
F 1 "8" H 7300 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 7300 3200 50  0001 C CNN
F 3 "~" H 7300 3200 50  0001 C CNN
	1    7300 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 3000 7550 3000
Wire Wire Line
	8100 3000 8150 3000
Wire Wire Line
	8700 3000 8750 3000
Wire Wire Line
	9300 3000 9350 3000
Wire Wire Line
	8900 3100 8900 3000
Wire Wire Line
	8300 3000 8300 3100
Connection ~ 8300 3100
Wire Wire Line
	8300 3100 8900 3100
Wire Wire Line
	7700 3000 7700 3100
Connection ~ 7700 3100
Wire Wire Line
	7700 3100 8300 3100
Wire Wire Line
	7100 3000 7050 3000
Wire Wire Line
	7050 3000 7050 3100
Connection ~ 4000 3100
Wire Wire Line
	4000 3100 4600 3100
Connection ~ 8900 3100
$Comp
L Switch:SW_Push E?
U 1 1 60632C91
P 9700 3000
AR Path="/60632C91" Ref="E?"  Part="1" 
AR Path="/60458A5B/60632C91" Ref="E?"  Part="1" 
F 0 "E?" H 9700 3193 50  0001 C CNN
F 1 "=" H 9700 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 9700 3200 50  0001 C CNN
F 3 "~" H 9700 3200 50  0001 C CNN
	1    9700 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 3000 9950 3000
Wire Wire Line
	9500 3100 9500 3000
Wire Wire Line
	8900 3100 9500 3100
Wire Wire Line
	9500 3100 10100 3100
Connection ~ 9500 3100
$Comp
L Switch:SW_Push E?
U 1 1 60632C9D
P 10300 3000
AR Path="/60632C9D" Ref="E?"  Part="1" 
AR Path="/60458A5B/60632C9D" Ref="E?"  Part="1" 
F 0 "E?" H 10300 3193 50  0001 C CNN
F 1 "BACKSPACE" H 10300 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 10300 3200 50  0001 C CNN
F 3 "~" H 10300 3200 50  0001 C CNN
	1    10300 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10500 3000 10550 3000
Wire Wire Line
	10100 3100 10100 3000
Wire Wire Line
	2650 3000 2650 3800
Connection ~ 2650 3800
Wire Wire Line
	3250 3000 3250 3800
Connection ~ 3250 3800
Wire Wire Line
	3850 3000 3850 3800
Connection ~ 3850 3800
Wire Wire Line
	4450 3000 4450 3800
Connection ~ 4450 3800
Wire Wire Line
	5100 3000 5100 3800
Connection ~ 5100 3800
Wire Wire Line
	5700 3000 5700 3800
Connection ~ 5700 3800
Wire Wire Line
	6300 3000 6300 3800
Connection ~ 6300 3800
Wire Wire Line
	6900 3000 6900 3800
Connection ~ 6900 3800
Wire Wire Line
	7550 3000 7550 3800
Connection ~ 7550 3800
Wire Wire Line
	8150 3000 8150 3800
Connection ~ 8150 3800
Wire Wire Line
	8750 3000 8750 3800
Connection ~ 8750 3800
Wire Wire Line
	9350 3000 9350 3800
Connection ~ 9350 3800
Wire Wire Line
	9950 3000 9950 3800
Connection ~ 9950 3800
Wire Wire Line
	10550 3000 10550 3800
Connection ~ 10550 3800
$Comp
L Switch:SW_Push A?
U 1 1 6075BA16
P 3000 6000
AR Path="/6075BA16" Ref="A?"  Part="1" 
AR Path="/60458A5B/6075BA16" Ref="A?"  Part="1" 
F 0 "A?" H 3000 6193 50  0001 C CNN
F 1 "Windows Key" H 3000 6193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3000 6200 50  0001 C CNN
F 3 "~" H 3000 6200 50  0001 C CNN
	1    3000 6000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push S?
U 1 1 6075BA1C
P 3600 6000
AR Path="/6075BA1C" Ref="S?"  Part="1" 
AR Path="/60458A5B/6075BA1C" Ref="S?"  Part="1" 
F 0 "S?" H 3600 6193 50  0001 C CNN
F 1 "Left Alt" H 3600 6193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3600 6200 50  0001 C CNN
F 3 "~" H 3600 6200 50  0001 C CNN
	1    3600 6000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push D?
U 1 1 6075BA22
P 4200 6000
AR Path="/6075BA22" Ref="D?"  Part="1" 
AR Path="/60458A5B/6075BA22" Ref="D?"  Part="1" 
F 0 "D?" H 4200 6193 50  0001 C CNN
F 1 "Space" H 4200 6193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4200 6200 50  0001 C CNN
F 3 "~" H 4200 6200 50  0001 C CNN
	1    4200 6000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Ctrl?
U 1 1 6075BA28
P 2400 6000
AR Path="/6075BA28" Ref="Ctrl?"  Part="1" 
AR Path="/60458A5B/6075BA28" Ref="Ctrl?"  Part="1" 
F 0 "Ctrl?" H 2400 6193 50  0001 C CNN
F 1 "Left Control" H 2400 6193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 2400 6200 50  0001 C CNN
F 3 "~" H 2400 6200 50  0001 C CNN
	1    2400 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 6000 2150 6100
Wire Wire Line
	2150 6100 2800 6100
Wire Wire Line
	4000 6100 4000 6000
Wire Wire Line
	2150 6000 2200 6000
Wire Wire Line
	3400 6000 3400 6100
Connection ~ 3400 6100
Wire Wire Line
	2800 6000 2800 6100
Connection ~ 2800 6100
Wire Wire Line
	2800 6100 3400 6100
Wire Wire Line
	2600 6000 2650 6000
Connection ~ 2650 6000
Wire Wire Line
	3200 6000 3250 6000
Wire Wire Line
	3800 6000 3850 6000
Wire Wire Line
	4400 6000 4450 6000
Wire Wire Line
	2650 6000 2650 6250
Wire Wire Line
	3250 6250 3250 6000
Connection ~ 3250 6000
Connection ~ 3850 6000
Wire Wire Line
	4450 6000 4450 6250
Connection ~ 4450 6000
Wire Wire Line
	2050 6100 2150 6100
Connection ~ 2150 6100
$Comp
L Switch:SW_Push A?
U 1 1 6075BA47
P 5450 6000
AR Path="/6075BA47" Ref="A?"  Part="1" 
AR Path="/60458A5B/6075BA47" Ref="A?"  Part="1" 
F 0 "A?" H 5450 6193 50  0001 C CNN
F 1 "Windows Key" H 5450 6193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 5450 6200 50  0001 C CNN
F 3 "~" H 5450 6200 50  0001 C CNN
	1    5450 6000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push S?
U 1 1 6075BA4D
P 6050 6000
AR Path="/6075BA4D" Ref="S?"  Part="1" 
AR Path="/60458A5B/6075BA4D" Ref="S?"  Part="1" 
F 0 "S?" H 6050 6193 50  0001 C CNN
F 1 "Function" H 6050 6193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6050 6200 50  0001 C CNN
F 3 "~" H 6050 6200 50  0001 C CNN
	1    6050 6000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push D?
U 1 1 6075BA53
P 6650 6000
AR Path="/6075BA53" Ref="D?"  Part="1" 
AR Path="/60458A5B/6075BA53" Ref="D?"  Part="1" 
F 0 "D?" H 6650 6193 50  0001 C CNN
F 1 "Right Control" H 6650 6193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6650 6200 50  0001 C CNN
F 3 "~" H 6650 6200 50  0001 C CNN
	1    6650 6000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Ctrl?
U 1 1 6075BA59
P 4850 6000
AR Path="/6075BA59" Ref="Ctrl?"  Part="1" 
AR Path="/60458A5B/6075BA59" Ref="Ctrl?"  Part="1" 
F 0 "Ctrl?" H 4850 6193 50  0001 C CNN
F 1 "Right Alt" H 4850 6193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4850 6200 50  0001 C CNN
F 3 "~" H 4850 6200 50  0001 C CNN
	1    4850 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 6000 4600 6100
Wire Wire Line
	4600 6100 5250 6100
Wire Wire Line
	6450 6100 6450 6000
Wire Wire Line
	4600 6000 4650 6000
Wire Wire Line
	5850 6000 5850 6100
Connection ~ 5850 6100
Wire Wire Line
	5850 6100 6450 6100
Wire Wire Line
	5250 6000 5250 6100
Connection ~ 5250 6100
Wire Wire Line
	5250 6100 5850 6100
Wire Wire Line
	5050 6000 5100 6000
Connection ~ 5100 6000
Wire Wire Line
	5650 6000 5700 6000
Wire Wire Line
	6250 6000 6300 6000
Wire Wire Line
	6850 6000 6900 6000
Wire Wire Line
	5100 6000 5100 6250
Wire Wire Line
	5700 6250 5700 6000
Connection ~ 5700 6000
Wire Wire Line
	6300 6000 6300 6250
Connection ~ 6300 6000
Wire Wire Line
	6900 6000 6900 6250
Connection ~ 6900 6000
Connection ~ 4600 6100
Wire Wire Line
	3850 6000 3850 6250
Wire Wire Line
	3400 6100 4000 6100
Connection ~ 4000 6100
Wire Wire Line
	2650 5100 2650 6000
Wire Wire Line
	3250 5100 3250 6000
Wire Wire Line
	3850 5100 3850 6000
Wire Wire Line
	4450 5100 4450 6000
Wire Wire Line
	5100 5100 5100 6000
Wire Wire Line
	5700 5100 5700 6000
Wire Wire Line
	6300 5100 6300 6000
Wire Wire Line
	6900 5100 6900 6000
$Comp
L Switch:SW_Push Q?
U 1 1 608618FE
P 4200 2150
AR Path="/608618FE" Ref="Q?"  Part="1" 
AR Path="/60458A5B/608618FE" Ref="Q?"  Part="1" 
F 0 "Q?" H 4200 2343 50  0001 C CNN
F 1 "F3" H 4200 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4200 2350 50  0001 C CNN
F 3 "~" H 4200 2350 50  0001 C CNN
	1    4200 2150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 60861904
P 4800 2150
AR Path="/60861904" Ref="W?"  Part="1" 
AR Path="/60458A5B/60861904" Ref="W?"  Part="1" 
F 0 "W?" H 4800 2343 50  0001 C CNN
F 1 "F4" H 4800 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 4800 2350 50  0001 C CNN
F 3 "~" H 4800 2350 50  0001 C CNN
	1    4800 2150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 6086190A
P 5400 2150
AR Path="/6086190A" Ref="E?"  Part="1" 
AR Path="/60458A5B/6086190A" Ref="E?"  Part="1" 
F 0 "E?" H 5400 2343 50  0001 C CNN
F 1 "F5" H 5400 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 5400 2350 50  0001 C CNN
F 3 "~" H 5400 2350 50  0001 C CNN
	1    5400 2150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 60861910
P 3600 2150
AR Path="/60861910" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/60861910" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 3600 2343 50  0001 C CNN
F 1 "F2" H 3600 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3600 2350 50  0001 C CNN
F 3 "~" H 3600 2350 50  0001 C CNN
	1    3600 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 2150 3850 2150
Wire Wire Line
	4400 2150 4450 2150
Wire Wire Line
	5000 2150 5050 2150
Wire Wire Line
	5600 2150 5650 2150
Wire Wire Line
	5200 2250 5200 2150
Wire Wire Line
	4600 2150 4600 2250
Connection ~ 4600 2250
Wire Wire Line
	4600 2250 5200 2250
Wire Wire Line
	4000 2150 4000 2250
Wire Wire Line
	4000 2250 4600 2250
Wire Wire Line
	3400 2150 3350 2150
Wire Wire Line
	3350 2150 3350 2250
$Comp
L Switch:SW_Push Q?
U 1 1 60861926
P 6650 2150
AR Path="/60861926" Ref="Q?"  Part="1" 
AR Path="/60458A5B/60861926" Ref="Q?"  Part="1" 
F 0 "Q?" H 6650 2343 50  0001 C CNN
F 1 "F7" H 6650 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6650 2350 50  0001 C CNN
F 3 "~" H 6650 2350 50  0001 C CNN
	1    6650 2150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 6086192C
P 7250 2150
AR Path="/6086192C" Ref="W?"  Part="1" 
AR Path="/60458A5B/6086192C" Ref="W?"  Part="1" 
F 0 "W?" H 7250 2343 50  0001 C CNN
F 1 "F8" H 7250 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 7250 2350 50  0001 C CNN
F 3 "~" H 7250 2350 50  0001 C CNN
	1    7250 2150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 60861932
P 7850 2150
AR Path="/60861932" Ref="E?"  Part="1" 
AR Path="/60458A5B/60861932" Ref="E?"  Part="1" 
F 0 "E?" H 7850 2343 50  0001 C CNN
F 1 "F9" H 7850 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 7850 2350 50  0001 C CNN
F 3 "~" H 7850 2350 50  0001 C CNN
	1    7850 2150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 60861938
P 6050 2150
AR Path="/60861938" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/60861938" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 6050 2343 50  0001 C CNN
F 1 "F6" H 6050 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 6050 2350 50  0001 C CNN
F 3 "~" H 6050 2350 50  0001 C CNN
	1    6050 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 2150 6300 2150
Wire Wire Line
	6850 2150 6900 2150
Wire Wire Line
	7450 2150 7500 2150
Wire Wire Line
	8050 2150 8100 2150
Wire Wire Line
	7650 2250 7650 2150
Wire Wire Line
	7050 2150 7050 2250
Connection ~ 7050 2250
Wire Wire Line
	7050 2250 7650 2250
Wire Wire Line
	6450 2150 6450 2250
Connection ~ 6450 2250
Wire Wire Line
	6450 2250 7050 2250
Wire Wire Line
	5850 2150 5800 2150
Wire Wire Line
	5800 2150 5800 2250
Connection ~ 5800 2250
Wire Wire Line
	5800 2250 6450 2250
$Comp
L Switch:SW_Push Q?
U 1 1 6086194D
P 9100 2150
AR Path="/6086194D" Ref="Q?"  Part="1" 
AR Path="/60458A5B/6086194D" Ref="Q?"  Part="1" 
F 0 "Q?" H 9100 2343 50  0001 C CNN
F 1 "F11" H 9100 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 9100 2350 50  0001 C CNN
F 3 "~" H 9100 2350 50  0001 C CNN
	1    9100 2150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 60861953
P 9700 2150
AR Path="/60861953" Ref="W?"  Part="1" 
AR Path="/60458A5B/60861953" Ref="W?"  Part="1" 
F 0 "W?" H 9700 2343 50  0001 C CNN
F 1 "F12" H 9700 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 9700 2350 50  0001 C CNN
F 3 "~" H 9700 2350 50  0001 C CNN
	1    9700 2150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 60861959
P 10300 2150
AR Path="/60861959" Ref="E?"  Part="1" 
AR Path="/60458A5B/60861959" Ref="E?"  Part="1" 
F 0 "E?" H 10300 2343 50  0001 C CNN
F 1 "HOME" H 10300 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 10300 2350 50  0001 C CNN
F 3 "~" H 10300 2350 50  0001 C CNN
	1    10300 2150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 6086195F
P 8500 2150
AR Path="/6086195F" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/6086195F" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 8500 2343 50  0001 C CNN
F 1 "F10" H 8500 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 8500 2350 50  0001 C CNN
F 3 "~" H 8500 2350 50  0001 C CNN
	1    8500 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 2150 8750 2150
Wire Wire Line
	9300 2150 9350 2150
Wire Wire Line
	9900 2150 9950 2150
Wire Wire Line
	10500 2150 10550 2150
Wire Wire Line
	10100 2250 10100 2150
Wire Wire Line
	9500 2150 9500 2250
Connection ~ 9500 2250
Wire Wire Line
	9500 2250 10100 2250
Wire Wire Line
	8900 2150 8900 2250
Connection ~ 8900 2250
Wire Wire Line
	8900 2250 9500 2250
Wire Wire Line
	8300 2150 8250 2150
Wire Wire Line
	8250 2150 8250 2250
Wire Wire Line
	8250 2250 8900 2250
Connection ~ 5200 2250
Wire Wire Line
	5200 2250 5800 2250
Connection ~ 10100 2250
$Comp
L Switch:SW_Push E?
U 1 1 6086197A
P 10900 2150
AR Path="/6086197A" Ref="E?"  Part="1" 
AR Path="/60458A5B/6086197A" Ref="E?"  Part="1" 
F 0 "E?" H 10900 2343 50  0001 C CNN
F 1 "END" H 10900 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 10900 2350 50  0001 C CNN
F 3 "~" H 10900 2350 50  0001 C CNN
	1    10900 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	11100 2150 11150 2150
Wire Wire Line
	10700 2250 10700 2150
Wire Wire Line
	10100 2250 10700 2250
Wire Wire Line
	5050 2150 5050 2950
Wire Wire Line
	5650 2150 5650 2950
Wire Wire Line
	7500 2150 7500 2950
Wire Wire Line
	11150 2150 11150 2950
Wire Wire Line
	3850 2150 3850 3000
Connection ~ 3850 3000
Wire Wire Line
	4450 2150 4450 3000
Connection ~ 4450 3000
Wire Wire Line
	5050 2950 5100 2950
Wire Wire Line
	5100 2950 5100 3000
Connection ~ 5100 3000
Wire Wire Line
	5650 2950 5700 2950
Wire Wire Line
	5700 2950 5700 3000
Connection ~ 5700 3000
Wire Wire Line
	6300 2150 6300 3000
Connection ~ 6300 3000
Wire Wire Line
	6900 2150 6900 3000
Connection ~ 6900 3000
Wire Wire Line
	7500 2950 7550 2950
Wire Wire Line
	7550 2950 7550 3000
Connection ~ 7550 3000
Wire Wire Line
	8100 2150 8100 2950
Wire Wire Line
	8100 2950 8150 2950
Wire Wire Line
	8150 2950 8150 3000
Connection ~ 8150 3000
Wire Wire Line
	8750 2150 8750 3000
Connection ~ 8750 3000
Wire Wire Line
	9350 2150 9350 3000
Connection ~ 9350 3000
Wire Wire Line
	9950 2150 9950 3000
Connection ~ 9950 3000
Wire Wire Line
	10550 2150 10550 3000
Connection ~ 10550 3000
$Comp
L Switch:SW_Push Q?
U 1 1 609FA53D
P 550 2150
AR Path="/609FA53D" Ref="Q?"  Part="1" 
AR Path="/60458A5B/609FA53D" Ref="Q?"  Part="1" 
F 0 "Q?" H 550 2343 50  0001 C CNN
F 1 "DELETE" H 550 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 550 2350 50  0001 C CNN
F 3 "~" H 550 2350 50  0001 C CNN
	1    550  2150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 609FA543
P 1150 2150
AR Path="/609FA543" Ref="W?"  Part="1" 
AR Path="/60458A5B/609FA543" Ref="W?"  Part="1" 
F 0 "W?" H 1150 2343 50  0001 C CNN
F 1 "NUMLOCK" H 1150 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 1150 2350 50  0001 C CNN
F 3 "~" H 1150 2350 50  0001 C CNN
	1    1150 2150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 609FA549
P 1750 2150
AR Path="/609FA549" Ref="E?"  Part="1" 
AR Path="/60458A5B/609FA549" Ref="E?"  Part="1" 
F 0 "E?" H 1750 2343 50  0001 C CNN
F 1 "PageUP" H 1750 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 1750 2350 50  0001 C CNN
F 3 "~" H 1750 2350 50  0001 C CNN
	1    1750 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 2150 1400 2150
Wire Wire Line
	1950 2150 2000 2150
Wire Wire Line
	1550 2250 1550 2150
Wire Wire Line
	950  2150 950  2250
Connection ~ 950  2250
Wire Wire Line
	950  2250 1550 2250
Wire Wire Line
	350  2150 350  2250
Connection ~ 350  2250
Wire Wire Line
	350  2250 950  2250
Wire Wire Line
	-300 2250 350  2250
$Comp
L Switch:SW_Push Q?
U 1 1 609FA55A
P 3000 2150
AR Path="/609FA55A" Ref="Q?"  Part="1" 
AR Path="/60458A5B/609FA55A" Ref="Q?"  Part="1" 
F 0 "Q?" H 3000 2343 50  0001 C CNN
F 1 "F1" H 3000 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 3000 2350 50  0001 C CNN
F 3 "~" H 3000 2350 50  0001 C CNN
	1    3000 2150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push Shift?
U 1 1 609FA560
P 2400 2150
AR Path="/609FA560" Ref="Shift?"  Part="1" 
AR Path="/60458A5B/609FA560" Ref="Shift?"  Part="1" 
F 0 "Shift?" H 2400 2343 50  0001 C CNN
F 1 "4" H 2400 2343 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 2400 2350 50  0001 C CNN
F 3 "~" H 2400 2350 50  0001 C CNN
	1    2400 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 2150 2650 2150
Wire Wire Line
	3200 2150 3250 2150
Wire Wire Line
	2800 2150 2800 2250
Connection ~ 2800 2250
Wire Wire Line
	2200 2150 2150 2150
Wire Wire Line
	2150 2150 2150 2250
Connection ~ 2150 2250
Wire Wire Line
	2150 2250 2800 2250
Connection ~ 1550 2250
Wire Wire Line
	1550 2250 2150 2250
Wire Wire Line
	2650 2150 2650 3000
Wire Wire Line
	3250 2150 3250 3000
Wire Wire Line
	2800 2250 3350 2250
Wire Wire Line
	4000 2250 3350 2250
Connection ~ 4000 2250
Connection ~ 3350 2250
Wire Wire Line
	7050 3100 7700 3100
Wire Wire Line
	7050 3100 6450 3100
Connection ~ 7050 3100
Connection ~ 6450 3100
Wire Wire Line
	8250 2250 7650 2250
Connection ~ 8250 2250
Connection ~ 7650 2250
Wire Wire Line
	7050 3900 6450 3900
Connection ~ 7050 3900
Connection ~ 6450 3900
Wire Wire Line
	7050 4600 6450 4600
Connection ~ 7050 4600
Connection ~ 6450 4600
$Comp
L Switch:SW_Push Q?
U 1 1 60D1C54E
P 550 3000
AR Path="/60D1C54E" Ref="Q?"  Part="1" 
AR Path="/60458A5B/60D1C54E" Ref="Q?"  Part="1" 
F 0 "Q?" H 550 3193 50  0001 C CNN
F 1 "Insert" H 550 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 550 3200 50  0001 C CNN
F 3 "~" H 550 3200 50  0001 C CNN
	1    550  3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 60D1C554
P 1150 3000
AR Path="/60D1C554" Ref="W?"  Part="1" 
AR Path="/60458A5B/60D1C554" Ref="W?"  Part="1" 
F 0 "W?" H 1150 3193 50  0001 C CNN
F 1 "Arrow UP" H 1150 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 1150 3200 50  0001 C CNN
F 3 "~" H 1150 3200 50  0001 C CNN
	1    1150 3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 60D1C55A
P 1750 3000
AR Path="/60D1C55A" Ref="E?"  Part="1" 
AR Path="/60458A5B/60D1C55A" Ref="E?"  Part="1" 
F 0 "E?" H 1750 3193 50  0001 C CNN
F 1 "PageDOWN" H 1750 3193 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 1750 3200 50  0001 C CNN
F 3 "~" H 1750 3200 50  0001 C CNN
	1    1750 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	750  3000 800  3000
Wire Wire Line
	1350 3000 1400 3000
Wire Wire Line
	1950 3000 2000 3000
Wire Wire Line
	1550 3100 1550 3000
Wire Wire Line
	950  3000 950  3100
Connection ~ 950  3100
Wire Wire Line
	950  3100 1550 3100
Wire Wire Line
	350  3000 350  3100
Connection ~ 350  3100
Wire Wire Line
	350  3100 950  3100
Wire Wire Line
	-300 3100 350  3100
Connection ~ 1550 3100
Wire Wire Line
	2000 3000 2000 3800
$Comp
L Switch:SW_Push Q?
U 1 1 60D45473
P 550 3800
AR Path="/60D45473" Ref="Q?"  Part="1" 
AR Path="/60458A5B/60D45473" Ref="Q?"  Part="1" 
F 0 "Q?" H 550 3993 50  0001 C CNN
F 1 "Arrow Left" H 550 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 550 4000 50  0001 C CNN
F 3 "~" H 550 4000 50  0001 C CNN
	1    550  3800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push W?
U 1 1 60D45479
P 1150 3800
AR Path="/60D45479" Ref="W?"  Part="1" 
AR Path="/60458A5B/60D45479" Ref="W?"  Part="1" 
F 0 "W?" H 1150 3993 50  0001 C CNN
F 1 "Arrow Down" H 1150 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 1150 4000 50  0001 C CNN
F 3 "~" H 1150 4000 50  0001 C CNN
	1    1150 3800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push E?
U 1 1 60D4547F
P 1750 3800
AR Path="/60D4547F" Ref="E?"  Part="1" 
AR Path="/60458A5B/60D4547F" Ref="E?"  Part="1" 
F 0 "E?" H 1750 3993 50  0001 C CNN
F 1 "Arrow Right" H 1750 3993 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 1750 4000 50  0001 C CNN
F 3 "~" H 1750 4000 50  0001 C CNN
	1    1750 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	750  3800 800  3800
Wire Wire Line
	1350 3800 1400 3800
Wire Wire Line
	1950 3800 2000 3800
Wire Wire Line
	1550 3900 1550 3800
Wire Wire Line
	950  3800 950  3900
Connection ~ 950  3900
Wire Wire Line
	950  3900 1550 3900
Wire Wire Line
	350  3800 350  3900
Connection ~ 350  3900
Wire Wire Line
	350  3900 950  3900
Wire Wire Line
	-300 3900 350  3900
Connection ~ 1550 3900
Wire Wire Line
	1400 3800 1400 4600
Wire Wire Line
	2000 2150 2000 3000
Connection ~ 2000 3000
Wire Wire Line
	1400 2150 1400 3000
Wire Wire Line
	1400 3800 1400 3000
Connection ~ 1400 3800
Connection ~ 1400 3000
Wire Wire Line
	1550 3900 2150 3900
Wire Wire Line
	1550 3100 2150 3100
Wire Wire Line
	750  2150 800  2150
Wire Wire Line
	800  2150 800  3000
Connection ~ 800  3000
Wire Wire Line
	800  3000 800  3800
Connection ~ 800  3800
Wire Wire Line
	800  3800 800  4650
Connection ~ 2650 3000
Connection ~ 3250 3000
Text HLabel 2050 6100 0    50   Input ~ 0
ROW0
Text HLabel 2050 5200 0    50   Input ~ 0
ROW1
Text HLabel -300 3900 0    50   Input ~ 0
ROW3
Text HLabel -300 3100 0    50   Input ~ 0
ROW4
Text HLabel -300 2250 0    50   Input ~ 0
ROW5
Wire Wire Line
	4500 5200 4600 5200
Wire Wire Line
	4500 6100 4600 6100
Wire Wire Line
	6950 5200 7050 5200
Wire Wire Line
	4000 5200 4600 5200
Wire Wire Line
	4000 6100 4600 6100
Wire Wire Line
	6450 5200 7050 5200
Wire Wire Line
	2000 3800 2000 4250
Connection ~ 2000 3800
Text HLabel 2050 4600 0    50   Input ~ 0
ROW2
Text HLabel 800  4650 3    50   Input ~ 0
COL0
Text HLabel 1400 4600 3    50   Input ~ 0
COL1
Text HLabel 2000 4250 3    50   Input ~ 0
COL2
Text HLabel 2650 6250 3    50   Input ~ 0
COL3
Text HLabel 3250 6250 3    50   Input ~ 0
COL4
Text HLabel 3850 6250 3    50   Input ~ 0
COL5
Text HLabel 4450 6250 3    50   Input ~ 0
COL6
Text HLabel 5100 6250 3    50   Input ~ 0
COL7
Text HLabel 5700 6250 3    50   Input ~ 0
COL8
Text HLabel 6300 6250 3    50   Input ~ 0
COL9
Text HLabel 6900 6250 3    50   Input ~ 0
COL10
Text HLabel 7550 5350 3    50   Input ~ 0
COL11
Text HLabel 8150 5350 3    50   Input ~ 0
COL12
Text HLabel 8750 5350 3    50   Input ~ 0
COL13
Text HLabel 9350 5350 3    50   Input ~ 0
COL14
Text HLabel 9950 5100 3    50   Input ~ 0
COL15
Text HLabel 10550 4500 3    50   Input ~ 0
COL16
Text HLabel 11150 2950 3    50   Input ~ 0
COL17
$EndSCHEMATC
