; *** Проверка присутствия на C0-C3, результат в STATUS 1 - есть отклик. *********************
GetPresence:
		push r16
		in r16,SREG
		push r16
		push r17
		; прижать шину
		ldi r16,$FF
		out DDRC,r16
			push R16
			push R17
			M_DELAY_US_RC   500, 8, R16, R17 ; Задержка 500 мкс минус 8 тактов 8 -- компенсация тактов: 2*push + 2*pop
			pop  R17
			pop  R16
		; отпустить шину
		ldi r16,$F0
		out DDRC,r16
//	cli
			push R16
			push R17
			M_DELAY_US_RC   64, 8, R16, R17 ; Задержка 64 мкс минус 8 тактов 8 -- компенсация тактов: 2*push + 2*pop
			pop  R17
			pop  R16
		; прочитать ответ с шины
		in r17,PINC
		andi r17,$0F
//	sei
			push R16
			push R17
			M_DELAY_US_RC   436, 8, R16, R17 ; Задержка 436 мкс минус 8 тактов 8 -- компенсация тактов: 2*push + 2*pop
			pop  R17
			pop  R16
		; посмотреть еще раз, что на шине
		in r16,PINC
		andi r16,$0F
		or r16,r17
		eor r17,r16
		sts DS_STATUS,r17
			; если получен ответ, то в соответствующем бите будет 1
		pop r17
		pop r16
		out SREG,r16
		pop r16
		ret

; *** Заапись бита в 4 линии C0-C3 из TMP_BIT ************************************
WriteBit:
		push r16
		in r16,SREG
		push r16
		; прижать шину
//	cli
		ldi r16,$FF	; шина на вывод
		out DDRC,r16
		ldi r16,$F0
		out PORTC,r16
			; 4 mks
			push R16
			push R17
			M_DELAY_US_RC   4, 8, R16, R17 ; Задержка 4 мкс минус 8 тактов 8 -- компенсация тактов: 2*push + 2*pop
			pop  R17
			pop  R16
		; выдать бит
		lds r16,DS_TMP_BYTE
		ori r16,$F0
		out PORTC,r16
			; 58 mks 
			push R16
			push R17
			M_DELAY_US_RC   58, 8, R16, R17 ; Задержка 58 мкс минус 8 тактов 8 -- компенсация тактов: 2*push + 2*pop
			pop  R17
			pop  R16
			; отпустить шину
		ldi r16,$F0	; шина на ввод
		out DDRC,r16
		ldi r16,$F0
		out PORTC,r16
//	sei
			; 8 mks
			push R16
			push R17
			M_DELAY_US_RC   8, 8, R16, R17 ; Задержка 8 мкс минус 8 тактов 8 -- компенсация тактов: 2*push + 2*pop
			pop  R17
			pop  R16
		pop r16
		out SREG,r16
		pop r16
		ret
; *** Байт из R16 передается сразу на 4 линии C0-C3 ******************************************
WriteByte:	; передача байта из R16
		push r17
		in r17,SREG
		push r17
		push r16
		push r18
			ldi r17,8
WriteByte_m1:
			cpi r17,0
			breq WriteByte_m2
			dec r17
				sbrs r16,0
				ldi r18,$00
				sbrc r16,0
				ldi r18,$0F
			ror r16
			sts DS_TMP_BYTE,r18
			call WriteBit
			jmp WriteByte_m1
WriteByte_m2:
			pop r18
			pop r16
			pop r17
			out SREG,r17
			pop r17
		ret
; **** Чтение бита(с четырех линий сразу C0-C3) - результат в TMP_BIT *********************************
ReadBit:
		push r16
		in r16,SREG
		push r16
		; прижать шину
//	cli
		ldi r16,$FF
		out DDRC,r16
		; 4 mks
			push R16
			push R17
			M_DELAY_US_RC   4, 8, R16, R17 ; Задержка 4 мкс минус 8 тактов 8 -- компенсация тактов: 2*push + 2*pop
			pop  R17
			pop  R16
		; отпустить шину
		ldi r16,$F0
		out DDRC,r16
		; 10 mks
			push R16
			push R17
			M_DELAY_US_RC   10, 8, R16, R17 ; Задержка 10 мкс минус 8 тактов 8 -- компенсация тактов: 2*push + 2*pop
			pop  R17
			pop  R16
		; прочитать ответ с шины
		in r16,PINC
		andi r16,$0F
		; сохранить в переменной
		sts DS_TMP_BYTE,r16
