#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

void escreve(char* arquivo, char* texto, int tam_texto){
	FILE * fil = fopen(arquivo, "w");
	fwrite(texto, sizeof(char), tam_texto, fil);
	fclose(fil);
}



int main(int argc, char *argv[]){

	//FILE *export;
	char * gpled1 = "53\n";
	char * gpled2 = "54\n";
	char * gpled3 = "55\n";
	char * gpled4 = "56\n";

	char * gpled1dir = "/sys/class/gpio/gpio53/direction";
	char * gpled2dir = "/sys/class/gpio/gpio54/direction";
	char * gpled3dir = "/sys/class/gpio/gpio55/direction";
	char * gpled4dir = "/sys/class/gpio/gpio56/direction";
	char* dir = "out\n";

	char * gpled1val = "/sys/class/gpio/gpio53/value";
	char * gpled2val = "/sys/class/gpio/gpio54/value";
	char * gpled3val = "/sys/class/gpio/gpio55/value";
	char * gpled4val = "/sys/class/gpio/gpio56/value";
	char * HIGH = "1\n";
	char * LOW = "0\n";

	char * exprt = "/sys/class/gpio/export";
	char * unexprt = "/sys/class/gpio/unexport";

	if(argc == 2){
		if(argv[1][0] == 'K'){ // KILL
			// Desliga
			escreve(gpled1val, LOW, 2);
			escreve(gpled2val, LOW, 2);
			escreve(gpled3val, LOW, 2);
			escreve(gpled4val, LOW, 2);
			// Des-exporta
			escreve(unexprt, gpled1, 3);
			escreve(unexprt, gpled2, 3);
			escreve(unexprt, gpled3, 3);
			escreve(unexprt, gpled4, 3);
			return 0;
		}
	}

	// Exporta
	escreve(exprt, gpled1, 3);
	escreve(exprt, gpled2, 3);
	escreve(exprt, gpled3, 3);
	escreve(exprt, gpled4, 3);

	// Como saída
	escreve(gpled1dir, dir, 4);
	escreve(gpled2dir, dir, 4);
	escreve(gpled3dir, dir, 4);
	escreve(gpled4dir, dir, 4);

	int ativo = 1;
	unsigned int msdelay = 250;
	clock_t init_time;
	while(1){
		if(ativo == 1){
			escreve(gpled1val, HIGH, 2);
		} else if(ativo == 2){
			escreve(gpled2val, HIGH, 2);
		} else if(ativo == 3){
			escreve(gpled3val, HIGH, 2);
		} else if(ativo == 4){
			escreve(gpled4val, HIGH, 2);
		}else if(ativo == 5){
			escreve(gpled1val, LOW, 2);
		}else if(ativo == 6){
			escreve(gpled2val, LOW, 2);
		}else if(ativo == 7){
			escreve(gpled3val, LOW, 2);
		}else if(ativo == 8){
			escreve(gpled4val, LOW, 2); ativo = 0;
		}
		ativo++;
		printf("%d\n",ativo);

		init_time = clock();
		while( clock() - init_time <= msdelay*1000) { }
		escreve(gpled1val, LOW, 2);
		init_time = clock();
		while( clock() - init_time <= msdelay*1000) { }
	}

	return 0;
}
