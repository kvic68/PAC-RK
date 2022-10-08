// ----------------------------------------------------------------
eachHundreedthSub:	//
					call sound
					ret
// ----------------------------------------------------------------
toneIndex:	.dw notone,tone0,tone1
// ----------------------------------------------------------------
notone:
tone0:	.db	133,125,118,112,105, 99, 94, 89, 84, 79, 74, 70, 66, 62, 59, 55
		.db  55, 59, 66, 74, 84, 94,105,118,133,$00,$00,$00,$00,$00,$00,$00
		.db $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
		.db $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
		.db $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
		.db $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
// ----------------------------------------------------------------
tone1:	.db  70, 70, 70, 70, 70, 70, 70, 70, 70, 70,$00,$00,$00,$00,$00,$00
		.db $00,$00,$00,$00, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70,$00,$00
		.db $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
		.db $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
		.db $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
		.db $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
// ----------------------------------------------------------------
// -- Извлекалка звуков -------------------------------------------
sound:				PUSHZ
					push r16
					push r17
					lds r16,(SOUNDNUMBER)
					tst r16
					brne soundChoice
					rjmp soundOffAndDisable

soundChoice:		LDZ toneIndex*2
					clr r17
					andi r16,$03
					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17
					lpm r16,Z+
					lpm r17,Z
					movw ZH:ZL,r17:r16
					add ZL,ZL
					adc ZH,ZH
					;LDZ tone0*2
					lds r16,(TONECOUNTER)
					ldi r17,$00
					add ZL,r16	adc ZH,r17
					inc r16
					;andi r16,$3F
					cpi r16,$60
					brcs skipZeroCounter
					clr r16
skipZeroCounter:	sts (TONECOUNTER),r16
					lpm r16,Z
					tst r16
					breq soundOffOnly

					sts OCR2A,r16
					ldi r16,soundOn
					sts TCCR2A,r16
	sbi portb,5
					rjmp soundExit

soundOffOnly:		ldi r16,soundOff
					sts TCCR2A,r16
	cbi portb,5
					rjmp soundExit

soundOffAndDisable:	ldi r16,soundOff
					sts TCCR2A,r16
					clr r16
					sts (TONECOUNTER),r16
	cbi portb,5
					rjmp soundExit

soundExit:			pop r17
					pop r16
					POPZ
					ret
//-----------------------------------------------------------------
changeCurrentRate:	push r16
					PUSHXYZ
					cpi r16,1
					breq ccr_M1
					ldi r16,0
					sts (CurrentRate+0),r16
					sts (CurrentRate+1),r16
					rjmp ccr_Ex
ccr_M1:				LDSX RateValue
					LDSY CurrentRate
					LDZ 100
					cp  YL,XL
					cpc YH,XH
					brcs C_lo_R
					breq set_CR
					rjmp C_gt_R
C_lo_R:				add YL,ZL
					adc YH,ZH
					cp  XL,YL
					cpc XH,YH
					brcs C_gt_R
					rjmp set_CR
C_gt_R:				movw Y,X
					rjmp set_CR
set_CR:				STSY CurrentRate
						call ocr1aCalc
ccr_Ex:				POPXYZ
					pop r16
					ret
// ----------------------------------------------------------------
eachTenthSub:	call KEY_ENHANCE_TIME_FOR_ALL_BUTTONS
				call newValueScan
//call TXmanager
			lds r16,(TIMEFLAGS)
			sbrc r16,3
			rjmp eachTenthEx

			lds r16,(KEY_FLAGS)
			sbrs r16,5
			rjmp eachTenthA
				CLRB r16,5
				sts (KEY_FLAGS),r16
				call SendRegData
				rjmp eachTenthChkFlagA

eachTenthA:		call LoadRegData
				lds r16,(DataFromStab)
				tst r16
				breq eachTenthChkFlagA

				ldi r16,$02
				call addTXQueue

eachTenthChkFlagA:	lds r16,(A_FLAGS)
					andi r16,$07

