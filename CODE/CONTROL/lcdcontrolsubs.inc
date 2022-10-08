.equ	command		= 0b10000000
.equ	cmdstream	= 0b00000000
.equ	onedata		= 0b11000000
.equ	datastream	= 0b01000000
//-----------------------------------------------------------------------------------------
LED_INIT_STREAM:		.db $D5,$80,$A8,$3F,$D3,$00,$40,$8D,$14,$A1,\
							$C8,$DA,$12,$81,$FF,$D9,$22,$DB,$30,$A4,$A6,$00,$10,$B0
//						
LED_INIT_STREAM_END:	.db $AF,$00		; AFh; Display ON

//-----------------------------------------------------------------------------------------
//

initlcd:		PUSHZ
				ldi ZH,high(50000)
				ldi ZL,low (50000)
initlcdwait1:	sbiw Z,1				;	2
				adiw Z,1 sbiw Z,1		;	2+2
				adiw Z,1 sbiw Z,1		;	2+2
				adiw Z,1 sbiw Z,1		;	2+2
				brne initlcdwait1		;	1/2 Zero=1

				ldi ZH,high(50000)
				ldi ZL,low (50000)
initlcdwait2:	sbiw Z,1				;	2
				adiw Z,1 sbiw Z,1		;	2+2
				adiw Z,1 sbiw Z,1		;	2+2
				adiw Z,1 sbiw Z,1		;	2+2
				brne initlcdwait2		;	1/2 Zero=1

				POPZ

				call i2c_start
				ldi r16,LED_ADDR
				call i2c_send

INIT_LED_CONT:	ldi r16,cmdstream	; режим потока команд
				call i2c_send
				// передача команд
				ldi r17,(LED_INIT_STREAM_END*2-LED_INIT_STREAM*2)
				LDZ LED_INIT_STREAM*2
INIT_LED_CYCLE:	lpm r16,Z+
				call i2c_send
				dec r17
				tst r17
				brne INIT_LED_CYCLE
				call i2c_stop
				ret
//-----------------------------------------------------------------------------------------
BYTE_TO_LCD:	; отправляет данные из r16 в LCD
				; адрес в DISPLAY_POSY - y, DISPLAY_POSX - x
			push r16
				call i2c_start
				ldi r16,LED_ADDR
				call i2c_send
				ldi r16,onedata
				call i2c_send
			pop r16
			push r17
			lds r17,(MASK)
			eor r16,r17
			pop r17
				call i2c_send
				call i2c_stop
				lds r16,DISPLAY_POSX
				inc r16
				andi r16,$7F			; x=[0...127]
				sts DISPLAY_POSX,r16
			ret
//-----------------------------------------------------------------------------------------
; -- Set position x=0...127 (r16) y=0...7 (r17)
SET_POS:	
				push r16
				push r17
				push r18

				mov r18,r17
				mov r17,r16
				
				sts DISPLAY_POSX,r17
				sts DISPLAY_POSY,r18

				call i2c_start
				ldi r16,LED_ADDR
				call i2c_send
				ldi r16,cmdstream
				call i2c_send

				mov r16,r18
				andi r16,$07
				ori r16,$B0
				call i2c_send

				mov r16,r17
				andi r16,$0F
				call i2c_send
				mov r16,r17
				swap r16
				andi r16,$0F
				ori r16,$10
				call i2c_send

				call i2c_stop

				pop r18
				pop r17
				pop r16
					
SET_POS_EXIT:
				ret
//-----------------------------------------------------------------------------------------
clearlcd:		push r16
				push r17
				push r18

				call disableAllOutputs
				//
				ldi r18,8
LED_CLR_O:		ldi r17,128
				dec r18
				push r17
				push r18
				ldi r17,0
				sts (DISPLAY_POSX),r17
				sts (DISPLAY_POSY),r18

				mov r16,r17
				mov r17,r18

				call SET_POS

				pop r18
				pop r17
				//
				call i2c_start
				ldi r16,LED_ADDR
				call i2c_send
				ldi r16,datastream
				call i2c_send
				//
LED_CLR_I:		ldi r16,$00
				//mov r16,r17
				call i2c_send
				dec r17
				brne LED_CLR_I
				call i2c_stop
				//
				tst r18
				brne LED_CLR_O

				//
				pop r18
				pop r17
				pop r16	
				ret
//-----------------------------------------------------------------------------------------
LED_ON:			call i2c_start
				ldi r16,LED_ADDR
				call i2c_send
				ldi r16,command
				call i2c_send
				ldi r16,$AF
				call i2c_send
				call i2c_stop
			ret	
//
// ----------------------------------------------------------------
// разрешение отображения переменной (R16) на экране
enableOutput:			PUSHY
						push r16
						subi r16,$80
						clr r17
						LDY positionTable
						add YL,r16		adc YH,r17
						add YL,r16		adc YH,r17
						ldd r16,Y+1
						SETB r16,6
						std Y+1,r16
						pop r16
						POPY
						ret
// ----------------------------------------------------------------
// запрет отображения переменной (R16) на экране
disableOutput:			PUSHY
						push r16
						subi r16,$80
						clr r17
						LDY positionTable
						add YL,r16		adc YH,r17
						add YL,r16		adc YH,r17
						ldd r16,Y+1
						CLRB r16,6
						std Y+1,r16
						pop r16
						POPY			
						ret
// ----------------------------------------------------------------
// запрет отображения всех переменных на экране
disableAllOutputs:		PUSHY
						push r16
						push r17
						LDY positionTable
						ldi r16,64
disableAllOutputsCycle:	tst r16
						breq disableAllOutputsEx
						dec r16
						ldd r17,Y+1
						CLRB r17,5
						std Y+1,r17
						adiw Y,2
						rjmp disableAllOutputsCycle
disableAllOutputsEx:	pop r17
						pop r16
						POPY
					ret
// ----------------------------------------------------------------
// запрет отображения переменной, отображаемой в позиции X,Y
disableThisXY:			PUSHY
						push r16
						push r17
						push r18
						push r19
						push r20
						LDY positionTable
						ldi r16,64
						lds r19,(DISPLAY_POSX)
						lds r20,(DISPLAY_POSY)
disableThisXYCycle:		tst r16
						breq disableThisXYEx
						dec r16
						ldd r17,Y+0
						andi r17,$7F
						ldd r18,Y+1
						andi r18,$07
						cp r17,r19
						cpc r18,r20
						brne disableThisXYNext
						ldd r18,Y+1
						andi r18,0b11011111
						std Y+1,r18
disableThisXYNext:		adiw Y,2
						rjmp disableThisXYCycle
disableThisXYEx:		pop r20
						pop r19
						pop r18
						pop r17
						pop r16
						POPY
						ret
// ----------------------------------------------------------------
