Mode0Screen:	.db F,0,XY,0,0,"�����",\
					F,2,XY,104,0,Range,\
					F,1,XY,4,3,SetPos,OutputV,TargetV,\
					Line,2,0,128,$18,\
					EOS

Mode1Screen:	.db F,0,XY,0,0,"������",\
					F,2,XY,92,0,TargetV,\
					Line,2,0,128,$18,\
					F,1,XY,4,3,OutputV,\
					EOS

Mode2Screen:	.db F,0,XY,0,0,"����",\
					F,2,XY,92,0,TargetV,\
					Line,2,0,128,$18,\
					F,1,XY,4,3,OutputV,\
					EOS

Mode3Screen:	.db F,0,XY,10,1,"���������",\
						XY,10,3,"���������",\
						XY,16,5,"��������",EOS

HelloScreen:	.db F,0,XY,28,0,"������",\
						XY,10,2,"���������",\
						XY,10,4,"������ ��",\
					F,2,XY,24,6,"21 05 2022",EOS