// ----------------------------------------------------------------
// ----------------------------------------------------------------
rotateEnc:		push r16
				push r17
				PUSHZ
					LDZ rotateSubsTable*2
					lds r16,(MODE)
					clr r17
					andi r16,$07
					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17
					lpm r16,Z+
					lpm r17,Z
					movw ZH:ZL,r17:r16
					icall
				POPZ
				pop r17
				pop r16
			ret
// ----------------------------------------------------------------
shortKeyPress:	push r16
				push r17
				PUSHZ
					LDZ shortKeyPressSubsTable*2
					lds r16,(MODE)
					clr r17
					andi r16,$07
					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17
					lpm r16,Z+
					lpm r17,Z
					movw ZH:ZL,r17:r16
					icall
				POPZ
				pop r17
				pop r16
				ret
// ----------------------------------------------------------------
longKeyHold:	push r16
				push r17
				PUSHZ
					LDZ longKeyHoldSubsTable*2
					lds r16,(MODE)
					clr r17
					andi r16,$07
					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17
					lpm r16,Z+
					lpm r17,Z
					movw ZH:ZL,r17:r16
					icall
				POPZ
				pop r17
				pop r16
				ret
// ----------------------------------------------------------------
nothingToDo:	ret
// ----------------------------------------------------------------
// ----------  Список режимов (MODE)  ----------
.equ	mainmode	=	$00	; - основной режим
.equ	mode01		=	$01	; - режим выбора датчика
.equ	mode02		=	$02	; - режим выбора пункта меню
.equ	mode03		=	$03	; - режим изменения значения в выбранном пункте меню
.equ	mode04		=	$04	; - 
.equ	mode05		=	$05	; - 
.equ	mode06		=	$06	;
.equ	mode07		=	$07	;
// ----------------------------------------------------------------
rotateSubsTable:
			.dw	changeRateValue			;	00 - основной режим, изменение скорости отбора
			.dw	changeTuningTab			;	01 - выбор температурного датчика(0-3) или наливайки(4)
			.dw	changeItemSelection		;	02 - смена пункта меню
			.dw	changeItemValue			;	03 - изменение значения в пункте меню
			.dw	nothingToDo				;	04
			.dw	nothingToDo				;	05
			.dw	nothingToDo				;	06
			.dw	nothingToDo				;	07
// ----------------------------------------------------------------
shortKeyPressSubsTable:
			.dw	togglePause				;	00 - основной режим, переход на паузу / снятие с паузы
			.dw	setMode02				;	01 - переход к выбору пункта меню (2)
			.dw	setMode03				;	02 - переход к режиму изменения значения пункта меню (3)
			.dw	clickInMode03			;	03 - возврат из режима 3 в режим 2
			.dw	nothingToDo				;	04 
			.dw	nothingToDo				;	05
			.dw	nothingToDo				;	06
			.dw	nothingToDo				;	07
// ----------------------------------------------------------------
longKeyHoldSubsTable:
			.dw	setMode01				;	00 - переход в режим настройки температурных датчиков и дозатора (1)
			.dw	setMode00				;	01 - переход в основной режим с записью изменений в eeprom (0)
			.dw	leaveMode02				;	02 - возврат из режима 2 в режим 1
			.dw	longHoldMode03			;	03 - если изменяем порог, то устанавливаем его на 2 шага вверх
			.dw	nothingToDo				;	04
			.dw	nothingToDo				;	05
			.dw	nothingToDo				;	06
			.dw	nothingToDo				;	07
// ----------------------------------------------------------------
// -- выбор подпрограммы изменения значения выбранной переменной --
changeItemValue:	push r16
					push r17
					PUSHZ
						lds r16,(ITEM_No)
						lds r17,(TAB_NUMBER)
						rol r17
						rol r17
						add r16,r17
						clr r17
						cpi r17,(6*4+4)
						brcc changeItemValueEx
						//andi r16,$1F
						LDZ changeItemValueSubsTable*2
						add ZL,r16	adc ZH,r17
						add ZL,r16	adc ZH,r17
						lpm r16,Z+
						lpm r17,Z
						movw ZH:ZL,r17:r16
						icall
changeItemValueEx:	POPZ
					pop r17
					pop r16
					ret