etsFlagA0		:	sbrs r16,2
					rjmp etsFlagA1
					;b0=0
					nop
						lds r16,(MSGNo+0)
						lds r17,(MSGNo+1)
						add r16,r17
						cpi r16,3
							breq eachTenthEx
						ldi r16,3
						ldi r17,0
						sts (MSGNo+0),r16
						sts (MSGNo+1),r17
						ldi r16,Message
						call setNewValFlag
					rjmp eachTenthEx

etsFlagA1:			sbrs r16,1
					rjmp etsFlagA2
					nop
						lds r16,(MSGNo+0)
						lds r17,(MSGNo+1)
						add r16,r17
						cpi r16,2
							breq eachTenthEx
						ldi r16,2
						ldi r17,0
						sts (MSGNo+0),r16
						sts (MSGNo+1),r17
						ldi r16,Message
						call setNewValFlag
					rjmp eachTenthEx

etsFlagA2:			sbrc r16,0
					rjmp etsFlagA3
					;b2=1
					nop
						lds r16,(MSGNo+0)
						lds r17,(MSGNo+1)
						add r16,r17
						cpi r16,1
							breq eachTenthEx
						ldi r16,1
						ldi r17,0
						sts (MSGNo+0),r16
						sts (MSGNo+1),r17
						ldi r16,Message
						call setNewValFlag
					rjmp eachTenthEx

etsFlagA3:			;b0=1,b1=0,b2=0
					clr r16
					sts(MSGNo+1),r16
					sts(MSGNo+0),r16
						ldi r16,Message
						call setNewValFlag
					rjmp eachTenthEx

eachTenthEx:
				

				ret
// ----------------------------------------------------------------
// ----------------------------------------------------------------
eachSecondSub:		call ds18ReadData
					call ds18NewConvert
skip_ds18:		lds r16,(BMPPresence)
				tst r16
				brne skip_bmp
				call BMP180_READ_D
				call BMP180_DECODE_NEW
				call calcAvgPressure
			//
skip_bmp:		call refreshControl
				call incSeconds

					ldi r16,Total
					call setNewValFlag

					ldi r16,Remain
					call setNewValFlag
					//
						lds r16,(MSGNo+0)
						lds r17,(MSGNo+1)
						cp r16,r17
						breq eachSecondSubM1
							sts (MsgNo+1),r16
							sts (MsgNo+0),r17
							ldi r16,Message
							call setNewValFlag
					//
eachSecondSubM1:
				ldi r16,$03
				call addTXQueue

				call returnToMainMenu
				ret
// ----------------------------------------------------------------
returnToMainMenu:	push r16
					push r17
					push r18.

					lds r16,(MODE)
					cpi r16,$00
					breq rtmmExit

					lds r16,(TAB_NUMBER)
					cpi r16,$05
					breq rtmmExit

					lds r16,(BACKCOUNTER0)
					cpi r16,60
					brcc rtmmRun
					inc r16
					sts (BACKCOUNTER0),r16
					rjmp rtmmExit

rtmmRun:			ldi r16,$00
					sts (BACKCOUNTER0),r16
					call setmode00

rtmmExit:			pop r18
					pop r17
					pop r16
					ret
// ----------------------------------------------------------------
incSeconds:		PUSHZ
				LDSZ SECONDS
				adiw Z,1
				STSZ SECONDS
				POPZ
				ret
// ----------------------------------------------------------------
// -- проверка на предмет одного нового значения ------------------
checkForOneNewValue:	PUSHY
						LDY positionTable						
						push r16
							subi r16,$80
							clr r17
							add YL,r16	adc YH,r17
							add YL,r16	adc YH,r17
							ldd r16,Y+1
							andi r16,0b01100000
							cpi  r16,0b01100000
						pop r16
							brne checkForOneNewValueEx
						push r16
							call printPositioning
							call printUsim

						pop r16
checkForOneNewValueEx:	POPY
						ret
// ----------------------------------------------------------------
// -- просмотр всех выводимых значений на предмет обновления ------
newValueScan:		ldi r16,$85

newValueScanCycle:	cpi r16,$C0
					brcc newValueScanEx

					call checkForOneNewValue
					inc r16
					rjmp newValueScanCycle

newValueScanEx:		ret
// ----------------------------------------------------------------
