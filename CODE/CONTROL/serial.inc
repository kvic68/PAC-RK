UD_OK:		PUSHF
			PUSHZ
			push r17
			push r16
			LDZ TX_BUFFER
			lds r16,(TX_PTR)
			ldi r17,0
			add ZL,r16
			adc ZH,r17
			inc r16
			andi r16,$7F
			cpi r16,$80
			breq StopTX
			sts (TX_PTR),r16
			ld r16,Z+

			cpi r16,0
			breq StopTX

			sts UDR0,r16

UD_Exit:	pop r16
			pop r17
			POPZ
			POPF
			reti
			; ----------
StopTX:		ldi r16,(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0)|(0<<UDRIE0)
			sts UCSR0B,r16
			ldi r16,$00
			sts (TX_BUSY),r16
			rjmp UD_Exit
			; ----------
RunTX:		ldi r16,$FF
			sts (TX_BUSY),r16
			ldi r16,(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0)|(1<<UDRIE0)
			sts UCSR0B,r16
			ret
; ----------------------------------------------------------------------------------------
TXmanager:	push r16
			push r17
			PUSHZ
			lds r16,(TX_BUSY)
			tst r16
			brne TXmanExit
			lds r16,(TX_QUEUE_POINTER)
				cpi r16,$00
			brne TXman_M1
				lds r16,(TX_QUEUE)
				cpi r16,$00
				brne TXman_M1
			rjmp TXmanExit
TXman_M1:	call prepareTX
			call shiftTXQueue
			call runTX	
TXmanExit:	POPZ
			pop r17
			pop r16
			ret
; ----------------------------------------------------------------------------------------
prepareTX:	LDZ TXVariantsTable*2
				lds r16,(TX_QUEUE)
				andi r16,$07
				clr r17
				add ZL,r16	adc ZH,r17
				add ZL,r16	adc ZH,r17
				lpm r16,Z+
				lpm r17,Z
				movw ZH:ZL,r17:r16
			icall
			ret
; ----------------------------------------------------------------------------------------
shiftTXQueue:		LDZ TX_QUEUE
					ldi r17,7

stxqLoop:			tst r17
					breq stxqEndLoop

					ldd r16,Z+1
					st Z+,r16
					dec r17
					rjmp stxqLoop

stxqEndLoop:		clr r16
					st Z,r16

					lds r16,(TX_QUEUE_POINTER)
					tst r16
					breq stxqExit
					dec r16
					sts (TX_QUEUE_POINTER),r16
stxqExit:
				ret
; ----------------------------------------------------------------------------------------
addTXQueue:		PUSHZ
				push r17
					cpi r16,8
					brcc atxqExit

					lds r17,(TX_QUEUE_POINTER)
					cpi r17,7
					brcs atxqM1

					rjmp atxqExit

atxqM1:				LDZ TX_QUEUE
					add ZL,r17
					clr r17
					adc ZH,r17
					ld r17,Z+
					cp r16,r17
					breq atxqExit

					st Z,r16
					lds r17,(TX_QUEUE_POINTER)
					inc r17
					sts (TX_QUEUE_POINTER),r17

atxqExit:		pop r17
				POPZ
				ret
; ----------------------------------------------------------------------------------------
TXVariantsTable: 	.dw	nothingToDo				; 00 - ���������� ������������ ������
					.dw	PrepareSpeedToSend		; 01 - �������� ������
					.dw	PrepareRegToSend		; 02 - ��������� �������������
					.dw	PrepareDataToSend		; 03 - ����������� ����� ������
					.dw	SendPumpData			; 04 - ���������� ����� �� 100 ��
					.dw	SendDose				; 05 - ����(��������) 
					.dw	nothingToDo		; 06 - 
					.dw	nothingToDo		; 07 - 
