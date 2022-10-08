//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
nothingToPrint:		ret
//----------------------------------------------------- 81
selectFont:			movw Z,X
						lpm r16,Z+			; читаем номер шрифта
						andi r16,$03
						sts (FontNo),r16	; и записываем его в нужное место
					movw X,Z
					ret
//----------------------------------------------------- 82
drawLine:			movw Z,X
					push r18
					push r19
						lpm r17,Z+	; координата Y
						lpm r18,Z+	; начало X
						lpm r19,Z+	; конец X
						lpm r16,Z+	; образец заполнения
					push r16
						sts (POSX),r18
						sts (POSY),r17
						call SET_LED_XY
						call softI2CStart
						lds r16,LED_ADDRESS
						call softI2CSendByte
						ldi r16,datastream
						call softI2CSendByte
					pop r16

drawLineCycle:			cp r18,r19
						breq drawLineExit
						brcc drawLineExit
						push r16
							call softI2CSendByte
						pop r16
						inc r18
						rjmp drawLineCycle
drawLineExit:		call softI2CStop
					pop r19
					pop r18
					movw X,Z
					ret
//----------------------------------------------------- 83
setXY:				movw Z,X
						lpm r16,Z+			; читаем X
						cpi r16,128
						brcs setXYnext
						ldi r16,127
setXYnext:				sts POSX,r16
						lpm r16,Z+			; читаем Y
						andi r16,$07
						sts POSY,r16
					movw X,Z
						call SET_LED_XY		; позиционируем
					ret
//----------------------------------------------------- 84
setdXdY:			movw Z,X
						lds r16,(POSX)
						lpm r17,Z+
						add r16,r17
						cpi r16,128
						brcs setdXdYnext
						ldi r16,127
setdXdYnext:			sts (POSX),r16
						lds r16,(POSY)
						lpm r17,Z+
						add r16,r17
						andi r16,$07
						sts (POSY),r16
					movw X,Z
						call SET_LED_XY
					ret
//----------------------------------------------------- 85
// позиционирование следующего управляющего кода (#8A-#BF) без разрешения вывода
disableNextExtra:	PUSHY
					push r16
					push r17
					movw Z,X
					lpm r16,Z+
					movw X,Z
					cpi r16,$C0
					brcc disableNextEx
					cpi r16,$8A
					brcs disableNextEx

					subi r16,$80
					clr r17
					LDY ExtrasConfig
					add YL,r16		adc YH,r17
					add YL,r16		adc YH,r17
					//	ldd r16,Y+1
					//	andi r16,0b11011111
					//	std Y+1,r16
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
							CLRB r17,6				;	запрет вывода бит 6
							CLRB r17,7				;	новое значение обнулить бит 7
							//ldd r16,Y+1
							//andi r16,$80
							//or r16,r17				;	 
							std Y+1,r17


disableNextEx:		pop r17
					pop r16
					POPY
					ret
//----------------------------------------------------- 88
printRange:			//call getVarPrintConfig
						lds r16,(FL_RANGE)
						cpi r16,$00
						breq printNoRange
						ldi r16,$3C		call printSymbol
						ldi r16,$3D		call printSymbol
						rjmp printRangeEx
printNoRange:			ldi r16,$3A		call printSymbol
						ldi r16,$3B		call printSymbol
printRangeEx:
					ret
//----------------------------------------------------- 89
printTargetV:		LDY TARGET_U
					rjmp commonPrintV
//----------------------------------------------------- 8A 
printOutputV:		LDY AVERAGE_U
					rjmp commonPrintV
//-----------------------------------------------------
commonPrintV:		ldd r22,Y+0
					ldd r23,Y+1
					clr r24
					clr r25
					call A32toBCD9
					call HideZeroes
					lds r16,(DIGIT3)	call printSymbol
					lds r16,(DIGIT2)	call printSymbol
					lds r16,(DIGIT1)	call printSymbol
					
					lds r16,(FontNo)
					cpi r16,1
					brne commonPrintV_A

					lds r16,(DIGIT0)	subi r16,(-10)	call printSymbol
					rjmp commonPrintV_Ex

