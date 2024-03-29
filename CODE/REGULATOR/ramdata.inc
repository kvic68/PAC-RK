.dseg

DInputIntegrator:			.BYTE 1
DButtonStatus:				.BYTE 1

OPTO_ON_TICK	:			.BYTE 2				; �������� ������� ��������� ������� ������������ � OCR1A
OPTO_OFF_TICK	:			.BYTE 2
COUNT_NEG		:			.BYTE 2
COUNT_POS		:			.BYTE 2

REG_AB			:			.BYTE 1				; ������� ��������
T0CompareEnabled:			.BYTE 1
BUTTON_STATE	:			.BYTE 1
BUTTON_COUNTER	:			.BYTE 1
ROTATE_COUNTER	:			.BYTE 1
ExtForcePrev	:			.BYTE 1
ExtForceCounter	:			.BYTE 1

STEP_U			:			.BYTE 2				; ��� ��������� ���������� 1 �� = 100 ��
FL_LOCK			:			.BYTE 1				; ���� ���������� ������ �� ���������
FL_MAX_U		:			.BYTE 1				; ���� ������������� �������� U
FL_RANGE		:			.BYTE 1				; ���� ���������� ���������� � ��������� 00 - ��� ���������, FF - � ���������
MODE			:			.BYTE 1				; ����� ������ 0 - ����������, 1 - ������(��������), 2 - ����(���������), 3 - ������� ����

ADC_CURRENT_SUM_QUAD	:	.BYTE 4						; ����� ��������� �������� ��� � ������� �������
ADC_CURRENT_VALUE_COUNT	:	.BYTE 2						; ���������� �������������� ��� � ������� �������
CURRENT_QUAD_U			:	.BYTE 4						; ������� ����������

AVERAGE_U				:	.BYTE 2						; ������� �������� ������������ URMS
TARGET_U				:	.BYTE 2						; ���������� ����������
OUTPUT_U_POINTER		:	.BYTE 2						; ������ �� ������������ �������� �������� ��������
//OUTPUT_U				:	.BYTE 2						; ������������ �� ���������� �������� �������
CURRENT_U				:	.BYTE 2						; ������� �������� URMS
CURRENT_U_HISTORY		:	.BYTE 2*CU_HISTORY_LENGTH
//AVERAGE_U_OLD			:	.BYTE 2						; ������ �������� ������������ URMS

EXP100MS				:	.BYTE 1						; ������� 100 �� 
BRIGHTNESS:					.BYTE 1

milliseconds:				.BYTE 1		; 1/1000 ���
santiseconds:				.BYTE 1		; 1/100
deciseconds:				.BYTE 1		; 1/10
seconds:					.BYTE 1		; 1 ���

ACTIONS_FLAGS			:	.BYTE 1		; ����� ��� ������ ������������ ��������
										; 0 - ��������� ��������� �� UART 
										; 1 - ��������� ��������� �� iic
										; 2 -
										; 3 - �������� ������� ������
										; 4 - ���������� ��������� ������
										; 5 - ������� � ����   ������� +
										; 6 - ������� � ������ ������� -
										; 7 - ������ ������ � ����
DATA_FLAGS				:	.BYTE 1		; 
										; 0 - 1000 ��
										; 1 -  100 ��
										; 2 -   10 ��
										; 3 -    1 ��
										; 4 -
										; 5 -
										; 6 -
										; 7 -
RX_BUFFER				:	.BYTE 32	; ����� ������
TX_BUFFER				:	.BYTE 32	; ����� ��������
RX_PTR					:	.BYTE 1		; ��������� ������ ������ 
TX_PTR					:	.BYTE 1		; ��������� ������ ��������

POSX:						.BYTE 1
POSY:						.BYTE 1
OLDX:						.BYTE 1
OLDY:						.BYTE 1
FontAddr:					.BYTE 2
FontNo:						.BYTE 1
StringCounter:				.BYTE 1
//MASK:						.BYTE 1

LED_ADDRESS				:	.BYTE 1
SOFT_ACK				:	.Byte 1
COUNTER_D				:	.BYTE 1		; ������� ����������� � ��������
ExtStopCounter			:	.BYTE 1		; ������� ������� ���������� ������� �������� ����� � �������� ������
ExtFullPowerCounter		:	.BYTE 1		;

DIGIT0					:	.BYTE 1
DIGIT1					:	.BYTE 1
DIGIT2					:	.BYTE 1
DIGIT3					:	.BYTE 1
DIGIT4					:	.BYTE 1
DIGIT5					:	.BYTE 1
DIGIT6					:	.BYTE 1
DIGIT7					:	.BYTE 1
DIGIT8					:	.BYTE 1
DIGIT9					:	.BYTE 1
HidedZeroesCount		:	.BYTE 1
//--
ExtrasConfig:		.BYTE 64			; ������� ��������� ������ ��������, �������������� ���������
										; byte+0:	���������� � (0...191)
										; byte+1:	���� 0...2 	- ���������� Y (0...7)
										; 			���	 4...5	- ������ ������ (0-3)
										; 			���  6		- ���������� ������ (0-����� ����, 1-����� ���)
										; 			���  7		- ������� ������������� �������� (0-������, 1-�����)