; ----------------------------------------------------------------------------------------
// ��� ��������� 
SendDose:			PUSHF
					PUSHXYZ
					push r16
					push r17

					LDX TX_BUFFER

					ldi r16,'<'
					st X+,r16
					ldi r16,'V'
					st X+,r16
					ldi r16,':'
					st X+,r16

					LDY DoseValue

					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16

					LDY RemainV

					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16
					ldi r16,'>'
					st X+,r16

					ldi r16,$0D
					st X+,r16
					ldi r16,$0A
					st X+,r16
					ldi r16,0
					st X+,r16
					sts (TX_PTR),r16


					pop r17
					pop r16
					POPXYZ
					POPF
					ret

; ----------------------------------------------------------------------------------------
// ���������� ���������� ����� �� 100 ��
SendPumpData:		PUSHF
					PUSHXYZ
					push r16
					push r17

					LDX TX_BUFFER

					ldi r16,'<'
					st X+,r16
					ldi r16,'Z'
					st X+,r16
					ldi r16,':'
					st X+,r16

					LDY stepsFor100ml
					ldd r16,Y+3
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+2
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16

					ldi r16,'>'
					st X+,r16

					ldi r16,$0D
					st X+,r16
					ldi r16,$0A
					st X+,r16
					ldi r16,0
					st X+,r16
					sts (TX_PTR),r16


					pop r17
					pop r16
					POPXYZ
					POPF
					ret
; ----------------------------------------------------------------------------------------
PrepareRegToSend:	PUSHF
					PUSHXYZ
					push r16
					push r17

					LDX TX_BUFFER

					ldi r16,'<'
					st X+,r16
					ldi r16,'U'
					st X+,r16
					ldi r16,':'
					st X+,r16

				LDY VOLTS
					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+3
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+2
					call HexToAscii
					st X+,r17
					st X+,r16
				LDY STABMODE	
					ldd r16,Y+0
					call HexToAscii
;					st X+,r17
					st X+,r16

				LDY STAB_RANGE		
					ldd r16,Y+0
					call HexToAscii
;					st X+,r17
					st X+,r16

				LDY STAB_MAX_U
					ldd r16,Y+0
					call HexToAscii
;					st X+,r17
					st X+,r16

					ldi r16,'>'
					st X+,r16

					ldi r16,$0D
					st X+,r16
					ldi r16,$0A
					st X+,r16
					ldi r16,0
					st X+,r16
					sts (TX_PTR),r16

					//call RunTX

PrepareRegToSendEx:
					pop r17
					pop r16
					POPXYZ
					POPF
					ret
; ----------------------------------------------------------------------------------------
PrepareSpeedToSend:	PUSHF
					PUSHXYZ
					push r16
					push r17

					LDX TX_BUFFER

					ldi r16,'<'
					st X+,r16
					ldi r16,'S'
					st X+,r16
					ldi r16,':'
					st X+,r16

					LDY RateValue
					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16

					ldi r16,'>'
					st X+,r16
					ldi r16,$0D
					st X+,r16
					ldi r16,$0A
					st X+,r16

					ldi r16,0
					st X+,r16
					sts (TX_PTR),r16
PrepareSpeedToSendEx:
					pop r17
					pop r16
					POPXYZ
					POPF
					ret
