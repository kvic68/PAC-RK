//==================================================================================================

refreshControl:
		PUSHF
		PUSHXYZ
		PUSHLOW
		push r16
		push r17
		push r18
; ----------
			ldi r16,0						; счетчик датчиков (номер датчика)
			mov r10,r16						; счетчик запросов на включение звука
			mov r11,r16						; счетчик запросов на стоп отбора
			mov r12,r16						; счетчик запросов на выключение нагрева
			ldi XH,0						; счетчик запросов на паузу
			ldi XL,0						; счетчик запросов на звук перегрева
			lds r0,(DS_STATUS)				; загружаем статус термодатчиков 0 - отсутствует, 1 - работает
			clr r1
			inc r1							; r1 - маска = 0000.0001 для побитовых извращений	

			LDZ DS_VALUE_0					; Z - указатель на значения термодатчиков (по 2 байта), DS_VALUE(x)
											; Z+8  - пороговые значения								DS_MARGIN(x)

			LDY DELAY_UP_0					; Y    - задержка включения							DELAY_UP(x)
											; Y+4  - задержка выключения						DELAY_DN(x)
											; Y+8  - код реакции на событие						REACTION(x)
											; Y+12 - обратные таймеры на срабатывание события	BACK_TIMER_T(x)

											; Y+16 - данные для вывода по действиям

	
			lds r17,(DRIVE)				; состояние выходов в R17
			andi r17,$0F					; только младшие 4 разряда
			; ----------
			; основной цикл начало
main_loop:	cpi r16,4
			brcs load_data
			rjmp main_exit					; выход по окончании перебора всех датчиков
load_data:		;inc r16						; увеличиваем счетчик основного цикла
				; загружаем данные для каждого датчика 
				ldd r9,Y+12					; обратный таймер
				ldd r8,Y+8					; код реакции
				ldd r7,Y+4					; задержка выкл
				ld  r6,Y					; задержка вкл
				; ----------
				ldd r2,Z+8					; BOUNDx LSB
				ld  r4,Z+					; DS18Tx LSB
				ldd r3,Z+8					; BOUNDx MSB
				ld  r5,Z+					; DS18Tx MSB
				; r5:r4 - текущая температура, r3:r2 - граничная температура
				; ----------
				ror r0						; С - состояние датчика 0 - хреновый, 1 - хороший
				brcs main_work				; если датчик работает - переходим к обработке
					// вывод -- хреновый датчик
							push r1					;	если выход во включенном состоянии
							and r1,r17				;	обрабатываем в обычногм порядке
							pop r1					;	задержка организована в 
							brne main_work			;	другом месте
				rjmp next_step				; иначе - к следующему датчику без изменений состояния 
				; ----------
main_work:			; если реация выключена
					sbrs r8,3
					rjmp noReaction				; то установить вывод в состояние "выключено"
					rjmp compareTB				; иначе переходим к сравнению температуры и порога
					; ----------
noReaction:				// вывод -- реакция выключена
setBitOff:			push r1						; выключаем ногу
					com r1
					and r17,r1	
					pop r1
					push r16
					push r17
						ldi r17,MARGIN0
						add r16,r17
						call clrInverseFlag		; отключаем инверсный вывод величины
						call setNewValFlag
					pop r17
					pop r16
					rjmp next_step
					; ----------
setBitOn:			or r17,r1					; включаем ногу
					push r16
					push r17
						ldi r17,MARGIN0
						add r16,r17
						call setInverseFlag		; включаем инверсный вывод величины
						call setNewValFlag
					pop r17
					pop r16
					rjmp next_step
				; ------------------------------
compareTB:			cp  r4,r2
					cpc r5,r3					; сравним пороговое значение и текущую температуру, C=1 при Т<порога, С=0 при Т>=порога
					brlt belowBound				; Т ниже границы
					rjmp aboveBound				; Т выше или равно границе

; ----------------------------------------------

				; -- Т ниже или равно порога срабатывания --
belowBound:			push r1
					and r1,r17					; проверим состояние выхода 0 - выкл (breq), 1 - вкл(brne)
					pop r1

					brne bePinIsOn_m1				; переход, если управляемый выход во включенном состоянии
					rjmp bePinIsOff_m1				; переход, если управляемый выход в выключенном состоянии

bePinIsOn_m1:		tst r9						; обратный таймер = 0 ?
					breq beDelayIs0_m1				; да - переходим вперед
					dec r9						; нет - уменьшаем значение таймера на 1
					std Y+12,r9					; сохраняем
					// на вывод - значение обратного таймера
					push r16
					push r17
						ldi r17,BACK_T0
						add r16,r17
						call setNewValFlag		; флаг нового значения обратного счетчика для вывода на экран
					pop r17
					pop r16
					//
					sbrc r8,0					;
					inc r10					; добавить запрос на включение звука
					sbrc r8,1					;
					inc r11					; добавить запрос на стоп отбора
					sbrc r8,2					;
					inc r12					; добавить запрос на выключение нагрева
bePinIsOn_m2:			
					rjmp next_step				; и переходим к следующему датчику

beDelayIs0_m1:		; сюда вставить отключение всех действий для текущего канала, r18 - код действия
				; ---------------------------------------------------------------------------------------
					push r16
					push r17
						ldi r17,MARGIN0
						add r16,r17
						call clrInverseFlag		; отключаем инверсный вывод величины
						call setNewValFlag
					pop r17
					pop r16
				; ---------------------------------------------------------------------------------------
					std Y+12,r6					; обратный таймер = задержка включения
						// на вывод - стрелка вверх
					rjmp setBitOff				; управляемый выход выключить
					;---------------------------------------------------------
					;---------------------------------------------------------
	