// ----------------------------------------------------------------
// изменение значения параметра ITEM на вкладке TAB при вращении энкодера
changeItemValueSubsTable:				; Tab-Item
			.dw changeMargin			;	0-0 - порог 		Т0
			.dw changeReaction			;	0-1 - действие 		Т0
			.dw changeDelayUp			;	0-2 - задержка вкл 	Т0
			.dw changeDelayDn			;	0-3 - задержка выкл Т0

			.dw changeMargin			;	1-0 - порог 		Т1
			.dw changeReaction			;	1-1 - действие 		Т1
			.dw changeDelayUp			;	1-2 - задержка вкл 	Т1
			.dw changeDelayDn			;	1-3 - задержка выкл Т1

			.dw changeMargin			;	2-0 - порог 		Т2
			.dw changeReaction			;	2-1 - действие 		Т2
			.dw changeDelayUp			;	2-2 - задержка вкл 	Т2
			.dw changeDelayDn			;	2-3 - задержка выкл Т2

			.dw changeMargin			;	3-0 - порог 		Т3
			.dw changeReaction			;	3-1 - действие 		Т3
			.dw changeDelayUp			;	3-2 - задержка вкл 	Т3
			.dw changeDelayDn			;	3-3 - задержка выкл Т3

			.dw nothingToDo				;	4-0 - отмена(RL) / сбросить общий счётчик(RR)
			.dw nothingToDo				;	4-1 -  
			.dw changeDoseValue			;	4-2 - изменить дозировку - +
			.dw resumeWork				;	4-3 - изменить вариант действия

			.dw changeRateValue			;	5-0 - скорость налива в режиме калибровки
			.dw nothingToDo				;	5-1 - указание истинно налитого количества
			.dw nothingToDo				;	5-2 - 
			.dw nothingToDo				;	5-3 - 

			.dw heatControlModeChange	;	6-0 - контроль разгона вкл/выкл
			.dw heatSensorSelect		;	6-1 - выбор датчика контроля разгона
			.dw heatMarginChange		;	6-2 - выбор порога отключения разгона
			.dw nothingToDo				;	6-3 - запись в долговременную память параметров контроля разгона

			.dw nothingToDo				;	7-0 - 
			.dw nothingToDo				;	7-1 - 
			.dw nothingToDo				;	7-2 - 
			.dw nothingToDo				;	7-3 - 

// ----------------------------------------------------------------
// -- выбор порога контроля разгона
heatMarginChange:		PUSHZ
						PUSHY
						;LDY HeatControlMargin
						LDZ HeatControlMargin
						ldd YL,Z+0
						ldd YH,Z+1

						lds r16,(KEY_FLAGS)
						sbrc r16,0
						rjmp heatMarginChangeRR		;rotateRight +
							sbrc r16,1
							rjmp heatMarginChangeRL	;rotateLeft -
						rjmp heatMarginChangeEx

heatMarginChangeRR:		adiw Y,1
						std Z+0,YL
						std Z+1,YH
						rjmp heatMarginChangeEx

heatMarginChangeRL:		sbiw Y,1
						std Z+0,YL
						std Z+1,YH
						rjmp heatMarginChangeEx

heatMarginChangeEx:		
						ldi r16,HMargin
						call setNewValFlag
						ldi r16,HString
						call setNewValFlag
						POPY
						POPZ
						ret
// ----------------------------------------------------------------
// -- выбор датчика контроля разгона
heatSensorSelect:		lds r17,(HeatControlSensor)
						lds r16,(KEY_FLAGS)
						sbrc r16,0
						rjmp heatSensorSelectRR
						sbrc r16,1
						rjmp heatSensorSelectRL
						rjmp heatSensorSelectEX

heatSensorSelectRR:		inc r17
						rjmp heatSensorSelectEX
heatSensorSelectRL:		dec r17

heatSensorSelectEX:		andi r17,$03
						sts (HeatControlSensor),r17
						ldi r16,HSensor
						call setNewValFlag
						ldi r16,HSensorVal
						call setNewValFlag
						ldi r16,HString
						call setNewValFlag
						ret
// ----------------------------------------------------------------
// -- включение/выключение контроля разгона
heatControlModeChange:	lds r16,(HeatControlStatus)
						cpi r16,$00
						breq HCMC00
						cpi r16,$FF
						breq HCMCFF
						ldi r16,$00
						rjmp HCMCSet
HCMC00:					ldi r16,$FF
						rjmp HCMCSet
HCMCFF:					ldi r16,$00
HCMCSet:				sts (HeatControlStatus),r16
						ldi r16,HStatus
						call setNewValFlag
						ldi r16,HString
						call setNewValFlag
						ret
// ----------------------------------------------------------------
// -- изменение реакции на событие --
changeReaction:			//
						lds r16,(TAB_NUMBER)
						ldi r17,0
						LDX REACTION_0
						add XL,r16
						adc XH,r17

						lds r16,(KEY_FLAGS)
						sbrc r16,0
						rjmp changeReactionRR
						sbrc r16,1
						rjmp changeReactionRL
						rjmp changeReactionEX

changeReactionRR:		ld r16,X
						inc r16
						andi r16,$0F;(03)
						;st X,r16
						rjmp changeReactionNorm

changeReactionRL:		ld r16,X
						dec r16
						andi r16,$0F;(03)
						;st X,r16

changeReactionNorm:		cpi r16,$01
						breq changeReactionM1
						cpi r16,$08
						brcs changeReactionM2
						rjmp changeReactionM3
changeReactionM1:		ldi r16,$08
						rjmp changeReactionM3
