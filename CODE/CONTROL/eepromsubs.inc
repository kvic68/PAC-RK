; ------------------------------------------------------------------------
chekForEEValues:
					LDZ EEnStepsFor100ml
					call EEReadByte		mov r22,r16
					call EEReadByte		mov r23,r16
					call EEReadByte		mov r24,r16
					call EEReadByte		mov r25,r16
					ldi r21,$FF
					ldi r20,$FF
					ldi r19,$FF
					ldi r18,$FF
					call CP_AB
					breq noEEDataFound
						rjmp EEDataFound
noEEDataFound:		LDZ defaultValues*2
					LDY DS_MARGIN_0
					// margin
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					// delay up
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					// delay down
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					// actions
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					// heat controls
					LDY HeatControlSensor
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					//
					call saveDefaultValues

					// default 100 ml
					LDZ stepsFor100mlD*2
					LDY stepsFor100ml
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					lpm r16,Z+	st Y+,r16
					//
						LDSA stepsFor100ml
						LDZ EEnStepsFor100ml
						mov r16,r22		call EEWriteByte
						mov r16,r23		call EEWriteByte
						mov r16,r24		call EEWriteByte
						mov r16,r25		call EEWriteByte

EEDataFound:		ret

; ------------------------------------------------------------------------
initValues:
		; ��������� ���������� ����� �� 100 ��
				LDZ EEnStepsFor100ml
					call EEReadByte		mov r22,r16
					call EEReadByte		mov r23,r16
					call EEReadByte		mov r24,r16
					call EEReadByte		mov r25,r16
					LDI_B minStepsFor100ml
					call CP_AB
					brcs setDefaultValue100
					LDI_B maxStepsFor100ml
					call CP_AB
					brcs nextEEpromValues
setDefaultValue100:	LDI_A defaultStepsFor100ml
nextEEpromValues:	STSA stepsFor100ml
					LDI_B 100
					call DIV3232U
					STSA stepsFor1ml

		; ��������� ����������� ������������ ��������
				LDZ emargins
				LDY DS_MARGIN_0
		; ---------- T0
					call EEReadByte		st Y+,r16
					call EEReadByte		st Y+,r16
		; ---------- T1
					call EEReadByte		st Y+,r16
					call EEReadByte		st Y+,r16
		; ---------- T2
					call EEReadByte		st Y+,r16
					call EEReadByte		st Y+,r16
		; ---------- T3
					call EEReadByte		st Y+,r16
					call EEReadByte		st Y+,r16
		; ��������� �������� ������������ ��� ��������� �����������
		; -- DELAY_UP_0
					call EEReadByte		st Y+,r16
		; -- DELAY_UP_1
					call EEReadByte		st Y+,r16
		; -- DELAY_UP_2
					call EEReadByte		st Y+,r16
		; -- DELAY_UP_3
					call EEReadByte		st Y+,r16
		; ��������� �������� ������������ ��� ��������� �����������
		; -- DELAY_DN_0
					call EEReadByte		st Y+,r16
		; -- DELAY_DN_1
					call EEReadByte		st Y+,r16
		; -- DELAY_DN_2
					call EEReadByte		st Y+,r16
		; -- DELAY_DN_3
					call EEReadByte		st Y+,r16
		; ��������� �������� ������� �� ������������ ��� ��������� �����������
					call EEReadByte		st Y+,r16
					call EEReadByte		st Y+,r16
					call EEReadByte		st Y+,r16
					call EEReadByte		st Y+,r16
		; heater controls
					LDZ HeatSensor
					LDY HeatControlSensor
					call EEReadByte		st Y+,r16
					call EEReadByte		st Y+,r16
					call EEReadByte		st Y+,r16
					call EEReadByte		st Y+,r16

					LDX 0
					STSX DoseValue 
					call stepBackCounterCalc

					LDX 0
					STSX RateValue
					call ocr1aCalc
			ret
