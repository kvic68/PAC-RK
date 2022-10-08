; ------------------------------------------------------------------------
; Умножение R7..R6 * R9..R8 = R13..12..11..10
MUL1616:		PUSHF
				push r15
				push r0
				push r1

				mul r7,r9
				movw r12,r0

				mul r6,r8
				movw r10,r0

				mul r6,r9
				clr r15
				add r11,r0
				adc r12,r1
				adc r13,r15

				mul r7,r8
				clr r15
				add r11,r0
				adc r12,r1
				adc r13,r15

				pop r1
				pop r0
				pop r15
				POPF
			ret
; ----------------------------------------------------------------------------------------
; Превращение HEX в два ASCII
; R16 - входное, выход: R17 - старший, R16 - младший
HexToAscii:		PUSHF
				mov r17,r16
				andi r16,$0F
				andi r17,$F0
				swap r17
				subi r16,(-$30)
				cpi r16,$3A
				brcs HTA_m1
				subi r16,(-7)
HTA_m1:			subi r17,(-$30)
				cpi r17,$3A
				brcs HTA_m2
				subi r17,(-7)
HTA_m2:
				POPF
				ret
; ----------------------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
; *****************************************************************************
; Ниже разные подпрограммы из простторов интернета
; *****************************************************************************
;-----------------------------------------------------------------------------:
; 32bit/32bit Unsigned Division
;
; Register Variables
;  Call:  var1[3:0] = dividend (0x00000000..0xffffffff)
;         var2[3:0] = divisor (0x00000001..0x7fffffff)
;         mod[3:0]  = <don't care>
;         r31        = <don't care> (high register must be allocated)
;
;  Result:var1[3:0] = var1[3:0] / var2[3:0]
;         var2[3:0] = <not changed>
;         mod[3:0]  = var1[3:0] % var2[3:0]
;         r31        = 0
;
; Size  = 26 words
; Clock = 549..677 cycles (+ret)
; Stack = 0 bytes
; --- делимое/результат
//.def r6	= r6	; младший разряд
//.def r7	= r7
//.def r8	= r8
//.def r9	= r9	;
; --- делитель
//.def r10	= r10	; младший разряд
//.def r11	= r11
//.def r12	= r12
//.def r13	= r13
; --- остаток
//.def mod0	= r0	; младший разряд
//.def mod1	= r1
//.def mod2	= r2
//.def mod3	= r3

//.def r31		= r31

div32u:		clr	r0		;initialize variables
			clr	r1		;  mod = 0;
			clr	r2		;  r31 = 32;
			clr	r3		;
			ldi	r31,32		;/
			;---- calcurating loop
			lsl	r6		;var1 = var1 << 1;
			rol	r7		;
			rol	r8		;
			rol	r9		;/
			rol	r0		;mod = mod << 1 + carry;
			rol	r1		;
			rol	r2		;
			rol	r3		;/
			cp	r0,r10	;if (mod => var2) {
			cpc	r1,r11	; mod -= var2; var1++;
			cpc	r2,r12	; }
			cpc	r3,r13	;
			brcs	PC+6		;
			inc	r6		;
			sub	r0,r10	;
			sbc	r1,r11	;
			sbc	r2,r12	;
			sbc	r3,r13	;/
			dec	r31		;if (--r31 > 0)
			brne	PC-19		; continue loop;
		ret
