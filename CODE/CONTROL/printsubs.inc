// ----------------------------------------------------
// -- ����� ������� �� R16 �������, �� ������� ��������� ���������� FontNo --
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
					cbi GPIOR0,1					;	�������� ���(1) ������� ��� $20
					cbi GPIOR0,0					;	�������� ���(0) ����������� ��� $00
						push r16
						lds r16,(Font_SIZE)
						andi r16,$01
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
							movw Z,r16				; ��������� �� ������ ������
						pop r16
						//LDSZ FontAddr				; ��������� �� ������ ������
						lpm r17,Z+					; ��� ���������� ������� ���������������
						lpm r18,Z+					; ��� ��������� ������� ��������������� (��)
						lpm r19,Z+					; ������ �������
						lpm r20,Z+					; ������ �������
						cpi r16,' '					; ���� ��� = ������, ������� � ����� ������
						breq printSymbolSpace
						cpi r16,$00
						breq printSymbolHalfSpace
						cp r16,r17					; ���� ��� ������� ������ ������ ������� ��,
						brcs printSymbolGotoExit	; �� �� �����
						cp r16,r18					; ���� ��� ������� 
						breq printSymbolInRange		; ����� ������� ������� ��
						brcs printSymbolInRange		; ��� ������ �, �� �������,
printSymbolGotoExit:	rjmp printSymbolExit		; ���� ������ ������� ������� ��,�� �� �����

printSymbolHalfSpace:	sbi GPIOR0,0
printSymbolSpace:		sbi GPIOR0,1;set

printSymbolInRange:		cpi r16,$80
						brlo printSymbolENG			; �� $7F - ���������
						cpi r16,$C0
						brlo printSymbolGotoExit	; ���������� $80-$BF - �� �������
printSymbolRUS:			cpi r17,$C0
						brcc printSymbolEng
						//cpi r17,$80
						//brcs shift40
shift40:				subi r16,$40				; �� $C0 - �������, ��������, ���������� � ������ $80
printSymbolENG:			push r16					; �������� ��� �������
							sub r16,r17					; ��������� ����� ������� � ���������������
							mul r19,r20					; ��������� ��� �������� � ��
							mul r16,r0					; ��������� �������� ������� �� ������ ��
							add ZL,r0	adc ZH,r1		; � Z ��������� �� ������ ������
//----	
							lds r16,(DISPLAY_POSX)
							sts (DISPLAY_POSX_OLD),r16			; ��������� ��������� ���������� X
							lds r16,(DISPLAY_POSY)
							sts (DISPLAY_POSY_OLD),r16			; ��������� ��������� ���������� Y
						pop r16						; �������� ��� �������
						cpi r16,'.'
						brne printSymbolNotComma	; ���� ������ �� �����, �� ����������
						clr r0
						lsr r19						; ��������� ������ � 2 ����
						//lsr r19						; � ��� � ��� ����
						adc r19,r0					; ��������� ������� /��� ����� (.) ������� ����������� ������/
						//-------------- ���� �� ��������� Y -----------
printSymbolNotComma:	push r20								; �������� �������� ������ �������
printSymbolRowCycle:	//-------------- ���� �� ����������� X ----------
								push r19						; �������� �������� ������ �������
printSymbolColCycle:			lds r16,(DISPLAY_POSX)
									ldi r16,$00					; ������ ��� ����������, ��������� ������ 
								sbis GPIOR0,1
								lpm r16,Z+
printSymbolSendByte:				call BYTE_TO_LCD

printSymbolColCycleCheck:		dec r19
								sbic GPIOR0,0
								dec r19
								brne printSymbolColCycle

								rjmp printSymbolColCycleEx
printSymbolColCycleSkip:		lpm r16,Z+
								rjmp printSymbolColCycleCheck
printSymbolColCycleEx:			pop r19							; ����������� �������� ������ �������
						//-------------- ����� ����� �� ����������� ----------
						dec r20
						breq printSymbolRowCycleEx
							lds r16,(DISPLAY_POSX_OLD)
							sts (DISPLAY_POSX),r16
							lds r16,(DISPLAY_POSY_OLD)
							inc r16
							andi r16,$07
							sts (DISPLAY_POSY),r16
							sts (DISPLAY_POSY_OLD),r16
											call SET_LED_XY
						rjmp printSymbolRowCycle
						//--------------
