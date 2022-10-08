Mode0Screen:	.db F,0,XY,0,0,"Выход",\
					F,2,XY,104,0,Range,\
					F,1,XY,4,3,SetPos,OutputV,TargetV,\
					Line,2,0,128,$18,\
					EOS

Mode1Screen:	.db F,0,XY,0,0,"Разгон",\
					F,2,XY,92,0,TargetV,\
					Line,2,0,128,$18,\
					F,1,XY,4,3,OutputV,\
					EOS

Mode2Screen:	.db F,0,XY,0,0,"Стоп",\
					F,2,XY,92,0,TargetV,\
					Line,2,0,128,$18,\
					F,1,XY,4,3,OutputV,\
					EOS

Mode3Screen:	.db F,0,XY,10,1,"Выключено",\
						XY,10,3,"аварийным",\
						XY,16,5,"сигналом",EOS

HelloScreen:	.db F,0,XY,28,0,"ПРИВЕТ",\
						XY,10,2,"регулятор",\
						XY,10,4,"версия от",\
					F,2,XY,24,6,"21 05 2022",EOS
