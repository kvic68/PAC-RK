.equ		F_CPU 					= 16000000 ; ������� ���������� � ��
.equ		MCP23017ADDR			= $40
.equ		LED_ADDR				= $78
.equ 		BMP180ADDR				= $EE
.equ		IIC_REGULATOR			= $32
.equ		BUFLEN 					= 16
.equ 		baudrate 				= 9600  
.equ 		bauddivider 			= (F_CPU/(16*baudrate))-1
.equ		defaultStepsFor100ml	= 185000	;250000	;	
.equ		minStepsFor100ml		= 100000	;150000	;		
.equ		maxStepsFor100ml		= 1500000	;400000	;
.equ		minRateValue			= 10;
.equ		maxRateValue			= 3600;
.equ		maxTargetVolt			= 2500;
.equ		maxDoseValue			= 5000;
	
; ----------------------------------------------------------------------------------------
; �������� ��� Timer1
.equ	T1Disable	= 0b0001000
.equ	T1Div1 		= 0b0001001
.equ	T1Div8 		= 0b0001010
.equ	T1Div64 	= 0b0001011
.equ	T1Div256  	= 0b0001100
.equ	T1Div1024	= 0b0001101
; ----------------------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
; MCP23017 expander http://www.gammon.com.au/forum/?id=10940

.equ	LCD_CS0		= 4	;(0) 15	23(GPA2)    CS1   	Chip select for IC1 (1 = active)  (see LCD_CS1)
.equ	LCD_CS1		= 3	;(1) 16	24(GPA3)    CS2	    Chip select for IC2 (1 = active)  (see LCD_CS2)
.equ	LCD_RES		= 2 ;( ) 17	25(GPA4)    ~RST  	1 = not reset, 0 = reset
.equ	LCD_DI		= 7	;(3) 4	26(GPA5)    D/~I   	1 = data, 0 = instruction    (see LCD_DATA)
.equ	LCD_RW		= 6	;( ) 5	27(GPA6)    R/~W	1 = read, 0 = write (to LCD) (see LCD_READ)
.equ	LCD_STROBE	= 5	;(2) 6	28(GPA7)	E		Enable data transfer on 1 -> 0 transition  (see LCD_ENABLE)
.equ	LCD_LED		= 1

// ��� ���� ����������
/*
.equ	LCD_CS0		= 2	;(0) 15	23(GPA2)    CS1   	Chip select for IC1 (1 = active)  (see LCD_CS1)
.equ	LCD_CS1		= 3	;(1) 16	24(GPA3)    CS2	    Chip select for IC2 (1 = active)  (see LCD_CS2)
.equ	LCD_RES		= 4 ;( ) 17	25(GPA4)    ~RST  	1 = not reset, 0 = reset
.equ	LCD_DI		= 5	;(3) 4	26(GPA5)    D/~I   	1 = data, 0 = instruction    (see LCD_DATA)
.equ	LCD_RW		= 6	;( ) 5	27(GPA6)    R/~W	1 = read, 0 = write (to LCD) (see LCD_READ)
.equ	LCD_STROBE	= 7	;(2) 6	28(GPA7)	E		Enable data transfer on 1 -> 0 transition  (see LCD_ENABLE)
.equ	LCD_LED		= 1
*/
; ----------------------------------------------------------------------------------------
; --- ��������� MCP 23017 ---
; -- �������� MCP23017
.equ	IOCON	=	$0A
.equ	IODIRA	=	$00
.equ	IODIRB	=	$01
.equ	GPPUA	=	$0C
.equ	GPPUB	=	$0D
.equ	GPIOA	=	$12
.equ	GPIOB	=	$13
.equ	OLATA	=	$14
.equ	OLATB	=	$15
; -- ���� �������� IOCON
.equ	BANK	=	7
.equ	MIRROR	=	6
.equ	SEQOP	=	5	; 1-disabled, 0-enabled
.equ	DISSLW	=	4
.equ	HAEN	=	3
.equ	ODR		=	2
.equ	INTPOL	=	1
; ----------------------------------------------------------------------------------------
; -- ������� ��� DS18B20 --
.equ	SearchROM		= $F0
.equ	ReadROM			= $33
.equ	MatchROM		= $55
.equ	SkipROM			= $CC
.equ	AlarmSearch		= $EC

.equ	ConvertT		= $44
.equ	WriteScratchpad	= $4E
.equ	ReadScratchpad	= $BE
.equ	CopyScratchpad	= $48
.equ	RecallE2		= $B8
.equ	ReadPowerSupply	= $B4

; ----------------------------------------------------------------------------------------
.equ  BUTTON_HOLDING_TIME  	= 0  ; ���� �����   DButtonStatus[4:0] = ������� ���������� ����������, � ������� ������� ������ ��������(����/����) "�������".  (��������� ����� �� 16���!)
.equ  BUTTON_STATUS_CODE  	= 5  ; � ��� ����� DButtonStatus[7:5] = ���������� �������� "������-��� ������" (��. ���� ���������������� ��������).
.equ  BUTTON_HOLDEN_LONG  	= 5  ; ���� "������� ���������" ������: 0-�������� ��� 1-�������.
.equ  BUTTON_IS_PRESSED  	= 6  ; ���� "������������� ����������� ������� ������": "0" - ������ �� ����������, "1" - ���� �������.
.equ  BUTTON_IS_HOLDDOWN  	= 7  ; ���� "������ ������������ � ������� ���������": "0" - ������ ������ "��������", "1" - ������ ������ "������ � ������������".

