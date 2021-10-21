project1:	main.o symbols.o  directives.o headers.h
	gcc -g -o project1 -Wall  -O0 main.o symbols.o directives.o instructions.c

main.o:	main.c headers.h 
	gcc -c -Wall  -O0 -g main.c

symbols.o:	symbols.c headers.h
	gcc -c -Wall  -O0 -g symbols.c

directives.o:	directives.c headers.h
	gcc -c -Wall  -O0 -g directives.c

instructions.o:   instructions.c headers.h
	gcc -c -Wall  -O0 -g instructions.c

clean:	
	rm *.o -f
	touch *.c
	rm project1 -f
