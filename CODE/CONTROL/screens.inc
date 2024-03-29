// ----------------------------------------------------------------
HELLOSTRING:	.db FONT,1,XY,28,10,"������",EOS
// ----------------------------------------------------------------

MAINSCREEN:		.db	XY,0,0,FONT,$01,"T",dXdY,-3,1,FONT,$00,$01,XY,14,0,FONT,$01,T0,\
					XY,0,2,FONT,$01,"T",dXdY,-3,1,FONT,$00,$02,XY,14,2,FONT,$01,T1,\
					FONT,$00,\
					XY,3,4,"T",$03,$00,"  ",T2,\
					XY,3,5,"T",$04,$00,"  ",T3,\
					XY,0,6,"���� ",PressureMM,\
					XY,0,7,StabModeV,\
					XY,85,0,R_On0,$00,MARGIN0,\
					XY,85,1,R_On1,$00,MARGIN1,\
					XY,85,2,R_On2,$00,MARGIN2,\
					XY,85,3,R_On3,$00,MARGIN3,\
					XY,127-(9*6),5,"���� ",XY,127-24,5,Rate,\
					XY,127-(9*6),6,"���� ",XY,127-30,6,Total,\
					XY,127-(9*6),7,"���� ",XY,127-24,7,Remain,\
					XY,127-(9*6),4,Message,EOS
// ----------------------------------------------------------------
heatControlTable:	.dw heatControlOff,heatControlOn
heatControlOff:		.db Disable,HSensor,Disable,HMargin,Inv,1,"�������� ���",EOS
heatControlOn:		.db Inv,1,$00,"�",Inv,1,HSensor,Inv,1," ��",$00,Inv,1,HMargin,Inv,1,$00,EOS
// ----------------------------------------------------------------
stabModeTable:	.dw stabMode0,stabMode1,stabMode2,stabMode3,stabModeErr
stabMode0:		.db Disable,VoltageA,Disable,HString,Disable,HSensor,Disable,HMargin,\
					XY,0,6,"���� ",PressureMM," ",$00,\
					XY,0,7,"U��� ",Inv,0,VoltageT,$00,"  ",dXdY,-12,0,StabInd,EOS

stabMode1:		.db Disable,VoltageT,Disable,VoltageA,Disable,StabInd,Disable,PressureMM,Disable,HSensor,Disable,HMargin,\
					XY,0,6,Inv,1,$00,"������",$00,VoltageT,Inv,1,$00,\
					XY,0,7,HString,EOS

stabMode2:		.db Disable,VoltageT,Disable,VoltageA,Disable,StabInd,Disable,PressureMM,Disable,\
					HString,Disable,HSensor,Disable,HMargin,\
					XY,0,6,Inv,1,$00,"������ ����",$00,\
					XY,0,7,Inv,1,$00,"����� ",$00,VoltageT,Inv,1,$00,EOS

stabMode3:		.db Disable,VoltageT,Disable,VoltageA,Disable,StabInd,Disable,PressureMM,Disable,\
					HString,Disable,HSensor,Disable,HMargin,\
					XY,0,6,Inv,1,$00,"������ ����",$00,\
					XY,0,7,Inv,1,"   ������   ",EOS

stabModeErr:	.db XY,0,6,"���� ",PressureMM," ",$00,\
					XY,0,7,"  ��������  ",disable,VoltageT,disable,VoltageA,Disable,StabInd,\
					Disable,HString,Disable,HSensor,Disable,HMargin,EOS
// ----------------------------------------------------------------
MENU_T0:		.db XY,10,0,FONT,0,"��������� ������ 0",						\
				 	XY,10,1,       "------------------",						\
					XY,10,2,       "�������",XY,85,2,T0,						\
					XY,10,3,       "------------------",						\
					XY,10,4,       "�����",XY,85,4,MARGIN0,					\
					XY,10,5,       "��������",XY,94,5,REACT_0,				\
					XY,10,6,       "��������  ���. ",XY,100,6,DEL_UP0,'"',	\
					XY,10,7,       "�������� ����. ",XY,100,7,DEL_DN0,'"',	\
					EOS
// ----------------------------------------------------------------
MENU_T1:		.db XY,10,0,FONT,0,"��������� ������ 1",\
					XY,10,1,       "------------------",\
					XY,10,2,       "�������",XY,85,2,T1,\
					XY,10,3,       "------------------",\
					XY,10,4,       "�����",XY,85,4,MARGIN1,\
					XY,10,5,       "��������",XY,94,5,REACT_1,\
					XY,10,6,       "��������  ���. ",XY,100,6,DEL_UP1,'"',\
					XY,10,7,       "�������� ����. ",XY,100,7,DEL_DN1,'"',\
					EOS
