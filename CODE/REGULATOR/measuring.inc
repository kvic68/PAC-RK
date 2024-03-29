;; ----------------------------------------------------------------------------------------
; ���������� �������������� ���
ADC_COMPLETE:
			PUSHF
			push ZL
			push ZH
			push r6
			push r7
			push r8
			push r9
			push r10
			push r11
			push r12
			push r13
			; ----------
			sbis MOC_PORT,MOC_PIN
			rjmp ADC_NORM
			clr r10
			clr r11
			clr r12
			clr r13
			rjmp ADC_ADD_NEW
			; ----------
			; ��������� �������� ���
ADC_NORM:	lds r6,ADCL
			lds r7,ADCH
			; ----------
			; �������� � ������� �������� ���
			movw r8,r6
			call MUL1616
			; ����������� ������ �������� ����� ���������
ADC_ADD_NEW:
			lds r6,(ADC_CURRENT_SUM_QUAD+0)
			lds r7,(ADC_CURRENT_SUM_QUAD+1)
			lds r8,(ADC_CURRENT_SUM_QUAD+2)
			lds r9,(ADC_CURRENT_SUM_QUAD+3)
			; ������� � ���� ����� �������
			add r6,r10
			adc r7,r11
			adc r8,r12
			adc r9,r13
			; �������� ����� ���������
			sts (ADC_CURRENT_SUM_QUAD+0),r6
			sts (ADC_CURRENT_SUM_QUAD+1),r7
			sts (ADC_CURRENT_SUM_QUAD+2),r8
			sts (ADC_CURRENT_SUM_QUAD+3),r9
			; �������� ������� ��������������
			lds ZL,(ADC_CURRENT_VALUE_COUNT+0)
			lds ZH,(ADC_CURRENT_VALUE_COUNT+1)
			adiw Z,1
			; �������� ������� ��������������
			sts (ADC_CURRENT_VALUE_COUNT+0),ZL
			sts (ADC_CURRENT_VALUE_COUNT+1),ZH
			; ----------
			pop r13
			pop r12
			pop r11
			pop r10
			pop r9
			pop r8
			pop r7
			pop r6
			pop ZH
			pop ZL
			POPF
			reti
; ----------------------------------------------------------------------------------------
; ������� �������� ���������� ����� 0 (���������� INT0)
ZEROCROSS:	PUSHF
			push r16
			push r17
			
			push r18
			push r19
			; ----------
			; ��������� ���������� INT0
			ldi r16,$00
			sts (EIMSK),r16
			; ----------
			; ����� ������
			sbi MOC_PORT,MOC_PIN
			; �������� ����� ������ �1	
			lds  r16, TCCR1B			;
				push r16
			; ������������� T1				
			andi r16,$F8			;
			sts TCCR1B,r16	
			; �������� ��������� �1
			lds r18,TCNT1L
			lds r19,TCNT1H
			; �������� T1		
			clr r16					; 
			sts TCNT1H,r16			
			sts TCNT1L,r16	
			; �������������  ������  ��������� �������
				lds r17,(OPTO_ON_TICK+1)		
				lds r16,(OPTO_ON_TICK+0)	;
			;
			sts OCR1AH,r17				;
			sts OCR1AL,r16				; 
			; �������������  ������  ���������� �������
			;	ldi r18, low(1000)
			;	ldi r19,high(1000)
			;	add r16,r18
			;	adc r17,r19
			;	sts (OPTO_OFF_TICK+0),r16
			;	sts (OPTO_OFF_TICK+1),r17
				//-----------------------
				lds r17,(OPTO_OFF_TICK+1)		
				lds r16,(OPTO_OFF_TICK+0)	;
			sts OCR1BH,r17				;
			sts OCR1BL,r16				; 
			; ����� ���������� �0,�1
			ldi r16,0b00000001		
			out GTCCR,r16
			; ���������� ����� ������ �1
				pop r16		
			; ��������� T1			
			sts TCCR1B,r16			
			; ----------
			; ----------
