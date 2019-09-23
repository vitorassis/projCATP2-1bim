#include <conio2.h>
#include <locale.h>

#define border 219

#define OPC_INCLUIR 10
#define OPC_CONSULTAR 11
#define OPC_ALTERAR 71
#define OPC_EXCLUIR 72
#define OPC_EXIBIR 73
#define OPC_SAIR 12

#define CURSOR 187
#define CURSOR_POS 31

#define OPC_CONSULTAR_RA 10
#define OPC_CONSULTAR_NOME 11
#define OPC_CONSULTAR_VOLTAR 12

#define OPC_CONSULTAR_OPTION_EXIBIR 11
#define OPC_CONSULTAR_OPTION_ALTERAR 12
#define OPC_CONSULTAR_OPTION_EXCLUIR 13
#define OPC_CONSULTAR_OPTION_MUDARBUSCA 10
#define OPC_CONSULTAR_OPTION_VOLTAR 14

#define OPC_CONSULTAR_OPTION_EXCLUIR_SIM 10
#define OPC_CONSULTAR_OPTION_EXCLUIR_CANCELAR 11

#define clearScreen clrscr
#include "functions.h"

void clearCoordinates(int xi, int yi, int xf=0, int yf=0){ 	//LIMPA COORDENADAS (MELHORA PROCESSAMENTO)
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
	clearCoordinates(2, 22, 79, 22);
}

int centralize(const char texto[]){
	return (78-strlen(texto))/2;
}

void showToast(const char texto[]){
	removeToast();
	gotoxy(centralize(texto), 22);printf("* %s *", texto);
}

void drawNotasInterface(aluno alunos[], int &size, int editable=1, int aluno_index=-1){
	
	int previous = aluno_index == -1 ? 0 : 1;
	aluno _aluno;
	int x_notas, i=0;
	
	do{
		clearCanvas();
		x_notas = 10;
		_aluno = getAlunoByRA(alunos, size, aluno_index);
		
		gotoxy(10, 10);printf("RA: "); 
		if(_aluno.ra != 0) 
			printf("%d", _aluno.ra);
			
		gotoxy(40, 10);printf("Nome: %s", _aluno.nome);
		for(i=0; i<4; i++){
			gotoxy(x_notas,14);printf("Nota %d: ", i+1); 
			if(_aluno.ra != 0)
				printf("%.1f", _aluno.notas[i]);
			x_notas+=15;
		}
		
		if(editable){
			if(aluno_index == -1)
				do{
					_aluno.ra = readIntVariable(14, 10, 39, 10, previous == 1 ? _aluno.ra: 0);
					if(checkAlunoExists(alunos, size, _aluno.ra))
						showToast("RA já cadastrado!");
				}while(checkAlunoExists(alunos, size, _aluno.ra) && _aluno.ra != 0);
			else{
				gotoxy(14, 10); printf("%d", _aluno.ra);
			}
			removeToast();
			if(_aluno.ra != 0){
				do{
					readStringVariable(_aluno.nome, 46, 10, 78, 10, previous);
					if(strcmp(_aluno.nome, "\0") == 0 && aluno_index == -1)
						showToast("Nome nulo!");
				}while(strcmp(_aluno.nome, "\0") == 0 && aluno_index == -1);
				removeToast();
				x_notas = 18;
				for(i=0; i<4; i++){
					do{
						_aluno.notas[i] = readFloatVariable(x_notas, 14, x_notas+6, 14, previous == 1 ? _aluno.notas[i]: 0);
						if(_aluno.notas[i] > 10)
							showToast("Nota maior que 10!");
						if(_aluno.notas[i] < 0)
							showToast("Nota menor que 0!");
					}while(!(_aluno.notas[i] <= 10 && _aluno.notas[i] >= 0));
					removeToast();
					x_notas += 15;
				}
				if(aluno_index== -1){
					if(insertAlunos(alunos, size, _aluno))
						showToast("Incluído com sucesso");
					else
						showToast("Erro na inclusão");
				}
				else if(editable){
					if(alterAlunos(alunos, size, _aluno))
						showToast("Alterado com sucesso");
					else
						showToast("Erro na alteração");
				}
					
				
			}
		}else{
			showToast("Pressione uma tecla para voltar");
			getch();
		}
		
	}while(_aluno.ra != 0 && (editable && aluno_index == -1));
}

int showConsultarMenu(){
	int tecla;
	int coord=OPC_CONSULTAR_RA;
	
	do{
		
		gotoxy(CURSOR_POS, coord); printf("%c", CURSOR);
		
		gotoxy(CURSOR_POS+2, OPC_CONSULTAR_RA);printf("RA");
		gotoxy(CURSOR_POS+2, OPC_CONSULTAR_NOME);printf("Nome");
		gotoxy(CURSOR_POS+2, OPC_CONSULTAR_VOLTAR);printf("Voltar");
		
		tecla = getch();
		
		clearCoordinates(CURSOR_POS, coord);
		switch(tecla){
			case 72: 	//UP
				coord = coord<=OPC_CONSULTAR_RA ? OPC_CONSULTAR_VOLTAR : coord-1;
				break;
			case 80: 	//DOWN
				coord = coord>=OPC_CONSULTAR_VOLTAR ? OPC_CONSULTAR_RA : coord+1;
		}
	}while(tecla != 13);
	return coord;
}

