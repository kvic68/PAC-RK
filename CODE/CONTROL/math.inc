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

						ldi r18,0							;	������� ������� �����
						ldi r19,9							;	������� ����� �� 1 ������, ��� ����� �����, ����� �� ������ ��������� ������
						LDY DIGIT9							;	��������� �� ������� �����

HideZeroesCycle:			lds r16,(DECIMAL_POS)
							cp r19,r16
							breq HideZeroesExit
							brcs HideZeroesExit
							ld r16,Y						;	������ �����
							cpi r16,'0'						;	����������
							brne HideZeroesExit				;	���� �� 0, �� ��������� ������� ������� ������� �����
								ldi r16,$20					;	�����, ����� ���������� �������� $20(������������ ������) ��� ������������ $00(�� ������)
								st Y,r16
								sbiw Y,1					;	��������� ���������
								inc r18						;	����������� ������� ������� ��������
								//
								dec r19						;	��������� �������
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
;����: R16  ����� �� 0 ... 255 
;�����: R18 ����� 2, R17 ����� 1, R16 ����� 0  = digits 
;������: 20 

;������ �� ���� ldi r16,255
;����� r18 = 00110010 ��� 2, r17 = 00110101 ��� 1, r16 = 00110101 ��� 0.                
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
; ����������� HEX � ��� ASCII
; R16 - �������, �����: R17 - �������, R16 - �������
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
; ������ ������� ���� A=F(A,B)
; A - �������� R25...22
; B - �������� R21...18
; R3...0 - ��������������� ��������
; R17 - �������
; ---------------- ��������� 24*8 ----------------
; �������: R24:R22 - ��������, R18 - ���������
; �����  : R25:R22 ������������ 
; ��������������� R1:R0
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
; ---------------- ��������� 24*16 ----------------
; �������: R24:R22 - ��������, R19:R18 - ���������
; �����  : R25:R22 ������������ 
; ��������������� R5:R0
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

; ---------------- ����������� ������� 32/32 ----------------
DIV3232UNR:				set
						rjmp DIV_AB_U
DIV3232U:				;clt
DIV_AB_U: 
; A=A/B
; �������: A = R25...22 - �������, B = R21...18 - ��������
; �����  :     R25...22 - �������, R3...0 - �������, R21...18 - ��������
; R17 - ������� ��������
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
						; �������  � R25...22 
						; �������  � R3...0
						; �������� � R21...18
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
							brcc DIV3232U4	; ���� ������� ������, ��� �������� ��������, �� ������ �� ������
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
; ---------------- �������� ������� 32/32 ----------------
DIV3232S:
div32ABs:
; A=A/B
; �������: A = R25...22 - �������, B = R21...18 - ��������
; �����  :     R25...22 - �������, R3...0 - ������ �������, R21...18 - ������ ��������
						eor  r25,r21 	;���������� �������� ���� �������� � �������� 
						bst  r25,7   	;���������� �� ����� T ���� �������� ��������
						eor  r25,r21  	;��������������� R25 
						sbrc R25,7    	;���� ������� A < 0,
						call NEG_A 		;�� �������� ���� A = -A
						sbrc R21,7    	;���� �������� B < 0,
						call NEG_B  	;�� �������� ���� B = -B
						call DIV3232U	;���������� ����� A/B
						brtc DIV3232S1	;���� ��������� ������ ���� �������������
						call NEG_A		;������ ��� ����
DIV3232S1:				ret
; ---------------- ����������� ��������� 16�16 ----------------
; A=A*B (0...65535)
; �������: A = R23...22, B = R19...18
; �����  : A(R25...22) = A*B, B(R21...18) = 256*A + B
; R0...2 - ���������������
MUL1616U:				movw r20,r22	; R21=AH, R20=AL, R19=BH, R18=BL
						mul  r20,r18  	; ������� AL*BL = R20*R18 � ������� ��� �
     					movw r22,R0   	; ������� ����� ������������ R23*R22
     					mul  r21,r19  	; ������� AH*BH = R21*R19 � ������� ��� �
     					movw r24,r0   	; ������� ����� ������������ R25*R24

     					mul  r21,r18  	; ������� AH*BL = R21*R18 � ���������� ��� �
     					clr  r2	    	; ������ R25:R24:R23 ������������
     					add  r23,r0   
     					adc  r24,r1    
     					adc  r25,r2   

     					mul  r19,r20  	;������� BH*AL = R19*R20 � ���������� ��� � 
     					clr  r2
     					add  r23,r0   	;������ R25:R24:R23 ������������ 
     					adc  r24,r1
     					adc  r25,r2  
						ret
