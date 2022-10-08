//---------------------------------------------------------------------------------------------------------------------
// -- вывод символа из R16 шрифтом, на который ссылается переменная FontNo --
printSymbol:		PUSHF
					push r16
					push r17
					push r18
					push r19
					push r20
					push r0
					push r1
					push ZH
					push ZL
					cbi GPIOR0,1					;	бит пробела код $20
					cbi GPIOR0,0					;	бит полупробела код $00
						push r16
						lds r16,(FontNo)
						//andi r16,$03
						ldi r17,0
						LDZ (FontAdressesTable*2)
							add ZL,r16	adc ZH,r17
							add ZL,r16	adc ZH,r17
							lpm r16,Z+
							lpm r17,Z+
							add r16,r16
							adc r17,r17
							sts (FontAddr+0),r16
							sts (FontAddr+1),r17
							movw Z,r16				; указатель на начало шрифта
						pop r16
						//LDSZ FontAddr				; указатель на начало шрифта
						lpm r17,Z+					; код начального символа знакогенератора
						lpm r18,Z+					; код конечного символа знакогенератора (ЗГ)
						lpm r19,Z+					; ширина символа
						lpm r20,Z+					; высота символа
						cpi r16,' '					; если код = пробел, выводим в любом случае
						breq printSymbolSpace
						cpi r16,$00
						breq printSymbolHalfSpace
						cp r16,r17					; если код символа меньше нижней границы ЗГ,
						brcs printSymbolGotoExit	; то на выход
						cp r16,r18					; если код символа 
						breq printSymbolInRange		; равен верхней границе ЗГ
						brcs printSymbolInRange		; или меньше её, то выводим,
printSymbolGotoExit:	rjmp printSymbolExit		; если больше верхней границы ЗГ,то на выход

printSymbolHalfSpace:	sbi GPIOR0,0
printSymbolSpace:		sbi GPIOR0,1;set

printSymbolInRange:		cpi r16,$80
						brlo printSymbolENG			; До $7F - латинские
						cpi r16,$C0
						brlo printSymbolGotoExit	; промежуток $80-$BF - не выводим
printSymbolRUS:			cpi r17,$C0
						brcc printSymbolEng
						//cpi r17,$80
						//brcs shift40
shift40:				subi r16,$40				; от $C0 - русские
printSymbolENG:			push r16					; сохраним код символа
							sub r16,r17
							mul r19,r20					; вычисляем шаг символов в ЗГ
							mul r16,r0					; вычисляем смещение символа от начала ЗГ
							add ZL,r0	adc ZH,r1		; в Z указатель на начало шрифта
//----	
							lds r16,(POSX)
							sts (OLDX),r16			; сохраняем начальную координату X
							lds r16,(POSY)
							sts (OLDY),r16			; сохраняем начальную координату Y
						pop r16						; вспомним код символа
						cpi r16,'.'
						brne printSymbolNotComma
						clr r0
						//lsr r19	
						lsr r19
						adc r19,r0					; для точки (.) выводим только левую четверть образа символа
						//-------------- цикл по строкам -----------
printSymbolNotComma:	push r20								; сохраним значение высоты символа
printSymbolRowCycle:	call softI2CStart
						lds r16,LED_ADDRESS
						call softI2CSendByte
						ldi r16,datastream
						call softI2CSendByte
						//-------------- цикл по столбцам ----------
								push r19						; сохраним значение ширины символа
printSymbolColCycle:			lds r16,(POSX)
								cpi r16,128
								brcc printSymbolColCycleSkip	; если упёрлись в край экрана, печатать не будем
								inc r16
								sts (POSX),r16
								ldi r16,$00
									//brts printSymbolSendByte
								sbis GPIOR0,1
								lpm r16,Z+
printSymbolSendByte:				call softI2CSendByte
printSymbolColCycleCheck:		dec r19
								sbic GPIOR0,0
								dec r19
								brne printSymbolColCycle
								rjmp printSymbolColCycleEx