int drawConsultarOptionsMenu(){
	int tecla;
	int coord=OPC_CONSULTAR_OPTION_MUDARBUSCA;
	
	do{
		
		gotoxy(CURSOR_POS, coord); printf("%c", CURSOR);
		
		gotoxy(CURSOR_POS+2, OPC_CONSULTAR_OPTION_EXIBIR);printf("Exibir");
		gotoxy(CURSOR_POS+2, OPC_CONSULTAR_OPTION_ALTERAR);printf("Alterar");
		gotoxy(CURSOR_POS+2, OPC_CONSULTAR_OPTION_EXCLUIR);printf("Excluir");
		gotoxy(CURSOR_POS+2, OPC_CONSULTAR_OPTION_MUDARBUSCA);printf("Mudar Busca");
		gotoxy(CURSOR_POS+2, OPC_CONSULTAR_OPTION_VOLTAR);printf("Voltar");
		
		tecla = getch();
		
		clearCoordinates(CURSOR_POS, coord);
		switch(tecla){
			case 72: 	//UP
				coord = coord<=OPC_CONSULTAR_OPTION_MUDARBUSCA ? OPC_CONSULTAR_OPTION_VOLTAR : coord-1;
				break;
			case 80: 	//DOWN
				coord = coord>=OPC_CONSULTAR_OPTION_VOLTAR ? OPC_CONSULTAR_OPTION_MUDARBUSCA : coord+1;
		}
	}while(tecla != 13);
	return coord;
}

int drawRemoveMenu(){
	int tecla;
	int coord=OPC_CONSULTAR_OPTION_EXCLUIR_SIM;
	
	do{
		
		gotoxy(CURSOR_POS, coord); printf("%c", CURSOR);
		
		gotoxy(CURSOR_POS+2, OPC_CONSULTAR_OPTION_EXCLUIR_SIM);printf("Sim");
		gotoxy(CURSOR_POS+2, OPC_CONSULTAR_OPTION_EXCLUIR_CANCELAR);printf("Cancelar");
		
		tecla = getch();
		
		clearCoordinates(CURSOR_POS, coord);
		switch(tecla){
			case 72: 	//UP
				coord = coord<=OPC_CONSULTAR_OPTION_EXCLUIR_SIM ? OPC_CONSULTAR_OPTION_EXCLUIR_CANCELAR : coord-1;
				break;
			case 80: 	//DOWN
				coord = coord>=OPC_CONSULTAR_OPTION_EXCLUIR_CANCELAR ? OPC_CONSULTAR_OPTION_EXCLUIR_SIM : coord+1;
		}
	}while(tecla != 13);
	return coord;
}

void drawRemoveInterface(aluno alunos[], int &size, int ra){
	clearCanvas();
	aluno _aluno = getAlunoByRA(alunos, size, ra);
	int coord;
	
	gotoxy(30, 6); printf("Deseja mesmo remover:");
	gotoxy(centralize(_aluno.nome), 7); printf("%s", _aluno.nome);
	
	coord = drawRemoveMenu();
	
	switch(coord){
		case OPC_CONSULTAR_OPTION_EXCLUIR_SIM:
			if(removeAlunos(alunos, size, ra))
				showToast("Removido com sucesso");
			else
				showToast("Excluído com sucesso");
			getch();
	}
	removeToast();
}

void drawSearchByRAInterface(aluno alunos[], int &size){
	int ra;
	int coord = OPC_CONSULTAR_OPTION_MUDARBUSCA;
	aluno _aluno;
	do{
		clearCanvas();
		removeToast();
		gotoxy(35, 6);printf("Consulta por RA:");
		gotoxy(20, 8);printf("RA (0 para voltar): ");
		if(coord == OPC_CONSULTAR_OPTION_MUDARBUSCA)
			ra = readIntVariable(40, 8, 79, 8);
		else{
			gotoxy(40,8);printf("%d", ra);
		}
		if(checkAlunoExists(alunos, size, ra)){
			removeToast();
			coord = drawConsultarOptionsMenu();
			switch(coord){
				case OPC_CONSULTAR_OPTION_EXIBIR:
					drawNotasInterface(alunos, size, 0, ra);
					break;
				case OPC_CONSULTAR_OPTION_ALTERAR:
					drawNotasInterface(alunos, size, 1, ra);
					break;
				case OPC_CONSULTAR_OPTION_EXCLUIR:
					drawRemoveInterface(alunos, size, ra);
					break;
				case OPC_CONSULTAR_OPTION_VOLTAR:
					ra = 0;
			}
		}
		else{
			showToast("RA não encontrado!");
			coord = OPC_CONSULTAR_OPTION_MUDARBUSCA;
			getch();
		}
	}while(ra != 0);	
}

void drawConsultarInterface(aluno alunos[], int &size){
	int coord;
	
	do{
		clearCanvas();
		gotoxy(35, 6);printf("Consultar por: ");
		coord = showConsultarMenu();
		clearCanvas();
		switch(coord){
			case OPC_CONSULTAR_RA:
				drawSearchByRAInterface(alunos, size);
				break;
			case OPC_CONSULTAR_NOME: 
				break;
		}
	}while(coord != OPC_CONSULTAR_VOLTAR);
	
}
