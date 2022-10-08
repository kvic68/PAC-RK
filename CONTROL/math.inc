; -------------------------------------------------
;***** Subroutine Register Variables

.equ	AtBCD0	=10		;address of tBCD0
.equ	AtBCD2	=14		;address of tBCD3

.def	tBCD0	=r10		;BCD value digits 1 and 0
.def	tBCD1	=r11		;BCD value digits 3 and 2
.def	tBCD2	=r12		;BCD value digit 4 5
.def	tBCD3	=r13		;BCD value digit 6 7
.def	tBCD4	=r14		;BCD value digit 8 9

.def	fbinB0	=r22		;binary value byte 0
.def	fbinB1	=r23		;binary value byte 1
.def	fbinB2	=r24		;binary value byte 2
.def	fbinB3	=r25		;binary value byte 3

.def	cnt16a	=r20		;loop counter
.def	tmp16a	=r21		;temporary value

;***** Code

A32toBCD9:
	PUSHF
	push cnt16a
	push tmp16a
	push fbinB0
	push fbinB1
	push fbinB2
	push fbinB3
	push ZH
	push ZL
	sbrc fBinB3,7
	call NEG_A
	ldi	cnt16a,32	;Init loop counter	
	clr	tBCD4		;clear result (5 bytes)
	clr	tBCD3		;clear result (4 bytes)
	clr	tBCD2		;clear result (4 bytes)
	clr	tBCD1		
	clr	tBCD0		
	clr	ZH		;clear ZH (not needed for AT90Sxx0x)
bBCDx_1:
	lsl	fbinB0		;shift input value
	rol	fbinB1		;through all bytes
	rol	fbinB2		;through all bytes
	rol	fbinB3		;through all bytes
	rol	tBCD0		;
	rol	tBCD1
	rol	tBCD2
	rol	tBCD3
	rol	tBCD4
	dec	cnt16a		;decrement loop counter
	breq bBCDx_4		;if counter not zero
;	rjmp bBCDx_2
bBCDx_2:
	ldi		r30,AtBCD2+1	;Z points to result MSB + 1
bBCDx_3:
	ld		tmp16a,-Z	;get (Z) with pre-decrement
	subi	tmp16a,-$03	;add 0x03
	sbrc	tmp16a,3	;if bit 3 not clear
	st		Z,tmp16a	;	store back
	ld		tmp16a,Z	;get (Z)
	subi	tmp16a,-$30	;add 0x30
	sbrc	tmp16a,7	;if bit 7 not clear
	st		Z,tmp16a	;	store back
	cpi		ZL,AtBCD0	;done all three?
	brne	bBCDx_3		;loop again if not
	rjmp	bBCDx_1		
bBCDx_4:
		mov  tmp16a,tBCD0
		andi tmp16a,$0F
		ori  tmp16a,$30
		sts (DIGIT0),tmp16a
		mov  tmp16a,tBCD0
		swap tmp16a
		andi tmp16a,$0F
		ori  tmp16a,$30
		sts (DIGIT1),tmp16a

		mov  tmp16a,tBCD1
		andi tmp16a,$0F
		ori  tmp16a,$30
		sts (DIGIT2),tmp16a
		mov  tmp16a,tBCD1
		swap tmp16a
		andi tmp16a,$0F
		ori  tmp16a,$30
		sts (DIGIT3),tmp16a

		mov  tmp16a,tBCD2
		andi tmp16a,$0F
		ori  tmp16a,$30
		sts (DIGIT4),tmp16a
		mov  tmp16a,tBCD2
		swap tmp16a
		andi tmp16a,$0F
		ori  tmp16a,$30
		sts (DIGIT5),tmp16a

		mov  tmp16a,tBCD3
		andi tmp16a,$0F
		ori  tmp16a,$30
		sts (DIGIT6),tmp16a
		mov  tmp16a,tBCD3
		swap tmp16a
		andi tmp16a,$0F
		ori  tmp16a,$30
		sts (DIGIT7),tmp16a

		mov  tmp16a,tBCD4
		andi tmp16a,$0F
		ori  tmp16a,$30
		sts (DIGIT8),tmp16a

		mov  tmp16a,tBCD4
		swap tmp16a
		andi tmp16a,$0F
		ori  tmp16a,$30
		sts (DIGIT9),tmp16a

	pop ZL
	pop ZH
	pop fbinB3
	pop fbinB2
	pop fbinB1
	pop fbinB0
	pop tmp16a
	pop cnt16a
	POPF
	ret			;   return
