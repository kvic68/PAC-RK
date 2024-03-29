.equ		i2c_MasterBytesRX = 3				;	������������ ���������� ������ ��� ������
.equ		i2c_MasterBytesTX = 5				;	������������ ���������� ������ ��� ��������
; ----------------------------------------------------------------------------------------
.dseg
i2c_SlaveIndex:		.BYTE 1						;	������� �������� ������
i2c_InBuff:			.BYTE(i2c_MasterBytesRX)	;	����� ������
i2c_OutBuff:		.BYTE(i2c_MasterBytesTX)	;	����� ��������
.cseg
; ----------------------------------------------------------------------------------------
TWIReceived:	PUSHF
				push r16
				push r17
				push r18
				lds r16,(i2c_SlaveIndex)
					cpi r16,1
					brne TWIR_2
					rjmp TWI_1B

TWIR_2:				cpi r16,2
					brne TWIR_3
					rjmp TWI_2B

TWIR_3:				cpi r16,3
					brne TWIR_4
					rjmp TWI_3B

TWIR_4:				rjmp TWIR_Ex

// ������ ���� ����, ��� ���������� �������
TWI_1B:			lds r16,(i2c_InBuff+0)
				cpi r16,3
				brcs TWIR1BOk
				breq TWIR1BOk
				rjmp TWIR_Ex

TWIR1BOk:		lds r17,(MODE)
				cpi r17,3
				brne TWIR1BOk2
				rjmp TWIR_Ex

TWIR1BOk2:		sts (MODE),r16
				call setMode
				rjmp TWIR_Ex

// ������ ��� �����, ��� ������� �������� ����������
TWI_2B:			lds r16,(i2c_InBuff+0)
				lds r17,(i2c_InBuff+1)
				sbrc r17,7
				rjmp TWIR_Ex
				ldi r18,low (MAX_U)
				cp  r16,r18
				ldi r18,high(MAX_U)
				cpc r17,r18
				breq TWIR2BOk
				brcs TWIR2BOk
				rjmp TWIR_Ex
TWIR2BOk:		sts (TARGET_U+0),r16
				sts (TARGET_U+1),r17

				ldi r16,TargetV
				call setNewValFlag

				rjmp TWIR_Ex

// ������ ��� �����, ������ ��� - ����, ������ - ����� ������
TWI_3B:			lds r16,(i2c_InBuff+0)
				lds r17,(i2c_InBuff+1)
				sbrc r17,7
				rjmp TWIR3BNoU

				ldi r18,low (MAX_U)
				cp  r16,r18
				ldi r18,high(MAX_U)
				cpc r17,r18
				breq TWIR3BOk1
				brcs TWIR3BOk1
				rjmp TWIR3BNoU

TWIR3BOk1:		sts (TARGET_U+0),r16
				sts (TARGET_U+1),r17

				ldi r16,TargetV
				call setNewValFlag

				rjmp TWIR_Ex

TWIR3BNoU:		lds r16,(i2c_InBuff+2)
				cpi r16,3
				brcs TWIR3BOk2
				breq TWIR3BOk2
				rjmp TWIR_Ex

TWIR3BOk2:		lds r17,(MODE)
				cpi r17,3
				brne TWIR3BOk3
				rjmp TWIR_Ex

TWIR3BOk3:		sts (MODE),r16
				call setMode

				rjmp TWIR_Ex
TWIR_Ex:
				pop r18
				pop r17
				pop r16
				POPF
			ret
; ----------------------------------------------------------------------------------------
TWI:		PUSHF
			push r16
			push r17
			push ZL
			push ZH
					ldi ZL,low (TWISubsTable*2)
					ldi ZH,high(TWISubsTable*2)
			lds r16,TWSR
			andi r16,$F8
			lsr r16
			lsr r16
			lsr r16
					clr r17
					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17

					lpm r16,Z+
					lpm r17,Z
					movw ZH:ZL,r17:r16
					icall
TWI_exit:	pop ZH
			pop ZL
			pop r17
			pop r16
			POPF
			reti
