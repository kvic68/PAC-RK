// ----------------------------------------------------------------
.org 	0x00		jmp start	; RESTART
.org	INT0addr 	RETI;		; External Interrupt Rorgest 0
.org	INT1addr 	RETI;		; External Interrupt Rorgest 1
.org	PCI0addr 	RETI;		; Pin Change Interrupt Rorgest 0
.org	PCI1addr 	RETI;		; Pin Change Interrupt Rorgest 1
.org	PCI2addr 	reti;		; Pin Change Interrupt Rorgest 2
.org	WDTaddr 	RETI;		; Watchdog Time-out Interrupt
.org	OC2Aaddr 	reti;jmp T2CompareA	;RETI;		; Timer/Counter2 Compare Match A
.org	OC2Baddr 	reti;jmp T2CompareB;RETI;		; Timer/Counter2 Compare Match B
.org	OVF2addr 	RETI;		; Timer/Counter2 Overflow
.org	ICP1addr 	RETI;		; Timer/Counter1 Capture Event
.org	OC1Aaddr 	jmp T1compareA;RETI;		; Timer/Counter1 Compare Match A
.org	OC1Baddr 	RETI;		; Timer/Counter1 Compare Match B
.org	OVF1addr 	RETI;		; Timer/Counter1 Overflow
.org	OC0Aaddr 	jmp T0CompareA; TimerCounter0 Compare Match A
.org	OC0Baddr 	RETI;		; TimerCounter0 Compare Match B
.org	OVF0addr 	RETI;		; Timer/Couner0 Overflow
.org	SPIaddr 	RETI;		; SPI Serial Transfer Complete
.org	URXCaddr 	jmp RX_OK;RETI;; USART Rx Complete
.org	UDREaddr 	jmp UD_OK;RETI;			; USART, Data Register Empty
.org	UTXCaddr 	RETI;; USART Tx Complete
.org	ADCCaddr 	RETI;		; ADC Conversion Complete
.org	ERDYaddr 	RETI;		; EEPROM Ready
.org	ACIaddr 	RETI;		; Analog Comparator
.org	TWIaddr 	RETI;		; Two-wire Serial Interface
.org	SPMRaddr 	RETI;		; Store Program Memory Read

// ----------------------------------------------------------------