ZEROCROSS_M0:	push r20				; ���� � ��������� ��������� ���������� �� ������
				push ZH
				push ZL
				push YH
				push YL
				; ----------
				PUSH_ALL_LOW
				; ----------
				lds r16,EICRA			; ��������, ����� ������� ������
				andi r16,$03
				cpi r16,$02
				breq FALLING			; ������� �� �����
				cpi r16,$03
				breq RISING				; ������� �� ������

				ldi r16,0b00001011		; ������������� �� �������� ������ INT0
				sts EICRA,r16
				; ----------
			jmp ZEROCROSS_END
; ---------------------------------------------------------------------------------------------
			; ������������ ����� �������� ����� 0
RISING:		ldi r16,ADC_ON				; ����� ���
			sts ADCSRA,r16
				lds r16,EICRA
				andi r16,$FC
				ori  r16,$02			; ������������� �� �������� ����� INT0
				sts EICRA,r16
				; ----------
				sts (COUNT_NEG+0),r18	; �������� ������������
				sts (COUNT_NEG+1),r19	; �������������� �����������
				; ----------
				; ----------
			jmp ZEROCROSS_END
; ---------------------------------------------------------------------------------------------
			; ������������ ���� �������� ����� 0
FALLING:	ldi r16,ADC_OFF				; ���� ���
			sts ADCSRA,r16
				lds r16,EICRA
				andi r16,$FC
				ori  r16,$03			; ������������� �� �������� ������ INT0
				sts EICRA,r16
				; ----------
				sts (COUNT_POS+0),r18	; �������� ������������
				sts (COUNT_POS+1),r19	; �������������� �����������
				; ----------
				; ----------
; ������������ ����������� �� ������������� ���������� ������
			; �������� ���������� �������� U_RMS
			lds r6,(ADC_CURRENT_SUM_QUAD+0)
			lds r7,(ADC_CURRENT_SUM_QUAD+1)
			lds r8,(ADC_CURRENT_SUM_QUAD+2)
			lds r9,(ADC_CURRENT_SUM_QUAD+3)
			lds r10,(ADC_CURRENT_VALUE_COUNT+0)
			lds r11,(ADC_CURRENT_VALUE_COUNT+1)
			; ----------	
			lsl r6						; ������� r9...r6 �� 4 ��� ������� ������������� ��������� �����
			rol r7
			rol r8
			rol r9
			lsl r6
			rol r7
			rol r8
			rol r9
			; ----------
			clr r12
			clr r13
			call div32u					; ��������� ������� RMS
			; ----------
			lsl r6						; ������� r9...r6 �� 4 
			rol r7
			rol r8
			rol r9
			lsl r6
			rol r7
			rol r8
			rol r9
			; ----------
			movw r20,r6
			movw r22,r8
			; ----------
			sts (CURRENT_QUAD_U+0),r6
			sts (CURRENT_QUAD_U+1),r7
			sts (CURRENT_QUAD_U+2),r8
			sts (CURRENT_QUAD_U+3),r9
			; ----------

			call sqr		; �������� ������ ���������� �� ����� ��������� �������� �� ������ ���������
							; � r9 r8 ������� TRUE RMS �������� ����������,
							; ���������� �� ��������� �����������, ������������ ������� ���������
			; ----------
			sts (CURRENT_U+0),r8	; �������� ������� �������� U RMS 
			sts (CURRENT_U+1),r9	; 
		; ----------
			clr r16
			sts (ADC_CURRENT_SUM_QUAD+0),r16		; ����� ������� ����� ���������
			sts (ADC_CURRENT_SUM_QUAD+1),r16
			sts (ADC_CURRENT_SUM_QUAD+2),r16
			sts (ADC_CURRENT_SUM_QUAD+3),r16
			sts (ADC_CURRENT_VALUE_COUNT+0),r16		; ������� ������� ��������������
			sts (ADC_CURRENT_VALUE_COUNT+1),r16
			; �������� ����������� U_RMS ��� ������ �� ��������� (���� �� �������)
			ldi ZH,high(CURRENT_U_HISTORY + 2*CU_HISTORY_LENGTH)
			ldi ZL,low (CURRENT_U_HISTORY + 2*CU_HISTORY_LENGTH)
			movw Y,Z				; ��������� �� ����� ����� ����������� ��������
			clr r16
			mov r8,r16
			mov r2,r16
			mov r3,r16
			ld r7, -Y							
			ld r6, -Y
			ldi r16,CU_HISTORY_LENGTH
