#include <conio2.h>

#define border 219

#define OPC_INCLUIR 10
#define OPC_CONSULTAR 11
#define OPC_ALTERAR 71
#define OPC_EXCLUIR 72
#define OPC_EXIBIR 73
#define OPC_SAIR 12

#define CURSOR 175
#define CURSOR_POS 31

#define clearScreen clrscr
#include "functions.h"

void clearCoordinates(int xi, int yi, int xf=0, int yf=0){ 	//LIMPA COORDENADAS (MELHORA PROCESSAMENTO
	xi = xi<1? 1 : xi;
	yi = yi<1? 1 : yi;
	
	xf = xf==0? xi : xf;
	yf = yf==0? yi : yf;
	
	int x, y;
	for (x = xi; x<=xf; x++){
		for (y = yi; y<=yf; y++){
			gotoxy(x, y);printf(" ");
		}                                                     
	}                             
}
	                                  
void clearCanvas(){               
	clearCoordinates(2, 4, 79, 21);   
}

void drawLine(int start, int finish, int coordinate, int horizontal=0){
	int i;
	for (i=start; i<=finish; i++){
		if(!horizontal)
			gotoxy(i, coordinate);
		else
			gotoxy(coordinate, i);
		printf("%c", border);
	}
}

void drawCanvas(){ 		//DESENHA INTERFACE (LINHAS E TÍTULO)
	drawLine(1, 80, 1); //TOPO
	drawLine(1, 24, 1, 1); //ESQUERDA
	drawLine(1, 80, 24); //BAIXO
	drawLine(1, 24, 80, 1); //DIREITA
	
	gotoxy(37,3);printf("NOTAS");
	
}


int showMainMenu(int size){
	int tecla;
	int coord=OPC_INCLUIR;
	
	char alunos_vazio[40];
	strcpy(alunos_vazio, size == 0? "|Sem registros|" : "");
	
	do{
		
		gotoxy(CURSOR_POS, coord); printf("%c", CURSOR);
		
		gotoxy(CURSOR_POS+2, OPC_INCLUIR);printf("Inserir");
		gotoxy(CURSOR_POS+2, OPC_CONSULTAR);printf("Consultar %s", alunos_vazio);
		//gotoxy(9, OPC_ALTERAR);printf("Alterar");
		//gotoxy(CURSOR_POS+2, OPC_EXCLUIR);printf("Excluir %s", alunos_vazio);
		//gotoxy(CURSOR_POS+2, OPC_EXIBIR);printf("Exibir %s", alunos_vazio);
		gotoxy(CURSOR_POS+2, OPC_SAIR);printf("Sair");
		
		tecla = getch();
		
		clearCoordinates(CURSOR_POS, coord);
		switch(tecla){
			case 72: 	//UP
				coord = coord<=OPC_INCLUIR ? OPC_SAIR : coord-1;
				if(size == 0){
					coord = OPC_INCLUIR;
				}
				break;
			case 80: 	//DOWN
				coord = coord>=OPC_SAIR ? OPC_INCLUIR : coord+1;
				if(size == 0){
					coord = OPC_SAIR;
				}
		}
	}while(tecla != 13);
	
	if(tecla == 13)
		return coord;
}

void removeToast(){
	clearCoordinates(30, 22, 52, 22);
}

void showToast(char texto[]){
	removeToast();
	gotoxy(30, 22);printf("* %s *", texto);
}

void drawNotasInterface(aluno alunos[], int &size, int editable=1, int aluno_index=-1){
	aluno _aluno;
	int x_notas, i=0;
	
	do{
		clearCanvas();
		x_notas = 10;
		_aluno = getAluno(alunos, size, aluno_index);
		
		gotoxy(10, 10);printf("RA: "); 
		if(_aluno.ra != 0) 
			printf("%d", _aluno.ra);
			
		gotoxy(40, 10);printf("Nome: %s", _aluno.nome);
		for(i=0; i<4; i++){
			gotoxy(x_notas,12);printf("Nota %d: ", i+1); 
			if(_aluno.ra != 0)
				printf("%.1f", _aluno.notas[i]);
			x_notas+=15;
		}
		
		if(editable){
			do{
				_aluno.ra = readIntVariable(14, 10, 39, 10);
				if(checkAlunoExists(alunos, size, _aluno.ra))
					showToast("RA já cadastrado!");
			}while(checkAlunoExists(alunos, size, _aluno.ra) && _aluno.ra != 0);
			removeToast();
			if(_aluno.ra != 0){
				do{
					readStringVariable(_aluno.nome, 46, 10, 78, 10);
					if(strcmp(_aluno.nome, "\0") == 0)
						showToast("Nome nulo!");
				}while(strcmp(_aluno.nome, "\0") == 0);
				removeToast();
				x_notas = 18;
				for(i=0; i<4; i++){
					do{
						_aluno.notas[i] = readFloatVariable(x_notas, 12, x_notas+6, 12);
						if(_aluno.notas[i] > 10)
							showToast("Nota maior que 10!");
						if(_aluno.notas[i] < 0)
							showToast("Nota menor que 0!");
					}while(!(_aluno.notas[i] <= 10 && _aluno.notas[i] >= 0));
					removeToast();
					x_notas += 15;
				}
				
				if(insertAlunos(alunos, size, _aluno))
					showToast("Incluído com sucesso");
				else
					showToast("Erro na inclusão");
			}
		}
	}while(_aluno.ra != 0);
}

