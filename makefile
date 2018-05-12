objects = List.o test.o main.o cmdTask.o rxTask.o txTask.o uart.o echo.o
lucy: $(objects) 
	gcc -Bstatic -Tlucy.lds $(objects) -o lucy -lpthread -L ./lib -lcurl
main.o: uart.h rxTask.h txTask.h
	gcc -c main.c -lpthread -g
	gcc -c List.c -o List.o -g
.PHONY: clean
.PHONY: check
	g++ check.c -o check `pkg-config --cflags --libs gtk+-2.0` -Wl,-Bstatic -L ./lib/ -lcurl -Wl,-Bdynamic -lrt
clean:
	rm -f lucy $(objects)