.equ  BSC_NotPressed  		= 0b000  ; "�� ������"    (�������� ��������� ��� ���� ������ - ������ ������ ����� "������")
.equ  BSC_ShortHold  		= 0b100  ; "�������� ���������"  (������ ������, � �� ��� ������������, ���� "��������" �����)
.equ  BSC_LongHold  		= 0b101  ; "������� ���������"  (������ ������, � �� ��� ������������, ��� "����������" �����)
.equ  BSC_ShortPress  		= 0b010  ; "�������� �������"  (������ ���� ������, � ����� ��������, � ����� � ��������� ���� "��������������")
.equ  BSC_LongPress  		= 0b011  ; "������� �������"  (������ ���� ������, � ����� ��������, � ����� � ��������� ���� "����������")

.equ  CShortButtonTouchDuration = 10  	; ������������ �������� ������� � �100 ��
.equ  CButtonInputChannelCount = 1		; ���������� ������

; ----------------------------------------------------------------------------------------
; -- ����������� ���� ��� ������ ---
.equ	EOS			=	$80		;	������� ����� ���������
.equ	FONT		=	$81		;	����� ������ � ��������� �����

.equ	XY			=	$83		;	���������� ���������� ������ ��������� 2 �����
.equ	dXdY		=	$84		;	�������� ����� �� �������� ��������� 2 ������
.equ	Inv			=	$85		;	�������� ��������� �����
.equ	Disable		=	$86		;	��������� ����� ���������� ������������ �������

; -- ������ ��� ������, �������������� ����� ������ --------------------------------------
.equ	MessageEx	=	$87		;	������� ������ �� ������ ��������� ������
.equ	Message		=	$88		;	���������
.equ	PressureMM	=	$89		;	����������� �������� ��.��.�� (�����������)
.equ	StepsCount	=	$8A		;	������� �����
.equ	StepsFor100	=	$8B		;	���������� ����� �� 100 ��
.equ	Rate		=	$8C		;	�������� ������ ��/���
.equ	Total		=	$8D		;	����� ����� ������, ��
.equ	Remain		=	$8E		;	������� �� �����, ��
.equ	Dose		=	$8F		;	������������� ��������� � ��, ���� 0, �� ��� ���������

.equ	T0			=	$90		;	� ������ 0
.equ	T1			=	$91		;	� ������ 1
.equ	T2			=	$92		;	� ������ 2
.equ	T3			=	$93		;	� ������ 3

.equ	MARGIN0		=	$94		;	����� ������������ ������ 0
.equ	MARGIN1		=	$95		;	����� ������������ ������ 1
.equ	MARGIN2		=	$96		;	����� ������������ ������ 2
.equ	MARGIN3		=	$97		;	����� ������������ ������ 3

.equ	DEL_UP0		=	$98		;	�������� ������������ ������ 0
.equ	DEL_UP1		=	$99		;	�������� ������������ ������ 1
.equ	DEL_UP2		=	$9A		;	�������� ������������ ������ 2
.equ	DEL_UP3		=	$9B		;	�������� ������������ ������ 3

.equ	DEL_DN0		=	$9C		;	�������� ���������� ������ 0
.equ	DEL_DN1		=	$9D		;	�������� ���������� ������ 1
.equ	DEL_DN2		=	$9E		;	�������� ���������� ������ 2
.equ	DEL_DN3		=	$9F		;	�������� ���������� ������ 3

.equ	REACT_0		=	$A0		;	��� ������� �� ������������ ������� 0
.equ	REACT_1		=	$A1		;	��� ������� �� ������������ ������� 1
.equ	REACT_2		=	$A2		;	��� ������� �� ������������ ������� 2
.equ	REACT_3		=	$A3		;	��� ������� �� ������������ ������� 3

.equ	BACK_T0		=	$A4		;	�������� ������� ������� 0
.equ	BACK_T1		=	$A5		;	�������� ������� ������� 1
.equ	BACK_T2		=	$A6		;	�������� ������� ������� 2
.equ	BACK_T3		=	$A7		;	�������� ������� ������� 3

.equ	StabModeV	=	$A8		;	����� ������ �������� ������������� ����������
.equ	VoltageT	=	$A9		;	������� ���������� ������������� � ������� ����� ������ VOLTS+0,+1
.equ	VoltageA	=	$AA		;	����������� ������� ���������� ������������� � ������� ����� ������ VOLTS+2,+3
.equ	StabInd		=	$AB		;	��������� ������������

.equ	R_On0		=	$AC		;	��������� ���������� ������� T0
.equ	R_On1		=	$AD		;	��������� ���������� ������� T0
.equ	R_On2		=	$AE		;	��������� ���������� ������� T0
.equ	R_On3		=	$AF		;	��������� ���������� ������� T0

.equ	HStatus		=	$B0		;	��������� �������� ������� ���/���� FF/00
.equ	HSensor		=	$B1		;	����� �������, ��������� �� ��������
.equ	HMargin		=	$B2		;	����� ���������� �������
.equ	HSensorVal	=	$B3		;	������� �������� ���������� �������
.equ	HString		=	$B4		;	������ ��������� ��������

; ----------------------------------------------------------------------------------------