;-----------------------------------------------------------------------------:
; https://radiokot.ru/forum/viewtopic.php?p=779071#p779071
;****************************************************************
;  //объявление функции с одним параметром - без знаковое целое 4  байта
;  unsigned short isqrt( unsigned long ul) { 
;  // объявление переменной - без знаковое целое 4  байта;  
;  unsigned long sqr = 0;   
;  // объявление переменной - без знаковое целое 4  байта
;  unsigned long temp;     
;  // объявление переменной   - без знаковое целое 4  байта. 
;  //В переменную записывается значение 0x40000000
;  unsigned long mask = 0x40000000; 
;  do{         //  цикл с пост условием (проверка произойдет после выполнения тела цикла)
;    temp = sqr | mask;  // побитное "или"  между  sqr и mask, результат записывается в temp
;    sqr >>= 1;          // сдвиг переменной sqr на 1 бит вправо
;    if( temp <= ul ){   // условный оператор если temp меньше равно  ul
;      sqr |= mask;      //  побитное "или"  между  sqr и mask, результат записывается в sqr
;      ul -= temp;       //  вычисляется разница между  ul и temp результат записывается в ul
;    }
;  }while( mask >>= 2 ); // Сдвинуть mask  на два бита в право, результат записать в mask,  выполнять цикл пока mask > 0.
;  if( sqr < ul && sqr < 0xFFFF ) ++sqr; // округление результата  (если sqr меньше ul "и" sqr меньше 0xFFFF в sqr записывается sqr+1)
;  return (unsigned short)sqr;           // возвращение результата работы функции, тип переменной приводится к без знаковое целое 1  байт

;SquareRoot32to16x16:
; r23...20 - входное  значение
; r9...r8  - выходное значение
;Регистры общего назначения
.def		sqr0		= r8
.def		sqr1		= r9
.def		sqr2		= r10
.def		sqr3		= r11

.def		mask0		= r12
.def		mask1		= r13
.def		mask2		= r14
.def		mask3		= r15

.def		temp0 		= r16
.def		temp1 		= r17
.def		temp2 		= r18
.def		temp3 		= r19

.def		src0		= r20
.def		src1		= r21
.def		src2		= r22
.def		src3		= r23

sqr:
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
		;
		clr		mask0				;  unsigned long mask = 0x40000000; 
		clr		mask1
		clr		mask2
		ldi		temp0,	0x40
		mov		mask3,	temp0
		;
		clr		sqr0				;  unsigned long sqr = 0;   
		clr		sqr1
		clr		sqr2
		clr		sqr3
		
_sr32loop:
			movw	temp0,	sqr0		;temp = sqr | mask; 
			movw	temp2,	sqr2
			or   temp0, mask0
			or   temp1, mask1
			or   temp2, mask2
			or   temp3, mask3
			lsr		sqr3	;sqr >>= 1; 
			ror		sqr2
			ror		sqr1
			ror		sqr0
			;
			cp		src0,	temp0		;if( temp <= ul )
			cpc		src1,	temp1
			cpc		src2,	temp2
			cpc		src3,	temp3
			; Если С=0, то условие выполнено
			brcs	_sr32_skipif		; пропустить, если С=1
				;							{
				or		sqr0,	mask0	; sqr |= mask
				or		sqr1,	mask1
				or		sqr2,	mask2
				or		sqr3,	mask3
				;
				sub		src0,	temp0 	; ul -= temp; 
				sbc		src1,	temp1
				sbc		src2,	temp2
				sbc		src3,	temp3
				;							}
_sr32_skipif:
			lsr		mask3		;( mask >>= 2 )
			ror		mask2
			ror		mask1
			ror		mask0
			;
			lsr		mask3
			ror		mask2
			ror		mask1
			ror		mask0
			;
			mov		temp0,	mask0   ; mask=0?
			or		temp0,	mask1
			or		temp0,	mask2
			or		temp0,	mask3
			;
			brne	_sr32loop		;while( mask=0 )
		;
		;sts		OutputSqr,	sqr0
		;sts		(OutputSqr+1), sqr1
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
ret
; *********************************************************************
; -------------------------------------------------
bin2BCD16:

//10		;address of r10
//12		;address of r13

//r10		;BCD value digits 1 and 0
//r11		;BCD value digits 3 and 2
//r12		;BCD value digit 4 5
//r13		;BCD value digit 6 7

//r16		;binary value Low byte
//r17		;binary value High byte
//r18		;binary value VHigh byte

//r19		;loop counter
//r20		;temporary value

			PUSHF
			push r16
			push r17
			push r19
			push r20
			ldi	r19,16	;Init loop counter	
			clr	r12		;clear result (3 bytes)
			clr	r11		
			clr	r10		
			clr	ZH		;clear ZH (not needed for AT90Sxx0x)
bin2BCD16_1:
			lsl	r16		;shift input value
			rol	r17		;through all bytes
			rol	r10		;
			rol	r11
			rol	r12
			dec	r19		;decrement loop counter
			brne bin2BCD16_2		;if counter not zero
nop
			pop r20
			pop r19
			pop r17
			pop r16
			POPF
			ret			;   return

bin2BCD16_2:
			ldi		r30,12+1	;Z points to result MSB + 1
bin2BCD16_3:
			ld		r20,-Z	;get (Z) with pre-decrement
			subi	r20,-$03	;add 0x03
			sbrc	r20,3	;if bit 3 not clear
			st		Z,r20	;	store back
			ld		r20,Z	;get (Z)
			subi	r20,-$30	;add 0x30
			sbrc	r20,7	;if bit 7 not clear
			st		Z,r20	;	store back
			cpi		ZL,10		;done all three?
			brne	bin2BCD16_3		;loop again if not
			rjmp	bin2BCD16_1		
; -------------------------------------------------

; -------------------------------------------------

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


A32toBCD9:
//	PUSHF
	push r20
	push tmp16a
	push fbinB0
	push fbinB1
	push fbinB2
	push fbinB3
	push ZH
	push ZL
	sbrc fBinB3,7
	call NEG_A
	ldi	r20,32	;Init loop counter	
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
//	POPF
	ret			;   return
; ****************************************************************
HideZeroes:	//PUSHF
			push r16
			push r17
			push r18
			//
			
			//
				clr r18
				ldi r17,'0'
					lds 	r16,(DIGIT9)
					cp 		r16,r17			; 
					brne PC+4				; 
						ldi r16,' '			; +1	ldi r16,$00
							inc r18
						rjmp PC+2			; +2
							ldi r17,' '		; +3	+1
						sts (DIGIT9),r16	; 		+2
					//----------
					lds 	r16,(DIGIT8)
					cp 		r16,r17	;
					brne PC+4		;
					ldi r16,' '	;
						inc r18
					rjmp PC+2		;
					ldi r17,' '		;
					sts (DIGIT8),r16
					//**********
					lds 	r16,(DIGIT7)
					cp 		r16,r17	;
					brne PC+4		;
						inc r18
					ldi r16,' '	;
					rjmp PC+2		;
					ldi r17,' '		;
					sts (DIGIT7),r16
					//----------
					lds 	r16,(DIGIT6)
					cp 		r16,r17	;
					brne PC+4		;
						inc r18
					ldi r16,' '	;
					rjmp PC+2		;
					ldi r17,' '		;
					sts (DIGIT6),r16
					//**********
					lds 	r16,(DIGIT5)
					cp 		r16,r17	;
					brne PC+4		;
						inc r18
					ldi r16,' '	;
					rjmp PC+2		;
					ldi r17,' '		;
					sts (DIGIT5),r16
					//----------
					lds 	r16,(DIGIT4)
					cp 		r16,r17	;
					brne PC+4		;
						inc r18
					ldi r16,' '	;
					rjmp PC+2		;
					ldi r17,' '		;
					sts (DIGIT4),r16
					//**********
					lds 	r16,(DIGIT3)
					cp 		r16,r17	;
					brne PC+4		;
						inc r18
					ldi r16,' '	;
					rjmp PC+2		;
					ldi r17,' '		;
					sts (DIGIT3),r16
					//----------
					lds 	r16,(DIGIT2)
					cp 		r16,r17	;
					brne PC+4		;
						inc r18
					ldi r16,' '	;
					rjmp PC+2		;
					ldi r17,' '		;
					sts (DIGIT2),r16
					//**********
					lds 	r16,(DIGIT1)
					//cp 		r16,r17	;
					//brne PC+4		;
					//	inc r18
					//ldi r16,' '	;
					//rjmp PC+2		;
					//ldi r17,' '		;
					sts (DIGIT1),r16
					//----------
					lds 	r16,(DIGIT0)
					sts (DIGIT0),r16
					//**********
					sts (HidedZeroesCount),r18	; количество ведущих не отображаемых нулей
			pop r18
			pop r17
			pop r16
			//POPF
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
