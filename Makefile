spcinv, spcdebug: main.c ./game/spcinv.c ./game/spcinv.h
	gcc main.c ./game/spcinv.c ./game/spcinv.h -lncurses -o spcinv
	gcc main.c ./game/spcinv.c ./game/spcinv.h -lncurses -o spcdebug -D DEBUG
