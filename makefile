all : my_ls

my_ls : 	pro.o attributes.o filters.o
		g++ -o my_ls pro.c attributes.c filters.c
pro.o : 	pro.c proHeader.h
		g++ -c pro.c
attributes.o :  attributes.c proHeader.h
	        g++ -c attributes.c
filters.o :	filters.c proHeader.h
		g++ -c filters.c
clean :
		rm -rf *.o  