printSymbolRowCycleEx:	pop r20
						lds r16,(DISPLAY_POSY)
						sub r16,r20
						inc r16
						andi r16,$07
						sts (DISPLAY_POSY),r16
											call SET_LED_XY
						//-------------- ����� ����� �� ��������� -----------
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
SET_LED_XY:			lds r16,(DISPLAY_POSX)
					lds r17,(DISPLAY_POSY)
					call SET_POS
				ret
// ----------------------------------------------------
// ----------------------------------------------------
// ������� �� R16 �������� �� (FONT_SIZE)
printChar:		rjmp printSymbol
// ----------------------------------------------------
// -- ������� ������, ������������� �� ������ Z, ������ �� 150 ���������� ������ ��� �� ������� EOS, �������� (FontNo) 00-6x8,01-12x16 ---
// -- ���� $80-$BF ���������������� ��� �����������.
printString:	PUSHF
				PUSHXYZ
				push r16
				push r17
				push r18

				add ZL,ZL
				adc ZH,ZH
				clr r16
				sts (CHAR_COUNTER),r16

prstrCycle:		lds r16,(CHAR_COUNTER)
				cpi r16,150
				brcs prstrCont
				rjmp prstrExit
				; ----------
prstrCont:		inc r16
				sts (CHAR_COUNTER),r16
				lpm r16,Z+

					cpi r16,EOS			;	80
					brcs prstrChar
					brne fontSize
					rjmp prstrExit

fontSize:			cpi r16,FONT		;	81
					brne prXY
					call setFontSize
					rjmp prstrCycle

prXY:				cpi r16,XY			;	83
					brne prdXdY
					call printSetXY
					rjmp prstrCycle

prdXdY:				cpi r16,dXdY		;	84
					brne prSetInv
					call printSetdXdY
					rjmp prstrCycle

prSetInv:			cpi r16,Inv			;	85
					brne prDisable
					call setInverse
					rjmp prstrCycle

prDisable:			cpi r16,disable		;	86
					brne prstrCont1
					call disableNext
					rjmp prstrCycle

prstrCont1:		cpi r16,$C0
				brcc prstrChar			;	C0...FF
				rjmp prstrUsym			;	85...BF
				; ----------
prstrChar:		cpi r16,$00
prstrChar1:		call printChar
				rjmp prstrCycle
				; ----------

prstrUsym:		call printUsim
				rjmp prstrCycle

				; ----------
prstrExit:		pop r18
				pop r17
				pop r16
				POPXYZ
				POPF
				ret
// ----------------------------------------------------
// -- ����� ���������� ������������ ������� �� R16 ----
printUsim:		;movw X,Z
				PUSHF
				PUSHXYZ	
				PUSHHIGH			
				LDZ printVectorsTable*2
				LDY positionTable
					push r16

					subi r16,$80
					ldi r17,0

					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17

					add YL,r16	adc YH,r17
					add YL,r16	adc YH,r17

						lpm r16,Z+
						lpm r17,Z

					movw ZH:ZL,r17:r16				;	����� ��������� ������

						lds r16,(DISPLAY_POSX)
						andi r16,$7F				;	������� �� X ���� 0..6
							std Y+0,r16

						lds r17,(DISPLAY_POSY)
						andi r17,$07				;	������� �� Y ���� 0..2
						lds r16,(FONT_SIZE)
						swap r16
						andi r16,0b00010000				;	������ ������ ��� 4, ��� 6 ����������, �.�. ����� "������ ��������"
						ori  r16,(1<<5)				;	���������� � ������ ��� 5
						or   r16,r17

						ldd r17,Y+1
						andi r17,$80
						or r16,r17
						lds r17,(MASK)
						cpi r17,$FF
						brne printUsim_M1
						ori r16,$80
						rjmp printUsim_M2
printUsim_M1:			andi r16,$7F
printUsim_M2:			std Y+1,r16
							
					pop r16
				icall
				ldi r16,$00
				sts (MASK),r16
				;movw Z,X
				POPHIGH
				POPXYZ
				POPF
				ret
