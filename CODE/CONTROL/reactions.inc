//==================================================================================================

refreshControl:
		PUSHF
		PUSHXYZ
		PUSHLOW
		push r16
		push r17
		push r18
; ----------
			ldi r16,0						; ������� �������� (����� �������)
			mov r10,r16						; ������� �������� �� ��������� �����
			mov r11,r16						; ������� �������� �� ���� ������
			mov r12,r16						; ������� �������� �� ���������� �������
			ldi XH,0						; ������� �������� �� �����
			ldi XL,0						; ������� �������� �� ���� ���������
			lds r0,(DS_STATUS)				; ��������� ������ ������������� 0 - �����������, 1 - ��������
			clr r1
			inc r1							; r1 - ����� = 0000.0001 ��� ��������� ����������	

			LDZ DS_VALUE_0					; Z - ��������� �� �������� ������������� (�� 2 �����), DS_VALUE(x)
											; Z+8  - ��������� ��������								DS_MARGIN(x)

			LDY DELAY_UP_0					; Y    - �������� ���������							DELAY_UP(x)
											; Y+4  - �������� ����������						DELAY_DN(x)
											; Y+8  - ��� ������� �� �������						REACTION(x)
											; Y+12 - �������� ������� �� ������������ �������	BACK_TIMER_T(x)

											; Y+16 - ������ ��� ������ �� ���������

	
			lds r17,(DRIVE)				; ��������� ������� � R17
			andi r17,$0F					; ������ ������� 4 �������
			; ----------
			; �������� ���� ������
main_loop:	cpi r16,4
			brcs load_data
			rjmp main_exit					; ����� �� ��������� �������� ���� ��������
load_data:		;inc r16						; ����������� ������� ��������� �����
				; ��������� ������ ��� ������� ������� 
				ldd r9,Y+12					; �������� ������
				ldd r8,Y+8					; ��� �������
				ldd r7,Y+4					; �������� ����
				ld  r6,Y					; �������� ���
				; ----------
				ldd r2,Z+8					; BOUNDx LSB
				ld  r4,Z+					; DS18Tx LSB
				ldd r3,Z+8					; BOUNDx MSB
				ld  r5,Z+					; DS18Tx MSB
				; r5:r4 - ������� �����������, r3:r2 - ��������� �����������
				; ----------
				ror r0						; � - ��������� ������� 0 - ��������, 1 - �������
				brcs main_work				; ���� ������ �������� - ��������� � ���������
					// ����� -- �������� ������
							push r1					;	���� ����� �� ���������� ���������
							and r1,r17				;	������������ � �������� �������
							pop r1					;	�������� ������������ � 
							brne main_work			;	������ �����
				rjmp next_step				; ����� - � ���������� ������� ��� ��������� ��������� 
				; ----------
main_work:			; ���� ������ ���������
					sbrs r8,3
					rjmp noReaction				; �� ���������� ����� � ��������� "���������"
					rjmp compareTB				; ����� ��������� � ��������� ����������� � ������
					; ----------
noReaction:				// ����� -- ������� ���������
setBitOff:			push r1						; ��������� ����
					com r1
					and r17,r1	
					pop r1
					push r16
					push r17
						ldi r17,MARGIN0
						add r16,r17
						call clrInverseFlag		; ��������� ��������� ����� ��������
						call setNewValFlag
					pop r17
					pop r16
					rjmp next_step
					; ----------
setBitOn:			or r17,r1					; �������� ����
					push r16
					push r17
						ldi r17,MARGIN0
						add r16,r17
						call setInverseFlag		; �������� ��������� ����� ��������
						call setNewValFlag
					pop r17
					pop r16
					rjmp next_step
				; ------------------------------
compareTB:			cp  r4,r2
					cpc r5,r3					; ������� ��������� �������� � ������� �����������, C=1 ��� �<������, �=0 ��� �>=������
					brlt belowBound				; � ���� �������
					rjmp aboveBound				; � ���� ��� ����� �������

; ----------------------------------------------

				; -- � ���� ��� ����� ������ ������������ --
belowBound:			push r1
					and r1,r17					; �������� ��������� ������ 0 - ���� (breq), 1 - ���(brne)
					pop r1

					brne bePinIsOn_m1				; �������, ���� ����������� ����� �� ���������� ���������
					rjmp bePinIsOff_m1				; �������, ���� ����������� ����� � ����������� ���������

bePinIsOn_m1:		tst r9						; �������� ������ = 0 ?
					breq beDelayIs0_m1				; �� - ��������� ������
					dec r9						; ��� - ��������� �������� ������� �� 1
					std Y+12,r9					; ���������
					// �� ����� - �������� ��������� �������
					push r16
					push r17
						ldi r17,BACK_T0
						add r16,r17
						call setNewValFlag		; ���� ������ �������� ��������� �������� ��� ������ �� �����
					pop r17
					pop r16
					//
					sbrc r8,0					;
					inc r10					; �������� ������ �� ��������� �����
					sbrc r8,1					;
					inc r11					; �������� ������ �� ���� ������
					sbrc r8,2					;
					inc r12					; �������� ������ �� ���������� �������
bePinIsOn_m2:			
					rjmp next_step				; � ��������� � ���������� �������

beDelayIs0_m1:		; ���� �������� ���������� ���� �������� ��� �������� ������, r18 - ��� ��������
				; ---------------------------------------------------------------------------------------
					push r16
					push r17
						ldi r17,MARGIN0
						add r16,r17
						call clrInverseFlag		; ��������� ��������� ����� ��������
						call setNewValFlag
					pop r17
					pop r16
				; ---------------------------------------------------------------------------------------
					std Y+12,r6					; �������� ������ = �������� ���������
						// �� ����� - ������� �����
					rjmp setBitOff				; ����������� ����� ���������
					;---------------------------------------------------------
					;---------------------------------------------------------
	
bePinIsOff_m1:		std Y+12,r6					; �������� ������ = �������� ���������
						// �� ����� - ������� �����
					rjmp next_step				; � ��������� � ���������� �������

; ----------------------------------------------

				; -- T ���� ������ ������������ --
aboveBound:			push r1
					and r1,r17					; �������� ��������� ������ 0 - ���� (breq), 1 - ���(brne)
					pop r1
					
					brne abPinIsOn_m1				; �������, ���� ����������� ����� �� ���������� ���������
					rjmp abPinIsOff_m1				; �������, ���� ����������� ����� � ����������� ���������

abPinIsOn_m1:		std Y+12,r7					; �������� ������ = �������� ����������
						// ����� - ������� ����
					sbrc r8,0					;
					inc r10					; �������� ������ �� ��������� �����
					sbrc r8,1					;
					inc r11					; �������� ������ �� ���� ������
					sbrc r8,2					;
					inc r12					; �������� ������ �� ���������� �������

abPinIsOn_m2:
					rjmp next_step				; � ��������� � ���������� �������
					;-------------------------------------------------------

abPinIsOff_m1:		tst r9						; �������� ������ = 0 ?
					breq abDelayIs0_m1				; �� - ��������� ������
					dec r9						; ��� - ��������� �������� ������� �� 1
					std Y+12,r9					; ���������
					// �� ����� - �������� �������
					push r16
					push r17
						ldi r17,BACK_T0
						add r16,r17
						call setNewValFlag		; ���� ������ �������� ��������� �������� ��� ������ �� �����
					pop r17
					pop r16
					//
abPinIsOff_m2:		rjmp next_step				; � ��������� � ���������� �������

abDelayIs0_m1:		push r16
					push r17
						ldi r17,MARGIN0
						add r16,r17
						call setInverseFlag
						call setNewValFlag
					pop r17
					pop r16
					//--------------------
					sbrc r8,0					;
					inc r10					; �������� ������ �� ��������� �����
					sbrc r8,1					;
					inc r11					; �������� ������ �� ���� ������
					sbrc r8,2					;
					inc r12					; �������� ������ �� ���������� �������

				; ---------------------------------------------------------------------------------------