AVG_LOOP:	tst r16
			breq AVG_LOOP_EX
			dec r16

			ld r1,-Y							
			ld r0,-Y							
				add r6, r0							
				adc r7, r1
				adc r8, r2	
				adc r9, r3						 
			st -Z,r1							
			st -Z,r0							
			; ----------
			jmp AVG_LOOP
			; ----------
AVG_LOOP_EX:ldi ZL, low(CU_HISTORY_LENGTH+1)
			ldi ZH,high(CU_HISTORY_LENGTH+1)
			movw r10,Z
			clr r12
			clr r13
			call div32u
			; ----------
			sts (AVERAGE_U+0),r6	; �������� �������������������� �������� U RMS 
			sts (AVERAGE_U+1),r7	; 
				; ----------
; ---------------------------------------------------------------------------------
; � ��� �������� ����� �� RMS ����������
OOS_U:			lds r16,(MODE)
				cpi r16,0
				brne modeCheck1
				rjmp mode0_STAB			; ������������

modeCheck1:		cpi r16,1
				brne modeCheck2
				rjmp mode1_RAZG			; �� ������

modeCheck2:		cpi r16,2
				brne modeCheck3
				rjmp mode2_STOP			; �� ����

modeCheck3:		cpi r16,3
				brne modeCheck4
				rjmp mode2_STOP			; �� ����

modeCheck4:
			rjmp ZEROCROSS_END
; ---------- ������
mode1_RAZG:			cbi PORTB,5
					sbi PORTD,7			; ���� ��������� ������������
					ldi r16,$00
					sts (FL_RANGE),r16	; ���� ������������ �����
//					clr r8
//					clr r9
//					inc r8
					lds ZL,(OPTO_ON_TICK+0)
					lds ZH,(OPTO_ON_TICK+1)
				LDY 200
					sub ZL,YL
					sbc ZH,YH
					breq mode1RAZG_M0
					brcc mode1RAZG_M1
mode1RAZG_M0:		clr ZL
					clr ZH
					adiw Z,1
mode1RAZG_M1:		movw r8,Z
					rjmp SET_OPTO_ON_TICK
; ---------- ����
mode2_STOP:			sbi PORTB,5
					sbi PORTD,7			; ���� ��������� ������������
					ldi r16,$00
					sts (FL_RANGE),r16	; ���� ������������ �����
					ldi r16, low(DEFAULT_OFF_VALUE)
					ldi r17,high(DEFAULT_OFF_VALUE)
					LDY 200
					lds r8,(OPTO_ON_TICK+0)
					lds r9,(OPTO_ON_TICK+1)
					add r8,YL
					adc r9,YH
					cp  r8,r16
					cpc r9,r17
					brcs mode2_CONT
					movw r8,r16
mode2_CONT:			rjmp SET_OPTO_ON_TICK
; ----------
mode0_STAB:			sbi PORTB,5
					lds r6,(CURRENT_U+0)		; �� �������� ����������
					lds r7,(CURRENT_U+1)
					lds r8,(TARGET_U+0)
					lds r9,(TARGET_U+1)
					sub r6,r8					; delta=CURRENT_U-target_u ([r7..6] = current - target)
					sbc r7,r9

					brcs DeltaMinus
					LDY 200
					rjmp DeltaPlus

DeltaMinus:			LDY -200
					cp r6,YL
					cpc r7,YH
					brcc mode0_CONT
					movw r6,Y
					rjmp mode0_CONT

DeltaPlus:			cp  r6,YL
					cpc r7,YH
					brcs mode0_CONT
					movw r6,Y
					rjmp mode0_CONT

					clc
					sbrc r7,7
					sec
					ror r7
					ror r6						; delta = delta/2

