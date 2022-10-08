indication:		; вывод возможен только тогда, когда сброшен флаг блокировки вывода
				lds r16,(FL_LOCK)
				tst r16
				breq indicationRun
indicationExit:	ret
//-------------------------------------------------------
indicationRun:		ldi r16,$FF
					sts (FL_LOCK),r16
					call newValueScan
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