// ----------------------------------------------------------------
MENU_T2:		.db XY,10,0,FONT,0,"��������� ������ 2",\
					XY,10,1,       "------------------",\
					XY,10,2,       "�������",XY,85,2,T2,\
					XY,10,3,       "------------------",\
					XY,10,4,       "�����",XY,85,4,MARGIN2,\
					XY,10,5,       "��������",XY,94,5,REACT_2,\
					XY,10,6,       "��������  ���. ",XY,100,6,DEL_UP2,'"',\
					XY,10,7,       "�������� ����. ",XY,100,7,DEL_DN2,'"',\
					EOS
// ----------------------------------------------------------------
MENU_T3:		.db XY,10,0,FONT,0,"��������� ������ 3",\
					XY,10,1,       "------------------",\
					XY,10,2,       "�������",XY,85,2,T3,\
					XY,10,3,       "------------------",\
					XY,10,4,       "�����",XY,85,4,MARGIN3,\
					XY,10,5,       "��������",XY,94,5,REACT_3,\
					XY,10,6,       "��������  ���. ",XY,100,6,DEL_UP3,'"',\
					XY,10,7,       "�������� ����. ",XY,100,7,DEL_DN3,'"',\
					EOS
// ----------------------------------------------------------------
MENU_PUMP:		.db XY,10,0,FONT,0,"��������� ��������",\
					XY,10,1,       "------------------",\
					XY,10,2,       "����      ",StepsCount,\
					XY,10,3,       "------------------",\
					XY,10,4,       "�����     ",Total," ��",\
					XY,10,5,       "���� ����� ",Remain," ��",\
					XY,10,6,       "���������  ",Dose," ��",\
					XY,10,7,       MessageEx,\
					EOS,$00
// ----------------------------------------------------------------
TUNE_PUMP:		.db XY,10,0,FONT,0,"����������  ������",\
					XY,10,1,       "------------------",\
					XY,10,2,       "�������  : ",StepsFor100,\
					XY,10,3,       "------------------",\
					XY,10,4,       "�������  :",StepsCount,\
					XY, 7,5,       "���� ",Rate," ����",Total,\
					XY,10,6,       "------------------",\
					XY,10,7,       "��������  � EEPROM",\
					EOS,$00
// ----------------------------------------------------------------
FULL_POWER:		.db XY,10,0,FONT,0," �������� ������� ",\
					XY,10,1,       "------------------",\
					XY,10,2,       "������� �   ",$00,HSensorVal,\
					XY,10,3,       "------------------",\
					XY,10,4,       "���������     ",HStatus,\
					XY,10,5,       "������          �",HSensor,\
					XY,10,6,       "�����       ",$00,HMargin,\
					XY,10,7,       "------------------",\
					EOS,$00
// ----------------------------------------------------------------
ITEMS_UNSELECT:	.db XY,0,0,FONT,0,$11,XY,122,0,$10,\
					XY,0,4,       " ",XY,122,4," ",\
					XY,0,5,       " ",XY,122,5," ",\
					XY,0,6,       " ",XY,122,6," ",\
					XY,0,7,       " ",XY,122,7," ",\
					EOS
// ----------------------------------------------------------------
ITEM0_SELECT:	.db XY,0,0,FONT,0," ",XY,122,0," ",\
					XY,0,4,       ">",XY,122,4,"<",\
					XY,0,5,       " ",XY,122,5," ",\
					XY,0,6,       " ",XY,122,6," ",\
					XY,0,7,       " ",XY,122,7," ",\
					EOS
// ----------------------------------------------------------------
ITEM1_SELECT:	.db XY,0,0,FONT,0," ",XY,122,0," ",\
					XY,0,4,       " ",XY,122,4," ",\
					XY,0,5,       ">",XY,122,5,"<",\
					XY,0,6,       " ",XY,122,6," ",\
					XY,0,7,       " ",XY,122,7," ",\
					EOS
// ----------------------------------------------------------------
ITEM2_SELECT:	.db XY,0,0,FONT,0," ",XY,122,0," ",\
					XY,0,4,       " ",XY,122,4," ",\
					XY,0,5,       " ",XY,122,5," ",\
					XY,0,6,       ">",XY,122,6,"<",\
					XY,0,7,       " ",XY,122,7," ",\
					EOS
