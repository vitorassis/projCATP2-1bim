#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define TF_SORT 15

void sortNotas(float array[], float sorted[]){
	int k, j, aux;
	
	sorted[0] = array[0];
	sorted[1] = array[1];
	sorted[2] = array[2];
	sorted[3] = array[3];

    for (k = 1; k < 4; k++) {

        for (j = 0; j < 3; j++) {

            if (sorted[j] > sorted[j + 1]) {
                aux = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = aux;
            }
        }
    }
}

int checkConceitoExists(aluno alunos[], int size, char conceito){
	int i=0;
	while(i<size && alunos[i].conceito != conceito)
		i++;
	if(i == size)
		return 0;
	else
		return 1;
}

float generateMedia(float notas[]){
	float ordenado[4];
	sortNotas(notas, ordenado);
	
	return (ordenado[1] + ordenado[2] + ordenado[3])/3;
}

char generateConceito(float media){
	char conceito;
	if(media > 8)
		conceito = 'A';
	else if(media > 6)
		conceito = 'B';
	else if(media > 4)
		conceito = 'C';
	else if(media > 2)
		conceito = 'D';
	else
		conceito = 'E';
		
	return conceito;
}

void setAluno(aluno &novo, aluno temp){
	novo.ra = temp.ra;
	strcpy(novo.nome, temp.nome);
	for (int i=0; i<4; i++){
		novo.notas[i] = temp.notas[i];
	}
	novo.media = generateMedia(novo.notas);
	novo.conceito = generateConceito(novo.media);
}

void ordenateAlenos(aluno alunos[], int size){
	int k, j;
	aluno aux;

    for (k = 1; k < size; k++) {

        for (j = 0; j < size-1; j++) {

            if (stricmp(alunos[j].nome, alunos[j + 1].nome) > 0) {
            	setAluno(aux, alunos[j]);
                setAluno(alunos[j], alunos[j+1]);
                setAluno(alunos[j+1], aux);
            }
        }
    }
}

int insertAlunos(aluno alunos[], int &size, aluno novo){ //INCLUIR NOTAS
	setAluno(alunos[size], novo);
	ordenateAlenos(alunos, size);
	
	return alunos[size++].ra == novo.ra;
}

int alterAlunos(aluno alunos[], int size, aluno temp){ 	//ALTERAR NOTAS
	int pos = 0;
	while(alunos[pos].ra != temp.ra && pos < size)
		pos++;
	if(pos == size)
		return 0;
	else{
		setAluno(alunos[pos], temp);
		ordenateAlenos(alunos, size);
		return 1;
	}
}

int removeAlunos(aluno alunos[], int &size, int ra){ 	//EXCLUIR NOTAS
	int pos = 0;
	while(alunos[pos].ra != ra && pos < size)
		pos++;
	if(pos == size)
		return 0;
		
	else{
		for(int i=pos+1; i<size; i++)
			setAluno(alunos[i-1], alunos[i]);
			
		size--;
		if(alunos[pos].ra != ra)
			return 1;
		else
			return 0;
	}
}

int removeAlunosByConceito(aluno alunos[], int &size, char conceito){
	int result = 1;
	for(int i=0; i<size; i++){
		if(alunos[i].conceito == conceito)
			result *= removeAlunos(alunos, size, alunos[i].ra);
	}
	return result;
}

aluno getAlunoByRA(aluno alunos[], int size, int key=-1){
	aluno _aluno;
	int i=0;
	while(key != -1 && alunos[i].ra != key && i < size)
		i++;
	if(i == size || key == -1){
		_aluno.ra = 0;
		strcpy(_aluno.nome, "\0");
		for(i; i<4; i++){
			_aluno.notas[i] = 0;
		}
	}
	else if(key != -1)
		setAluno(_aluno, alunos[i]);
	
	return _aluno;
}

void getAlunoByNome(aluno alunos[], aluno alunos_return[], int &size_return, int size, char key[]){
	aluno _alunos[TF];
	int i=0;
	size_return = 0;
	while(i<size){
		if(strstr(alunos[i].nome, key)){
			setAluno(alunos_return[size_return], alunos[i]);
			size_return ++;
		}
			
		i++;
	}
}

int checkAlunoExists(aluno alunos[], int size, int ra){
	aluno _aluno = getAlunoByRA(alunos, size, ra);
	return _aluno.ra == 0 ? 0 : 1;
}

void clearCoordinates(int , int , int, int );


