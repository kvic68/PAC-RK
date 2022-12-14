soft_scl_One:		cbi TM_DDR,TM_CLK			;	????
					sbi TM_PORT,TM_CLK			;	? ?????????
				ret
//---------------------------------------------------------------------------------------------------------------------
soft_scl_Null:		cbi TM_PORT,TM_CLK			;	? ????? ?? ??????
					sbi TM_DDR,TM_CLK			;	?????
				ret
//---------------------------------------------------------------------------------------------------------------------
soft_sda_One:		cbi TM_DDR,TM_DIO			;	????
					sbi TM_PORT,TM_DIO			;	? ?????????
				ret
//---------------------------------------------------------------------------------------------------------------------
soft_sda_Null:		cbi TM_PORT,TM_DIO			;	? ????? ?? ??????
					sbi TM_DDR,TM_DIO			;	?????
				ret
//---------------------------------------------------------------------------------------------------------------------
softI2CInit:		push r18
					call soft_sda_One
					call soft_scl_One
					call softI2CStop
					pop r18
					ret
//---------------------------------------------------------------------------------------------------------------------
softI2CStart:		push r18
					call soft_sda_Null	; SDA->0
					M_DELAY_US_RA 1,r18
					call soft_scl_Null	; SCL->0
					M_DELAY_US_RA 1,r18
					pop r18
					ret
//---------------------------------------------------------------------------------------------------------------------
softI2CRestart:		push r18
					call soft_sda_One
					M_DELAY_US_RA 1,r18
					call soft_scl_One
					M_DELAY_US_RA 1,r18
					call soft_sda_Null
					M_DELAY_US_RA 1,r18
					call soft_scl_Null
					M_DELAY_US_RA 1,r18
					pop r18
					ret
//---------------------------------------------------------------------------------------------------------------------
softI2CStop:		push r18
					call soft_scl_Null	; SCL->0
					M_DELAY_US_RA 1,r18
					call soft_sda_Null	; SDA->0
					M_DELAY_US_RA 1,r18
					call soft_scl_One	; SCL->1
					M_DELAY_US_RA 1,r18
					call soft_sda_One	; SDA->1
					M_DELAY_US_RA 2,r18
					pop r18
					ret
//---------------------------------------------------------------------------------------------------------------------
// send byte
softI2CSendByte:		push r17
						push r18
						ldi r17,8
softI2CSendByteCycle:	rol r16
						brcc softI2CSendByteNull
						call soft_sda_One	; SDA->1
						rjmp softI2CSendByteA
softI2CSendByteNull:	call soft_sda_Null	; SDA->0

softI2CSendByteA:		M_DELAY_US_RA 1,r18
						call soft_scl_One	; SCL->1
						M_DELAY_US_RA 1,r18
						call soft_scl_Null	; SCL->0
						dec r17
						brne softI2CSendByteCycle
						//ACK bit
						call soft_sda_One	; SDA->1
						M_DELAY_US_RA 1,r18
						call soft_scl_One	; SCL->1
						M_DELAY_US_RA 1,r18

						in r17,TM_PIN
						bst r17,TM_DIO		; T=0 ACK, T=1 NACK
						call soft_scl_Null	; SCL->0
					pop r18
					pop r17
					ret
//---------------------------------------------------------------------------------------------------------------------
; ------------------------------------------------------------------------