; ---------------- �������� ��������� 16�16 ----------------
; A=A*B (-32768...32767)
; �������: A = R23...22, B = R19...18
; �����  : A(R25...22) = A*B, B(R21...18) = 256*A + B
; R0...2 - ���������������
MUL1616S:				movw  r20,r22	; R21=AH, R20=AL, R19=BH, R18=BL
						mul   r20,r18  	; ������� ALU*BLU = R20*R18 � ������� ��� �
     					movw  r22,R0   	; ������� ����� ������������ R23*R22
     					muls  r21,r19  	; ������� AHS*BHS = R21*R19 � ������� ��� �
     					movw  r24,r0   	; ������� ����� ������������ R25*R24

     					mulsu r21,r18  ; ������� AHS*BLU = R21*R18 � ���������� ��� �
     					clr   r17	    ; ������ R25:R24:R23 ������������
						sbci  r17,0
     					add   r23,r0   
     					adc   r24,r1    
     					adc   r25,r17   

     					mulsu r19,r20  	;������� BHS*ALU = R19*R20 � ���������� ��� � 
     					clr   r17
						sbci  r17,0
     					add   r23,r0   	;������ R25:R24:R23 ������������ 
     					adc   r24,r1
     					adc   r25,r17  
						ret
; ---------------- ����� A ������ �� N ������� (N=R17) ----------------
; ���� A (R25...22), R17 - ����� �������
; ����� A(R25...22) ��� ���������. A=A/2^N
R_SHIFT_A:				andi r17,31		; ���� ���������� �������
R_SHIFT_A1:				tst r17
						breq R_SHIFT_A_EXIT
						dec r17
						lsr r25
						ror r24
						ror r23
						ror r22
						rjmp R_SHIFT_A1
R_SHIFT_A_EXIT:			ret
; ---------------- ����� A ����� �� N ������� (N=R17) ----------------
; ����  A(R25...22), R17 - ����� �������
; ����� A(R25...22) ��� ���������. A=A*2^N
L_SHIFT_A:				andi r17,31		; ���� ���������� �������
L_SHIFT_A1:				tst r17
						breq L_SHIFT_A_EXIT
						dec r17
						lsl r22
						rol r23
						rol r24
						rol r25
						rjmp L_SHIFT_A1
L_SHIFT_A_EXIT:			ret
; ---------------- ����� B ������ �� N ������� (N=R17) ----------------
; ���� B (R25...22), R17 - ����� �������
; ����� B(R25...22) ��� ���������. B=B/2^N
R_SHIFT_B:				andi r17,31		; ���� ���������� �������
R_SHIFT_B1:				tst r17
						breq R_SHIFT_B_EXIT
						dec r17
						lsr r25
						ror r24
						ror r23
						ror r22
						rjmp R_SHIFT_B1
R_SHIFT_B_EXIT:			ret
; ---------------- ��������  32+32 A=A+B ----------------
ADD3232:				add r22,r18
						adc r23,r19
						adc r24,r20
						adc r25,r21
						ret
; ---------------- ��������� 32-32 A=A-B ----------------
SUB3232:				sub	r22,r18
						sbc r23,r19
						sbc r24,r20
						sbc r25,r21
						ret
; ---------------- ��������� ����� A ----------------
NEG_A:					com r22
						com r23
						com r24
						com r25
						subi r22,(-1)
						sbci r23,(-1)
						sbci r24,(-1)
						sbci r25,(-1)
						ret
; ---------------- ��������� ����� B ----------------
NEG_B:					com r18
						com r19
						com r20
						com r21
						subi r18,(-1)
						sbci r19,(-1)
						sbci r20,(-1)
						sbci r21,(-1)
						ret
; ---------------- ��������� � � �  ----------------
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