; ----------------------------------------------------------------------------------------
TWISubsTable:	;---  Master  ---
				.dw case_0x00			; 00		Bus Fail (������� ��������)
				.dw TWI_NoAction		; 08		����� ���
				.dw TWI_NoAction		; 10		��������� ����� ���
				.dw TWI_NoAction		; 18		��� ������ SLA+W �������� ACK
				.dw TWI_NoAction		; 20		��� ������ SLA+W �������� NACK - ����� ���� �����, ���� ��� ��� ����.
				.dw TWI_NoAction		; 28		���� ������ �������, �������� ACK
				.dw TWI_NoAction		; 30		���� ����, �� �������� NACK ������ ���. 1� �������� �������� ������� � ��� ����. 2� ����� �������.
				.dw TWI_NoAction		; 38		�������� �� ����. ������� ��� �� ���������
				.dw TWI_NoAction		; 40		������� SLA+R �������� ���.
				.dw TWI_NoAction		; 48		������� SLA+R, �� �������� NACK. ������ slave ����� ��� ��� ��� ����. 
				.dw TWI_NoAction		; 50		������� ����.
				.dw TWI_NoAction		; 58		��� �� ����� ��������� ����, ������� NACK ����� �������� � �����.
				;---  Slave  ---
				.dw case_0x60			; 60	RCV SLA+W  Incoming?				// ��� ������ �������� ���� �����
				.dw TWI_NoAction		; 68	RCV SLA+W Low Priority				// ������� ���� ����� �� ����� �������� ��������
				.dw TWI_NoAction;case_0x70			; 70	RCV SLA+W  Incoming? (Broascast)	// ��� ����������������� �����
				.dw TWI_NoAction		; 78	RCV SLA+W Low Priority (Broadcast)	// ��� ��� ��� ����������������� �����
				.dw case_0x80			; 80	RCV Data Byte						// � ��� �� ������� ���� ����
				.dw case_0x88			; 88	RCV Last Byte						// ������� ��������� ����
				.dw TWI_NoAction;case_0x90			; 90	RCV Data Byte (Broadcast)
				.dw TWI_NoAction;case_0x98			; 98	RCV Last Byte (Broadcast)
				.dw case_0xA0			; A0		�������� ��������� ����� ��� ����
				.dw case_0xA8			; A8		������ ������� ���� ����� �� ������
				.dw TWI_NoAction		; B0		������� ���� ����� �� ������ �� ����� �������� ��������
				.dw case_0xB8			; B8		������� ����, �������� ACK
				.dw case_0xC0			; C0		�� ������� ��������� ����, ������ � ��� ���, �������� NACK
				.dw case_0xC8			; C8		��� ACK. � ������ ������ ��� ���. �.�. ������ ������ � ��� ���
				;---         ---
				.dw TWI_NoAction		; D0
				.dw TWI_NoAction		; D8
				.dw TWI_NoAction		; E0
				.dw TWI_NoAction		; E8
				.dw TWI_NoAction		; F0
				.dw TWI_NoAction		; F8

; ----------------------------------------------------------------------------------------
case_0x00:		ldi r16,0<<TWSTA|1<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE
				sts TWCR,r16
				ret
; ----------------------------------------------------------------------------------------
case_0x68:
case_0x78:
case_0x60:		;	������ �������� ���� �����
case_0x70:		ldi r16,0
				sts i2c_SlaveIndex,r16
				lds r16,i2c_MasterBytesRX
				cpi r16,1
				brne case_70_1
				ldi r16,0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE
				rjmp case_70_2
case_70_1:		ldi r16,0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE
case_70_2:		sts TWCR,r16
				ret
; ----------------------------------------------------------------------------------------
case_0x80:		;	RCV Data Byte		
case_0x90:		clr r16
				lds r17,i2c_SlaveIndex
				LDZ i2c_InBuff
				add ZL,r17
				adc ZH,r16
				lds r16,TWDR
				st Z,r16
				inc r17
				sts i2c_SlaveIndex,r17
				ldi r16,i2c_MasterBytesRX
				dec r16
				cp r17,r16
				brne case_90_1
				ldi r16,0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE
				rjmp case_90_2
case_90_1:		ldi r16,0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE
case_90_2:		sts TWCR,r16
				ret
; ----------------------------------------------------------------------------------------
case_0x88:		;	RCV Last Byte
case_0x98:		clr r16
				lds r17,i2c_SlaveIndex
				LDZ i2c_InBuff
				add ZL,r17
				adc ZH,r16
				lds r16,TWDR
				st Z,r16
					inc r17
					sts i2c_SlaveIndex,r17
				ldi r16,0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE
				sts TWCR,r16
				// ��������, ��� ���� �������, ���� ������������
					lds r16,(ACTIONS_FLAGS)
					SETB r16,1
					sts (ACTIONS_FLAGS),r16
				ret
; ----------------------------------------------------------------------------------------
case_0xA0:		;	�������� ��������� ����� ��� ����
				ldi r16,0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE
				sts TWCR,r16
				// ��������, ��� ���� �������, ���� ������������
					lds r16,(ACTIONS_FLAGS)
					SETB r16,1
					sts (ACTIONS_FLAGS),r16
				ret
; ----------------------------------------------------------------------------------------
case_0xB0:
case_0xA8:		;	������� ���� ����� �� ������
				ldi r17,0
				sts i2c_SlaveIndex,r17
				LDZ i2c_OutBuff
				clr r16
				add ZL,r17
				adc ZH,r16
				ld r16,Z
				sts TWDR,r16
				lds r16,i2c_MasterBytesTX
				cpi r16,1
				brne case_A8_1
				ldi r16,0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE
				rjmp case_A8_2
case_A8_1:		ldi r16,0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE
case_A8_2:		sts TWCR,r16
				ret
; ----------------------------------------------------------------------------------------
case_0xB8:		;	������� ����, �������� ACK
				clr r16
				lds r17,i2c_SlaveIndex
				inc r17
				sts i2c_SlaveIndex,r17
				LDZ i2c_OutBuff
				add ZL,r17
				adc ZH,r16
				ld r16,Z
				sts TWDR,r16
				ldi r16,i2c_MasterBytesTX
				dec r16
				cp r17,r16
				brne case_B8_1
				ldi r16,0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE
				rjmp case_B8_2
case_B8_1:		ldi r16,0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE
case_B8_2:		sts TWCR,r16
				ret
; ----------------------------------------------------------------------------------------
case_0xC0:
case_0xC8:		ldi r16,0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE
				sts TWCR,r16
				ret
; ----------------------------------------------------------------------------------------
TWI_NoAction:	ret
; ----------------------------------------------------------------------------------------