mode0_CONT:			lds r8,(OPTO_ON_TICK+0)
					lds r9,(OPTO_ON_TICK+1)
					add r8,r6					; opto_on_tick = opto_on_tick + delta
					adc r9,r7					; ����� �������� ������� ��������� ���������

			; �������� �� ������� ������ �� ���������� ��������
			; �� 1 �� DEFAULT_OFF_VALUE

					sbrs r9,7					; �������, ���� ��������������� (0000...7FFF)
					rjmp N1

			; ������������� �������� - ��������� ������������� �� ������

OOS_MAX_U:	ldi r16,1
			ldi r17,0
			movw r8,r16					; ����� OPTO_ON_TICK = 1 (������������ ����������)

			lds r17,(FL_MAX_U)
			ldi r16,$FF					; � ������������� ���� 
			sts (FL_MAX_U),r16			; ������������� ���������� �� ������

			cpse r16,r17
			rjmp SET_OPTO_ON_TICK
			
			//ldi r16,outputV
			//call setNewValFlag						;
										;
			rjmp SET_OPTO_ON_TICK

			; 
N1:			ldi r16, low(DEFAULT_OFF_VALUE)
			ldi r17,high(DEFAULT_OFF_VALUE)
			cp  r8,r16
			cpc r9,r17
			brcs N2						; �������, ���� ������ DEFAULT_OFF_VALUE

			; ������������ ����������� �������� 0
			movw r8,r16					; ����� OPTO_ON_TICK = DEFAULT_OFF_VALUE
;	clr r16						; � TARGET_U = 0
;	sts (TARGET_U+0),r16
;	sts (TARGET_U+1),r16
			; �������� ���������

			; ���������� ��������
N2:			lds r16,(FL_MAX_U)
			tst r16
			breq SET_OPTO_ON_TICK

			clr r16						; ���������� ���� 
			sts (FL_MAX_U),r16			; ������������� ���������� �� ������
			; ��������� ������� �� ����������

SET_OPTO_ON_TICK:	sts (OPTO_ON_TICK+0),r8
					sts (OPTO_ON_TICK+1),r9

; ---------------------------------------------------------------------------------
; ---------------------------------------------------------------------------------
			; ��������� ��������� ������������ ��������� ���������� � ���������� �������
CHECK_RANGE:		lds r16,(MODE)
					cpi r16,0
					breq CHECK_RANGE_RUN
					rjmp RANGE_END

CHECK_RANGE_RUN:	lds ZL,(AVERAGE_U+0)
					lds ZH,(AVERAGE_U+1)
					lds r16,(TARGET_U+0)
					lds r17,(TARGET_U+1)
					sub ZL,r16
					sbc ZH,r17
					brcc POSITIVE_DELTA
					com ZL
					com ZH
					adiw Z,1
POSITIVE_DELTA:		ldi r16, low(3)			; +- 0.3 V
					ldi r17,high(3)			;
					cp  ZL,r16
					cpc ZH,r17
					brcc NOT_RANGE

					clr r16
					sts (COUNTER_D),r16
					cbi PORTD,7				; ��� ��������� ������������ � ���������
					lds r17,(FL_RANGE)
					ldi r16,$FF
					sts (FL_RANGE),r16		; ���� ������������ ����������	 
					rjmp RANGE_EX

NOT_RANGE:			lds r16,(COUNTER_D)
					cpi r16,25
					brcc NOT_RANGE_A
					inc r16
					sts (COUNTER_D),r16
					rjmp RANGE_END

NOT_RANGE_A:		sbi PORTD,7				; ���� ��������� ������������
					lds r17,(FL_RANGE)
					ldi r16,$00
					sts (FL_RANGE),r16		; ���� ������������ �����

RANGE_EX:			cpse r16,r17
					rjmp RANGE_END

					ldi r16,Range
					call setNewValFlag
RANGE_END:
					; ----------
ZEROCROSS_END:
	
					; ----------
					POP_ALL_LOW
					; ----------
					pop YL
					pop YH
					pop ZL
					pop ZH
					pop r20
					; ----------
ZEROCROSS_E:		pop r19
					pop r18
					pop r17
					pop r16
					POPF
				reti
; ----------------------------------------------------------------------------------------