printSymbolColCycleSkip:		lpm r16,Z+
								rjmp printSymbolColCycleCheck
printSymbolColCycleEx:			pop r19							; восстановим значение ширины символа
						//-------------- конец цикла по столбцам ----------
						call softI2CStop
						dec r20
						breq printSymbolRowCycleEx
							lds r16,(OLDX)
							sts (POSX),r16
							lds r16,(OLDY)
							inc r16
							andi r16,$07
							sts (POSY),r16
							sts (OLDY),r16
							call SET_LED_XY
						rjmp printSymbolRowCycle
						//--------------
printSymbolRowCycleEx:	pop r20
						lds r16,(POSY)
						sub r16,r20
						inc r16
						andi r16,$07
						sts (POSY),r16
						call SET_LED_XY
						//-------------- конец цикла по строкам -----------
//----
printSymbolExit:	pop ZL
					pop ZH
					pop r1
					pop r0
					pop r20
					pop r19
					pop r18
					pop r17
					pop r16
					POPF
				ret
//---------------------------------------------------------------------------------------------------------------------
// -- Вывод строки из ПЗУ на дисплей ссылка на начало строки в Z --
printString:		PUSHF
					push r16
					push r17
					push XL
					push XH
					push YL
					push YH

					add ZL,ZL
					adc ZH,ZH
					clr r16
					sts (StringCounter),r16

printStringCycle:		lds r16,(StringCounter)
						cpi r16,128
						brcs printStringNext
						rjmp printStringExit
printStringNext:		inc r16
						sts (printStringNext),r16
						lpm r16,Z+
						cpi r16,EOS
						brne printStringNotEnd
						rjmp printStringExit

printStringNotEnd:		
						cpi r16,$80
						brcs printStringSymbol
						cpi r16,$C0
						brcc printStringSymbol
							
							call printExtras
							rjmp printStringCycle

printStringSymbol:		call printSymbol
						
						rjmp printStringCycle

printStringExit:	pop YH
					pop YL
					pop XH
					pop XL
					pop r17
					pop r16
					POPF
					ret	
//---------------------------------------------------------------------------------------------------------------------
// -- вывод значения переменной, коды $80-$BF (Usim)
printExtras:		movw X,Z
						LDZ printExtraRoutinesTable*2
						LDY ExtrasConfig
						push r16

							subi r16,$80
							ldi r17,0

							add ZL,r16	adc ZH,r17
							add ZL,r16	adc ZH,r17

							add YL,r16	adc YH,r17
							add YL,r16	adc YH,r17

							lpm r16,Z+
							lpm r17,Z

							movw ZH:ZL,r17:r16			;	адрес программы вывода

							lds r16,(POSX)
							std Y+0,r16

							lds r17,(POSY)
							andi r17,$07				;	позиция по Y биты 0..2
							lds r16,(FontNo)
							swap r16
							andi r16,0b00110000
							or r17,r16
							//bst r16,0	bld r17,4				;	размер шрифта биты 4,5 из бита 0,1
							//bst r16,1	bld r17,5
							SETB r17,6				;	разрешение к выводу бит 6
							CLRB r17,7				;	новое значение обнулить бит 7
							//ldd r16,Y+1
							//andi r16,$80
							//or r16,r17				;	 
							std Y+1,r17
						pop r16

					icall
//					ldi r16,$00
//					sts (MASK),r16					;	сбросили признак инверсии

					movw Z,X
					ret
//---------------------------------------------------------------------------------------------------------------------
// запрет отображения переменной (R16) на экране
disableOutput:			PUSHY
						push r16
						subi r16,$80
						clr r17
						LDY ExtrasConfig
						add YL,r16		adc YH,r17
						add YL,r16		adc YH,r17
						ldd r16,Y+1
						CLRB r16,6
						std Y+1,r16
						pop r16
						POPY			
						ret
//---------------------------------------------------------------------------------------------------------------------
// разрешение отображения переменной (R16) на экране
enableOutput:			PUSHY
						push r16
						subi r16,$80
						clr r17
						LDY ExtrasConfig
						add YL,r16		adc YH,r17
						add YL,r16		adc YH,r17
						ldd r16,Y+1
						SETB r16,6
						std Y+1,r16
						pop r16
						POPY
						ret