abDelayIs0_m1a:		std Y+12,r7					; �������� ������ = �������� ����������
					// ����� - ������� ����
					rjmp setBitOn				; ����������� ����� ��������
					;---------------------------------------------------------
					;-------------------------------------------------------

; --------------------

next_step:			clc
					rol r1						; �������� ����� �����
					adiw Y,1					; ���������� ��������� ������
					inc r16						; ����������� ������� ��������� �����
					rjmp main_loop

; --------------------

main_exit:			sts (DRIVE),r17
					in r16,pinb
					andi r16,$F0
					com r17
					andi r17,$0F
					or r16,r17
					mov r17,r16
					out portb,r17				; �������� ����� �����
					;----------------------

					lds r16,(SOUNDNUMBER)
					tst r10						; ����� �� ����?
					breq me_no_alarm_sound
					ldi r16,$01					; ���� �����
					rjmp me_sv_alarm_sound
me_no_alarm_sound:	cpi r16,1
					brne me_sv_alarm_sound
					clr r16
me_sv_alarm_sound:	sts (SOUNDNUMBER),r16
					


					lds r16,(A_FLAGS)
					CLRB r16,2
					lds r17,(STABMODE)
					cpi r17,3
					breq me_stop
					tst r11						; ����� �� ���� ������?
					breq me_nostop

me_stop:			SETB r16,2					; ���� �����
me_nostop:			sts (A_FLAGS),r16

					tst r12						; ����� �� ���������� �������?
					breq me_heat_on
					lds r17,(STABMODE)
					cpi r17,3
					breq me_heat_on
					push r16					; ������ ���������
					ldi r16,$03
					LDY DataToStab
					std Y+2,r16
					ldi r16,$FF
					std Y+0,r16
					std Y+1,r16
					lds r16,(KEY_FLAGS)
					SETB r16,5
					sts (KEY_FLAGS),r16
					pop r16
					rjmp me_end
me_heat_on:						
; --------------------
; ��������� � ��������� ������ ��� �������������
					lds r16,(HeatControlStatus)
					cpi r16,$00
					brne me_chk1
					rjmp me_end					; ���� �������� �� �������� ��������, �� �� �����

me_chk1:			lds r16,(HeatControlSensor)
					clr r17
					LDZ DS_VALUE
					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17
					ldd YL,Z+0
					ldd YH,Z+1					; � Y - ����������� ��������������� �������
					ldi r16,$00
					ldi r17,$80
					cp  r16,YL
					cpc r17,YH
					brne me_chk2
					rjmp me_end					; ���� �������������� ������ ����������, �� �� �����

me_chk2:			LDZ HeatControlMargin
					ldd XL,Z+0
					ldd XH,Z+1					; � X ����� ���������� �������
					cp  YL,XL
					cpc YH,XH
					brcc me_chk3
					rjmp me_end

me_chk3:			lds r16,(STABMODE)
					cpi r16,$01
					breq me_chk4
					rjmp me_end

me_chk4:			lds r16,(HeatControlCounter)
					cpi r16,3
					//breq me_stop_fire
					brcc me_stop_fire
					inc r16
					sts (HeatControlCounter),r16
					rjmp me_exit

me_stop_fire:		ldi r16,$00
					LDY DataToStab
					std Y+2,r16
					ldi r16,$FF
					std Y+0,r16
					std Y+1,r16
					lds r16,(KEY_FLAGS)
					SETB r16,5
					sts (KEY_FLAGS),r16
		
; --------------------
me_end:			ldi r16,$00
				sts (HeatControlCounter),r16
me_exit:	pop r18
			pop r17
			pop r16
			POPLOW
			POPXYZ
			POPF
	ret
; ----------

//==================================================================================================

