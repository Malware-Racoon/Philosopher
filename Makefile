all: dphil

dphil: philospher.o 
	gcc -pthread philospher.o -o dphil

philospher.o: philospher.c
	gcc -c  -pthread philospher.c

clean:
	rm -f *o philospher	