; ****************************************************************
; -------------------------------------------------
HideZeroes:				push r16
						push r17
						PUSHB
						PUSHY

						ldi r18,0							;	счетчик скрытых нулей
						ldi r19,9							;	счетчик цикла на 1 меньше, чем длина числа, чтобы не гасить последний разряд
						LDY DIGIT9							;	указатель на старшую цифру

HideZeroesCycle:			lds r16,(DECIMAL_POS)
							cp r19,r16
							breq HideZeroesExit
							brcs HideZeroesExit
							ld r16,Y						;	читаем цифру
							cpi r16,'0'						;	сравниваем
							brne HideZeroesExit				;	если не 0, то завершаем процесс скрытия ведущих нулей
								ldi r16,$20					;	иначе, гасим знакоместо пробелом $20(выравнивание вправо) или полупробелом $00(по центру)
								st Y,r16
								sbiw Y,1					;	уменьшаем указатель
								inc r18						;	увеличиваем счетчик скрытых разрядов
								//
								dec r19						;	уменьшаем счетчик
								brne HideZeroesCycle

HideZeroesExit:			sts (HidedZeroesCount),r18
						clr r16
						sts (DECIMAL_POS),r16
						POPY
						POPB
						pop r17
						pop r16
					ret
; -------------------------------------------------
; ****************************************************************
;вход: R16  число от 0 ... 255 
;выход: R18 число 2, R17 число 1, R16 число 0  = digits 
;байтов: 20 

;пример на вход ldi r16,255
;выход r18 = 00110010 это 2, r17 = 00110101 это 1, r16 = 00110101 это 0.                
HexToDec:	    LDI r18,-1+'0' 
bcd1:           INC r18
                SUBI r16,100 
                BRCC bcd1 
                LDI r17,10+'0' 
bcd2:           DEC r17 
                SUBI r16,-10 
                BRCS bcd2 
                SBCI r16,-'0' 
                RET
; ----------------------------------------------------------------------------------------
; Превращение HEX в два ASCII
; R16 - входное, выход: R17 - старший, R16 - младший
HexToAscii:	PUSHF
				mov r17,r16
				andi r16,$0F
				andi r17,$F0
				swap r17
				subi r16,(-$30)
				cpi r16,$3A
				brcs HTD_1
				subi r16,(-7)
HTD_1:			subi r17,(-$30)
				cpi r17,$3A
				brcs HTD_2
				subi r17,(-7)
HTD_2:
				POPF
				ret
; ---------------------------------------------------------------------
; ---------------- ----------------
; Всякие функции вида A=F(A,B)
; A - регистры R25...22
; B - регистры R21...18
; R3...0 - вспомогательные регистры
; R17 - счетчик
; ---------------- умножение 24*8 ----------------
; входные: R24:R22 - множимое, R18 - множитель
; выход  : R25:R22 произведение 
; вспомогательные R1:R0
MUL24x8U:				push r23
						mul  r22,r18
						movw r22,r0
						mul  r24,r18
						movw r24,r0
						pop  r0
						mul  r0,r18
						add r23,r0
						adc r24,r1
						clr r0
						adc r25,r0
						ret
; ---------------- умножение 24*16 ----------------
; входные: R24:R22 - множимое, R19:R18 - множитель
; выход  : R25:R22 произведение 
; вспомогательные R5:R0
MUL24x16U:		mul r22,r18
				movw r2,r0
				mul r24,r18
				movw r4,r0
				mul r23,r18
				add r3,r0
				adc r4,r1
				clr r1
				adc r5,r1
				mul r22,r19
				add r3,r0
				adc r4,r1
				clr r1
				adc r5,r1
				mul r23,r19
				add r4,r0
				adc r5,r1
				mul r24,r19
				add r5,r0
				movw r22,r2
				movw r24,r4
				ret