changeReactionM2:		ldi r16,$00
						rjmp changeReactionM3

changeReactionM3:		st X,r16
					
changeReactionEX:		lds r16,(TAB_NUMBER)
						ldi r17,REACT_0
						add r16,r17
						call setNewValFlag
						ret
// ----------------------------------------------------------------
// -- изменение задержки выключения --
changeDelayDn:			//
						lds r16,(TAB_NUMBER)
						ldi r17,0
						LDX DELAY_DN_0
						add XL,r16
						adc XH,r17

						lds r16,(KEY_FLAGS)
						sbrc r16,0
						rjmp changeDelayDnRR
						sbrc r16,1
						rjmp changeDelayDnRL
						rjmp changeDelayDnEX
changeDelayDnRR:		ld r16,X
						cpi r16,20
						brcc changeDelayDnEX
						inc r16
						st X,r16
						rjmp changeDelayDnEX
changeDelayDnRL:		ld r16,X
						tst r16
						breq changeDelayDnEX
						dec r16
						st X,r16
						
changeDelayDnEX:		lds r16,(TAB_NUMBER)
						ldi r17,DEL_DN0
						add r16,r17
						call setNewValFlag
						ret
// ----------------------------------------------------------------
// -- изменение задержки включения --
changeDelayUp:			//
						lds r16,(TAB_NUMBER)
						ldi r17,0
						LDX DELAY_UP_0
						add XL,r16
						adc XH,r17

						lds r16,(KEY_FLAGS)
						sbrc r16,0
						rjmp changeDelayUpRR
						sbrc r16,1
						rjmp changeDelayUpRL
						rjmp changeDelayUpEX
changeDelayUpRR:		ld r16,X
						cpi r16,20
						brcc changeDelayUpEX
						inc r16
						st X,r16
						rjmp changeDelayUpEX
changeDelayUpRL:		ld r16,X
						tst r16
						breq changeDelayUpEX
						dec r16
						st X,r16
						
changeDelayUpEX:		lds r16,(TAB_NUMBER)
						ldi r17,DEL_UP0
						add r16,r17
						call setNewValFlag
						ret
// ----------------------------------------------------------------
// -- выбор вариантов продолжения работы --
resumeWork:				lds r16,(KEY_FLAGS)
						sbrc r16,0
						rjmp resumeWorkRR	; rotateRight
						sbrc r16,1
						rjmp resumeWorkRL	; rotateLeft
						rjmp resumeWorkEx
resumeWorkRR:				lds r16,(MSGEx)
							subi r16,-1
							andi r16,$03
							rjmp resumeWorkEx
resumeWorkRL:				lds r16,(MSGEx)
							subi r16,1
							andi r16,$03
							rjmp resumeWorkEx
resumeWorkEx:			sts (MSGEx),r16
						ldi r16,MessageEx
						call setNewValFlag
						ret
// ----------------------------------------------------------------
// -- сброс общего счетчика --
resetTotal:				;lds r16,(KEY_FLAGS)
						;sbrc r16,0
						;rjmp resetTotalRR
						;sbrc r16,1
						;rjmp resetTotalRL
						;rjmp resetTotalEX
resetTotalRR:			clr r16
						sts (STEPCOUNTER+0),r16
						sts (STEPCOUNTER+1),r16
						sts (STEPCOUNTER+2),r16
						sts (STEPCOUNTER+3),r16
							ldi r16,StepsCount
							call setNewValFlag
						call stepBackCounterCalc
resetTotalRL:			call clickInMode03
resetTotalEX:			ret
// ----------------------------------------------------------------
// -- изменение порога срабатывания --
changeMargin:			PUSHY
						PUSHZ
						push r16
						push r17
					
						lds r16,(TAB_NUMBER)
						clr r17
						LDZ DS_MARGIN_0
						add ZL,r16	adc ZH,r17
						add ZL,r16	adc ZH,r17
						ldd YL,Z+0
						ldd YH,Z+1

						lds r16,(KEY_FLAGS)
						sbrc r16,0
						rjmp changeMarginValueRR		;rotateRight +
							sbrc r16,1
							rjmp changeMarginValueRL	;rotateLeft -
						rjmp changeMarginValueEx

changeMarginValueRR:		adiw Y,1
						std Z+0,YL
						std Z+1,YH
						rjmp changeMarginValueEx

changeMarginValueRL:		sbiw Y,1
						std Z+0,YL
						std Z+1,YH
						rjmp changeMarginValueEx

changeMarginValueEx:	lds r16,(TAB_NUMBER)
						ldi r17,MARGIN0
						add r16,r17
						call setNewValFlag

						pop r17
						pop r16
						POPZ
						POPY
						ret
// ----------------------------------------------------------------
// -- длинное удержание в режима изменения переменной --
longHoldMode03:			//
						lds r16,(TAB_NUMBER)
						cpi r16,$04
						brcc longHoldMode03Chk4
						rjmp longHoldMode03T0_3
						//