; ----------------------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
PrepareDataToSend:	PUSHF
					PUSHXYZ
					push r16
					push r17

					LDX TX_BUFFER

					ldi r16,'<'
					st X+,r16

					ldi r16,'T'
					st X+,r16
					ldi r16,':'
					st X+,r16

				LDY DS_VALUE_0
					;---------- T0	{ 3-6 }
					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- T1	{ 7-10 }
					ldd r16,Y+3
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+2
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- T2	{ 11-14 }
					ldd r16,Y+5
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+4
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- T3	{ 15-18 }
					ldd r16,Y+7
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+6
					call HexToAscii
					st X+,r17
					st X+,r16

				LDY DS_MARGIN_0
					;---------- Margin T0	{ 19-22 }
					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- Margin T1	{ 23-26 }
					ldd r16,Y+3
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+2
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- Margin T2	{ 27-30 }
					ldd r16,Y+5
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+4
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- Margin T3	{ 31-34 }
					ldd r16,Y+7
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+6
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- Pressure PA	{ 35-40 } (6)
				LDY PRESSURE_PA_AVG
					ldd r16,Y+2
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- Rate	{ 41-44 }
				LDY RateValue
					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- Calc Total	
						PUSHHIGH
						LDSA stepsFor100ml
							LDI_B 100
							call DIV3232U
							COPY_A_B
						LDSA STEPCOUNTER
						call DIV3232U
						sts (TotalV+0),r22
						sts (TotalV+1),r23
						sts (TotalV+2),r24
						POPHIGH	
					;---------- Total	{ 45-49 }
				LDY TotalV
					ldd r16,Y+2
					call HexToAscii
//					st X+,r17
					st X+,r16

					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- Dose (autostop)	{ 50-53 }
				LDY DoseValue
					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- Calc Remain 	
						PUSHHIGH
						LDSA stepsFor100ml
						LDI_B 100
						call DIV3232U
						COPY_A_B
						LDSA STEPBACKCOUNTER
						call DIV3232U
						sts (RemainV+0),r22
						sts (RemainV+1),r23
						POPHIGH
					;---------- Remain		{ 54-57 }
				LDY RemainV
					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- ��������� ����������� �������	{ 58 }
				LDY DRIVE
					ldd r16,Y+0
					call HexToAscii
					st X+,r16
					;---------- ���� ������������� �������� �� ���������� ������ 	{ 59-62 }
				LDY REACTION_0
					ldd r16,Y+0
					call HexToAscii
					st X+,r16
					ldd r16,Y+1
					call HexToAscii
					st X+,r16
					ldd r16,Y+2
					call HexToAscii
					st X+,r16
					ldd r16,Y+3
					call HexToAscii
					st X+,r16
					;---------- ����� ���������	{ 63 }
				LDY A_FLAGS
					ldd r16,Y+0
					call HexToAscii
					st X+,r16

					;---------- ���� ����� 	{ 64-67 }
				LDY VOLTS
					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- ���������� ����� 	{ 68-71 }
					ldd r16,Y+3
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+2
					call HexToAscii
					st X+,r17
					st X+,r16
					;---------- ����� ������������� { 72 }
				LDY STABMODE	
					ldd r16,Y+0
					call HexToAscii
//					st X+,r17
					st X+,r16
					;---------- ��������� �������� ������� { 73 }
				LDY HeatControlStatus
					ldd r16,Y+0
					call HexToAscii
				//	st X+,r17
					st X+,r16
					;---------- ����� �������, ��������������� ������ { 74 }
				LDY HeatControlSensor
					ldd r16,Y+0
					call HexToAscii
					//st X+,r17
					st X+,r16
					;---------- ����� ���������� ������� { 75-76 }
				LDY HeatControlMargin
					ldd r16,Y+1
					call HexToAscii
					st X+,r17
					st X+,r16
					ldd r16,Y+0
					call HexToAscii
					st X+,r17
					st X+,r16
					;----------
					ldi r16,'>'
					st X+,r16
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
					ldi r16,$0D
					st X+,r16
					ldi r16,$0A
					st X+,r16
//------------------------------------------------------------------
					ldi r16,0
					st X+,r16
					sts (TX_PTR),r16

PrepareDataToSendEx:
					pop r17
					pop r16
					POPXYZ
					POPF
					ret
; ----------------------------------------------------------------------------------------.
; --------------------------------------------------------
strToHex:			cpi r16,$30
					brcs sthError
					cpi r16,$3A
					brcc sthCheck1
					subi r16,$30
					clc
					rjmp sthExit
sthCheck1:			cpi r16,$41
					brcs sthError
					cpi r16,$47
					brcc sthCheck2
					subi r16,$37
					clc 
					rjmp sthExit