// ----------------------------------------------------
// �������� ���� ���������� ������, � R16 - ���
clrInverseFlag:	PUSHY
				push r16
				push r17
					
				LDY positionTable
				subi r16,$80
				clr r17
				add YL,r16	adc YH,r17
				add YL,r16	adc YH,r17	
				ldd r17,Y+1	
				CLRB r17,7					;	�������� ����
				std Y+1,r17
				
				pop r17
				pop r16
				POPY
				ret
// ----------------------------------------------------
// ���������� ���� ���������� ������, � R16 - ���
setInverseFlag:	PUSHY
				push r16
				push r17
					
				LDY positionTable
				subi r16,$80
				clr r17
				add YL,r16	adc YH,r17
				add YL,r16	adc YH,r17	
				ldd r17,Y+1	
				SETB r17,7					;	���������� ����
				std Y+1,r17
				
				pop r17
				pop r16
				POPY
				ret
// ----------------------------------------------------
// ���������� ���� ������ ��������, � R16 - ���
setNewValFlag:	PUSHY
				push r16
				push r17
					
				LDY positionTable
				subi r16,$80
				clr r17
				add YL,r16	adc YH,r17
				add YL,r16	adc YH,r17	
				ldd r17,Y+1	
				SETB r17,6					;	���������� ���� ������ ��������
				std Y+1,r17
				
				pop r17
				pop r16
				POPY
				ret
// ----------------------------------------------------
// ���������� � ��������� ���������� ������, � R16 - ���
printPositioning:
				push r16
				push r17
				PUSHY
				LDY positionTable
				subi r16,$80
				clr r17
				add YL,r16	adc YH,r17
				add YL,r16	adc YH,r17		;	Y - ��������� �� ����� �����������
				ldd r16,Y+0					;	POSX
				ldd r17,Y+1					;	POSY
				call SET_POS
				; ----------
				CLRB r17,6					;	����� ����� ������ ��������
				SETB r17,5					;	���������� ������ �� �����
				std Y+1,r17					;	
				; ----------
				sbrc r17,7
				ldi r16,$FF
				sbrs r17,7
				ldi r16,$00
				sts (MASK),r16
				; ----------
				swap r17
				andi r17,$0F
				sts (FONT_SIZE),r17
				POPY
				pop r17
				pop r16

				ret
// ----------------------------------------------------
printNoValue:	ldi r16,' '	call printChar	ldi r16,'-' call printChar	call printChar
				ldi r16,'.'	call printChar
				ldi r16,'-'	call printChar	call printChar
				ret	
// ---------------------------------------------------- 80
nothingToPrint:	ret
// ---------------------------------------------------- 81  
setFontSize:	lpm r16,Z+
				andi r16,$01
				sts (FontNo),r16
				sts (FONT_SIZE),r16
				ret
// ----------------------------------------------------
setSmallFont:	ldi r16,0
				sts (FONT_SIZE),r16
				ret
// ---------------------------------------------------- 82
setLargeFont:	ldi r16,1
				sts (FONT_SIZE),r16
				ret
// ---------------------------------------------------- 83
printSetXY:		lpm r16,Z+
				andi r16,$7F
				lpm r17,Z+
				andi r17,$07
				call SET_POS
				ret
// ---------------------------------------------------- 84
printSetdXdY:		lpm r18,Z+
					lds r16,DISPLAY_POSX
					add r16,r18
					andi r16,$7F
					lpm r18,Z+
					lds r17,DISPLAY_POSY
					add r17,r18
					andi r17,$07
					call SET_POS
				ret
// ---------------------------------------------------- 85
//
setInverse:			lpm r17,Z+
					tst r17
					breq clrInverse
					ldi r17,$FF;call setInverseFlag
					sts (MASK),r17
					rjmp setInverseEx
clrInverse:			ldi r17,$00;call clrInverseFlag
					sts (MASK),r17
setInverseEx:
				ret