longHoldMode03Chk4:		brne longHoldMode03Chk5
							lds r16,(ITEM_No)
							cpi r16,$00
							brne longHoldMode03ChkE
							call resetTotal
						rjmp longHoldMode03ChkE
						//
longHoldMode03Chk5:		cpi r16,$05
						brne longHoldMode03ChkE
						rjmp longHoldMode03T5
longHoldMode03ChkE:		rjmp longHoldMode03Ex
						//
longHoldMode03T0_3:		// удержание на пороге дает порогу +2 шага сетки
						lds r16,(ITEM_No)
						tst r16
						breq longHoldMode03T0_3I0
						rjmp longHoldMode03Ex
longHoldMode03T0_3I0:	lds r16,(TAB_NUMBER)
						clr r17
						LDZ DS_VALUE_0
						add ZL,r16		adc ZH,r17
						add ZL,r16		adc ZH,r17

;						LDY DS_MARGIN_0
;						add YL,r16		adc YH,r17
;						add YL,r16		adc YH,r17

						ldd XL,Z+0
						ldd XH,Z+1
						LDY $8000
						cp XL,YL	cpc XH,YH
						brne longHoldMode03T0_3I1
						rjmp longHoldMode03T0_3I2

longHoldMode03T0_3I1:	LDY DS_MARGIN_0
						add YL,r16		adc YH,r17
						add YL,r16		adc YH,r17
						adiw X,2
						std Y+0,XL
						std Y+1,XH
						ldi r17,MARGIN0
						add r16,r17
						call setNewValFlag
longHoldMode03T0_3I2:	rjmp longHoldMode03Ex
						//
longHoldMode03T5:		// удержание даёт сброс количества шагов при ITEM_No=0
						lds r16,(ITEM_No)
						tst r16
						breq longHoldMode03T5I0
						rjmp longHoldMode03T5Chk3
longHoldMode03T5I0:		LDSX RateValue
						or XL,XH
						brne longHoldMode03Ex	// сброс только при скорости 0
						clr r16
						sts (STEPCOUNTER+0),r16
						sts (STEPCOUNTER+1),r16
						sts (STEPCOUNTER+2),r16
						sts (STEPCOUNTER+3),r16
						ldi r16,StepsCount
						call setNewValFlag
						LDX 0
						STSX DoseValue 
						call stepBackCounterCalc
						ldi r16,Dose
						call setNewValFlag
						rjmp longHoldMode03Ex
longHoldMode03T5Chk3:	cpi r16,$03
						breq longHoldMode03T5I3
						rjmp longHoldMode03Ex
longHoldMode03T5I3:		call save100mlStepsCount
						rjmp longHoldMode03Ex	
longHoldMode03Ex:
						ret
// ----------------------------------------------------------------
// -- включение режима изменения значения выбранной переменной --
setMode03:			ldi r16,$03
					sts (MODE),r16

					lds r16,(TAB_NUMBER)
					cpi r16,$05
					breq setMode03T5I0
					rjmp setMode03_0
					// ---------------------------
setMode03T5I0:		lds r16,(ITEM_No)
					cpi r16,$00
					brne setMode03T5I1
						// вход в первый пункт меню закладки №5
						LDX 0
 						STSX RateValue
						call ocr1aCalc
						LDSX DoseValue
						STSX DoseValueRC
						LDX 0
						STSX DoseValue 
						call stepBackCounterCalc
						call releasePause
					rjmp setMode03_0

setMode03T5I1:		lds r16,(ITEM_No)
					cpi r16,$01
					brne setMode03T5I2
 					//
					rjmp setMode03_0

setMode03T5I2:		lds r16,(ITEM_No)
					cpi r16,$02
					brne setMode03T5I3
 					//
					rjmp setMode03_0

setMode03T5I3:		lds r16,(ITEM_No)
					cpi r16,$03
					brne setMode03T5I4
 					//
					rjmp setMode03_0

setMode03T5I4:		nop			
					// --------------------------
setMode03_0:		lds r16,(ITEM_No)

					cpi r16,$00
					brne setMode03_1
					LDZ ITEM0_CHANGE
					rjmp setMode03Ex

setMode03_1:		cpi r16,$01
					brne setMode03_2
					LDZ ITEM1_CHANGE
					rjmp setMode03Ex

setMode03_2:		cpi r16,$02
					brne setMode03_3
					LDZ ITEM2_CHANGE
					rjmp setMode03Ex

setMode03_3:		cpi r16,$03
					brne setMode03_4
					LDZ ITEM3_CHANGE
					rjmp setMode03Ex

setMode03_4:		rjmp setMode03Ex

setMode03Ex:		call printstring
					ret
// ----------------------------------------------------------------
// -- переход из режима изменения значения переменной к режиму выбора переменной
clickInMode03:		lds r16,(TAB_NUMBER)
					cpi r16,$04
					brcc clickInMode03A
					rjmp clickInMode03Def
