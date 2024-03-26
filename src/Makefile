all: test

utils.o: ../utils/utils.c ../utils/utils.h
	gcc -c ../utils/utils.c ../utils/utils.h 

model.o: model.c
	gcc -c model.c

test: model.o utils.o
	gcc model.o utils.o -lm -o test

clean:
	rm -f *.o *.a *.gch