// ---------------------------------------------------- 86
// ���������� ������ ���������� ������������ ���� (#80-#BF)
disableNext:		PUSHY
					push r16
					push r17
					lpm r16,Z+
					cpi r16,$C0
					brcc disableNextEx
					cpi r16,$80
					brcs disableNextEx

					subi r16,$80
					clr r17
					LDY positionTable
					add YL,r16		adc YH,r17
					add YL,r16		adc YH,r17
						ldd r16,Y+1
						andi r16,0b11011111
						std Y+1,r16

disableNextEx:		pop r17
					pop r16
					POPY
					ret
// ---------------------------------------------------- 87
printMessageEx:		// MSGEx - ����� �������� ������
					LDZ messageExTable*2
					lds r16,(MSGEx)
					andi r16,$03
					clr r17
					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17
					lpm r16,Z+
					lpm r17,Z
					movw ZH:ZL,r17:r16
					call printString
				ret
// ---------------------------------------------------- 88
printMessage:		// r16 - message number
					LDZ pumpMessagesTable*2
					lds r16,(MSGNo)
					andi r16,$07
					clr r17
;					andi r16,$07
					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17
					lpm r16,Z+
					lpm r17,Z
					movw ZH:ZL,r17:r16
					call printString
				ret
// ---------------------------------------------------- 89
printAtmoMM:		call printPositioning

					lds r16,(BMPPresence)
					tst r16
					breq printAtmoMMYes

printAtmoMMYES:		LDY PRESSURE_MM_AVG
					ldd r22,Y+0
					ldd r23,Y+1
					ldd r24,Y+2
					ldd r25,Y+3
					call A32toBCD9

					lds r16,(DIGIT4)	call printChar
					lds r16,(DIGIT3)	call printChar
					lds r16,(DIGIT2)	call printChar
					ldi r16,'.'			call printChar
					lds r16,(DIGIT1)	call printChar
					lds r16,(DIGIT0)	call printChar

				ret
// ---------------------------------------------------- 8A
printStepsCount:		call printPositioning
						LDY STEPCOUNTER
						ldi r16,8
					rjmp common32
// ---------------------------------------------------- 8B
print100mlSteps:		call printPositioning
						LDY stepsFor100ml
						ldi r16,7
					rjmp common32
// ---------------------------------------------------- 8C
printRateValue:			call printPositioning
						LDY RateValue;OCR1AValue;
						ldi r16,4
					rjmp common16
// ---------------------------------------------------- 8D
printTotalValue:		call printPositioning
						LDSA stepsFor100ml
						LDI_B 100
						call DIV3232U
						COPY_A_B
						LDSA STEPCOUNTER
						call DIV3232U
						ldi r16,5
					rjmp common
// ---------------------------------------------------- 8E
printRemainValue:		call printPositioning
						LDSY DoseValue
						ldi r16,0
						cp  YL,r16
						cpc YH,r16
						brne printRemainValueNZ
						ldi r16,'�'		call printChar
						ldi r16,'�'		call printChar
						ldi r16,'�'		call printChar
						ldi r16,'�'		call printChar
						ret

printRemainValueNZ:		LDSA stepsFor100ml
						LDI_B 100
						call DIV3232U
						COPY_A_B
						LDSA STEPBACKCOUNTER
						call DIV3232U
						ldi r16,4
					rjmp common
// ---------------------------------------------------- 8F
printDoseValue:			call printPositioning
						LDY DoseValue
						ldi r16,4
					rjmp common16
// ------------------------
common16:			ldd r22,Y+0
					ldd r23,Y+1
					clr r24
					clr r25
					rjmp common
common32:			ldd r22,Y+0
					ldd r23,Y+1
					ldd r24,Y+2
					ldd r25,Y+3
common:				call A32toBCD9
					call HideZeroes
					cpi r16,9	breq common9
					cpi r16,8	breq common8
					cpi r16,7	breq common7
					cpi r16,6	breq common6
					cpi r16,5	breq common5
					cpi r16,4	breq common4
					cpi r16,3	breq common3
					cpi r16,2	breq common2
					cpi r16,1	breq common1
					lds r16,(DIGIT9)	call printChar