clickInMode03A:		cpi r16,$04
					brne clickInMode03B
					rjmp clickInMode03T4		;
clickInMode03B:		cpi r16,$05
					brne clickInMode03C
					rjmp clickInMode03T5		;
clickInMode03C:		rjmp clickInMode03Def
					// ------------------
clickInMode03T4:	lds r16,(ITEM_No)
					cpi r16,$03
					breq clickInMode03T4I3
					rjmp clickInMode03Def
					// -------------------------------------------------
					// -- отработка варианта выхода из режима настройки отбора -- TAB_NUMBER=04 ITEM_No=03 
clickInMode03T4I3:		lds r16,(MSGEx)								  		//
							cpi r16,$00									  	//
						brne clickInMode03_1
						call releasePause								  	//
						call setMode00wos										//
						rjmp clickInMode03Ex							  	//
						//
clickInMode03_1:			cpi r16,$01									  	//
						brne clickInMode03_2							  	//
						call setPause							  		//
						call setMode00wos						  	  		//
						rjmp clickInMode03Ex							  	//
						//
clickInMode03_2:			cpi r16,$02									  	//
						brne clickInMode03_3							  	//
						rjmp clickInMode03Def						      	//
						rjmp clickInMode03Ex							  	//
						//
clickInMode03_3:			cpi r16,$03									  	//
						brne clickInMode03_4							  	//
						ldi r16,$01		sts (MODE),r16				  		//
						ldi r16,$05		sts (TAB_NUMBER),r16			  		//
						ldi r16,$00		sts (ITEM_No),r16			  		//
						call setMode01_0							  		//
						rjmp clickInMode03Ex							  	//
						//
clickInMode03_4:		rjmp clickInMode03Ex							  	//
					// -------------------------------------------------
					// -- короткое нажатие на вкладке калибровки в режиме изменения переменной -- TAB_NUMBER=5 
clickInMode03T5:	lds r16,(ITEM_No)
					cpi r16,$00
					brne clickInMode03T5A
						rjmp clickInMode03T5I0
clickInMode03T5A:	cpi r16,$01
					brne clickInMode03T5B
						rjmp clickInMode03T5I1
clickInMode03T5B:	cpi r16,$02
					brne clickInMode03T5C
						rjmp clickInMode03T5I2
clickInMode03T5C:	cpi r16,$03
					brne clickInMode03T5D
						rjmp clickInMode03T5I3
clickInMode03T5D:	rjmp clickInMode03Def
					// ------------------- ITEM_No=0
clickInMode03T5I0://	LDSX RateValue
				//	or XL,XH
				//	brne clickInMode03T5I0a
				//	call setPause
				//		LDSX DoseValueRC
				//		STSX DoseValue
				//		call stepBackCounterCalc
			//	ldi r16,$00
			//	sts (SOUNDNUMBER),r16
			//		rjmp clickInMode03Def
clickInMode03T5I0a:	LDX 0
					STSX RateValue
					call ocr1aCalc
				ldi r16,$00
				sts (SOUNDNUMBER),r16
					rjmp clickInMode03Def
					// ------------------- ITEM_No=1
clickInMode03T5I1:	
					rjmp clickInMode03Def
					// ------------------- ITEM_No=2
clickInMode03T5I2:	
					rjmp clickInMode03Def
					// ------------------- ITEM_No=3
clickInMode03T5I3:	
					rjmp clickInMode03Def
					// -------------------
					rjmp clickInMode03Def
					// -------------------------------------------------
clickInMode03Def:		ldi r16,$02
					sts (MODE),r16
					call setMode02_0
clickInMode03Ex:		ret
// ----------------------------------------------------------------
// -- включение режима выбора переменной --
setMode02:			ldi r16,$02
					sts (MODE),r16

					clr r16
					sts (ITEM_No),r16

					lds r16,(TAB_NUMBER)
					cpi r16,$04
					breq setMode02T4
					rjmp setMode02_0

setMode02T4:		call setPause		; сюда попадаем в режиме настройки отбора
					ldi r16,$03		
					sts (ITEM_No),r16
					ldi r16,$00
					sts (MSGEx),r16
						ldi r16,MessageEx
						call setNewValFlag


setMode02_0:		lds r16,(ITEM_No)

					cpi r16,$00
					brne setMode02_1
					LDZ ITEM0_SELECT
					rjmp setMode02Ex

setMode02_1:		cpi r16,$01
					brne setMode02_2
					LDZ ITEM1_SELECT
					rjmp setMode02Ex

setMode02_2:		cpi r16,$02
					brne setMode02_3
					LDZ ITEM2_SELECT
					rjmp setMode02Ex

setMode02_3:		cpi r16,$03
					brne setMode02_4
					LDZ ITEM3_SELECT
					rjmp setMode02Ex