sthCheck2:			cpi r16,$61
					brcs sthError
					cpi r16,$67
					brcc sthError
					subi r16,$57
					clc 
					rjmp sthExit
sthError:			sec
sthExit:			ret
; ----------------------------------------------------------------------------------------
TX_OK:		reti
; ----------------------------------------------------------------------------------------
RX_OK:		PUSHF
			PUSHZ
			push r17
			push r16
			lds r16,(A_FLAGS)
			sbrc r16,3
			rjmp RX_EXIT				; ���� ���������� ���������� �� ����������, ���������� ��

			LDZ RX_BUFFER				; ������ ������ ������

			lds r16,UDR0				; ��������� ����
			
			cpi r16,$0D
			brne RX_CHK_0
			rjmp RX_0D

RX_CHK_0:	cpi r16,'G'					; ������� �� G �� Z ������������ ��� ��������� ����, ��� ���� �������
			brcc RX_CHK_1
				rjmp RX_CHK_4
RX_CHK_1:	cpi r16,'Z'
				brlo RX_CHK_5
				breq RX_CHK_5
			cpi r16,'a'
			brcc RX_CHK_2
				rjmp RX_EXIT
RX_CHK_2:	cpi r16,'z'
				brlo RX_CHK_3
				breq RX_CHK_3
			rjmp RX_EXIT

RX_CHK_3:	subi r16,$20
			rjmp RX_CHK_0

RX_CHK_4:	rjmp RX_HEXDIGITS
RX_CHK_5:	rjmp RX_PREFIX
//---------------------------
; ---------- ������ �������� (G..Z)
RX_PREFIX:	st Z,r16
			clr r16
			sts (RX_PTR),r16			; ��������� �� 0
			rjmp RX_EXIT	
; ---------- ��� ������ ������� ����� #0D
RX_HEXDIGITS:	cpi r16,'0'
					brlo RX_nodigit
				cpi r16,'9'
					brlo RX_digit
					breq RX_digit
RX_HEX_A:			cpi r16,'A'
					brlo RX_nodigit
				cpi r16,'F'
					brlo RX_digit
					breq RX_digit
				subi r16,$20
				rjmp RX_HEX_A
				//----------
RX_nodigit:		rjmp RX_EXIT
RX_digit:
				lds r17,(RX_PTR)
				inc r17
				cpi r17,$0F			; ��������� ������������� ������
				brlo RX_nofull
				breq RX_nofull
				rjmp RX_EXIT
				//----------
RX_nofull:		sts (RX_PTR),r17
				add ZL,r17
				ldi r17,0
				adc ZH,r17
					st Z,r16
			rjmp RX_EXIT
; ---------- ������ ������ #0D
RX_0D:		lds r16,(A_FLAGS)
			SETB r16,3						; ������������� ���� ������ ����������
			sts (A_FLAGS),r16			; � RX_PTR ���������� �������� ����� ������������ (G..Z)
RX_EXIT:
			pop r16
			pop r17
			POPZ
			POPF
			reti
; ----------------------------------------------------------------------------------------
RX_DECODE:			PUSHF
//					cli
					PUSHZ
					PUSHY
					push r16
					push r17
					push r18

					LDZ RX_BUFFER
					lds r17,(RX_PTR)

					ld r16,Z
					cpi r16,'P'
						brne sR_1
						rjmp received_P		; ��������� ��������
sR_1:				cpi r16,'M'
						brne sR_2
						rjmp received_M		; ��������� ������� � �������
sR_2:				cpi r16,'T'
						brne sR_3
						rjmp received_T		; ��������� �������� �������� ���������� � ���������� ����������
sR_3:				cpi r16,'V'
						brne sR_4
						rjmp received_V		; ��������� �������� ����
sR_4:				cpi r16,'U'
						brne sR_5
						rjmp received_U		; ��������� ������ �������������	
sR_5:				cpi r16,'R'
						brne sR_6
						rjmp received_R		; ��������� ������ ������
