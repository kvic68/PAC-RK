SET_SCL:			cbi TM_DDR,TM_CLK			;	вход
					sbi TM_PORT,TM_CLK			;	с подтяжкой

SET_SCL_wait:		sbis TM_PIN,TM_CLK
					rjmp SET_SCL_wait
				ret
//---------------------------------------------------------------------------------------------------------------------
CLR_SCL:			cbi TM_PORT,TM_CLK			;	с нулем на выходе
					sbi TM_DDR,TM_CLK			;	выход
				ret
//---------------------------------------------------------------------------------------------------------------------
SET_SDA:			cbi TM_DDR,TM_DIO			;	вход
					sbi TM_PORT,TM_DIO			;	с подтяжкой
				ret
//---------------------------------------------------------------------------------------------------------------------
CLR_SDA:			cbi TM_PORT,TM_DIO			;	с нулем на выходе
					sbi TM_DDR,TM_DIO			;	выход
				ret
//---------------------------------------------------------------------------------------------------------------------
softI2CInit:		call SET_SDA
					call SET_SCL
					call softI2CStop
				ret
//---------------------------------------------------------------------------------------------------------------------
softI2CStart:		push r18
					call CLR_SDA			; SDA->0
					M_DELAY_US_RA 2,r18
					call CLR_SCL			; SCL->0
					M_DELAY_US_RA 2,r18
					pop r18
				ret
//---------------------------------------------------------------------------------------------------------------------
softI2CRestart:		push r18
					call SET_SDA
					M_DELAY_US_RA 2,r18
					call SET_SCL
					M_DELAY_US_RA 2,r18
					call CLR_SDA
					M_DELAY_US_RA 2,r18
					call CLR_SCL
					M_DELAY_US_RA 2,r18
					pop r18
				ret
//---------------------------------------------------------------------------------------------------------------------
softI2CStop:		push r18
					call CLR_SCL			; SCL->0
					M_DELAY_US_RA 2,r18
					call CLR_SDA			; SDA->0
					M_DELAY_US_RA 2,r18
					call SET_SCL			; SCL->1
					M_DELAY_US_RA 2,r18
					call SET_SDA			; SDA->1
					M_DELAY_US_RA 2,r18
					pop r18
				ret
//---------------------------------------------------------------------------------------------------------------------
// send byte r16
softI2CSendByte:	PUSH r17
					PUSHFand r18
						ldi r17,8

softI2CSendByteCycle:	rol r16
						brcc softI2CSendBit0

						call SET_SDA			; SDA->1
						rjmp softI2CSendBit1

softI2CSendBit0:		call CLR_SDA			; SDA->0

softI2CSendBit1:		M_DELAY_US_RA 2,r18
						call SET_SCL			; SCL->1
						M_DELAY_US_RA 2,r18
						call CLR_SCL			; SCL->0
						dec r17
						brne softI2CSendByteCycle

						//ACK bit
						call SET_SDA			; SDA->1
						M_DELAY_US_RA 2,r18
						call SET_SCL			; SCL->1
						M_DELAY_US_RA 2,r18

						lds r18,$FF				;	NACK
						sbis TM_PIN,TM_DIO
						clr r18					;	ACK
						sts (SOFT_ACK),r18
						call CLR_SCL			; SCL->0

					POPFand r18
					POP r17
					ret
; ------------------------------------------------------------------------
