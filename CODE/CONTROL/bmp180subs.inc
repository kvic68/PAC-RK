.equ			oss	=	0
; ------------------------------------------
BMPCheck:		call i2c_start
				ldi r16,BMP180ADDR
				call i2c_send
				lds r16,(IIC_Status)
				cpi r16,$20				;	NACK на адрес записи
				ldi r16,$00
				brne BMPPresent
				ldi r16,$FF
BMPPresent:		sts (BMPPresence),r16
				call i2c_stop
				ret
; ------------------------------------------
BMP180_READ_D:
	push r16
	in r16,SREG
	push r16
	push r17
	push ZH
	push ZL
		; --- измеряем температуру ------
		call i2c_start
		ldi r16,BMP180ADDR
		call i2c_send
		//
		ldi r16,$F4
		call i2c_send
		ldi r16,$2E
		call i2c_send
		call i2c_stop
			; wait 5 ms
			ldi ZH,high(20000)
			ldi ZL,low(20000)
wait_5ms:	sbiw Z,1
			brne wait_5ms
		call i2c_start
		ldi r16,BMP180ADDR
		call i2c_send
		//
		ldi r16,$F6
		call i2c_send
		call i2c_start
		ldi r16,BMP180ADDR
		ori r16,$01
		call i2c_send
		call i2c_receive
			sts UT1H,r16
		call i2c_receive_last
			sts UT1L,r16
		call i2c_stop
		; --- измеряем давление ------
		call i2c_start
		ldi r16,BMP180ADDR
		call i2c_send
		ldi r16,$F4
		call i2c_send
		;ldi r16,$34		; oss=0	t= 4.5 ms max
		;ldi r16,$74		; oss=1	t= 7.5 ms max
		;ldi r16,$B4		; oss=2	t=13.5 ms max
		ldi r16,$F4			; oss=3	t=25.5 ms max
		call i2c_send
		call i2c_stop
			; wait 28 ms
			ldi ZH,high(30000)
			ldi ZL,low (30000)
wait_28ms:	sbiw Z,1
			adiw Z,1 sbiw Z,1
			adiw Z,1 sbiw Z,1
			adiw Z,1 sbiw Z,1
			brne wait_28ms
		call i2c_start
		ldi r16,BMP180ADDR
		call i2c_send
		ldi r16,$F6
		call i2c_send
		call i2c_start
		ldi r16,BMP180ADDR
		ori r16,$01
		call i2c_send

		call i2c_receive
			sts UP1H,r16
		call i2c_receive
			sts UP1L,r16
		call i2c_receive_last
			sts UP1E,r16

		call i2c_stop
	pop ZL
	pop ZH
	pop r17
	pop r16
	out SREG,r16
	pop r16
	ret
; ------------------------------------------
BMP180_READ_K:
	push r16
	in r16,SREG
	push r16
	push r17
	push ZH
	push ZL

		ldi ZH,HIGH(AC1)
		ldi ZL,LOW(AC1)
		ldi r17,21				; количество байт коэффициентов - 21

		call i2c_start
		ldi r16,BMP180ADDR		; BMP180 SLA
		call i2c_send
		ldi r16,$AA				; адрес начала блока расчётных коэффициентов
		call i2c_send
		call i2c_start
		ldi r16,BMP180ADDR+1
		call i2c_send

BMP180_READ_K_LOOP:
		call i2c_receive
			st Z+,r16
			dec r17
		brne BMP180_READ_K_LOOP

		call i2c_receive_last
			st Z+,r16
		call i2c_stop
	pop ZL
	pop ZH
	pop r17
	pop r16
	out SREG,r16
	pop r16
	ret

; ----------
shift_r20:
	; сдвиг вправо на ZH (деление на степень двойки) 
	; регистров R23...R20
sh20_loop:
	cpi ZH,0
	breq sh20_exit
	asr r23 
	ror r22
	ror r21
	ror r20
	dec ZH
	jmp sh20_loop
sh20_exit:
	adc r16,ZH
	adc r17,ZH
	adc r18,ZH
	adc r19,ZH
	ret
; ----------------------------------------------------------------------

BMP180_DECODE_NEW:
	push r0
	in r0,SREG
	push r0		push r1		push r2		push r3
	push r4		push r5		push r6		push r7
	push r8		push r9		push r10	push r11
	push r12	push r13	push r14	push r15
	push r16	push r17	push r18	push r19
	push r20	push r21	push r22	push r23
	push r24	push r25	push r26	push r27
	push r28	push r29	push r30	push r31