common9:			lds r16,(DIGIT8)	call printChar
common8:			lds r16,(DIGIT7)	call printChar
common7:			lds r16,(DIGIT6)	call printChar
common6:			lds r16,(DIGIT5)	call printChar
common5:			lds r16,(DIGIT4)	call printChar
common4:			lds r16,(DIGIT3)	call printChar
common3:			lds r16,(DIGIT2)	call printChar
common2:			lds r16,(DIGIT1)	call printChar
common1:			lds r16,(DIGIT0)	call printChar
				ret
// ---------------------------------------------------- 90-97
// -- ����� �������� ����������� � ������� ��.��
print2x2:		call printPositioning

print2x2A:		lds r17,(DS_STATUS)

checkForT0:		cpi r16,T0
				brne checkForT1
				sbrc r17,0
				rjmp printOther2x2
				call printNoValue
				rjmp print2x2ex

checkForT1:		cpi r16,T1
				brne checkForT2
				sbrc r17,1
				rjmp printOther2x2
				call printNoValue
				rjmp print2x2ex

checkForT2:		cpi r16,T2
				brne checkForT3
				sbrc r17,2
				rjmp printOther2x2
				call printNoValue
				rjmp print2x2ex

checkForT3:		cpi r16,T3
				brne printOther2x2
				sbrc r17,3
				rjmp printOther2x2
				call printNoValue
				rjmp print2x2ex

printOther2x2:	LDY DS_VALUE_0
				subi r16,T0
				clr r17
				add YL,r16	adc YH,r17
				add YL,r16	adc YH,r17		;	Y - ��������� �� ������

print2x2Y:			ldd r22,Y+0
					ldd r23,Y+1
					ldi r18, low(625)
					ldi r19,high(625)
					call MUL1616S
					call A32toBCD9
					ldi r16,4
					sts (DECIMAL_POS),r16
					call HideZeroes

					LDI_B 10000000
					call CP_AB
					brcs positiveT
					LDI_B -999375
					call CP_AB
					brcc negativeT
					rjmp print2x2ex
negativeT:			ldi r16,'-'
					rjmp commonT
positiveT:			lds r16,(DIGIT6)	
commonT:			call printSymbol
					lds r16,(DIGIT5)	call printSymbol
					lds r16,(DIGIT4)	call printSymbol
					ldi r16,'.'			call printSymbol
					lds r16,(DIGIT3)	call printSymbol
						lds r17,(DIGIT1)	ldi r16,'4'		cp r16,r17		ldi r17,$00
						lds r16,(DIGIT2)	adc r16,r17		call printSymbol
					rjmp print2x2ex
//------------------------------------------
print2x2ex:			ret
// ---------------------------------------------------- 98-9F
print0_99:		call printPositioning

				LDY DELAY_UP_0
				subi r16,DEL_UP0
				clr r17
				add YL,r16	adc YH,r17		;	Y - ��������� �� ������
	
					ldd r16,Y+0
					call HexToDec
					cpi r17,'0'
					brne pr0_99a
					ldi r17,' '
pr0_99a:			push r16
					mov r16,r17
					call printChar
					pop r16
					call printChar
				ret
// ---------------------------------------------------- A0-A3
printReaction:		call printPositioning
					LDY REACTION_0
					subi r16,REACT_0
					clr r17
					add YL,r16	adc YH,r17		;	Y - ��������� �� ������
					ldd r16,Y+0
					LDZ  reactionsTable*2
					andi r16,$0F
					clr r17
					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17
					lpm r16,Z+
					lpm r17,Z
					movw ZH:ZL,r17:r16
					call printString
					ret
// ---------------------------------------------------- A4-A7
printBackTimerT:	call printPositioning

				LDY BACK_TIMER_T0
				subi r16,BACK_T0
				clr r17
				add YL,r16	adc YH,r17		;	Y - ��������� �� ������
	
					ldd r16,Y+0
					call HexToDec
prbt99a:			push r16
					mov r16,r17
					call printChar
					pop r16
					call printChar
				ret
// ---------------------------------------------------- A8
// ����� ������ ������ �������������
printStabMode:		call printPositioning
					LDZ stabModeTable*2
					lds r16,(STABMODE)
						cpi r16,$FF
						breq printStabModeC
						cpi r16,$03
						brcs printStabModeA
						breq printStabModeA
						rjmp printStabModeEx