setMode02_4:
setMode02Ex:		call printstring
					ret
// ----------------------------------------------------------------
// -- выбор переменной --
changeItemSelection:	push r18
						push r17
						push r16
						lds r18,(TAB_NUMBER)
						lds r17,(ITEM_No)
						lds r16,(KEY_FLAGS)
						sbrc r16,0
						rjmp changeItemSelRR		;rotateRight
							sbrc r16,1
							rjmp changeItemSelRL	;rotateLeft
						rjmp changeItemSelEx
changeItemSelRR:		cpi r17,$03
						brne changeItemSelRR1
						rjmp changeItemSelEx1
changeItemSelRR1:		// TAB_NUMBER=5,ITEM_No=0 => следующее ITEM_No=3
						ldi r16,$05	cp r18,r16	ldi r16,$00	cpc r17,r16
						brne changeItemSelRR2
						ldi r17,$02
						rjmp changeItemSelRR3
						// TAB_NUMBER=4,ITEM_No=0 => следующее ITEM_No=2
changeItemSelRR2:		ldi r16,$04	cp r18,r16	ldi r16,$00	cpc r17,r16
						brne changeItemSelRR3
						ldi r17,$01
changeItemSelRR3:		inc r17
						rjmp changeItemSelEx
changeItemSelRL:		tst r17
						brne changeItemSelRL1
							call leaveMode02
						rjmp changeItemSelEx1
changeItemSelRL1:		// TAB_NUMBER=5,ITEM_No=3 => следующее ITEM_No=0
						ldi r16,$05	cp r18,r16	ldi r16,$03	cpc r17,r16
						brne changeItemSelRL2
						ldi r17,$01
						rjmp changeItemSelRL3
						// TAB_NUMBER=4,ITEM_No=2 => следующее ITEM_No=0
changeItemSelRL2:		ldi r16,$04	cp r18,r16	ldi r16,$02	cpc r17,r16
						brne changeItemSelRL3
						ldi r17,$01
changeItemSelRL3:		dec r17
changeItemSelEx:		andi r17,$03
						sts (ITEM_No),r17
						call setMode02_0
changeItemSelEx1:		pop r16
						pop r17
						pop r18
						ret
// ----------------------------------------------------------------
// -- переход из режима выбора переменной к режиму выбора датчика --
leaveMode02:		ldi r16,$01
					sts (MODE),r16
					LDZ ITEMS_UNSELECT
					call printstring
					ret
// ----------------------------------------------------------------
// -- установка основного режима --
setMode00:			call saveDefaultValues	;	запись изменённых значений в EEPROM
setMode00wos:		ldi r16,$00
					sts (MODE),r16
					call clearlcd
					LDZ MAINSCREEN
					call printstring
					ret
// ----------------------------------------------------------------
// -- включение режима выбора датчика для настройки --
setMode01:			ldi r16,$01
					sts (MODE),r16
					//call clearlcd

					ldi r17,$04
					lds r16,(A_FLAGS)
					sbrc r16,1
					sts (TAB_NUMBER),r17

setMode01_0:		lds r16,(TAB_NUMBER)

					cpi r16,$00
					brne setMode01_1
					LDZ MENU_T0
					rjmp setMode01Ex

setMode01_1:		cpi r16,$01
					brne setMode01_2
					LDZ MENU_T1
					rjmp setMode01Ex

setMode01_2:		cpi r16,$02
					brne setMode01_3
					LDZ MENU_T2
					rjmp setMode01Ex

setMode01_3:		cpi r16,$03
					brne setMode01_4
					LDZ MENU_T3
					rjmp setMode01Ex

setMode01_4:		cpi r16,$04
					brne setMode01_5
					LDZ MENU_PUMP
					rjmp setMode01Ex

setMode01_5:		cpi r16,$05
					brne setMode01_6
					LDZ TUNE_PUMP
					rjmp setMode01Ex

setMode01_6:		cpi r16,$06
					brne setMode01_7
					LDZ FULL_POWER
					rjmp setMode01Ex

setMode01_7:

setMode01Ex:			call clearlcd
						call printstring
					LDZ ITEMS_UNSELECT
					call printstring
					ret
// ----------------------------------------------------------------
// -- смена датчика, подлежащего настройке --
changeTuningTab:	push r16
					push r17
					lds r17,(TAB_NUMBER)
					lds r16,(KEY_FLAGS)
					sbrc r16,0
					rjmp changeTuningTabRR		;rotateRight
						sbrc r16,1
						rjmp changeTuningTabRL	;rotateLeft
					rjmp changeTuningTabEx

changeTuningTabRR:	cpi r17,$06	///////
					brcs changeTuningTabRR1
					ldi r17,$FF
changeTuningTabRR1:	inc r17
					cpi r17,5
					breq changeTuningTabRR1
					sts (TAB_NUMBER),r17
					call setMode01_0
					rjmp changeTuningTabEx	
									