sR_6:				cpi r16,'Z'
						brne sR_7
						rjmp received_Z		; ��������� ������
sR_7:				cpi	r16,'Y'
						brne sR_8
						rjmp received_Y		; ��������� �������� �������
sR_8:
					rjmp stringReceivedExit
; ----------
// ��������� �������� �������
received_Y:			cpi r17,6
					breq recvY_six
					cpi r17,1
					breq recvY_one
					cpi r17,2
					breq recvY_two
					cpi r17,4
					breq recvY_four
					rjmp recvY_ex
recvY_one:			rjmp recvY_ex
recvY_two:			rjmp recvY_ex
recvY_four:			rjmp recvY_ex
recvY_six:			rjmp recvY_1

recvY_1:			// ������ ���� - �����(���/���� 1/0 FF/00)
					ldd r16,Z+1
					call strToHex
					brcc recvY_1A
					rjmp recvY_ex

recvY_1A:			cpi r16,$0F
					breq recvY_1AA
					andi r16,$01
					sbrc r16,0
					ldi r16,$FF				
					sts (HeatControlStatus),r16

					// ������ ���� - ����� �������
recvY_1AA:			ldd r16,Z+2
					call strToHex
					brcc recvY_1AB
					rjmp recvY_ex
recvY_1AB:			cpi r16,$0F
					breq recvY_1B
					andi r16,$03
					sts (HeatControlSensor),r16	; ��������/��������� FF/00

					// 3,4,5,6 ����� - ����� ���������� (�������/�������)
recvY_1B:			ldd r16,Z+3
					call strToHex
					brcc recvY_2
					rjmp recvY_ex

recvY_2:			swap r16
					andi r16,$F0
					mov r17,r16

					ldd r16,Z+4
					call strToHex
					brcc recvY_3
					rjmp recvY_ex
recvY_3:			andi r16,$0F
					or r17,r16				; � R17 ������� ���� ������
	
					ldd r16,Z+5
					call strToHex
					brcc recvY_4 
					rjmp recvY_ex

recvY_4:			swap r16
					andi r16,$F0
					mov r18,r16

					ldd r16,Z+6
					call strToHex
					brcc recvY_5 
					rjmp recvY_ex
recvY_5:			andi r16,$0F
					or r18,r16				; � R18 ������� ���� ������

					ldi r16,$A0
					cp  r16,r18
					ldi r16,$0F
					cpc r16,r17

					breq recvY_6
					brcc recvY_6
					rjmp recvY_end

recvY_6:			sts (HeatControlMargin+1),r17
					sts (HeatControlMargin+0),r18

recvY_end:			ldi r16,HStatus	
					call SetNewValFlag
					ldi r16,HSensor
					call SetNewValFlag
					ldi r16,HMargin
					call SetNewValFlag
			
					ldi r16,HString
					call SetNewValFlag

					call saveDefaultValues
					
recvY_ex:		
					rjmp stringReceivedExit
; ----------
// ��������� ������
received_Z:			cpi r17,1
					breq recvZ_one			; ���� ����� ����� ����� ������ ���������� �����
					cpi r17,9
					breq recvZ_nine			; ������ ���� ����� �����
					rjmp recvZ_EX
; ���� ����� ����� �����
recvZ_one:			ldd r16,Z+1
					call strToHex
					brcc recvZ_one0
					rjmp stringReceivedExit

recvZ_one0:			ldi r17,0
					cpse r17,r16
					rjmp recvZ_one1

					ldi r16,4
					call addTXQueue
					rjmp stringReceivedExit

recvZ_one1:			inc r17
					cpse r17,r16
					rjmp recvZ_one2
						ldi r16,0
						sts (STEPCOUNTER+3),r16
						sts (STEPCOUNTER+2),r16
						sts (STEPCOUNTER+1),r16
						sts (STEPCOUNTER+0),r16
					rjmp stringReceivedExit