printStabModeC:			LDZ stabModeErr
						rjmp printStabModeB
printStabModeA:		andi r16,$03
					clr r17
					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17
					lpm r16,Z+
					lpm r17,Z
					movw ZH:ZL,r17:r16
printStabModeB:		call printString
printStabModeEx:	ret
// ---------------------------------------------------- A9
// ����� ���������� �������������
printTargetU:		call printPositioning
					LDY VOLTS
printAnyU:			ldd r22,Y+0
					ldd r23,Y+1
					clr r24
					clr r25
					call A32toBCD9
					ldi r16,1
					sts (DECIMAL_POS),r16
					call HideZeroes
					lds r16,(DIGIT3)	call printChar
					lds r16,(DIGIT2)	call printChar
					lds r16,(DIGIT1)	call printChar
					ldi r16,'.'			call printChar
					lds r16,(DIGIT0)	call printChar

					ret
// ---------------------------------------------------- AA
// ����� ���������� �������������
printAverageU:		call printPositioning
					LDY (VOLTS+2)
					rjmp printAnyU
// ---------------------------------------------------- AB
// ����� ���������� ������������
printStabStatus:	
					call printPositioning

					lds r18,(STAB_RANGE)
					lds r17,(StabStatus)
					cpse r18,r17
					rjmp statusIsNotEqual

statusIsEqual:		rjmp StabStatusExit

statusIsNotEqual:	sts (StabStatus),r18
					ldi r16,' '
					ldi r17,$00
					cpse r18,r17
					ldi r16,$07
					call printSymbol
StabStatusExit:
					ret
// ---------------------------------------------------- AC-AF
// ����� ��������� ���������� ������� �� �����
ReactionStatus:		call printPositioning
					LDY REACTION
					subi r16,R_On0
					mov r17,r16
					inc r17
					add YL,r16
					ldi r16,0
					adc YH,r16
					ldd r16,Y+0
					cpi r16,8
					brcs ReactionStatusOff
ReactionStatusOn:	ldi r16,$FF
					sts (MASK),r16
					rjmp ReactionStatusExit
ReactionStatusOff:	ldi r16,$00
					sts (MASK),r16
					;ldi r16,' '
ReactionStatusExit:	mov r16,r17
					call printSymbol
					ret
// ---------------------------------------------------- B0
// ����� ��������� �������� ������� ���/����
printHeatStatus:	call printPositioning
					lds r16,(HeatControlStatus)
					cpi r16,$FF
					breq HeatStatusOn
HeatStatusOff:		LDZ isOff
					rjmp printHeatStatusEx
HeatStatusOn:		LDZ isOn
printHeatStatusEx:	call printString
					ret
// ---------------------------------------------------- B1
// ����� ������ ������� �������� �������
printHeatSensor:	call printPositioning
					lds r16,(HeatControlSensor)
					call HexToAscii
					call printSymbol
					ret
// ---------------------------------------------------- B2
// ����� ������ ������� �������� �������
printHeatMargin:	call printPositioning
					LDY HeatControlMargin
					call print2x2Y
					ret
// ---------------------------------------------------- B3
// ����� �������� �������� ������� �������� �������
printHeatSensorVal:	call printPositioning
					lds r16,(HeatControlSensor)
					ldi r17,T0
					add r16,r17
					call print2x2A	
					ret
// ---------------------------------------------------- B4
// ����� ��������� �������� ������� ��������
printHeatString:		call printPositioning
						lds r16,(HeatControlSensor)
						clr r17
						LDZ DS_VALUE
						add ZL,r16	adc ZH,r17
						add ZL,r16	adc ZH,r17
						ldd YL,Z+0
						ldd YH,Z+1
						ldi r16,$00
						ldi r17,$80
						cp  r16,YL
						cpc r17,YH
						LDZ heatControlTable*2
						breq printHeatControlOff
						rjmp printHeatControlNotOff

printHeatControlOff:	adiw z,0
						rjmp printHeatControlEx

printHeatControlNotOff:	lds r16,(HeatControlStatus)
						cpi r16,$00
						breq printHeatControlOff
						adiw z,2
											
