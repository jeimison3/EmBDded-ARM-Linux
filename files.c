#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void escreve_arquivo(char* arquivo, char* texto){
	#ifdef DEBUG
	printf("\"%s\" >> %s\n",texto, arquivo);
	#endif
	FILE * fil = fopen(arquivo, "w");
    fputs(texto, fil); // Não testado
	//fwrite(texto, sizeof(char), tam_texto, fil);
	fclose(fil);
}

size_t le_arquivo(char* arquivo, char* saida){
    FILE * fil = fopen(arquivo, "r");
    fgets(saida, 4096, fil);
	//fread(saida, sizeof(char), tam_texto, fil);
	fclose(fil);
    return strlen(saida);
}