snap.o: snap.c snap.h z80.h util.h
config.o: config.c exit.h config.h
util.o: util.c util.h exit.h
kbbmp.o: kbbmp.c kbbmp.h
exit.o: exit.c exit.h
tape.o: tape.c tape.h util.h gui.h config.h
symtochar.o: symtochar.c symtochar.h
font.o: font.c font.h
audio.o: audio.c audio.h util.h
expr.o: expr.c
z80.o: z80.c z80.h z80_private.h
z80_decode.o: z80_decode.c z80.h z80_private.h
z80_dis.o: z80_dis.c
