all: embdded-arm-linux

# Compilador:
#CC = arm-linux-gnueabihf-
CC = 

C_FLAGS = -Wall -std=gnu11 -ggdb



# Nome do cliente
BOARDNAME = Placa1

# Trajeto:
#C_FLAGS := ${C_FLAGS} -DBOARD_BEAGLEBONE
#C_FLAGS := ${C_FLAGS} -DBOARD_RASPI3
C_FLAGS := ${C_FLAGS} -DNOBOARD -DNO_GPIO


C_FLAGS := ${C_FLAGS} -DCLIENT_NAME=\"${BOARDNAME}\"

# Habilitar depuração?
C_FLAGS := ${C_FLAGS} -DDEBUG

## Envio SCP
SSH_IPADDR = 192.168.1.3
SSH_USR = osmc
SSH_DESTPATH = /home/osmc/

# Dinâmico:
C_FLAGS := ${C_FLAGS} -fPIC




# Estratégia de compilação: https://github.com/shenki/linux-i2c-example

# Objetos:
OBJFILES = main.o gpioPins.o files.o linux.o http.o protocolo.o atributosControl.o

embdded-arm-linux: ${OBJFILES}
	${CC}gcc ${L_FLAGS} ${OBJFILES} -o embdded-arm-linux
	cp ./embdded-arm-linux ../sistemadearquivos/root/

%.o: %.c
	${CC}gcc -c $< ${C_FLAGS} -o $@


init:
	sudo cp S60embdded ../sistemadearquivos/etc/init.d/

ssh:
	scp embdded-arm-linux ${SSH_USR}@${SSH_IPADDR}:${SSH_DESTPATH}

clean:
	rm *.o embdded-arm-linux