//	sei
		; 56 mks
			push R16
			push R17
			M_DELAY_US_RC   56, 8, R16, R17 ; Задержка 56 мкс минус 8 тактов 8 -- компенсация тактов: 2*push + 2*pop
			pop  R17
			pop  R16
		pop r16
		out SREG,r16
		pop r16
		ret

; *** Чтение байта с 4 линий сразу C0-C3. Результат в BYTE_FROM_DS0...3 *********************************
ReadByte:
		push r16
		in r16,SREG
		push r16
		push r17
		push r20
		push r21
		push r22
		push r23
		clr r20
		clr r21
		clr r22
		clr r23
		ldi r17,8
ReadByte_m1:
			cpi r17,0
				breq ReadByte_m2
			dec r17
				call ReadBit
				lds r16,DS_TMP_BYTE
			ror r16
			ror r20
			ror r16
			ror r21
			ror r16
			ror r22
			ror r16
			ror r23
		jmp ReadByte_m1
ReadByte_m2:
			sts BYTE_FROM_DS0,r20
			sts BYTE_FROM_DS1,r21
			sts BYTE_FROM_DS2,r22
			sts BYTE_FROM_DS3,r23
		pop r23
		pop r22
		pop r21
		pop r20
		pop r17
		pop r16
		out SREG,r16
		pop r16
		ret
// ----------------------------------------------------------------
// запуск нового преобразования температурных датчиков
ds18NewConvert:	push r16
				call GetPresence
				ldi r16,SkipROM
				call WriteByte
				ldi r16,ConvertT
				call WriteByte
				pop r16
				ret

// ----------------------------------------------------------------
saveOldTValues:	PUSHZ
				PUSHY
				push r16

				LDZ DS_VALUE_0
				LDY DS_OLD_VALUE_0
				ld r16,Z+	st Y+,r16
				ld r16,Z+	st Y+,r16

				ld r16,Z+	st Y+,r16
				ld r16,Z+	st Y+,r16

				ld r16,Z+	st Y+,r16
				ld r16,Z+	st Y+,r16

				ld r16,Z+	st Y+,r16
				ld r16,Z+	st Y+,r16

				pop r16
				POPY
				POPZ

				ret
// ----------------------------------------------------------------
// CRC
UPDATE_CRC:
						PUSHZ
						PUSHY

                        LDI     ZL, 8
                        LDI     YH, 0x8C

CRC_LOOP:
                        MOV     ZH, r17
                        EOR     ZH, r16

                        LSR     r17
                        LSR     r16
                        
                        SBRC    ZH, 0
                        EOR     r17, YH

                        DEC     ZL
                        BRNE    CRC_LOOP
                        
						POPY
						POPZ
                        RET
// ----------------------------------------------------------------
// Чтение температурных датчиков
ds18ReadData:	push r16
				push r17
				push r18
				PUSHXYZ

				call saveOldTValues

				call GetPresence
				ldi r16,SkipROM
				call WriteByte
				ldi r16,ReadScratchpad
				call WriteByte
				; --
				LDZ SCRATCHPAD_0
				LDY CRC_0
				clr r16
				std Y+0,r16	std Y+1,r16
				std Y+2,r16	std Y+3,r16

				ldi r18,8

rd_crc_loop:
				call ReadByte			;T_LSB
				LDX BYTE_FROM_DS0
				ld r16,X+	std Z+ 0,r16	ldd r17,Y+0 	call UPDATE_CRC		std Y+0,r17
				ld r16,X+	std Z+ 9,r16	ldd r17,Y+1 	call UPDATE_CRC		std Y+1,r17
				ld r16,X+	std Z+18,r16	ldd r17,Y+2 	call UPDATE_CRC		std Y+2,r17
				ld r16,X+	std Z+27,r16	ldd r17,Y+3 	call UPDATE_CRC		std Y+3,r17
				adiw Z,1

				dec r18
				brne rd_crc_loop



				call ReadByte			;CRC
				LDX BYTE_FROM_DS0