commonPrintV_A:		ldi r16,'.'			call printSymbol
					lds r16,(DIGIT0)	call printSymbol
					rjmp commonPrintV_Ex
commonPrintV_Ex:
					ret
//----------------------------------------------------- 
// ----------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
// ---------- Таблица вызовов подпрограмм печати спецсимволов (#80-#BF) ----------
printExtraRoutinesTable:
			.dw	nothingToPrint			; #80 - конец строки	
			.dw	selectFont				; #81 - переключить на шрифт No, номер в следующем байте
			.dw	drawLine				; #82 - нарисовать линию. строка, начальный Х, конечный Х, паттерн	
			.dw	setXY					; #83 - позиционировать XY абсолютно, два следующих байта координаты	
			.dw	setdXdY					; #84 - позиционировать XY относительно, два следующих байта смещение
			.dw disableNextExtra		; #85 - Отключить вывод кода, следующего за.	
			.dw nothingToPrint			; #86 -
			.dw nothingToPrint			; #87 - 

			.dw printRange				; #88 - Индикатор стабилизации 	
			.dw printTargetV			; #89 - Вольтметр целевого напряжения
			.dw printOutputV			; #8A - Вольтметр измеренного напряжения	

			.dw nothingToPrint			; #8B - 
			.dw nothingToPrint			; #8C - 
			.dw nothingToPrint			; #8D - 
			.dw nothingToPrint			; #8E - 
			.dw nothingToPrint			; #8F - 

			.dw nothingToPrint			; #90 - 
			.dw nothingToPrint			; #91 - 
			.dw nothingToPrint			; #92 - 
			.dw nothingToPrint			; #93 - 
			.dw nothingToPrint			; #94 - 
			.dw nothingToPrint			; #95 - 
			.dw nothingToPrint			; #96 - 
			.dw nothingToPrint			; #97 - 
			.dw nothingToPrint			; #98 - 
			.dw nothingToPrint			; #99 - 
			.dw nothingToPrint			; #9A - 
			.dw nothingToPrint			; #9B - 
			.dw nothingToPrint			; #9C - 
			.dw nothingToPrint			; #9D - 
			.dw nothingToPrint			; #9E - 
			.dw nothingToPrint			; #9F - 

			.dw nothingToPrint			; #A0 - 
			.dw nothingToPrint			; #A1 - 
			.dw nothingToPrint			; #A2 - 
			.dw nothingToPrint			; #A3 - 

			.dw nothingToPrint			; #A4 - 
			.dw nothingToPrint			; #A5 - 
			.dw nothingToPrint			; #A6 - 
			.dw nothingToPrint			; #A7 - 
			.dw nothingToPrint			; #A8 - 
			.dw nothingToPrint			; #A9 - 
			.dw nothingToPrint			; #AA - 
			.dw nothingToPrint			; #AB
			.dw nothingToPrint			; #AC
			.dw nothingToPrint			; #AD
			.dw nothingToPrint			; #AE
			.dw nothingToPrint			; #AF
			.dw nothingToPrint			; #B0
			.dw nothingToPrint			; #B1
			.dw nothingToPrint			; #B2
			.dw nothingToPrint			; #B3
			.dw nothingToPrint			; #B4
			.dw nothingToPrint			; #B5
			.dw nothingToPrint			; #B6
			.dw nothingToPrint			; #B7
			.dw nothingToPrint			; #B8
			.dw nothingToPrint			; #B9
			.dw nothingToPrint			; #BA
			.dw nothingToPrint			; #BB
			.dw nothingToPrint			; #BC
			.dw nothingToPrint			; #BD
			.dw nothingToPrint			; #BE
			.dw nothingToPrint			; #BF 
			