changeTuningTabRL:	cpi r17,$00
					brne changeTuningTabRL1
					ldi r17,$07 ///////
changeTuningTabRL1:	dec r17
					cpi r17,5
					breq changeTuningTabRL1
					sts (TAB_NUMBER),r17
					call setMode01_0
					rjmp changeTuningTabEx
					
changeTuningTabEx:	pop r17
					pop r16
					ret
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
setPause:			lds r16,(A_FLAGS)
					
					CLRB r16,0
					CLRB r16,1
					sts (A_FLAGS),r16

					ret
// ----------------------------------------------------------------
releasePause:		lds r16,(A_FLAGS)
					SETB r16,0
					CLRB r16,1
					sts (A_FLAGS),r16

					ldi r16,$00
					sts (SOUNDNUMBER),r16

					ret
// ----------------------------------------------------------------
togglePause:		push r16
					push r17

					lds r16,(SOUNDNUMBER)
					tst r16
					breq togglePauseRun
					clr r16
					sts (SOUNDNUMBER),r16
					rjmp togglePauseEnd

togglePauseRun:		lds r16,(A_FLAGS)
					sbrc r16,2				;	 если стоп по перегреву, ждем охлаждения
					rjmp togglePauseEnd

					mov r17,r16
					andi r16,$07
					cpi r16,$01					;	если нет никаких пауз, ставим паузу от оператора
					breq togglePauseOn

					mov r16,r17
					SETB r16,0
					andi r16,0b11111001		;	иначе снимаем все варианты пауз
					rjmp togglePauseSet

togglePauseOn:		mov r16,r17
					andi r16,0b11111000
togglePauseSet:		sts (A_FLAGS),r16
					
togglePauseEnd:		pop r17
					pop r16
					ret
					
// ----------------------------------------------------------------
changeDoseValue	:	lds r16,(KEY_FLAGS)
					sbrc r16,0
					rjmp changeDoseValueRR		;rotateRight
						sbrc r16,1
						rjmp changeDoseValueRL	;rotateLeft
					rjmp changeDoseValueEx

changeDoseValueRR:	call increaseDoseValue
					rjmp changeDoseValueEx		;rotateEx

changeDoseValueRL:	call decreaseDoseValue
changeDoseValueEx:	call stepBackCounterCalc
						ldi r16,Remain
						call setNewValFlag
					ret
// ----------------------------------------------------------------
increaseDoseValue:	PUSHF
					PUSHHIGH
					lds r22,DoseValue+0
					lds r23,DoseValue+1
					ldi r24,0
					ldi r25,0
					LDI_B 5000				;	максимальный разовый объем
					call CP_AB
					brcs idv1000_5000
					ldi r22, low(5000)
					ldi r23,high(5000)
					LDI_B 0
					rjmp idvAdd
idv1000_5000:		LDI_B 1000				;	граница между большим и средним шагом
					call CP_AB
					brcs idv100_1000
					LDI_B 100				;	большой шаг
					rjmp idvAdd
idv100_1000:		LDI_B 100				;	граница между средним и малым шагом
					call CP_AB
					brcs idv0_100
					LDI_B 50				;	средний шаг
					rjmp idvAdd
idv0_100:			LDI_B 10				;	малый шаг
idvAdd:				call ADD3232
					sts DoseValue+0,r22
					sts DoseValue+1,r23
					ldi r16,Dose
					call setNewValFlag

					POPHIGH
					POPF
					ret
// ----------------------------------------------------------------
decreaseDoseValue:	PUSHF
					PUSHHIGH
					lds r22,DoseValue+0
					lds r23,doseValue+1
					ldi r24,0
					ldi r25,0
					LDI_B 10				;	минимальный разовый объем
					call CP_AB
					brcs doseMinAndBelow	;	меньше
					breq doseMinAndBelow	;	или равно
					rjmp doseAboveMin
doseMinAndBelow:	ldi r22, low(0)
					ldi r23,high(0)
					LDI_B 0
					rjmp ddvSub
doseAboveMin:		LDI_B 100
					call CP_AB
					brcs doseBelowAndMid		
					breq doseBelowAndMid
					rjmp doseAboveMid
doseBelowAndMid:	LDI_B 10
					rjmp ddvSub
doseAboveMid:		LDI_B 1000
					call CP_AB
					brcs doseBelowAndMax
					breq doseBelowAndMax
					rjmp doseAboveMax
doseBelowAndMax:	LDI_B 50
					rjmp ddvSub
doseAboveMax:		LDI_B 100
ddvSub:				call SUB3232
					sts doseValue+0,r22
					sts doseValue+1,r23

					sts RemainV+0,r22
					sts RemainV+1,r23

					ldi r16,Dose
					call setNewValFlag

					POPHIGH
					POPF
					ret
