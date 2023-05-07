main:
	cc main.c -O1 -Wall -lm -std=c99 -Wno-missing-braces -L ./lib/ -lraylib
