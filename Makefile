FLAGS = -Wall -Wextra

app: main.o memory.o log.o command.o
	@gcc $(FLAGS) main.o memory.o log.o command.o -o app.exe

main.o: main.c
	@gcc $(FLAGS) -c main.c -o main.o

memory.o: memory.c
	@gcc $(FLAGS) -c memory.c -o memory.o

log.o: log.c
	@gcc $(FLAGS) -c log.c -o log.o

command.o: command.c
	@gcc $(FLAGS) -c command.c -o command.o

.PHONY: clean
clean:
	@rm -rf *.o
