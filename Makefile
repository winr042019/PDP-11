FLAGS = -Wall -Wextra

app: main.o memory.o log.o
	@gcc $(FLAGS) main.o memory.o log.o -o app.exe

main.o: main.c
	@gcc $(FLAGS) -c main.c -o main.o

memory.o: memory.c
	@gcc $(FLAGS) -c memory.c -o memory.o

log.o: log.c
	@gcc $(FLAGS) -c log.c -o log.o

.PHONY: clean
clean:
	@rm -rf *.o
