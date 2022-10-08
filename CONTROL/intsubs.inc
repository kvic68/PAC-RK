// ----------------------------------------------------------------
// отрабатываем прерывание 1мс
T0CompareA: PUSHF
			push r16
			push r17
			push r18
			// 1ms
			lds r17,(TIMEFLAGS)
			SETB r17,0
			sts (TIMEFLAGS),r17
			lds r16,(MILLISECONDS)
			cpi r16,9
			brcs T0Compare0
			// 10 ms
			SETB r17,1
			sts (TIMEFLAGS),r17
			ldi r16,-1
T0Compare0:	inc r16
			sts (MILLISECONDS),r16
			brcs T0CompareE
			;
			lds r16,(SANTISECONDS)
			cpi r16,9
			brcs T0Compare1
			// 100 ms
			SETB r17,2
			sts (TIMEFLAGS),r17
			ldi r16,-1
T0Compare1:	inc r16
			sts (SANTISECONDS),r16
			brcs T0CompareE
			;
			lds r16,(DECISECONDS)
			cpi r16,9
			brcs T0Compare2
			// 1000 ms
			SETB r17,3
			sts (TIMEFLAGS),r17
			ldi r16,-1
T0Compare2:	inc r16
			sts (DECISECONDS),r16
	
T0CompareE:	PUSHXYZ
			call encoder
			call pressButton
			POPXYZ
			pop r18
			pop r17
			pop r16
			POPF		
			reti
// ----------------------------------------------------------------
// выдача тактовых импульсов на драйвер ШД, определение момента автостопа
T1CompareA:	PUSHF
			PUSHXYZ
			PUSHHIGH
T1Compare0:	in r16,PIND
				sbrc r16,4		;	если на ноге высокий уровень,
				rjmp T1CompareH
				sbrs r16,4		;	если на ноге низкий уровень,
				rjmp T1CompareL
			rjmp T1CompareE

T1CompareH:	// загрузим OCR1A из переменной
			lds XL,(RateValue+0)
			lds XH,(RateValue+1)
			ldi r17,0
			cp  XL,r17
			cpc XH,r17
			brne T1CompH0	
			rjmp T1CompareE		;	если Rate = 0, то на выход

T1CompH0:	lds YL,(OCR1AValue+0)
			lds YH,(OCR1AValue+1)
			lds XL,(T1Prescaler)
			sts OCR1AH,YH		;	иначе
			sts OCR1AL,YL		;	перезагружаем OCR1A
			sts TCCR1B,XL		;	и коэффициент деления
			// тут проверки на признак паузы
			lds  r16,(A_FLAGS)
			sbrs r16,0			;	если установлен флаг, разрешающий шаги, то продолжаем
			rjmp T1CompareE		;	иначе выход

			sbrc r16,1			;	если не установлен флаг достижения обратным счетчиком 0, то продолжаем
			rjmp T1CompareE		;	иначе выход

			sbrc r16,2			;	если не установлен флаг срабатывания на температурный порог, то продолжаем
			rjmp T1CompareE		;	иначе выход

T1CompH1:	cbi portd,4			;	сбрасывем шагательную ногу в 0
			rjmp T1CompareE


T1CompareL:	sbi portd,4			;	устанавливаем в 1, завершая такт
			; увеличиваем счетчик шагов
			LDSX (STEPCOUNTER)
			LDSY (STEPCOUNTER+2)
			adiw X,1
			brcc PC+2
			adiw Y,1
			STSX (STEPCOUNTER)
			STSY (STEPCOUNTER+2)
				; проверка на предмет достижения счётчиком 100 литров
/*				LDSA (stepsFor100ml)
				LDI_B 1000
				PUSHLOW
				call MUL24x16U
				POPLOW
				call MOVW32_BA

				LDSA (STEPCOUNTER)
				call CP_AB
			brcs T1CompL0
			; сто литров превращаются в нуль
			clr r16
			mov XL,r16	mov XH,r16	mov YL,r16	mov YH,r16
			STSX (STEPCOUNTER)
			STSY (STEPCOUNTER+2)
*/
					//--------------------//
T1CompL0:			ldi r16,StepsCount
					call setNewValFlag
					//--------------------//
			; уменьшаем обратный счетчик дозатора
			LDSX (STEPBACKCOUNTER)
			LDSY (STEPBACKCOUNTER+2)
			ldi r17,0
			cp  XL,r17
			cpc XH,r17
			cpc YL,r17
			cpc YH,r17
			brne T1CompL1		;	если не 0, то идём на уменьшение счетчика
			rjmp T1CompareE		;	иначе пропускаем
T1CompL1:	sbiw X,1
			brcc PC+2
			sbiw Y,1
			STSX (STEPBACKCOUNTER)
			STSY (STEPBACKCOUNTER+2)
			ldi r17,0
			cp  XL,r17
			cpc XH,r17
			cpc YL,r17
			cpc YH,r17
			brne T1CompL2
				lds r16,(A_FLAGS)			;	если это последний шаг,
				SETB r16,1					;	устанавливаем флаг достижения нуля 
				sts (A_FLAGS),r16			;	обратным счетчиком шагов
				call stepBackCounterCalc	;	перезагружаем обратный счетчик

				//ldi r16,2
				//sts (MSGNo+0),r16
				//clr r16
				//sts (MSGNo+1),r16
				//ldi r16,Message
				//call setNewValFlag
				ldi r16,$02
				sts (SOUNDNUMBER),r16
					//--------------------//
T1CompL2:			;ldi r16,StepsBCount
					;call setNewValFlag
					//--------------------//
T1CompareE:	
			POPHIGH
			POPXYZ
			POPF
			reti
// ----------------------------------------------------------------