; X1=(UT-AC6)*AC5/2^15
			;LDS_A UT
			lds r22,UT1L
			lds r23,UT1H
			clr r24
			clr r25
			LDS_B AC6
				call SUB3232
			LDS_B AC5
				call MUL1616U
				ldi r17,15
				call R_SHIFT_A			; A = X1=4743
			SAVE_A_R4					; сохраним X1					
; X2=MC*2^11/(X1+MD)
			LDS_B MD
				call ADD3232
			COPY_A_B
			LDS_A MC
				ldi r17,11
				call L_SHIFT_A
				call DIV3232S			; A = X2=-2344
; B5=X1+X2
			LOAD_B_R4					; B = X1 (востановим)
				call ADD3232			; A = B5=2399
			SAVE_A_R4					; сохраним B5
; T=(B5+8)/2^4
			LDI_B 8
				call ADD3232
			LDI_B 16
				call DIV3232S
				nop						; A = T
;
; B6=B5-4000
			LOAD_A_R4
			LDI_B 4000
				call SUB3232			; A = B6
			SAVE_A_R4					; сохраним B6
; X1=(B2*(B6*B6/2^12))/2^11
			COPY_A_B
				call MUL1616S
				ldi r17,12
				call R_SHIFT_A
			LDS_B B2
				call MUL1616S
				ldi r17,11
				call R_SHIFT_A
			SAVE_A_R8					; сохраним X1
; X2=AC2*B6/2^11
			LDS_A AC2
			LOAD_B_R4					; B6 восстановим в B
				call MUL1616S
				ldi r17,11
				call R_SHIFT_A			; A = X2
; X3=X1+X2
			LOAD_B_R8					; X1 восстановим в B
				call ADD3232
			SAVE_A_R8					; сохраним X3
; B3=(((AC1*4+X3)<<oss)+2)/4
			LDS_A AC1
			LDI_B 4
				call MUL1616S
			LOAD_B_R8					; X3 восстановим в B
				call ADD3232
				ldi r17,oss
				call L_SHIFT_A
			LDI_B 2
				call ADD3232
			LDI_B 4
				call DIV3232S
			SAVE_A_R8					; сохраним B3
; X1=AC3*B6/2^13
			LDS_A AC3
			LOAD_B_R4					; A = B6
				call MUL1616S
				ldi r17,13
				call R_SHIFT_A	
			SAVE_A_R12					; сохраним X1
; X2=(B1*(B6*B6/2^12))/2^16
			LOAD_A_R4					; A = B6
			COPY_A_B
				call MUL1616S
				ldi r17,12
				call R_SHIFT_A
			LDS_B B1
				call MUL1616S
				ldi r17,16
				call R_SHIFT_A			; A = X2
; X3=((X1+X2)+2)/2^2
			LOAD_B_R12					; B = X1
				call ADD3232
			LDI_B 2
				call ADD3232
				ldi r17,2
				call R_SHIFT_A			; A = X3
; B4=AC4*(UL)(X3+32768)/2^15
			LDI_B 32768
				call ADD3232
			LDS_B AC4
				call MUL1616U
				ldi r17,15
				call R_SHIFT_A			; A = B4
			SAVE_A_R4					; сохраним B4 в R4
; B7=((UL)UP-B3)*(50000>>oss)
			;LDS_A UP
			lds r22,UP1L
			lds r23,UP1H
			clr r24
			clr r25
			LOAD_B_R8					; B = B3
				call SUB3232
			LDI_B 50000
				ldi r17,oss
				call R_SHIFT_B
				call MUL1616U			; A = B7
; if B7<0x8000000 {p=(B7*2)/B4}
				sbrc r25,7
				rjmp LARGE_B7
				ldi r17,1
				call L_SHIFT_A
			LOAD_B_R4					; B = B4
				call DIV3232U
				rjmp NEXT_X1
; else  {p=(B7/B4)*2}
LARGE_B7:	LOAD_B_R4
				call DIV3232U
				ldi r17,1
				call L_SHIFT_A			
; X1=(p/2^8)*(p/2^8)
NEXT_X1:	SAVE_A_R4					; сохраним p (здесь оно >16 бит) в R4
			LDI_B 256
				call DIV3232S
			COPY_A_B
				call MUL1616S			; A = X1 (здесь оно >16 бит)