; ---------------- беззнаковое деление 32/32 ----------------
DIV3232UNR:				set
						rjmp DIV_AB_U
DIV3232U:				;clt
DIV_AB_U: 
; A=A/B
; входные: A = R25...22 - делимое, B = R21...18 - делитель
; выход  :     R25...22 - частное, R3...0 - остаток, R21...18 - делитель
; R17 - счетчик итераций
                        clr  r0 
                        clr  r1 
                        clr  r2
                        clr  r3
                        ldi  r17,32 
DIV3232U1:              lsl  r22 
                        rol  r23
                        rol  r24
                        rol  r25 
                        rol  r0 
                        rol  r1 
                        rol  r2
                        rol  r3
                        sub  r0,r18 
                        sbc  r1,r19 
                        sbc  r2,r20
                        sbc  r3,r21
                        brcc DIV3232U2 
                        add  r0,r18 
                        adc  r1,r19 
                        adc  r2,r20
                        adc  r3,r21
                        rjmp DIV3232U3 
DIV3232U2:              sbr  r22,1 
DIV3232U3:              dec  r17 
                        brne DIV3232U1 
						; частное  в R25...22 
						; остаток  в R3...0
						; делитель в R21...18
						lsr r21
						ror r20 
						ror r19
						ror r18
						ror r17
						;brts DIV3232U4
							cp  r18,r0
							cpc r19,r1
							cpc r20,r2
							cpc r21,r3
							brcc DIV3232U4	; если остаток меньше, чем половина делителя, то ничего не делаем
							ldi r17,1
							add r22,r17
							ldi r17,0
							adc r23,r17
							adc r24,r17
							adc r25,r17
DIV3232U4:				rol r17
						rol r18
						rol r19
						rol r20
						rol r21
                        ret 
; ---------------- знаковое деление 32/32 ----------------
DIV3232S:
div32ABs:
; A=A/B
; входные: A = R25...22 - делимое, B = R21...18 - делитель
; выход  :     R25...22 - частное, R3...0 - модуль остатка, R21...18 - модуль делителя
						eor  r25,r21 	;сравниваем знаковые биты делимого и делителя 
						bst  r25,7   	;запоминаем во флаге T знак будущего частного
						eor  r25,r21  	;восстанавливаем R25 
						sbrc R25,7    	;если делимое A < 0,
						call NEG_A 		;то изменяем знак A = -A
						sbrc R21,7    	;если делитель B < 0,
						call NEG_B  	;то изменяем знак B = -B
						call DIV3232U	;беззнаково делим A/B
						brtc DIV3232S1	;если результат должен быть отрицательным
						call NEG_A		;меняем его знак
DIV3232S1:				ret
; ---------------- беззнаковое умножение 16х16 ----------------
; A=A*B (0...65535)
; входные: A = R23...22, B = R19...18
; выход  : A(R25...22) = A*B, B(R21...18) = 256*A + B
; R0...2 - вспомогательные
MUL1616U:				movw r20,r22	; R21=AH, R20=AL, R19=BH, R18=BL
						mul  r20,r18  	; находим AL*BL = R20*R18 и заносим его в
     					movw r22,R0   	; младшие байты произведения R23*R22
     					mul  r21,r19  	; находим AH*BH = R21*R19 и заносим его в
     					movw r24,r0   	; старшие байты произведения R25*R24

     					mul  r21,r18  	; находим AH*BL = R21*R18 и прибавляем его к
     					clr  r2	    	; байтам R25:R24:R23 произведения
     					add  r23,r0   
     					adc  r24,r1    
     					adc  r25,r2   

     					mul  r19,r20  	;находим BH*AL = R19*R20 и прибавляем его к 
     					clr  r2
     					add  r23,r0   	;байтам R25:R24:R23 произведения 
     					adc  r24,r1
     					adc  r25,r2  
						ret
