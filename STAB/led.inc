; ------------------------------------------------------------------------
; LED BLOCK
.equ	command		= 0b10000000
.equ	cmdstream	= 0b00000000
.equ	onedata		= 0b11000000
.equ	datastream	= 0b01000000
//.equ	led_1306	= 0b01111000	; 0b01111010
; ------------------------------------------------------------------------
LED_ON:			call softI2CStart
				lds r16,LED_ADDRESS
				call softI2CSendByte
				ldi r16,command
				call softI2CSendByte
				ldi r16,$AF
				call softI2CSendByte
				call softI2CStop
			ret
; ------------------------------------------------------------------------
; очистка строки, номер в R16
LED_CLRLINE:	push r16
				push r17
				push r18
				ldi r17,0
				mov r18,r16
				sts (POSX),r17
				sts (POSY),r18
				call SET_LED_XY
				call softI2CStart

				lds r16,LED_ADDRESS
				call softI2CSendByte
				ldi r16,datastream
				call softI2CSendByte

				ldi r17,132
LED_CLRL:		ldi r16,$00
				call softI2CSendByte
				dec r17
				brne LED_CLRL
				call softI2CStop

				pop r18
				pop r17
				pop r16	
			ret
; ------------------------------------------------------------------------
; очистка строки, номер в R16, образец в R18
LED_FIL_LINE:	push r16
				push r17
				push r18
					ldi r17,0
					mov r18,r16
				sts (POSX),r17
				sts (POSY),r18
				call SET_LED_XY
				call softI2CStart

				lds r16,LED_ADDRESS
				call softI2CSendByte
				ldi r16,datastream
				call softI2CSendByte

				ldi r17,128
				pop r18
LED_FILL:		mov r16,r18
				call softI2CSendByte
				dec r17
				brne LED_FILL
				call softI2CStop

				//pop r18
				pop r17
				pop r16	
			ret
; ------------------------------------------------------------------------
INIT_LED:		call softI2CStart
				lds r16,LED_ADDRESS
				call softI2CSendByte
				cpi r16,$FF
				brne INIT_LED_CONT
				nop
INIT_LED_CONT:	ldi r16,cmdstream	; режим потока команд
				call softI2CSendByte
				// передача команд
				ldi r17,(LED_INIT_STREAM_END*2-LED_INIT_STREAM*2)
				LDZ LED_INIT_STREAM*2
INIT_LED_CYCLE:	lpm r16,Z+
				call softI2CSendByte
				dec r17
				tst r17
				brne INIT_LED_CYCLE
				call softI2CStop
				//
			ret
; ------------------------------------------------------------------------
LED_INIT_STREAM:		.db $D5,$80,$A8,$3F,$D3,$00,$40,$8D,$14,$A1,\
							$C8,$DA,$12,$81,$FF,$D9,$22,$DB,$30,$A4,$A6,$00,$10,$B0
//						
LED_INIT_STREAM_END:	.db $AF,$00		; AFh; Display ON