recvZ_one2:			
					rjmp stringReceivedExit
; ������ ���� ����� �����
recvZ_nine:			ldd r16,Z+2
					call strToHex
					brcc recvZ_nine1
					rjmp recvZ_EX

recvZ_nine1:		swap r16
					andi r16,$F0
					mov r25,r16

					ldd r16,Z+3
					call strToHex
					brcc recvZ_nine2
					rjmp recvZ_EX
recvZ_nine2:		andi r16,$0F
					or r25,r16				; � R25 ������� ����

					ldd r16,Z+4
					call strToHex
					brcc recvZ_nine3
					rjmp recvZ_EX

recvZ_nine3:		swap r16
					andi r16,$F0
					mov r24,r16

					ldd r16,Z+5
					call strToHex
					brcc recvZ_nine4
					rjmp recvZ_EX
recvZ_nine4:		andi r16,$0F
					or r24,r16				; � R24 

					ldd r16,Z+6
					call strToHex
					brcc recvZ_nine5
					rjmp recvZ_EX

recvZ_nine5:		swap r16
					andi r16,$F0
					mov r23,r16

					ldd r16,Z+7
					call strToHex
					brcc recvZ_nine6
					rjmp recvZ_EX
recvZ_nine6:		andi r16,$0F
					or r23,r16				; � R23
					
					ldd r16,Z+8
					call strToHex
					brcc recvZ_nine7
					rjmp recvZ_EX

recvZ_nine7:		swap r16
					andi r16,$F0
					mov r22,r16

					ldd r16,Z+9
					call strToHex
					brcc recvZ_nine8
					rjmp recvZ_EX
recvZ_nine8:		andi r16,$0F
					or r22,r16				; � R22 ������� ����

					ldd r16,Z+1
					call strToHex
					brcc recvZ_nine9
					rjmp recvZ_EX
recvZ_nine9:		andi r16,$0F
					mov r17,r16				; � R17 ��� ��������

				push r17
					STSA stepsFor100ml
					LDI_B 100
					call DIV3232U
					STSA stepsFor1ml
				pop r17

			lds r16,(A_FLAGS)			;	
			push r16
				push r17
					CLRB r16,0					;	��������� ����
					sts (A_FLAGS),r16			;	
					
						LDI_A 0
						STSA STEPCOUNTER
					
					call stepBackCounterCalc	;	������������� �������� �������
					call ocr1aCalc
				pop r17
					cpi r17,1
					brne recvZ_nine10
						LDSA stepsFor100ml
						LDZ EEnStepsFor100ml
						mov r16,r22		call EEWriteByte
						mov r16,r23		call EEWriteByte
						mov r16,r24		call EEWriteByte
						mov r16,r25		call EEWriteByte
recvZ_nine10:		ldi r16,4
					call addTXQueue
			pop r16
			sts (A_FLAGS),r16			;	
					
recvZ_EX:			rjmp stringReceivedExit
; ----------
// ��������� ������ ������
received_R:			cpi r17,1
					breq recvR_0
					rjmp recvR_EX
recvR_0:			lds r16,(A_FLAGS)
					mov r17,r16
					andi r17,$07
recvR_1:			cpi r17,0
					brne recvR_2
					SETB r16,0
					rjmp recvR_End 
recvR_2:			cpi r17,1
					brne recvR_3
					CLRB r16,0
					rjmp recvR_End
recvR_3:			cpi r17,2
					brne recvR_4
					CLRB r16,1
					SETB r16,0
					rjmp recvR_End
recvR_4:			cpi r17,3
					brne recvR_5
					CLRB r16,1
					SETB r16,0
					rjmp recvR_End
recvR_5:			rjmp recvR_EX
recvR_End:			sts (A_FLAGS),r16//					call togglePause
						ldi r16,3
						call addTXQueue
						clr r16
						sts (SOUNDNUMBER),r16
