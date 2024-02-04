; --------------------------------------------------------------------------------
INCREASE_U:		PUSHF
				push r16
				push r17
				push r18
				push r19

				lds r16,(MODE)
				lds r17,(LED_ADDRESS)
				cpi r17,$FF
				breq INCREASE_L
				cpi r17,$00
				breq INCREASE_B
				rjmp INCREASE_A
INCREASE_L:		cpi r16,$00
				brne INCREASE_B
INCREASE_A:		cpi r16,3
				brne INCREASE_RUN

INCREASE_B:		rjmp INCREASE_EX
				; ----------
INCREASE_RUN:	lds r16,(FL_MAX_U)		; Если выходное напряжение на максимуме,
				cpi r16,$FF				; то ничего не делаем.
				brne INCREASE_NORM		; Иначе приступаем к регулировке.
				rjmp INCREASE_EX
				; ----------
INCREASE_NORM:	lds r16,(TARGET_U+0)
				lds r17,(TARGET_U+1)
				lds r18,(STEP_U+0)
				lds r19,(STEP_U+1)
				add r16,r18
				adc r17,r19	
				call normU
				ldi r18, low(MIN_U)
				ldi r19,high(MIN_U)
				cp  r16,r18
				cpc r17,r19
				brcc INCREASE_MAX		; Если следующее значение больше или равно минимальному, то устанавливаем его.
				movw r16,r18			; Иначе следующее значение устанавливаем = MIN_U
				rjmp INCREASE_SET

INCREASE_MAX:	ldi r18, low(MAX_U)
				ldi r19,high(MAX_U)
				cp  r16,r18
				cpc r17,r19
				brcs INCREASE_SET
				breq INCREASE_SET
				movw r16,r18

INCREASE_SET:	sts (TARGET_U+0),r16
				sts (TARGET_U+1),r17

				ldi r16,TargetV
				call setNewValFlag
			
				; ----------
INCREASE_EX:	pop r19
				pop r18
				pop r17
				pop r16
				POPF
			ret
				; ----------
; --------------------------------------------------------------------------------
				; ----------
; --------------------------------------------------------------------------------
DECREASE_U:		PUSHF
				push r6
				push r7
				push r8
				push r9
				push r10
				push r11
				push r12
				push r13
				push r16
				push r17
				push r18
				push r19
				; ----------
				lds r16,(MODE)
				lds r17,(LED_ADDRESS)
				cpi r17,$FF
				breq DECREASE_L
				cpi r17,$00
				breq DECREASE_B
				rjmp DECREASE_A
DECREASE_L:		cpi r16,$00
				brne DECREASE_B
DECREASE_A:		cpi r16,3
				brne DECREASE_RUN
DECREASE_B:		rjmp DECREASE_EX
				; ----------
DECREASE_RUN:	lds ZL,(FL_MAX_U)
				cpi ZL,$FF
				brne DECREASE_NORM

				lds r16,(MODE)
				cpi r16,0
				brne DECREASE_NORM
				; ----------
				lds r8,(STEP_U+0)
				lds r9,(STEP_U+1)
				add r8,r8		adc r9,r9
				add r8,r8		adc r9,r9
				add r8,r8		adc r9,r9
				add r8,r8		adc r9,r9		; Y=STEP*16
				lds r6,(CURRENT_U+0)			; 
				lds r7,(CURRENT_U+1)			; 
				sub r6,r8
				sbc r7,r9						; [R7:6]=CURRENT_U-Y
				clr r8
				clr r9
				lds r10,(STEP_U+0)
				lds r11,(STEP_U+1)
				clr r12
				clr r13
				call div32u						; X=[(CURRENT_U-16*STEP)/STEP]
				lds r8,(STEP_U+0)
				lds r9,(STEP_U+1)
				call MUL1616					; X=X*STEP
				movw r16,r10
				; ----------
					ldi r18, low(5000)
					ldi r19,high(5000)
					sts (OPTO_ON_TICK+0),r18
					sts (OPTO_ON_TICK+1),r19
				; ----------
				lds r18,(STEP_U+0)
				lds r19,(STEP_U+1)
				rjmp DECREASE_CONT	
				; ----------
DECREASE_NORM:	lds r16,(TARGET_U+0)
				lds r17,(TARGET_U+1)
				lds r18,(STEP_U+0)
				lds r19,(STEP_U+1)