crc_chk_0:		ld r16,X+	std Z+ 0,r16	ldd r17,Y+0		cp r16,r17		breq crc_chk_1		MOVRAM16 SCRATCHPAD_0,DS_OLD_VALUE_0,r18
crc_chk_1:		ld r16,X+	std Z+ 9,r16	ldd r17,Y+1		cp r16,r17		breq crc_chk_2		MOVRAM16 SCRATCHPAD_1,DS_OLD_VALUE_1,r18
crc_chk_2:		ld r16,X+	std Z+18,r16	ldd r17,Y+2		cp r16,r17		breq crc_chk_3		MOVRAM16 SCRATCHPAD_2,DS_OLD_VALUE_2,r18
crc_chk_3:		ld r16,X+	std Z+27,r16	ldd r17,Y+3		cp r16,r17		breq crc_chk_4		MOVRAM16 SCRATCHPAD_3,DS_OLD_VALUE_3,r18
crc_chk_4:		
				; --

				LDZ SCRATCHPAD_0

				LDY DS_VALUE_0

				ldd r16,Z+ 0	st Y+,r16
				ldd r16,Z+ 1	st Y+,r16

				ldd r16,Z+ 9	st Y+,r16
				ldd r16,Z+10	st Y+,r16

				ldd r16,Z+18	st Y+,r16
				ldd r16,Z+19	st Y+,r16

				ldd r16,Z+27	st Y+,r16
				ldd r16,Z+28	st Y+,r16

				; --

ds18RD_m8:			LDZ $8000
					lds r16,(DS_STATUS)
					sbrc r16,0
					rjmp ds18RD_m9
					STSZ DS_VALUE_0

ds18RD_m9:			sbrc r16,1
					rjmp ds18RD_m10
					STSZ DS_VALUE_1

ds18RD_m10:			sbrc r16,2
					rjmp ds18RD_m11
					STSZ DS_VALUE_2

ds18RD_m11:			sbrc r16,3
					rjmp ds18RD_m12
					STSZ DS_VALUE_3
					
ds18RD_m12:			LDZ $8000
					LDSX DS_OLD_VALUE_0
					LDSY DS_VALUE_0
					cp  XL,ZL
					cpc XH,ZH
					breq ds18RD_m21
ds18RD_m20:			cp  XL,YL
					cpc XH,YH
					breq ds18RD_m22
						ldi r16,T0
						call setNewValFlag
					rjmp ds18RD_m22
ds18RD_m21:			LDZ $0550
					cp  YL,ZL
					cpc YH,ZH
					brne ds18RD_m20
					LDY $8000
					STSY DS_VALUE_0
ds18RD_m22:			; --------
					LDZ $8000
					LDSX DS_OLD_VALUE_1
					LDSY DS_VALUE_1
					cp  XL,ZL
					cpc XH,ZH
					breq ds18RD_m24
ds18RD_m23:			cp  XL,YL
					cpc XH,YH
					breq ds18RD_m25
						ldi r16,T1
						call setNewValFlag
					rjmp ds18RD_m25
ds18RD_m24:			LDZ $0550
					cp  YL,ZL
					cpc YH,ZH
					brne ds18RD_m23
					LDY $8000
					STSY DS_VALUE_1
ds18RD_m25:			; --------
					LDZ $8000
					LDSX DS_OLD_VALUE_2
					LDSY DS_VALUE_2
					cp  XL,ZL
					cpc XH,ZH
					breq ds18RD_m27
ds18RD_m26:			cp  XL,YL
					cpc XH,YH
					breq ds18RD_m28
						ldi r16,T2
						call setNewValFlag
					rjmp ds18RD_m28
ds18RD_m27:			LDZ $0550
					cp  YL,ZL
					cpc YH,ZH
					brne ds18RD_m26
					LDY $8000
					STSY DS_VALUE_2
ds18RD_m28:			; --------
					LDZ $8000
					LDSX DS_OLD_VALUE_3
					LDSY DS_VALUE_3
					cp  XL,ZL
					cpc XH,ZH
					breq ds18RD_m30
ds18RD_m29:			cp  XL,YL
					cpc XH,YH
					breq ds18RD_m31
						ldi r16,T3
						call setNewValFlag

					rjmp ds18RD_m31
ds18RD_m30:			LDZ $0550
					cp  YL,ZL
					cpc YH,ZH
					brne ds18RD_m29
					LDY $8000
					STSY DS_VALUE_3
ds18RD_m31:			ldi r16,HSensorVal
					call setNewValFlag
					ldi r16,HString
					call setNewValFlag
				POPXYZ
				pop r18
				pop r17
				pop r16
				ret
// ----------------------------------------------------------------