; ------------------------------------------------------------------------
save100mlStepsCount:	LDSA STEPCOUNTER
						LDI_B minStepsFor100ml
						call CP_AB
						brcs save100mlStepsCountEx
						LDI_B maxStepsFor100ml
						call CP_AB
						brcc save100mlStepsCountEx

						LDI_B 100
						call DIV3232U
						STSA stepsFor1ml

						LDI_B 100
						call MUL24x8U

						STSA stepsFor100ml
						LDZ EEnStepsFor100ml
						mov r16,r22		call EEWriteByte
						mov r16,r23		call EEWriteByte
						mov r16,r24		call EEWriteByte
						mov r16,r25		call EEWriteByte
;						LDI_B 100
;						call DIV3232U
						ldi r16,StepsFor100
						call setNewValFlag
save100mlStepsCountEx:	ret
; ------------------------------------------------------------------------
saveDefaultValues:	
		; ��������� ����������� ������������ ��������
				LDZ emargins
				LDY DS_MARGIN_0
		; ---------- T0
					ld r16,Y+		call EEWriteByte
					ld r16,Y+		call EEWriteByte
		; ---------- T1
					ld r16,Y+		call EEWriteByte
					ld r16,Y+		call EEWriteByte
		; ---------- T2
					ld r16,Y+		call EEWriteByte
					ld r16,Y+		call EEWriteByte
		; ---------- T3
					ld r16,Y+		call EEWriteByte
					ld r16,Y+		call EEWriteByte
		; ��������� �������� ������������ ��� ��������� �����������
		; -- DELAY_UP_0
					ld r16,Y+		call EEWriteByte
		; -- DELAY_UP_1
					ld r16,Y+		call EEWriteByte
		; -- DELAY_UP_2
					ld r16,Y+		call EEWriteByte
		; -- DELAY_UP_3
					ld r16,Y+		call EEWriteByte
		; ��������� �������� ������������ ��� ��������� �����������
		; -- DELAY_DN_0
					ld r16,Y+		call EEWriteByte
		; -- DELAY_DN_1
					ld r16,Y+		call EEWriteByte
		; -- DELAY_DN_2
					ld r16,Y+		call EEWriteByte
		; -- DELAY_DN_3
					ld r16,Y+		call EEWriteByte
		; ��������� �������� ������� �� ������������ ��� ��������� �����������
					ld r16,Y+		call EEWriteByte
					ld r16,Y+		call EEWriteByte
					ld r16,Y+		call EEWriteByte
					ld r16,Y+		call EEWriteByte
		; heat controls
					LDZ HeatSensor
					LDY HeatControlSensor
					ld r16,Y+		call EEWriteByte
					ld r16,Y+		call EEWriteByte
					ld r16,Y+		call EEWriteByte
					ld r16,Y+		call EEWriteByte

					ret
; ------------------------------------------------------------------------
EEWriteByte:	
					sbic 	EECR,EEPE		; ���� ���� ����� ��������� ������� ������.
					rjmp	EEWriteByte		; ����� �������� � �����.

					PUSHF
					push r17
					cli
						push r16
						call EEReadByte
						mov r17,r16
						pop r16
						sbiw Z,1
						cp r16,r17
						breq EEWriteByteSkip
					out 	EEARL, ZL		; ��������� ����� ������ ������
					out  	EEARH, ZH 		; ��� ������� � ������� �����
					out		EEDR,  r16		; � ���� ������, ������� ��� ����� ���������
					sbi 	EECR,EEMPE		; ������� ��������������
					sbi 	EECR,EEPE		; ���������� ����

EEWriteByteSkip:	pop r17
					POPF
					adiw Z,1
					ret
; ------------------------------------------------------------------------
EEReadByte:	
			sbic 	EECR,EEPE		; ���� ���� ����� ��������� ������� ������.
			rjmp	EEReadByte		; ����� �������� � �����.
			out 	EEARL, ZL		; ��������� ����� ������ ������
			out  	EEARH, ZH 		; ��� ������� � ������� �����
			sbi 	EECR,EERE 		; ���������� ��� ������
			in 		R16, EEDR 		; �������� �� �������� ������ ���������
			adiw Z,1
			ret
; ------------------------------------------------------------------------
; ���������� �� ���������� EEPROM
EEReady:	reti
; ------------------------------------------------------------------------