DECREASE_CONT:	sub r16,r18
				sbc r17,r19
				call normU
				ldi r18, low(MIN_U)
				ldi r19,high(MIN_U)
				cp  r16,r18
				cpc r17,r19
				sbrc r17,7
				rjmp DECREASE_ZERO
				brcc DECREASE_SET		; Если следующее значение больше или равно минимальному, то устанавливаем его.
DECREASE_ZERO:	clr r16					; Иначе следующее значение устанавливаем = 0
				clr r17					;

DECREASE_SET:	sts (TARGET_U+0),r16
				sts (TARGET_U+1),r17

				ldi r16,TargetV
				call setNewValFlag

DECREASE_EX:	pop r19
				pop r18
				pop r17
				pop r16
				pop r13
				pop r12
				pop r11
				pop r10
				pop r9
				pop r8
				pop r7
				pop r6
				POPF
				ret
				; ----------
//-----------------------------------------------------------

normU:			movw r6,r16
				clr r8
				clr r9
				lds r10,(STEP_U+0)
				lds r11,(STEP_U+1)
				clr r12
				clr r13
					call div32u						; X=[TARGET_U/STEP]
				lds r8,(STEP_U+0)
				lds r9,(STEP_U+1)
					call MUL1616					; X=X*STEP
				movw r16,r10					; R17..16 - TARGET_U

				ret
; ------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
; Отработка сигнала аварии на ногах PC
PCI_ONE:	
			//ret
			PUSHF
			push r16
			push r17

			lds r16,(MODE)
			cpi r16,3
			brne PCI_ONE_RUN
			rjmp PCI_ONE_EX

PCI_ONE_RUN:		in r16,PINC
					andi r16,0b00000011				//	Биты: 0 - аварийный вход, 1 - внешний разгон
					sbrs r16,0	
					rjmp EXT_STOP					//	Если активен аварийный вход, то переходим на его обработку
						
						ldi r17,0
						sts (ExtStopCounter),r17	//	Иначе сбрасываем счетчик задержки срабатывания аварийного входа

					
					andi r16,0b00000010				//	Выделяем сигнал внешнего разгона
					lds r17,(ExtForcePrev)
					cp r16,r17
					brne NewExtForceValue
					clr r17
					sts (ExtForceCounter),r17
					rjmp PCI_ONE_EX

NewExtForceValue:	lds r17,(ExtForceCounter)
					cpi r17,10
					brcc ExtForceRun
					inc r17
					sts (ExtForceCounter),r17
					rjmp PCI_ONE_EX

ExtForceRun:		sts (ExtForcePrev),r16
					clr r17
					sts (ExtForceCounter),r17
					sbrc r16,1
					rjmp EXT_FULLPOWER_OFF			//	Переход на обработчик НЕактивного уровня сигнала внешнего разгона

					sbrs r16,1
					rjmp EXT_FULLPOWER_ON			//	Переход на обработчик активного уровня сигнала внешнего разгона

					rjmp PCI_ONE_EX
// ---------------------------------------------------------
//
//	Выключение разгона внешним сигналом
//
EXT_FULLPOWER_OFF:		lds r16,(MODE)
						cpi r16,1
						brne EXT_FULLPOWER_OFF_EX
						//	lds r16,(ExtFullPowerCounter)
						//	cpi r16,128
						//	brne EXT_FULLPOWER_OFF_EX
						ldi r16,0
						//sts (ExtFullPowerCounter),r16
						call setMode
EXT_FULLPOWER_OFF_EX:	rjmp PCI_ONE_EX
//
//	Включение разгона внешним сигналом
//
EXT_FULLPOWER_ON:		lds r16,(MODE)
						cpi r16,0
						brne EXT_FULLPOWER_ON_EX

						ldi r16,1
						call setMode
						//	ldi r16,128
						//	sts (ExtFullPowerCounter),r16

EXT_FULLPOWER_ON_EX:	rjmp PCI_ONE_EX
// ---------------------------------------------------------
EXT_STOP:				lds r17,(MODE)
						cpi r17,3
						brne EXT_STOP_CHK
						ldi r16,0
						rjmp EXT_STOP_SET_CNT
EXT_STOP_CHK:			lds r16,(ExtStopCounter)
						cpi r16,10
						brcc EXT_STOP_RUN
						inc r16
EXT_STOP_SET_CNT:		sts (ExtStopCounter),r16
						rjmp PCI_ONE_EX
EXT_STOP_RUN:			ldi r16,3
						call setMode
//						ldi r16,2
//						sts (SOUNDNUMBER),r16
					rjmp PCI_ONE_EX
PCI_ONE_EX:	pop r17
			pop r16
			POPF
			ret
