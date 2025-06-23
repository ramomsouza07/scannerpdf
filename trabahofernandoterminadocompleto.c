#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    FILE *path, *file, *file2, *file3;
    char caminho[100], comando[100];

    path = fopen("path.txt", "w");
    printf("\nDigite o caminho da pasta aonde estao os arquivos: \n");
    fgets(caminho, sizeof(caminho), stdin);
    caminho[strlen(caminho) - 1] = 0;
    fprintf(path, "dir /s /b \"%s\\*.txt\" > listaTXT.txt", caminho);
    fclose(path);

    path = fopen("path.txt", "r");
    fgets(comando, sizeof(comando), path);
    system(comando);
    fclose(path);

    path = fopen("path.txt", "w");
    fprintf(path, "dir /b \"%s\\*.pdf\" > listaPDF.txt", caminho);
    fclose(path);

    path = fopen("path.txt", "r");
    fgets(comando, sizeof(comando), path);
    system(comando);
    fclose(path);

    system("del /f path.txt");

    file2 = fopen("listaTXT.txt", "r");
    char saberLinhas[100];
    int linhas = 0;
    while ((fgets(saberLinhas, sizeof(saberLinhas), file2)) != NULL) {
        linhas++;
    }
    fclose(file2);

    int valido = 0;
    char palavra[100];
    do{
        printf("\nInsira a palavra chave para a busca: \n");
        fgets(palavra, sizeof(palavra), stdin);
        palavra[strlen(palavra) - 1] = 0;

        if (strlen(palavra) == 0) {
            printf("\nNão há palavra chave. Escreva novamente.\n");
        } else {
            valido = 1;
        }
    }while (!valido);
    
    file3 = fopen("listaPDF.txt", "r");
    file2 = fopen("listaTXT.txt", "r");

    char nomesArquivos[100][100];   // nomes dos arquivos
    int ocorrencias[100];           // ocorrências por arquivo
    int totalResultados = 0;
    int naoAberto = linhas;

    while (linhas) {
    	
        char arquivo[100], arquivoPDF[100];
		
        fgets(arquivo, sizeof(arquivo), file2);
        arquivo[strlen(arquivo) - 1] = 0;
		
        fgets(arquivoPDF, sizeof(arquivoPDF), file3);
        arquivoPDF[strlen(arquivoPDF) - 1] = 0;
		
        file = fopen(arquivo, "r");
		
        if (file == NULL) {
            printf("\nErro ao abrir o arquivo\n");
            naoAberto--;
            linhas--;
            continue;
        } else {
            char linha[100];
            int contavel = 0;

            while ((fgets(linha, sizeof(linha), file)) != NULL) {
                int i = 0;
                while (i < strlen(linha)) {
                    int j, igual = 0, k = 0;
                    for (j = i; j < i + strlen(palavra); j++) {
                        if (palavra[k] == linha[j])
                            igual++;
                        k++;
                    }
                    if (igual == strlen(palavra))
                        contavel++;
                    i++;
                }
            }
            fclose(file);
            
            int len = strlen(arquivoPDF);
			for (int c = 0; c <= len; c++) {
    			nomesArquivos[totalResultados][c] = arquivoPDF[c];
			}	
			ocorrencias[totalResultados] = contavel;
			totalResultados++;
        }
        linhas--;
    }

    fclose(file3);
    fclose(file2);

    // ordem crescente
	if(naoAberto){
		for (int i = 0; i < totalResultados - 1; i++) {
	    	for (int j = i + 1; j < totalResultados; j++) {
	        	if (ocorrencias[i] > ocorrencias[j]) {
	    	        int temp = ocorrencias[i];
	    	        ocorrencias[i] = ocorrencias[j];
	    	        ocorrencias[j] = temp;
	
	    	        char tempNome[100];
	    	        int k = 0;
	    	        while (nomesArquivos[i][k] != '\0') {
	    	            tempNome[k] = nomesArquivos[i][k];
	    	            k++;
	        	    }
	        	    tempNome[k] = '\0';
	
	        	    k = 0;
	        	    while (nomesArquivos[j][k] != '\0') {
	       		        nomesArquivos[i][k] = nomesArquivos[j][k];
	        	        k++;
	       		    }
	            	nomesArquivos[i][k] = '\0';
	
	            	k = 0;
	            	while (tempNome[k] != '\0') {
	            	    nomesArquivos[j][k] = tempNome[k];
	            	    k++;
	            	}
	            	nomesArquivos[j][k] = '\0';
	        	}
	    	}
		}
			
		printf("\nAnalisando arquivos");
		sleep(1);
		printf(".");
		sleep(1);
		printf(".");
		sleep(1);
		printf(".\n");
		
	    // Impressão final ordenada
	    printf("\nResultados da busca (ordem crescente de ocorrencias):\n");
	    for (int i = 0; i < totalResultados; i++) {
	    	sleep(1);
	        printf("Arquivo: %s - Ocorrencias: %d\n", nomesArquivos[i], ocorrencias[i]);
	    }
	
	    // Resumo final
	    int encontrados = 0;
	    for (int i = 0; i < totalResultados; i++) {
	        if (ocorrencias[i] > 0) {
	            encontrados++;
	        }
	    }
	    sleep(1);
	    printf("\nResumo: Palavra encontrada em %d dos %d arquivos.\n", encontrados, totalResultados);
	}else{
		printf("\nNenhum arquivo foi aberto! Analise inconclusiva!");
	}
	
    system("del /f listaTXT.txt");
    system("del /f listaPDF.txt");

    return 0;
}