recvR_EX:		rjmp stringReceivedExit
; ----------
// ��������� ������ ������ ����������
received_U:		cpi r17,1
					breq recvU_1
					rjmp recvU_EX
recvU_1:			ldd r16,Z+1
					call strToHex
					brcc recvU_2
					rjmp recvU_EX

recvU_2:			andi r16,$03

					LDY DataToStab
					std Y+2,r16
					ldi r16,$FF
					std Y+0,r16
					std Y+1,r16
					
					lds r16,(KEY_FLAGS)
					SETB r16,5
					sts (KEY_FLAGS),r16

						ldi r16,2
						call addTXQueue

recvU_EX:			rjmp stringReceivedExit
; ----------
; ----------
; ��������� �������� �������� ���������� � ���������� ���������� Txxxx, xxxx - ���������� � ������� ����� ������ (hex)
received_T:			//nop
				cpi r17,4
					breq recvT_1
					rjmp recvT_EX

recvT_1:			ldd r16,Z+1
					call strToHex
					brcc recvT_2
					rjmp recvT_EX

recvT_2:			swap r16
					andi r16,$F0
					mov r17,r16

					ldd r16,Z+2
					call strToHex
					brcc recvT_3
					rjmp recvT_EX
recvT_3:			andi r16,$0F
					or r17,r16				; � R17 ������� ���� ����������
	
					ldd r16,Z+3
					call strToHex
					brcc recvT_4 
					rjmp recvT_EX

recvT_4:			swap r16
					andi r16,$F0
					mov r18,r16

					ldd r16,Z+4
					call strToHex
					brcc recvT_5 
					rjmp recvT_EX
recvT_5:				andi r16,$0F
					or r18,r16				; � R18 ������� ���� ����������

					LDY DataToStab
					std Y+0,r18
					std Y+1,r17

					lds r17,$FF
					std Y+2,r17

					lds r16,(KEY_FLAGS)
					SETB r16,5
					sts (KEY_FLAGS),r16
//02
						ldi r16,2
						call addTXQueue


recvT_EX:			rjmp stringReceivedExit
; ----------
; ----------
// ������� ���������� ��������� ������� � �������, ����� 7 �������� Mnxxxxy, n-����� �������(0..3) xxxx-�������� ������ y-��� �������
received_M:			cpi r17,6
					breq recvM_1
					rjmp stringReceivedExit

recvM_1:			ldd r16,Z+6				; ��� �������
					call strToHex
					brcs sR_M3

					cpi r16,$0F
					brlo sR_M2
					breq sR_M2
					rjmp sR_M3

sR_M2:				mov r18,r16

					ldd r16,Z+1				; ����� �������
					call strToHex
					brcc sR_M2A
					rjmp sR_M4

sR_M2A:				LDY REACTION_0
					push r16
					add YL,r16
					ldi r16,0
					adc YH,r16
					st Y,r18				; ���������� ����� ��� ������� ��� ������� � �������
					pop r16

					ldi r17,REACT_0
					push r16
					add r16,r17
						call setNewValFlag
					pop r16
					ldi r17,R_On0
					add r16,r17
						call setNewValFlag


sR_M3:				ldd r16,Z+2
					call strToHex
					brcs sr_M4
					swap r16
					andi r16,$F0
					mov r17,r16

					ldd r16,Z+3
					call strToHex
					brcs sr_M4
					andi r16,$0F
					or r17,r16				; � R17 ������� ����

					ldd r16,Z+4
					call strToHex
					brcs sr_M4
					swap r16
					andi r16,$F0
					mov r18,r16

					ldd r16,Z+5
					call strToHex
					brcs sr_M4
					andi r16,$0F
					or r18,r16				; � R18 ������� ����

				LDY $FC90
				cp  r18,YL
				cpc r17,YH
				brcc sR_M3A					; �� -55 �� -1/16
				LDY $0801
				cp  r18,YL
				cpc r17,YH
				brcc sR_M4
				