bePinIsOff_m1:		std Y+12,r6					; обратный таймер = задержка включения
						// на вывод - стрелка вверх
					rjmp next_step				; и переходим к следующему датчику

; ----------------------------------------------

				; -- T выше порога срабатывания --
aboveBound:			push r1
					and r1,r17					; проверим состояние выхода 0 - выкл (breq), 1 - вкл(brne)
					pop r1
					
					brne abPinIsOn_m1				; переход, если управляемый выход во включенном состоянии
					rjmp abPinIsOff_m1				; переход, если управляемый выход в выключенном состоянии

abPinIsOn_m1:		std Y+12,r7					; обратный таймер = задержка выключения
						// вывод - стрелки вниз
					sbrc r8,0					;
					inc r10					; добавить запрос на включение звука
					sbrc r8,1					;
					inc r11					; добавить запрос на стоп отбора
					sbrc r8,2					;
					inc r12					; добавить запрос на выключение нагрева

abPinIsOn_m2:
					rjmp next_step				; и переходим к следующему датчику
					;-------------------------------------------------------

abPinIsOff_m1:		tst r9						; обратный таймер = 0 ?
					breq abDelayIs0_m1				; да - переходим вперед
					dec r9						; нет - уменьшаем значение таймера на 1
					std Y+12,r9					; сохраняем
					// на вывод - значение таймера
					push r16
					push r17
						ldi r17,BACK_T0
						add r16,r17
						call setNewValFlag		; флаг нового значения обратного счетчика для вывода на экран
					pop r17
					pop r16
					//
abPinIsOff_m2:		rjmp next_step				; и переходим к следующему датчику

abDelayIs0_m1:		push r16
					push r17
						ldi r17,MARGIN0
						add r16,r17
						call setInverseFlag
						call setNewValFlag
					pop r17
					pop r16
					//--------------------
					sbrc r8,0					;
					inc r10					; добавить запрос на включение звука
					sbrc r8,1					;
					inc r11					; добавить запрос на стоп отбора
					sbrc r8,2					;
					inc r12					; добавить запрос на выключение нагрева

				; ---------------------------------------------------------------------------------------
abDelayIs0_m1a:		std Y+12,r7					; обратный таймер = задержка выключения
					// вывод - стрелки вниз
					rjmp setBitOn				; управляемый выход включить
					;---------------------------------------------------------
					;-------------------------------------------------------

; --------------------

next_step:			clc
					rol r1						; сдвигаем маску влево
					adiw Y,1					; продвигаем указатель данных
					inc r16						; увеличиваем счетчик основного цикла
					rjmp main_loop

; --------------------

main_exit:			sts (DRIVE),r17
					in r16,pinb
					andi r16,$F0
					com r17
					andi r17,$0F
					or r16,r17
					mov r17,r16
					out portb,r17				; изменяем линии порта
					;----------------------

					lds r16,(SOUNDNUMBER)
					tst r10						; нужен ли звук?
					breq me_no_alarm_sound
					ldi r16,$01					; звук нужен
					rjmp me_sv_alarm_sound
me_no_alarm_sound:	cpi r16,1
					brne me_sv_alarm_sound
					clr r16
me_sv_alarm_sound:	sts (SOUNDNUMBER),r16
					


					lds r16,(A_FLAGS)
					CLRB r16,2
					lds r17,(STABMODE)
					cpi r17,3
					breq me_stop
					tst r11						; нужен ли стоп отбора?
					breq me_nostop

me_stop:			SETB r16,2					; стоп нужен
me_nostop:			sts (A_FLAGS),r16

					tst r12						; нужно ли выключение нагрева?
					breq me_heat_on
					lds r17,(STABMODE)
					cpi r17,3
					breq me_heat_on
					push r16					; нагрев выключить
					ldi r16,$03
					LDY DataToStab
					std Y+2,r16
					ldi r16,$FF
					std Y+0,r16
					std Y+1,r16
					lds r16,(KEY_FLAGS)
					SETB r16,5
					sts (KEY_FLAGS),r16
					pop r16
					rjmp me_end
me_heat_on:						
; --------------------
; проверить и отключить разгон при необходимости
					lds r16,(HeatControlStatus)
					cpi r16,$00
					brne me_chk1
					rjmp me_end					; если контроль за разгоном отключен, то на выход

me_chk1:			lds r16,(HeatControlSensor)
					clr r17
					LDZ DS_VALUE
					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17
					ldd YL,Z+0
					ldd YH,Z+1					; в Y - температура контролирующего датчика
					ldi r16,$00
					ldi r17,$80
					cp  r16,YL
					cpc r17,YH
					brne me_chk2
					rjmp me_end					; если контролирующий датчик недоступен, то на выход

me_chk2:			LDZ HeatControlMargin
					ldd XL,Z+0
					ldd XH,Z+1					; в X порог отключения разгона
					cp  YL,XL
					cpc YH,XH
					brcc me_chk3
					rjmp me_end

me_chk3:			lds r16,(STABMODE)
					cpi r16,$01
					breq me_chk4
					rjmp me_end

me_chk4:			lds r16,(HeatControlCounter)
					cpi r16,3
					//breq me_stop_fire
					brcc me_stop_fire
					inc r16
					sts (HeatControlCounter),r16
					rjmp me_exit

me_stop_fire:		ldi r16,$00
					LDY DataToStab
					std Y+2,r16
					ldi r16,$FF
					std Y+0,r16
					std Y+1,r16
					lds r16,(KEY_FLAGS)
					SETB r16,5
					sts (KEY_FLAGS),r16
		
; --------------------
me_end:			ldi r16,$00
				sts (HeatControlCounter),r16
me_exit:	pop r18
			pop r17
			pop r16
			POPLOW
			POPXYZ
			POPF
	ret
; ----------

//==================================================================================================