printHeatControlEx:		lpm r16,Z+
						lpm r17,Z
						movw ZH:ZL,r17:r16
						call printstring
						ret
// ----------------------------------------------------
// ---------- ������� ������� ����������� ������ ������������ (#80-#BF) ----------
printVectorsTable:
			.dw	nothingToPrint			; #80 - ����� ������	
			.dw	setSmallFont			; #81 - ����������� �� ����� 6�8	
			.dw	setLargeFont			; #82 - ����������� �� ����� 12�16	
			.dw	printSetXY				; #83 - ��������������� XY ���������, ��� ��������� ����� ����������	
			.dw	printSetdXdY			; #84 - ��������������� XY ������������, ��� ��������� ����� ��������
			.dw nothingToPrint			; #85	
			.dw disableNext				; #86 

			.dw printMessageEx			; #87 - ����� �������� ������ �� ������ ��������� ������
			.dw printMessage			; #88 - ����� ��������� �� ������� ��������� ��� ������� � r16	

			.dw printAtmoMM				; #89 - ����������� �������� � ��.��.��	
			.dw printStepsCount			; #8A - ������� �����	

			.dw print100mlSteps			; #8B - ���������� ����� �� 100 ��
			.dw printRateValue			; #8C - �������� ������ ��/���
			.dw printTotalValue			; #8D - ����� ����� ������, ��
			.dw printRemainValue		; #8E - ������� �� �����, ��
			.dw printDoseValue			; #8F - ������������� ��������� � ��, ���� 0, �� ��� ���������

			.dw print2x2				; #90 - ����������� ������ 0
			.dw print2x2				; #91 - ����������� ������ 1
			.dw print2x2				; #92 - ����������� ������ 2
			.dw print2x2				; #93 - ����������� ������ 3

			.dw print2x2				; #94 - ����������� ������������ ������ 0
			.dw print2x2				; #95 - ����������� ������������ ������ 1
			.dw print2x2				; #96 - ����������� ������������ ������ 2
			.dw print2x2				; #97 - ����������� ������������ ������ 3

			.dw print0_99				; #98 - �������� ������������ ��� ��������� ����������� ������ 0
			.dw print0_99				; #99 - �������� ������������ ��� ��������� ����������� ������ 1
			.dw print0_99				; #9A - �������� ������������ ��� ��������� ����������� ������ 2
			.dw print0_99				; #9B - �������� ������������ ��� ��������� ����������� ������ 3

			.dw print0_99				; #9C - �������� ������������ ��� ��������� ����������� ������ 0
			.dw print0_99				; #9D - �������� ������������ ��� ��������� ����������� ������ 1
			.dw print0_99				; #9E - �������� ������������ ��� ��������� ����������� ������ 2
			.dw print0_99				; #9F - �������� ������������ ��� ��������� ����������� ������ 3

			.dw printReaction			; #A0 - ������� ������ 0
			.dw printReaction			; #A1 - ������� ������ 1
			.dw printReaction			; #A2 - ������� ������ 2
			.dw printReaction			; #A3 - ������� ������ 3

			.dw printBackTimerT			; #A4 - �������� ������ ������� 0
			.dw printBackTimerT			; #A5 - �������� ������ ������� 1
			.dw printBackTimerT			; #A6 - �������� ������ ������� 2
			.dw printBackTimerT			; #A7 - �������� ������ ������� 3

			.dw printStabMode			; #A8 - ����� ������ �������������
			.dw printTargetU			; #A9 - ������� ���������� �������������
			.dw printAverageU			; #AA - ����������� ������� ���������� �������������
			.dw printStabStatus			; #AB - ��������� ������������

			.dw ReactionStatus			; #AC - ��������� ��������� ������� T0
			.dw ReactionStatus			; #AD - ��������� ��������� ������� T1
			.dw ReactionStatus			; #AE - ��������� ��������� ������� T2
			.dw ReactionStatus			; #AF - ��������� ��������� ������� T3

			.dw printHeatStatus			; #B0
			.dw printHeatSensor			; #B1
			.dw printHeatMargin			; #B2
			.dw printHeatSensorVal		; #B3
			.dw printHeatString			; #B4
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
			.dw disableNext				; #BF - ������ ������ � ������� �������
				
