soft_scl_1:			cbi TM_DDR,TM_CLK			;	вход
					sbi TM_PORT,TM_CLK			;	с подтяжкой
soft_scl_1_wait:	in r18, TM_PIN
					sbrs r18,TM_CLK
					rjmp soft_scl_1_wait					
				ret
//---------------------------------------------------------------------------------------------------------------------
soft_scl_0:			cbi TM_PORT,TM_CLK			;	с нулем на выходе
					sbi TM_DDR,TM_CLK			;	выход
				ret
//---------------------------------------------------------------------------------------------------------------------
soft_sda_1:			cbi TM_DDR,TM_DIO			;	вход
					sbi TM_PORT,TM_DIO			;	с подтяжкой
				ret
//---------------------------------------------------------------------------------------------------------------------
soft_sda_0:			cbi TM_PORT,TM_DIO			;	с нулем на выходе
					sbi TM_DDR,TM_DIO			;	выход
				ret
//---------------------------------------------------------------------------------------------------------------------
softI2CInit:		push r18
					call soft_sda_1
					call soft_scl_1
					call softI2CStop
					pop r18
				ret
//---------------------------------------------------------------------------------------------------------------------
softI2CStart:		push r18
					call soft_sda_0	; SDA->0
					M_DELAY_US_RA 1,r18
					call soft_scl_0	; SCL->0
					M_DELAY_US_RA 1,r18
					pop r18
				ret
//---------------------------------------------------------------------------------------------------------------------
softI2CRestart:		push r18
					call soft_sda_1
					M_DELAY_US_RA 1,r18
					call soft_scl_1
					M_DELAY_US_RA 1,r18
					call soft_sda_0
					M_DELAY_US_RA 1,r18
					call soft_scl_0
					M_DELAY_US_RA 1,r18
					pop r18
				ret
//---------------------------------------------------------------------------------------------------------------------
softI2CStop:		push r18
					call soft_scl_0	; SCL->0
					M_DELAY_US_RA 1,r18
					call soft_sda_0	; SDA->0
					M_DELAY_US_RA 1,r18
					call soft_scl_1	; SCL->1
					M_DELAY_US_RA 1,r18
					call soft_sda_1	; SDA->1
					M_DELAY_US_RA 1,r18
					pop r18
				ret
//---------------------------------------------------------------------------------------------------------------------
// send byte
softI2CSendByte:		push r17
						push r18
						ldi r17,8
softI2CSendByteCycle:	rol r16
						brcc softI2CSendByteNull
						call soft_sda_1	; SDA->1
						rjmp softI2CSendByteA
softI2CSendByteNull:	call soft_sda_0	; SDA->0

softI2CSendByteA:		M_DELAY_US_RA 1,r18
						call soft_scl_1	; SCL->1
						M_DELAY_US_RA 1,r18
						call soft_scl_0	; SCL->0
						dec r17
						brne softI2CSendByteCycle

						//ACK bit
						call soft_sda_1	; SDA->1
						M_DELAY_US_RA 1,r18
						call soft_scl_1	; SCL->1
						M_DELAY_US_RA 1,r18

						in r17,TM_PIN
							bst r17,TM_DIO		; T=0 ACK, T=1 NACK
						call soft_scl_0	; SCL->0

					pop r18
					pop r17
					ret
; ------------------------------------------------------------------------