int readIntVariable(int xi, int yi, int xf, int yf, int previous=0){
	int aux;
	int clear_untill;
	clearCoordinates(xi, yi, xf, yf);
	if(previous != 0){
		gotoxy(xi, yi+1); printf("(Atual: %d)", previous);
	}
	fflush(stdin);
	gotoxy(xi, yi); scanf("%d",&aux);
	
	if(previous != 0){
		clear_untill = xf+10 < 79 ? xf+10 : 79;
		clearCoordinates(xi, yi+1, clear_untill, yf+1);
	}
	
	return aux;
}

float readFloatVariable(int xi, int yi, int xf, int yf, float previous=0){
	float aux;
	int clear_untill;
	clearCoordinates(xi, yi, xf, yf);
	if(previous != 0){
		gotoxy(xi, yi+1); printf("(Atual: %.1f)", previous);
	}
	fflush(stdin);
	gotoxy(xi, yi); scanf("%f", &aux);
	if(previous != 0){
		clear_untill = xf+10 < 79 ? xf+10 : 79;
		clearCoordinates(xi, yi+1, clear_untill, yf+1);
	}
	
	return aux;
}

void readStringVariable(char variable[], int xi, int yi, int xf, int yf, int previous = 0){
	int clear_untill;
	char ancient[40];
	clearCoordinates(xi, yi, xf, yf);
	if(previous){
		strcpy(ancient, variable);
		gotoxy(xi, yi+1); printf("(Atual: %s)", variable);
	}
	fflush(stdin);
	gotoxy(xi, yi); gets(variable);
	
	if(previous != 0){
		clear_untill = xf+10 < 79 ? xf+10 : 79;
		clearCoordinates(xi, yi+1, clear_untill, yf+1);
		if(stricmp(variable, "\0") == 0){
			gotoxy(xi, yi); printf("%s", ancient);
			strcpy(variable, ancient);
		}
	}
}

int getAlunosByConceito(aluno alunos[], int size, aluno splitByConceito[], char conceito){
	int i, size_return = 0;
	for(i=0; i<size; i++){
		if(alunos[i].conceito == conceito)
			setAluno(splitByConceito[size_return++], alunos[i]);
	}
	return size_return;
}

int generateRelatorio(aluno alunos[], int size, char nome_arq[]){
	aluno splitByConceito[5][TF];
	int sizeSplited[5];
	for(int i=0, letra=65; i<5; i++)
		sizeSplited[i] = getAlunosByConceito(alunos, size, splitByConceito[i], letra++);
	
	
	FILE *fp;	
	fp = fopen(strcat(nome_arq, ".txt") ,"w");

	char letra;
	
	for(int conceito=0; conceito<5; conceito++){
		letra = conceito+'A';
		fprintf(fp, "Conceito %c:\n", letra);
		for(int item=0; item<sizeSplited[conceito]; item++){
			fprintf(fp, "=============\n");
			fprintf(fp, "\tRA: %d\tNome: %s\n", splitByConceito[conceito][item].ra, splitByConceito[conceito][item].nome);
			for(int y=0; y<4; y++)
				fprintf(fp, "\tNota %d: %.1f", y+1, splitByConceito[conceito][item].notas[y]);
			fprintf(fp, "\n");
			fprintf(fp, "\tMédia: %.1f\n", splitByConceito[conceito][item].media);
		}
		if(sizeSplited[conceito]==0)
			fprintf(fp, "\t*Sem alunos nesse conceito");
	}
	
	fclose(fp);
	return 1;
}

void autoSeed(aluno alunos[], int &size, int qtd){
	char nomes[TF_SORT][25]={
		"Maria", "Joao", "Jeremias", "Margarida", "Francisco", "Abraham", "Luis", "Lauro", "Hugo", "Kaio", "Pedro", "Obi-wan Kenobi", "Vanessa", "Luan Bixa", "Xiaomi > Apple"
	};
	
	aluno temp;
	int i = 0;
	
	for(i; size<TF && i<qtd; i++){
		int ra_temp;
		do{
			ra_temp = rand() % 20000+1;
		}while(checkAlunoExists(alunos, size, ra_temp));
		temp.ra = ra_temp;
		
		strcpy(temp.nome, nomes[rand() % TF_SORT]);
		
		for(int y=0; y<4; y++)
			temp.notas[y] = rand()%10;
			
		insertAlunos(alunos, size, temp);
	}
	
}