sR_M3A:				ldd r16,Z+1
					call strToHex			; � r16 �������� (����� �������)
					brcs sR_M4

					LDY DS_MARGIN_0
					push r16
					add r16,r16
					add YL,r16
					ldi r16,0
					adc YH,r16
					std Y+0,r18
					std Y+1,r17
					pop r16

					ldi r17,MARGIN0
					add r16,r17
						call setNewValFlag
						call saveDefaultValues
						ldi r16,3
						call addTXQueue

sR_M4:				rjmp stringReceivedExit
			
; ----------
; ���������� ��������� �������� ���� Vxxxx xxxx - ����
received_V:			cpi r17,4
					breq recvV_1
					rjmp recvV_EX

recvV_1:			ldd r16,Z+1
					call strToHex
					brcc recvV_2 
					rjmp recvV_EX
recvV_2:				swap r16
					andi r16,$F0
					mov r17,r16

					ldd r16,Z+2
					call strToHex
					brcc recvV_3 
					rjmp recvV_EX
recvV_3:				andi r16,$0F
					or r17,r16				; � R17 ������� ���� ���������
	
					ldd r16,Z+3
					call strToHex
					brcc recvV_4 
					rjmp recvV_EX
recvV_4:				swap r16
					andi r16,$F0
					mov r18,r16

					ldd r16,Z+4
					call strToHex
					brcc recvV_5 
					rjmp recvV_EX
recvV_5:				andi r16,$0F
					or r18,r16				; � R18 ������� ���� ���������

					ldi r16,low  (maxDoseValue+1)
					cp  r18,r16
					ldi r16,high (maxDoseValue+1)
					cpc r17,r16
					brcc recvV_EX


					lds r16,(A_FLAGS)
				push r16
					CLRB r16,0
					sts (A_FLAGS),r16

					LDY DoseValue
					std Y+0,r18
					std Y+1,r17

					LDY RemainV
					std Y+0,r18
					std Y+1,r17

					call stepBackCounterCalc

					ldi r16,Dose
					call setNewValFlag

					ldi r16,Remain
					call setNewValFlag

				pop r16
					sts (A_FLAGS),r16
						ldi r16,5
						call addTXQueue


recvV_EX:		rjmp stringReceivedExit
; ----------
; ���������� ��������� �������� Pxxxx xxxx - ��������
received_P:			cpi r17,4
					breq recvP_1
					rjmp recvP_EX

recvP_1:			ldd r16,Z+1
					call strToHex
					brcc recvP_2
					rjmp recvP_EX

recvP_2:				swap r16
					andi r16,$F0
					mov r17,r16

					ldd r16,Z+2
					call strToHex
					brcc recvP_3
					rjmp recvP_EX
recvP_3:				andi r16,$0F
					or r17,r16				; � R17 ������� ���� ��������
	
					ldd r16,Z+3
					call strToHex
					brcc recvP_4 
					rjmp recvP_EX

recvP_4:				swap r16
					andi r16,$F0
					mov r18,r16

					ldd r16,Z+4
					call strToHex
					brcc recvP_5 
					rjmp recvP_EX
recvP_5:				andi r16,$0F
					or r18,r16				; � R18 ������� ���� ��������


					ldi r16,low  (maxRateValue+1)
					cp  r18,r16
					ldi r16,high (maxRateValue+1)
					cpc r17,r16
					brcc recvP_EX


					LDY RateValue
					std Y+0,r18
					std Y+1,r17

					call ocr1aCalc
					ldi r16,Rate
					call setNewValFlag
//1
						ldi r16,1
						call addTXQueue

recvP_EX:		rjmp stringReceivedExit
; ----------
stringReceivedExit:	clr r16
					sts (RX_PTR),r16
					lds r16,(A_FLAGS)
					CLRB r16,3
					sts (A_FLAGS),r16

					pop r18
					pop r17
					pop r16
					POPY
					POPZ
					POPF
					ret

; ----------------------------------------------------------------------------------------
