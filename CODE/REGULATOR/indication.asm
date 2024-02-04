indication:		; вывод возможен только тогда, когда сброшен флаг блокировки вывода
						lds r16,(FL_LOCK)
						tst r16
						breq indicationRun
indicationExit:		ret
//-------------------------------------------------------
indicationRun:			ldi r16,$FF
						sts (FL_LOCK),r16

						lds r16,LED_ADDRESS
						cpi r16,$FF
						breq TM1637prepVal		;	FF - адрес семисегментного на TM1637

						cpi r16,$00
						breq indicationEnd		;	00 - нет индикатора, отображать не на чем

							call newValueScan	;	все остальные адреса подразумевают матричный индикатор 128х64

indicationEnd:			ldi r16,$00
						sts (FL_LOCK),r16
					ret
//---------------------------------------------------------------------------------------------------------------------
TM1637prepVal:			lds r16,(MODE)
						cpi r16,0
							breq TMMode0
						cpi r16,1
							breq TMMode1
						cpi r16,2
							breq TMMode2
						cpi r16,3
							breq TMMode3
						rjmp TMModeX
				//	Норма
TMMode0:				lds r16,(FL_MAX_U)
						tst r16
						brne TMMode0A

						lds r16,(ExtrasConfig+2*(TargetV-$80)+1)
						sbrs r16,7
						rjmp TM1637prepValEx
						CLRB r16,7
						sts (ExtrasConfig+2*(TargetV-$80)+1),r16

						lds r22,Target_U+0
						lds r23,Target_U+1
						rjmp TM1637prepCommon

TMMode0A:				
						lds r16,(ExtrasConfig+2*(OutputV-$80)+1)
						sbrs r16,7
						rjmp TM1637prepValEx
						CLRB r16,7
						sts (ExtrasConfig+2*(OutputV-$80)+1),r16
						
						lds r22,Average_U+0
						lds r23,Average_U+1
						rjmp TM1637prepCommon
				//	Разгон	
TMMode1:				ldi r16,$29	sts(DIGIT3),r16
						ldi r16,$28	sts(DIGIT2),r16
						ldi r16,$33	sts(DIGIT1),r16
						ldi r16,$2B	sts(DIGIT0),r16
						rjmp TM1637prepCommonA
						
						lds r22,Average_U+0
						lds r23,Average_U+1
						rjmp TM1637prepCommon

				//	Стоп
TMMode2:				ldi r16,$29	sts(DIGIT3),r16
						ldi r16,$3C	sts(DIGIT2),r16
						ldi r16,$3E	sts(DIGIT1),r16
						ldi r16,$3E	sts(DIGIT0),r16
						rjmp TM1637prepCommonA
				//	Авария
TMMode3:				ldi r16,$28	sts(DIGIT3),r16
						ldi r16,$38	sts(DIGIT2),r16
						ldi r16,$28	sts(DIGIT1),r16
						ldi r16,$29	sts(DIGIT0),r16
						rjmp TM1637prepCommonA
				//	Неизвестный режим
TMModeX:				ldi r16,$3B	sts(DIGIT3),r16
						ldi r16,$3B	sts(DIGIT2),r16
						ldi r16,$3B	sts(DIGIT1),r16
						ldi r16,$3B	sts(DIGIT0),r16	
						rjmp TM1637prepCommonA

TM1637prepCommon:		clr r24
						clr r25
						call A32toBCD9
						call HideZeroes
						lds r16,(DIGIT1)
						ori r16,$80				;	десятичная точка
						sts (DIGIT1),r16

TM1637prepCommonA:		call TM1637sendString
TM1637prepValEx:							
					rjmp indicationEnd
//---------------------------------------------------------------------------------------------------------------------
TM1637sendString:		PUSHF
						push r17
						push r16
						PUSHZ


						lds r16,(MODE)
						cpi r16,3
						brne TM1637sendStringA

						call softI2CStart
						lds r16,(BRIGHTNESS)
						dec r16
						andi r16,$07
						sts (BRIGHTNESS),r16
						ori r16,$88
						call mirrorbyte
						call softI2CSendByte
						call softI2CStop
						rjmp TM1637sendStringB

TM1637sendStringA:		call softI2CStart
						ldi r16,$8A
						call mirrorbyte
						call softI2CSendByte
						call softI2CStop

TM1637sendStringB:		M_DELAY_US_RA 48,r17
						call softI2CStart
						ldi	r16,$C0				;УСТАНОВКА НАЧАЛЬНОГО АДРЕСА (0хC0 - крайнее левое знакоместо)
						call mirrorByte
						call softI2CSendByte


					ldi ZL, low(D7FONT*2)
					ldi ZH,high(D7FONT*2)

						lds r16,(DIGIT3)
						call TM1637recode
						call mirrorByte
						call softI2CSendByte

						lds r16,(DIGIT2)
						call TM1637recode
						call mirrorByte
						call softI2CSendByte

						lds r16,(DIGIT1)
						call TM1637recode
						call mirrorByte
						call softI2CSendByte

						lds r16,(DIGIT0)
						call TM1637recode
						call mirrorByte
						call softI2CSendByte

						call softI2CStop

						POPZ
						pop r16
						pop r17
						POPF
						ret
//---------------------------------------------------------------------------------------------------------------------
TM1637recode:			PUSHZ

						bst r16,7
						andi r16,$7F
						cpi r16,$20
						brcs TM1637recodeA
						cpi r16,$40
						brcc TM1637recodeA
						subi r16,$20						
						rjmp TM1637recodeB						
TM1637recodeA:			ldi r16,$00

TM1637recodeB:			add ZL,r16
						ldi r16,0
						adc ZH,r16
						lpm r16,Z
						bld r16,7

						POPZ
						ret
//---------------------------------------------------------------------------------------------------------------------
TM1637init:				call	softI2CStart		;маркер начала посылки
						ldi		r16,0x8C			;включение дисплея. Яркость 88-8F
						call	mirrorByte
						call	softI2CSendByte		;вывожу команду
						rcall	softI2CStop			;маркер конца посылки
						M_DELAY_US_RA 48,r17
						M_DELAY_US_RA 48,r17
						;включение режима передачи данных с автоинкрементом адреса
						rcall	softI2CStart		;маркер начала посылки
						ldi		r16,0x40			;режим передачи данных с автоинкрементом адреса
						call 	mirrorByte
						rcall	softI2CSendByte		;вывожу команду
						rcall	softI2CStop			;маркер конца посылки
						M_DELAY_US_RA 48,r17
						M_DELAY_US_RA 48,r17

				ret
//---------------------------------------------------------------------------------------------------------------------
DIGITS7:		.db	$3F,$06,$5B,$4F,$66,$6D,$7D,$07,$7F,$6F,$00,$80,$5C,$F9,$71,$00
//---------------------------------------------------------------------------------------------------------------------
D7FONT:			.db $00,$00,$00,$00,$00,$00,$00,$00	//	$20-$27
				.db $77,$73,$5F,$31,$00,$00,$00,$00	//	$28-$2F A,P,a,Г
				.db $3F,$06,$5B,$4F,$66,$6D,$7D,$07	//	$30-$37	0,1,2,3,4,5,6,7
				.db $7F,$6F,$00,$80,$5C,$F9,$71,$00	//	$38-$3F	8.9, ,.,o,E.,F
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
//	зеркальное отображение байта в r16
mirrorByte:			push r17
					push r18  
					ldi r18, 8
mirrorByteCycle:	ror r16
  					rol r17
  					dec r18
  					brne mirrorByteCycle
					mov r16,r17
					pop r18
					pop r17
					ret
//-------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
	