// ----------------------------------------------------------------
changeRateValue	:	lds r16,(KEY_FLAGS)
					sbrc r16,0
					rjmp changeRateValueRR;rotateRight
						sbrc r16,1
						rjmp changeRateValueRL;rotateLeft
					rjmp changeRateValueEx

changeRateValueRR:	call increaseRateValue
					rjmp changeRateValueEx;rotateEx

changeRateValueRL:	call decreaseRateValue
changeRateValueEx:	ret
// ----------------------------------------------------------------
increaseRateValue:	PUSHF
					PUSHHIGH
					lds r22,RateValue+0
					lds r23,RateValue+1
					ldi r24,0
					ldi r25,0
					LDI_B maxRateValue-100				;	максимальная скорость - максимальный шаг
					call CP_AB
					brcs irv1000_2000
					ldi r22, low(maxRateValue)
					ldi r23,high(maxRateValue)
					LDI_B 0
					rjmp irvAdd
irv1000_2000:		LDI_B 1000				;	граница между большим и средним шагом
					call CP_AB
					brcs irv200_1000
					LDI_B 100				;	большой шаг
					rjmp irvAdd
irv200_1000:		LDI_B 200				;	граница между средним и малым шагом
					call CP_AB
					brcs irv0_200
					LDI_B 50				;	средний шаг
					rjmp irvAdd
irv0_200:			LDI_B 10				;	малый шаг
irvAdd:				call ADD3232
					sts RateValue+0,r22
					sts RateValue+1,r23

					call ocr1aCalc
					ldi r16,$01
					call addTXQueue

					POPHIGH
					POPF
					ret
// ----------------------------------------------------------------
decreaseRateValue:	PUSHHIGH
					lds r22,RateValue+0
					lds r23,RateValue+1
					ldi r24,0
					ldi r25,0
					LDI_B minRateValue				;	минимальная скорость
					call CP_AB
					brcs rateMinAndBelow	;	меньше
					breq rateMinAndBelow	;	или равно
					rjmp rateAboveMin

rateMinAndBelow:	ldi r22, low(0)
					ldi r23,high(0)
					LDI_B 0
					rjmp drvSub

rateAboveMin:		LDI_B 200
					call CP_AB
					brcs rateBelowAndMid		
					breq rateBelowAndMid
					rjmp rateAboveMid

rateBelowAndMid:	LDI_B 10
					rjmp drvSub

rateAboveMid:		LDI_B 1000
					call CP_AB
					brcs rateBelowAndMax
					breq rateBelowAndMax
					rjmp rateAboveMax

rateBelowAndMax:	LDI_B 50
					rjmp drvSub

rateAboveMax:		LDI_B 100

drvSub:				call SUB3232
					sts RateValue+0,r22
					sts RateValue+1,r23

					call ocr1aCalc

					ldi r16,$01
					call addTXQueue
		
					POPHIGH
					ret
// ----------------------------------------------------------------
// ----------------------------------------------------------------
stepBackCounterCalc:	PUSHHIGH
						
						LDSA stepsFor1ml
						lds r18,(DoseValue+0)
						lds r19,(DoseValue+1)
						call MUL24x16U

						STSA STEPBACKCOUNTER
							ldi r16,Dose
							call setNewValFlag
						POPHIGH
						ret
// ----------------------------------------------------------------
ocr1aCalc:			PUSHHIGH
					LDI_A 3600000000;
					LDSB stepsFor1ml
					call DIV3232U
						lds r18,RateValue+0
						lds r19,RateValue+1
						clr r20	clr r21
						cp  r18,r20	
						cpc r19,r20
						brne ocr1aCalcNotZero
						LDI_A 65535
						rjmp ocr1aCalcEnd

ocr1aCalcNotZero:	call DIV3232U
					clr r0
					cp  r24,r0
					cpc r25,r0
					brne setT1div64
					ldi XL,T1Div8
					rjmp ocr1aCalcEnd

setT1div64:			LDI_B 8
					call DIV3232U
					clr r0
					cp  r24,r0
					cpc r25,r0
					brne setT1Div256
					ldi XL,T1Div64
					rjmp ocr1aCalcEnd

setT1Div256:		LDI_B 8
					call DIV3232U
					ldi XL,T1Div256
					rjmp ocr1aCalcEnd

ocr1aCalcEnd:		LDI_B 1
					call SUB3232

					ldi r16,T1Disable
					sts TCCR1B,r16
						lds r24,(OCR1AValue+0)
						lds r25,(OCR1AValue+1)
						sts (OCR1AValue+0),r22
						sts (OCR1AValue+1),r23
						cp  r22,r24
						cpc r23,r25
						breq noNewOCR

							sts OCR1AH,r23			;	
							sts OCR1AL,r22			;	перезагружаем OCR1A
noNewOCR:
						sts (T1Prescaler),XL
						sts TCCR1B,XL


ocr1aCalcExit:		
					ldi r16,Rate
						call setNewValFlag
					POPHIGH
					ret
// ----------------------------------------------------------------