;LED_INIT_STREAM:		.db $D5, $80	; Divide Ratio/Oscillator Frequency Mode Set: (D5H) (Double Bytes Command) 80
;						.db $A8, $3F	; Set Multiplex Ration: (Double Bytes Command)
;						.db $D3, $00	; Display Offset Mode Set: (D3H) Display Offset Data Set: (00H~3FH)
;						.db $40			; Set Display Start Line
;						.db $8D, $14		; 8Dh ; Charge Pump Setting (14)
;							.db  $A1			; Set Segment Re-map (POR = A0/A1) зеркало по горизонтали
;							.db  $C8			; Set Common Output Scan Direction (POR = C0/С8) зеркало по вертикали
;						.db $DA, $12	; Common Pads Hardware Configuration Mode Set: (DAH) (Double Bytes Command)
;						.db $81, $FF	; The Contrast Control Mode Set: 00-FF яркость
;							.db $D9, $22
;							.db $DB, $30
;						.db $A4			; Set Entire Display OFF/ON: (A4H - A5H)
;							.db $A6			; Set Normal/Reverse Display: (A6H -A7H)
;						.db $00,$10,$B0	; установка начального адреса вывода 1106
;										; 14h ; Enable Charge Pump
;LED_INIT_STREAM_END:	.db $AF,$00		; AFh; Display ON
/*LED_INIT_STREAM:		.db $A8, $3F	; Set Multiplex Ration: (Double Bytes Command)
						.db $D3, $00	; Display Offset Mode Set: (D3H) Display Offset Data Set: (00H~3FH)
						.db $40			; Set Display Start Line
							.db $A1			; Set Segment Re-map (POR = A0/A1) зеркало по горизонтали
							.db $C8			; Set Common Output Scan Direction (POR = C0/С8) зеркало по вертикали
						.db $DA, $12	; Common Pads Hardware Configuration Mode Set: (DAH) (Double Bytes Command)
						.db $81, $FF	; The Contrast Control Mode Set: 00-FF яркость
						.db $A4			; Set Entire Display OFF/ON: (A4H - A5H)
							.db $A6			; Set Normal/Reverse Display: (A6H -A7H)
						.db $D5, $80	; Divide Ratio/Oscillator Frequency Mode Set: (D5H) (Double Bytes Command) 80
//						.db $00,$10,$B0	; установка начального адреса вывода 1106
						.db $8D,$14		; 8Dh ; Charge Pump Setting
										; 14h ; Enable Charge Pump
LED_INIT_STREAM_END:	.db $AF,$00		; AFh; Display ON
						.db $AF			; Display OFF/ON: (AEH - AFH)*/
; ------------------------------------------------------------------------
LED_CLEAR:		push r16
				push r17
				push r18

				call disableAllOutputs
				
				ldi r18,8
LED_CLR_O:		ldi r17,132
				dec r18
				push r17
				ldi r17,0
				sts (POSX),r17
				sts (POSY),r18
				call SET_LED_XY
				pop r17
				//
				call softI2CStart
				lds r16,LED_ADDRESS
				call softI2CSendByte
				ldi r16,datastream
				call softI2CSendByte
				//
LED_CLR_I:		ldi r16,$00
				//mov r16,r17
				call softI2CSendByte
				dec r17
				brne LED_CLR_I
				call softI2CStop
				//
				tst r18
				brne LED_CLR_O

				
				pop r18
				pop r17
				pop r16	
			ret
; ------------------------------------------------------------------------
; ------------------------------------------------------------------------
// r17 - X(0...127), r18 - Y(0...7)
// (POSX)->r17,(POSY)->r18
SET_LED_XY:		lds r17,(POSX)
				lds r18,(POSY)
				sbrc r17,7
				rjmp SET_LED_XY_EXIT
				push r16
				
				sts LCD_X,r17
				sts LCD_Y,r18

				call softI2CStart
				lds r16,LED_ADDRESS
				call softI2CSendByte
				ldi r16,cmdstream
				call softI2CSendByte

				mov r16,r18
				andi r16,$07
				ori r16,$B0
				call softI2CSendByte

				mov r16,r17
				andi r16,$0F
				call softI2CSendByte
				mov r16,r17
				swap r16
				andi r16,$0F
				ori r16,$10
				call softI2CSendByte

				call softI2CStop

				pop r16
					
SET_LED_XY_EXIT:ret
; ------------------------------------------------------------------------
//TARGET_SMALL_OUT:	ret
					
; ------------------------------------------------------------------------
; ------------------------------------------------------------------------
//VOLT_OUT_LED:	
//		ret
; ------------------------------------------------------------------------
; r16 - код символа
; ------------------------------------------------------------------------
; r16 - код символа
//PRINT8x16:		
//		ret

; ------------------------------------------------------------------------
; ------------------------------------------------------------------------
; ------------------------------------------------------------------------
; r16 - код символа
//PRINT12x16:		
//		ret

; ------------------------------------------------------------------------
; Z - начало образа символа
//PRINT_BIG_DIG:	
//		ret

