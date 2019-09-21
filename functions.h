int insertAlunos(aluno alunos[], int &size, aluno novo){ //INCLUIR NOTAS
	size++;
	alunos[size].ra = novo.ra;
	strcpy(alunos[size].nome, novo.nome);
	for (int i=0; i<4; i++){
		alunos[size].notas[i] = novo.notas[i];
	}
	
	return alunos[size].ra == novo.ra;
}

void consultarAlunos(){ 	//CONSULTAR NOTAS
}

void alterarAlunos(){ 	//ALTERAR NOTAS
	
}

void excluirAlunos(){ 	//EXCLUIR NOTAS
	
}

void exibirAlunos(){ 	//EXIBIR ALUNOS
	
}

aluno getAluno(aluno alunos[], int size, int index=-1){
	aluno _aluno;
	int i=0;
	printf("%d", alunos[i].ra);
	while(alunos[i].ra != index && i < size && index != -1){
		printf("%d", alunos[i].ra);
		i++;
	}
	if(i == size || index == -1){
		_aluno.ra = 0;
		strcpy(_aluno.nome, "\0");
		for(i; i<4; i++){
			_aluno.notas[i] = 0;
		}
	}
	else if(index != -1)
		_aluno = alunos[i];
	
	return _aluno;
}

int checkAlunoExists(aluno alunos[], int size, int ra){
	aluno _aluno = getAluno(alunos, size, ra);
	return _aluno.ra == 0 ? 0 : 1;
}

void clearCoordinates(int , int , int, int );


int readIntVariable(int xi, int yi, int xf, int yf){
	int aux;
	clearCoordinates(xi, yi, xf, yf);
	fflush(stdin);
	gotoxy(xi, yi); scanf("%d",&aux);
	
	return aux;
}

void readStringVariable(char variable[], int xi, int yi, int xf, int yf){
	clearCoordinates(xi, yi, xf, yf);
	fflush(stdin);
	gotoxy(xi, yi); gets(variable);
}

float readFloatVariable(int xi, int yi, int xf, int yf){
	float aux;
	clearCoordinates(xi, yi, xf, yf);
	fflush(stdin);
	gotoxy(xi, yi); scanf("%f",&aux);
	return aux;
}
