all: embdded-arm-linux

CC = arm-linux-gnueabihf-
C_FLAGS = -Wall -std=gnu11 -ggdb


# Habilitar depuração?
C_FLAGS := ${C_FLAGS} -DDEBUG

## Envio SCP
SSH_IPADDR = 192.168.1.3
SSH_USR = osmc
SSH_DESTPATH = /home/osmc/

## Bibliotecas

# Estático:
#L_FLAGS = -static
# Dinâmico:
C_FLAGS := ${C_FLAGS} -fPIC


# Estratégia de compilação: https://github.com/shenki/linux-i2c-example

embdded-arm-linux: main.o gpioPins.o files.o linux.o
	${CC}gcc ${L_FLAGS} main.o gpioPins.o files.o -o embdded-arm-linux
	cp ./embdded-arm-linux ../sistemadearquivos/root/
main.o: main.c
	${CC}gcc -c main.c ${C_FLAGS} -o main.o
gpioPins.o: gpioPins.c
	${CC}gcc -c gpioPins.c ${C_FLAGS} -o gpioPins.o
files.o: files.c
	${CC}gcc -c files.c ${C_FLAGS} -o files.o
linux.o: linux.c
	${CC}gcc -c linux.c ${C_FLAGS} -o linux.o

init:
	sudo cp S60embdded ../sistemadearquivos/etc/init.d/

ssh:
	scp embdded-arm-linux ${SSH_USR}@${SSH_IPADDR}:${SSH_DESTPATH}

clean:
	rm *.o embdded-arm-linux