#include <string.h>
#include <ctype.h>

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

float generateMedia(aluno _aluno){
	float ordenado[4];
	sortNotas(_aluno.notas, ordenado);
	
	return (ordenado[1] + ordenado[2] + ordenado[3])/3;
}

char generateConceito(aluno _aluno){
	char conceito;
	if(_aluno.media > 8)
		conceito = 'A';
	else if(_aluno.media > 6)
		conceito = 'B';
	else if(_aluno.media > 4)
		conceito = 'C';
	else if(_aluno.media > 2)
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
	novo.media = generateMedia(novo);
	novo.conceito = generateConceito(novo);
}

int insertAlunos(aluno alunos[], int &size, aluno novo){ //INCLUIR NOTAS
	setAluno(alunos[size], novo);
	
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


