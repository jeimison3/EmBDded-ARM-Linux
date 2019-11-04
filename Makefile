all: gp

CC = arm-linux-gnueabihf-
C_FLAGS = -Wall -std=gnu11 -ggdb

# Depuração
#C_FLAGS := ${C_FLAGS} -DDEBUG

# Estático:
#L_FLAGS = -static

#Dinâmico:
C_FLAGS := ${C_FLAGS} -fPIC


# Estratégia de compilação: https://github.com/shenki/linux-i2c-example

gp: main.o gpioPins.o files.o
	${CC}gcc ${L_FLAGS} main.o gpioPins.o files.o -o gp
	cp ./gp ../sistemadearquivos/root/
main.o: main.c
	${CC}gcc -c main.c ${C_FLAGS} -o main.o
gpioPins.o: gpioPins.c
	${CC}gcc -c gpioPins.c ${C_FLAGS} -o gpioPins.o
files.o: files.c
	${CC}gcc -c files.c ${C_FLAGS} -o files.o

clean:
	rm *.o gp