// ----------------------------------------------------------------
ITEM3_SELECT:	.db XY,0,0,FONT,0," ",XY,122,0," ",\
					XY,0,4,       " ",XY,122,4," ",\
					XY,0,5,       " ",XY,122,5," ",\
					XY,0,6,       " ",XY,122,6," ",\
					XY,0,7,       ">",XY,122,7,"<",\
					EOS
// ----------------------------------------------------------------
ITEM0_CHANGE:	.db XY,0,0,FONT,0," ",XY,122,0," ",\
					XY,0,4,       $10,XY,122,4,$11,\
					XY,0,5,       " ",XY,122,5," ",\
					XY,0,6,       " ",XY,122,6," ",\
					XY,0,7,       " ",XY,122,7," ",\
					EOS
// ----------------------------------------------------------------
ITEM1_CHANGE:	.db XY,0,0,FONT,0," ",XY,122,0," ",\
					XY,0,4,       " ",XY,122,4," ",\
					XY,0,5,       $10,XY,122,5,$11,\
					XY,0,6,       " ",XY,122,6," ",\
					XY,0,7,       " ",XY,122,7," ",\
					EOS
// ----------------------------------------------------------------
ITEM2_CHANGE:	.db XY,0,0,FONT,0," ",XY,122,0," ",\
					XY,0,4,       " ",XY,122,4," ",\
					XY,0,5,       " ",XY,122,5," ",\
					XY,0,6,       $10,XY,122,6,$11,\
					XY,0,7,       " ",XY,122,7," ",\
					EOS
// ----------------------------------------------------------------
ITEM3_CHANGE:	.db XY,0,0,FONT,0," ",XY,122,0," ",\
					XY,0,4,       " ",XY,122,4," ",\
					XY,0,5,       " ",XY,122,5," ",\
					XY,0,6,       " ",XY,122,6," ",\
					XY,0,7,       $10,XY,122,7,$11,\
					EOS
// ----------------------------------------------------------------
messageExTable:	.dw msgEx00,msgEx01,msgEx02,msgEx03
//              "------------------"
msgEx00:	.db "����������  ������",EOS,$00
msgEx01:	.db "���������� � �����",EOS,$00
msgEx02:	.db "����� � ����������",EOS,$00
msgEx03:	.db "����������  ������",EOS,$00
// ----------------------------------------------------------------
pumpMessagesTable:	.dw	pumpmsg00,pumpmsg01,pumpmsg02,pumpmsg03,pumpmsg04,pumpmsg05,pumpmsg06,pumpmsg07
// ----------------------------------------------------------------
pumpmsg00:		.db "---------",EOS,$00
pumpmsg01:		.db Inv,1,"--�����--",Inv,0,EOS,$00
pumpmsg02:		.db Inv,1,$00,"��������",$00,EOS,$00
pumpmsg03:		.db Inv,1,$00,"- ���� -",$00,EOS,$00
pumpmsg04:		.db EOS,$00
pumpmsg05:		.db EOS,$00
pumpmsg06:		.db EOS,$00
pumpmsg07:		.db EOS,$00
// ----------------------------------------------------------------
reactionsTable:	.dw	reaction0,reaction1,reaction2,reaction3,reaction4,reaction5,reaction6,reaction7
				.dw	reaction8,reaction9,reactionA,reactionB,reactionC,reactionD,reactionE,reactionF
reaction0:		.db "����",EOS
reaction1:		.db "����", EOS
reaction2:		.db "����", EOS
reaction3:		.db "����", EOS
reaction4:		.db "����", EOS
reaction5:		.db "����", EOS
reaction6:		.db "����", EOS
reaction7:		.db "����", EOS
reaction8:		.db $00,$20,$0F,$20,$00,EOS
reaction9:		.db $20,$0F,$0D,$20,EOS
reactionA:		.db $20,$0F,$09,$20,EOS
reactionB:		.db $00,$0F,$09,$0D,$00,EOS
reactionC:		.db $20,$0F,$19,$20,EOS
reactionD:		.db $00,$0F,$19,$0D,$00,EOS
reactionE:		.db $00,$0F,$19,$09,$00,EOS
reactionF:		.db $0F,$19,$09,$0D,EOS
// ----------------------------------------------------------------
isOn:			.db " ���",EOS
isOff:			.db "����",EOS