; X1=(X1*3038)/2^16
			LDI_B 3038
				push r24				; сохраняем часть Х1, вылезшую из 16 бит
				call MUL1616U
					pop r17
					mul r17,r18
					add r24,r0
					adc r25,r1
					mul r17,r19
					add r25,r0
				ldi r17,16
				call R_SHIFT_A
			SAVE_A_R8					; сохраним новое X1  в R8 (3454)
; X2=(-7357*p)/2^16
			LOAD_A_R4					; A = p
			LDI_B 7357
				push r24
				call MUL1616U
					pop r17
					mul r17,r18
					add r24,r0
					adc r25,r1
					mul r17,r19
					add r25,r0			; A=-X2
					ldi r17,16
					call R_SHIFT_A
; p=p+(X1+X2+3791)/2^4
			COPY_A_B
				call ADD3232
			LOAD_A_R8
				call SUB3232
			LDI_B 3791
				call ADD3232
			LDI_B 16
				call DIV3232S
			LOAD_B_R4
				call ADD3232

		sts PRESSURE_PA+0,R22
		sts PRESSURE_PA+1,R23
		sts PRESSURE_PA+2,R24
		sts PRESSURE_PA+3,R25

; переведем в мм.рт.ст		
		movw r16,r22
		movw r18,r24
		lsl r16
		rol r17
		rol r18
		rol r19
		add r22,r16
		adc r23,r17
		adc r24,r18
		adc r25,r19
		asr r25	ror r24	ror r23	ror r22
		asr r25	ror r24	ror r23	ror r22
		sts PRESSURE_MM+0,R22
		sts PRESSURE_MM+1,R23
		sts PRESSURE_MM+2,R24
		sts PRESSURE_MM+3,R25
;
	pop r31		pop r30		pop r29		pop r28
	pop r27		pop r26		pop r25		pop r24
	pop r23		pop r22		pop r21		pop r20
	pop r19		pop r18		pop r17		pop r16
	pop r15		pop r14		pop r13		pop r12
	pop r11		pop r10		pop r9		pop r8
	pop r7		pop r6		pop r5		pop r4
	pop r3		pop r2		pop r1		pop r0
	out SREG,r0
	pop r0
ret
; ----------------------------------------------------
calcAvgPressure:	PUSHF
					PUSHXYZ
					PUSHHIGH
					
					ldi r16,BUFLEN
					LDZ PRESSURE_PA+4*(BUFLEN+0)
					LDY PRESSURE_PA+4*(BUFLEN+0)
					ld r25,-Y
					ld r24,-Y
					ld r23,-Y
					ld r22,-Y
					dec r16
calcAvgCycle:		tst r16
					breq calcAvgCycleEnd
					dec r16
					
					ld r21,-Y
					ld r20,-Y
					ld r19,-Y
					ld r18,-Y
					st -Z,r21
					st -Z,r20
					st -Z,r19
					st -Z,r18

					add r22,r18
					adc r23,r19
					adc r24,r20
					adc r25,r21
					
					rjmp calcAvgCycle

calcAvgCycleEnd:	

					ldi r18,byte1(BUFLEN)
					ldi r19,byte2(BUFLEN)
					ldi r20,byte3(BUFLEN)
					ldi r21,byte4(BUFLEN)
					call DIV3232U

					LDZ PRESSURE_PA_AVG
					st Z+,r22
					st Z+,r23
					st Z+,r24
					st Z+,r25

					movw r16,r22
					movw r18,r24
					lsl r16
					rol r17
					rol r18
					rol r19
					add r22,r16
					adc r23,r17
					adc r24,r18
					adc r25,r19
					asr r25	ror r24	ror r23	ror r22
					asr r25	ror r24	ror r23	ror r22

					lds r18,PRESSURE_MM_AVG+0
					lds r19,PRESSURE_MM_AVG+1
					lds r20,PRESSURE_MM_AVG+2
					lds r21,PRESSURE_MM_AVG+3
					sts PRESSURE_MM_AVG+0,R22
					sts PRESSURE_MM_AVG+1,R23
					sts PRESSURE_MM_AVG+2,R24
					sts PRESSURE_MM_AVG+3,R25
					cp  r18,r22
					cpc r19,r23
					cpc r20,r24
					cpc r21,r25
					breq noChangeMMAvg
					//--------------------//
					ldi r16,PressureMM
					call setNewValFlag
					//--------------------//
noChangeMMAvg:		
					POPHIGH
					POPXYZ
					POPF
ret
; -------------------------------------------------