; ---------------- знаковое умножение 16х16 ----------------
; A=A*B (-32768...32767)
; входные: A = R23...22, B = R19...18
; выход  : A(R25...22) = A*B, B(R21...18) = 256*A + B
; R0...2 - вспомогательные
MUL1616S:				movw  r20,r22	; R21=AH, R20=AL, R19=BH, R18=BL
						mul   r20,r18  	; находим ALU*BLU = R20*R18 и заносим его в
     					movw  r22,R0   	; младшие байты произведения R23*R22
     					muls  r21,r19  	; находим AHS*BHS = R21*R19 и заносим его в
     					movw  r24,r0   	; старшие байты произведения R25*R24

     					mulsu r21,r18  ; находим AHS*BLU = R21*R18 и прибавляем его к
     					clr   r17	    ; байтам R25:R24:R23 произведения
						sbci  r17,0
     					add   r23,r0   
     					adc   r24,r1    
     					adc   r25,r17   

     					mulsu r19,r20  	;находим BHS*ALU = R19*R20 и прибавляем его к 
     					clr   r17
						sbci  r17,0
     					add   r23,r0   	;байтам R25:R24:R23 произведения 
     					adc   r24,r1
     					adc   r25,r17  
						ret
; ---------------- сдвиг A вправо на N позиций (N=R17) ----------------
; Вход A (R25...22), R17 - число сдвигов
; Выход A(R25...22) уже сдвинутое. A=A/2^N
R_SHIFT_A:				andi r17,31		; макс количество сдвигов
R_SHIFT_A1:				tst r17
						breq R_SHIFT_A_EXIT
						dec r17
						lsr r25
						ror r24
						ror r23
						ror r22
						rjmp R_SHIFT_A1
R_SHIFT_A_EXIT:			ret
; ---------------- сдвиг A влево на N позиций (N=R17) ----------------
; Вход  A(R25...22), R17 - число сдвигов
; Выход A(R25...22) уже сдвинутое. A=A*2^N
L_SHIFT_A:				andi r17,31		; макс количество сдвигов
L_SHIFT_A1:				tst r17
						breq L_SHIFT_A_EXIT
						dec r17
						lsl r22
						rol r23
						rol r24
						rol r25
						rjmp L_SHIFT_A1
L_SHIFT_A_EXIT:			ret
; ---------------- сдвиг B вправо на N позиций (N=R17) ----------------
; Вход B (R25...22), R17 - число сдвигов
; Выход B(R25...22) уже сдвинутое. B=B/2^N
R_SHIFT_B:				andi r17,31		; макс количество сдвигов
R_SHIFT_B1:				tst r17
						breq R_SHIFT_B_EXIT
						dec r17
						lsr r25
						ror r24
						ror r23
						ror r22
						rjmp R_SHIFT_B1
R_SHIFT_B_EXIT:			ret
; ---------------- сложение  32+32 A=A+B ----------------
ADD3232:				add r22,r18
						adc r23,r19
						adc r24,r20
						adc r25,r21
						ret
; ---------------- вычитание 32-32 A=A-B ----------------
SUB3232:				sub	r22,r18
						sbc r23,r19
						sbc r24,r20
						sbc r25,r21
						ret
; ---------------- изменение знака A ----------------
NEG_A:					com r22
						com r23
						com r24
						com r25
						subi r22,(-1)
						sbci r23,(-1)
						sbci r24,(-1)
						sbci r25,(-1)
						ret
; ---------------- изменение знака B ----------------
NEG_B:					com r18
						com r19
						com r20
						com r21
						subi r18,(-1)
						sbci r19,(-1)
						sbci r20,(-1)
						sbci r21,(-1)
						ret
; ---------------- сравнение А с В  ----------------
CP_AB:					cp  r22,r18
						cpc r23,r19
						cpc r24,r20
						cpc r25,r21
						ret
; ---------------- ----------------
MOVW32_BA:				movw r18,r22
						movw r20,r24
						ret
; ---------------- ----------------
MOVW32_AB:				movw r22,r18
						movw r24,r20
						ret
//---------------------------------------------------------------------------------------------------------------------