//---------------------------------------------------------------------------------------------------------------------
// запрет отображения всех переменных на экране
disableAllOutputs:		PUSHY
						push r16
						push r17
						LDY ExtrasConfig
						ldi r16,64
disableAllOutputsCycle:	tst r16
						breq disableAllOutputsEx
						dec r16
						ldd r17,Y+1
							CLRB r17,6				; запрет вывода бит 6
							CLRB r17,7
						std Y+1,r17
						adiw Y,2
						rjmp disableAllOutputsCycle
disableAllOutputsEx:	pop r17
						pop r16
						POPY
					ret
//---------------------------------------------------------------------------------------------------------------------
// -- проверка на предмет одного нового значения и вывод, если надо ------------------
checkForOneNewValue:	PUSHY
						LDY ExtrasConfig						
						push r16
							subi r16,$80
							clr r17
							add YL,r16	adc YH,r17
							add YL,r16	adc YH,r17
							ldd r16,Y+1
							andi r16,0b11000000		;	бит 6 - разрешение вывода, 7 - новое значение
							cpi  r16,0b11000000
						pop r16
							brne checkForOneNewValueEx
						push r16
							call getVarPrintConfig
							call printExtras

						pop r16
checkForOneNewValueEx:	POPY
						ret
//---------------------------------------------------------------------------------------------------------------------
// -- просмотр всех выводимых значений на предмет обновления ------
newValueScan:			push r16
						ldi r16,$88			;	начальный код сканирования

newValueScanCycle:		cpi r16,$90			;	конечный код сканирования
						brcc newValueScanEx

						call checkForOneNewValue
						inc r16
						rjmp newValueScanCycle

newValueScanEx:			pop r16
						ret
//---------------------------------------------------------------------------------------------------------------------
// установить флаг нового значения переменной, в R16 - код переменной
setNewValFlag:			PUSHY
						push r16
						push r17
					
						LDY ExtrasConfig
						subi r16,$80
						clr r17
						add YL,r16	adc YH,r17
						add YL,r16	adc YH,r17	
						ldd r17,Y+1	
						SETB r17,7					;	установить флаг нового значения бит 7
						std Y+1,r17

						pop r17
						pop r16
						POPY
						ret
//---------------------------------------------------------------------------------------------------------------------
// установить флаг инверсного вывода, в R16 - код
/*setInverseFlag:			PUSHY
						push r16
						push r17
					
						LDY ExtrasConfig
						subi r16,$80
						clr r17
						add YL,r16	adc YH,r17
						add YL,r16	adc YH,r17	
						ldd r17,Y+1	
						SETB r17,7					;	установить флаг
						std Y+1,r17
				
						pop r17
						pop r16
						POPY
						ret	*/
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
// -- Загружаем определённые ранее параметры вывода переменной, номер переменной в R16 --
getVarPrintConfig:
				push r16
				push r17
				PUSHY
				LDY ExtrasConfig
				subi r16,$80
				clr r17
				add YL,r16	adc YH,r17
				add YL,r16	adc YH,r17		;	Y - указатель на место отображения
				ldd r16,Y+0					;	POSX
				ldd r17,Y+1					;	POSY
				sts (POSX),r16
				sts (POSY),r17
				; ----------
				CLRB r17,7					;	сброс флага нового значения бит 7
				SETB r17,6					;	разрешение вывода на экран бит 6
				std Y+1,r17					;	
				; ----------
				//sbrc r17,7
				//ldi r16,$FF
				//sbrs r17,7
				//ldi r16,$00
				//sts (MASK),r16				; режим вывода прямой/инверсный
				; ----------
				swap r17
				andi r17,$03
				sts (FontNo),r17
		
				call SET_LED_XY

				POPY
				pop r17
				pop r16

				ret

//---------------------------------------------------------------------------------------------------------------------
						

