objects = List.o test.o main.o cmdTask.o rxTask.o txTask.o uart.o echo.o
lucy: $(objects) 
	gcc -Bstatic -Tlucy.lds $(objects) -o lucy -lpthread
main.o: uart.h rxTask.h txTask.h
	gcc -c main.c -lpthread -g
	gcc -c List.c -o List.o -g
.PHONY: clean
clean:
	rm -f lucy $(objects)
