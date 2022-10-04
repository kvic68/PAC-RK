;*****************************************************************
LCD_INIT:	cbi LCD_PORT,LCD_RES
			M_DELAY_US 1000
			sbi LCD_PORT,LCD_RES
			M_DELAY_US 10000
			cbi LCD_PORT,LCD_SCE			; начало передачи
				cbi LCD_PORT,LCD_DC			; передаем команды
				ldi r16,$21
				call SEND_BYTE
				ldi r16,$90
				call SEND_BYTE
				ldi r16,$20
				call SEND_BYTE
				ldi r16,$0C
				call SEND_BYTE
			sbi LCD_PORT,LCD_SCE			; конец передачи
			call CLEAR_SCR
			ret
; ------------------------------------------------------------------------
SEND_BYTE:		push r16
				push r17
				ldi r17,8
SEND_BYTE_LOOP:	cbi LCD_PORT,LCD_SCLK			; строб вниз
				rol r16
				brcc BIT_IS_ZERO
				sbi LCD_PORT,LCD_SDIN
				rjmp SEND_NEXT_BIT
BIT_IS_ZERO:	cbi LCD_PORT,LCD_SDIN
SEND_NEXT_BIT:	//M_DELAY_CLK_A 24,r18
				sbi LCD_PORT,LCD_SCLK			; строб вверх
				//M_DELAY_CLK_A 24,r18
				dec r17
				brne SEND_BYTE_LOOP				
				sbi LCD_PORT,LCD_SDIN
				pop r17
				pop r16
			ret
; ------------------------------------------------------------------------
CLEAR_SCR:		push r16
				push r17
				cbi LCD_PORT,LCD_SCE			; начало передачи	
				sbi LCD_PORT,LCD_DC				; передаем данные
					ldi r16,$00
				LDZ 504
CLEAR_LOOP:		call SEND_BYTE
				sbiw Z,1
				brne CLEAR_LOOP
				sbi LCD_PORT,LCD_SCE			; конец передачи
				ldi r16,0
				ldi r17,0
				call SET_XY
				pop r17
				pop r16
			ret
; ------------------------------------------------------------------------
; --- r16 - X, r17 - Y ---
SET_XY:			push r16
				push r17
				cbi LCD_PORT,LCD_SCE			; начало передачи
				//M_DELAY_US_RA 1,r18
				cbi LCD_PORT,LCD_DC			; передаем команды
				cpi r16,84
				brcs goodX
				ldi r16,83
goodX:			sts LCD_X,r16
				ori  r16,$80
				call SEND_BYTE				; X
				mov r16,r17
				andi r16,$07
				sts LCD_Y,r16
				ori  r16,$40
				call SEND_BYTE				; Y
				sbi LCD_PORT,LCD_SCE			; конец передачи
				//M_DELAY_US_RA 1,r18
				sbi LCD_PORT,LCD_DC			; передаем данные
				pop r17
				pop r16
			ret
; ------------------------------------------------------------------------
VOLT_OUT:		PUSHF
				PUSHZ
				push r16
				push r17

				ldi r16,$FF
				sts (FL_LOCK),r16

				ldi r16,2
				ldi r17,1
				call SET_XY

				sbi LCD_PORT,LCD_DC			; передаем данные
				

				lds r16,(TM_BUFFER+0)
				lds r17,(TM_BUFFER+1)
				call bin2bcd16

				/*ldi r16,$90
				mov r11,r16
				ldi r16,$75
				mov r10,r16*/

				; цифири в R11,R10
				ldi ZL, low(bigDigits*2)
				ldi ZH,high(bigDigits*2)

				movw r12,Z
				clr r17
				mov r16,r11
				swap r16
				andi r16,$0F
				tst r16
				brne vo0
				ldi r16,20
vo0:			ldi r17,80
				mul r16,r17
				movw r16,r0
				add ZL,r16
				adc ZH,r17
				call PRINT_BIG_DIG

				movw Z,r12
				clr r17
				mov r16,r11
				tst r16
				brne vo1
				ldi r16,20
				rjmp vo2
vo1:			andi r16,$0F
vo2:			ldi r17,80
				mul r16,r17
				movw r16,r0
				add ZL,r16
				adc ZH,r17
				call PRINT_BIG_DIG

				movw Z,r12
				clr r17
				mov r16,r10
				swap r16
				andi r16,$0F
				ldi r17,80
				mul r16,r17
				movw r16,r0
				add ZL,r16
				adc ZH,r17
				call PRINT_BIG_DIG

				movw Z,r12
				clr r17
				mov r16,r10
				andi r16,$0F
					subi r16,(-10)
				ldi r17,80
				mul r16,r17
				movw r16,r0
				add ZL,r16
				adc ZH,r17
				call PRINT_BIG_DIG

				ldi r16,$00
				sts (FL_LOCK),r16

				pop r17
				pop r16
				POPZ
				POPF
		ret
; ------------------------------------------------------------------------
PRINT_BIG_DIG:	//cbi LCD_PORT,LCD_SCE			; начало передачи
				push r16
				push r17

				lds r16,LCD_X
				lds r17,LCD_Y
				push r16
				push r17

				ldi r17,4
outer_loop:		cbi LCD_PORT,LCD_SCE			; начало передачи
				//M_DELAY_US_RA 1,r16
				push r17
				
				ldi r17,20
inner_loop:			lpm r16,Z+
					call SEND_BYTE
					dec r17
					brne inner_loop
				// восстановить X, увеличить Y	
				lds r16,LCD_X
				lds r17,LCD_Y
				inc r17
					call SET_XY

				pop r17
				dec r17
				brne outer_loop
				// новое X, новое Y
				pop r17
				pop r16
				subi r16,(-20)
					call SET_XY

				sbi LCD_PORT,LCD_SCE			; конец передачи
				//M_DELAY_US_RA 1,r16
				pop r17
				pop r16
		ret
