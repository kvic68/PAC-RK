indication:		; вывод возможен только тогда, когда сброшен флаг блокировки вывода
				//PUSHF
				//cli
				//ret
				lds r16,(FL_LOCK)
				tst r16
				breq indicationRun
				rjmp indicationExit
indicationExit:	ret
//-------------------------------------------------------
indicationRun:		ldi r16,$FF
					sts (FL_LOCK),r16
						lds r16,(LED_ADDRESS)
						//cpi r16,$FF
						//breq out_TM1637
						rjmp out_SSD1306
//-------------------------------------------------------
/*
out_TM1637:		; вывод на индикатор ТМ1637 по флагу
				lds r16,(FL_NEW_U)
				tst r16
				breq notShowU1637
				
				clr r16
				sts (FL_NEW_U),r16

				lds r16,(MODE)

				cpi r16,0
				breq showStab1637
				//-------------------------
				cpi r16,1
				breq showCurrent1637
				//-------------------------
				cpi r16,2
				breq showStop
				//-------------------------
				cpi r16,3
				breq showExtStop
				//-------------------------
				rjmp notShowU1637
								
				clr r16
				clr r17
				rjmp showU1637

				//-------------------------
showStop:		ldi r16,$EE
				ldi r17,$AC
				rjmp showU1637hex
				//-------------------------
showExtStop:	ldi r16,$EE
				ldi r17,$DC
				rjmp showU1637hex
				//-------------------------
showStab1637:		lds r16,(FL_MAX_U)
					tst r16
				breq showTarget1637
				//-------------------------
showCurrent1637:	lds r16,(AVERAGE_U+0)
					lds r17,(AVERAGE_U+1)
				rjmp showU1637
				//-------------------------
showTarget1637:		lds r16,(TARGET_U+0)
					lds r17,(TARGET_U+1)
				//-------------------------
showU1637:			sts (TM_BUFFER+0),r16
					sts (TM_BUFFER+1),r17
					call tm1637
					rjmp notShowU1637 
				//-------------------------
showU1637hex:		sts (TM_BUFFER+0),r16
					sts (TM_BUFFER+1),r17
					call tm1637hex
					rjmp notShowU1637 
	
notShowU1637:		nop	


					ldi r16,$00
					sts (FL_LOCK),r16
			rjmp indicationExit
*/
//-------------------------------------------------------
out_SSD1306:		call newValueScan
					ldi r16,$00
					sts (FL_LOCK),r16
				ret
//-------------------------------------------------------
	

//*********************************************
// вопилка при невозможности поддерживать выходное напряжение при падении входного
.equ	max_counter_b = 20;
top_rail:		ret					// закомментировать для включения
/*
				lds r16,(MODE)
				tst r16
				breq trrA
				ldi r16,0
				sts (COUNTER_B),r16
				//
				lds r16,(MODE)
				cpi r16,3
				breq trr0
				ldi r16,0
				sts (SOUNDNUMBER),r16
trr0:			rjmp trrExit
					
trrA:			lds r16,(SOUNDNUMBER)
				tst r16
				brne trrB
				rjmp trrG

trrB:			lds r16,(FL_MAX_U)
				tst r16
				breq trrC
				rjmp trrE
trrC:			lds r16,(COUNTER_B)
				cpi r16,max_counter_b
				breq trrD
				rjmp trrF
trrD:			ldi r16,0
				sts (SOUNDNUMBER),r16
				sts (COUNTER_B),r16
				rjmp trrExit
trrE:			ldi r16,0
				sts (COUNTER_B),r16
				rjmp trrExit
trrF:			lds r16,(COUNTER_B)
				inc r16
				sts (COUNTER_B),r16
				rjmp trrExit
trrG:			lds r16,(FL_MAX_U)
				tst r16
				breq trrE
trrH:			lds r16,(COUNTER_B)
				cpi r16,max_counter_b
				breq trrI
				rjmp trrF
trrI:			ldi r16,1
				sts (SOUNDNUMBER),r16
				rjmp trrE
trrExit:	
				ret*/
; -----------------------------------

