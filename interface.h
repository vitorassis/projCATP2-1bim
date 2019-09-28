#include <conio2.h>
#include <locale.h>

#define border 219

#define OPC_INCLUIR 10
#define OPC_AUTOSEEDER 11
#define OPC_CONSULTAR 12
#define OPC_REMOVERPORCATEGORIA 13
#define OPC_GERARRELAORIO 14
#define OPC_SAIR 15

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

#define OPC_REMOVERPORCATEGORIA_A 10
#define OPC_REMOVERPORCATEGORIA_B 11
#define OPC_REMOVERPORCATEGORIA_C 12
#define OPC_REMOVERPORCATEGORIA_D 13
#define OPC_REMOVERPORCATEGORIA_E 14
#define OPC_REMOVERPORCATEGORIA_VOLTAR 15

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
		gotoxy(CURSOR_POS+2, OPC_AUTOSEEDER);printf("AutoSeed");
		gotoxy(CURSOR_POS+2, OPC_CONSULTAR);printf("Consultar %s", alunos_vazio);
		gotoxy(CURSOR_POS+2, OPC_REMOVERPORCATEGORIA);printf("Remover por categoria %s", alunos_vazio);
		gotoxy(CURSOR_POS+2, OPC_GERARRELAORIO);printf("Gerar Relatório %s", alunos_vazio);
		gotoxy(CURSOR_POS+2, OPC_SAIR);printf("Sair");
		
		tecla = getch();
		
		clearCoordinates(CURSOR_POS, coord);
		switch(tecla){
			case 72: 	//UP
				coord = coord<=OPC_INCLUIR ? OPC_SAIR : coord-1;
				while(size == 0 && (coord == OPC_GERARRELAORIO || coord == OPC_REMOVERPORCATEGORIA || coord == OPC_CONSULTAR))
					coord--;
				coord = coord<OPC_INCLUIR ? OPC_SAIR : coord;
				break;
			case 80: 	//DOWN
				coord = coord>=OPC_SAIR ? OPC_INCLUIR : coord+1;
				while(size == 0 && (coord == OPC_CONSULTAR || coord == OPC_REMOVERPORCATEGORIA || coord == OPC_GERARRELAORIO))
					coord++;
				coord = coord>OPC_SAIR ? OPC_INCLUIR : coord;
		}
	}while(tecla != 13);
	
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
		
		if(aluno_index != -1){
			gotoxy(10, 18); printf("Média: %.1f", _aluno.media);
			gotoxy(40, 18); printf("Conceito: %c", _aluno.conceito);	
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
		else if(ra != 0){
			showToast("RA não encontrado!");
			coord = OPC_CONSULTAR_OPTION_MUDARBUSCA;
			getch();
		}
	}while(ra != 0);	
}

int drawSelectByNomeMenu(aluno resultSearch[], int resultSize){
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


void drawConsultarInterface(aluno alunos[], int &size){
	int coord;
	drawSearchByRAInterface(alunos, size);
	
//	do{
//		clearCanvas();
//		gotoxy(35, 6);printf("Consultar por: ");
//		coord = showConsultarMenu();
//		clearCanvas();
//		switch(coord){
//			case OPC_CONSULTAR_RA:
//				drawSearchByRAInterface(alunos, size);
//				break;
//			case OPC_CONSULTAR_NOME: 
//				break;
//		}
//	}while(coord != OPC_CONSULTAR_VOLTAR);
	
}

int drawRemoverPorCategoriaMenu(aluno alunos[], int size){
	int tecla;
	int coord = OPC_REMOVERPORCATEGORIA_A;
	
	char alunos_vazio[5][40];
	strcpy(alunos_vazio[0], checkConceitoExists(alunos, size, 'A') == 0? "|Sem registros|" : "\0");
	strcpy(alunos_vazio[1], checkConceitoExists(alunos, size, 'B') == 0? "|Sem registros|" : "\0");
	strcpy(alunos_vazio[2], checkConceitoExists(alunos, size, 'C') == 0? "|Sem registros|" : "\0");
	strcpy(alunos_vazio[3], checkConceitoExists(alunos, size, 'D') == 0? "|Sem registros|" : "\0");
	strcpy(alunos_vazio[4], checkConceitoExists(alunos, size, 'E') == 0? "|Sem registros|" : "\0");
	
	for(int i=65; i<70 && !checkConceitoExists(alunos, size, i); i++){
			coord++;
		printf("%c => %d => %d\n", i, checkConceitoExists(alunos, size, i), coord);
	}
		
	
	do{
		
		gotoxy(CURSOR_POS, coord); printf("%c", CURSOR);
		
		gotoxy(CURSOR_POS+2, OPC_REMOVERPORCATEGORIA_A);printf("A %s", alunos_vazio[0]);
		gotoxy(CURSOR_POS+2, OPC_REMOVERPORCATEGORIA_B);printf("B %s", alunos_vazio[1]);
		gotoxy(CURSOR_POS+2, OPC_REMOVERPORCATEGORIA_C);printf("C %s", alunos_vazio[2]);
		gotoxy(CURSOR_POS+2, OPC_REMOVERPORCATEGORIA_D);printf("D %s", alunos_vazio[3]);
		gotoxy(CURSOR_POS+2, OPC_REMOVERPORCATEGORIA_E);printf("E %s", alunos_vazio[4]);
		gotoxy(CURSOR_POS+2, OPC_REMOVERPORCATEGORIA_VOLTAR);printf("Voltar");
		
		tecla = getch();
		
		clearCoordinates(CURSOR_POS, coord);
		switch(tecla){
			case 72: 	//UP
				coord = coord<=OPC_REMOVERPORCATEGORIA_A ? OPC_REMOVERPORCATEGORIA_VOLTAR : coord-1;
				
				if(coord == OPC_REMOVERPORCATEGORIA_E && !checkConceitoExists(alunos, size, 'E'))
					coord--;
					
				if(coord == OPC_REMOVERPORCATEGORIA_D && !checkConceitoExists(alunos, size, 'D'))
					coord--;
					
				if(coord == OPC_REMOVERPORCATEGORIA_C && !checkConceitoExists(alunos, size, 'C'))
					coord--;
					
				if(coord == OPC_REMOVERPORCATEGORIA_B && !checkConceitoExists(alunos, size, 'B'))
					coord--;
					
				if(coord == OPC_REMOVERPORCATEGORIA_A && !checkConceitoExists(alunos, size, 'A'))
					coord--;
					
				coord = coord < OPC_REMOVERPORCATEGORIA_A ? OPC_REMOVERPORCATEGORIA_VOLTAR : coord;
				break;
			case 80: 	//DOWN
				coord = coord>=OPC_REMOVERPORCATEGORIA_VOLTAR ? OPC_REMOVERPORCATEGORIA_A : coord+1;
				
				if(coord == OPC_REMOVERPORCATEGORIA_A && !checkConceitoExists(alunos, size, 'A'))
					coord++;
					
				if(coord == OPC_REMOVERPORCATEGORIA_B && !checkConceitoExists(alunos, size, 'B'))
					coord++;
					
				if(coord == OPC_REMOVERPORCATEGORIA_C && !checkConceitoExists(alunos, size, 'C'))
					coord++;
					
				if(coord == OPC_REMOVERPORCATEGORIA_D && !checkConceitoExists(alunos, size, 'D'))
					coord++;
					
				if(coord == OPC_REMOVERPORCATEGORIA_E && !checkConceitoExists(alunos, size, 'E'))
					coord++;
						
				coord = coord>OPC_REMOVERPORCATEGORIA_VOLTAR ? OPC_REMOVERPORCATEGORIA_A : coord;
		}
	}while(tecla != 13);
	
	return coord;
}

void drawRemoverPorCategoriaInterface(aluno alunos[], int &size){
	int coord;
	char conceito;
	do{
		clearCanvas();
		gotoxy(25, 6); printf("Remover por categoria:");
	
		coord = drawRemoverPorCategoriaMenu(alunos, size);
		
		removeToast();
		
		switch(coord){
			case OPC_REMOVERPORCATEGORIA_A:
				conceito = 'A';
				break;
			case OPC_REMOVERPORCATEGORIA_B:
				conceito = 'B';
				break;
			case OPC_REMOVERPORCATEGORIA_C:
				conceito = 'C';
				break;
			case OPC_REMOVERPORCATEGORIA_D:
				conceito = 'D';
				break;
			case OPC_REMOVERPORCATEGORIA_E:
				conceito = 'E';
		}
		if(coord!=OPC_REMOVERPORCATEGORIA_VOLTAR && removeAlunosByConceito(alunos, size, conceito))
			showToast("Removidos com sucesso");
	}while(coord != OPC_REMOVERPORCATEGORIA_VOLTAR);
}

void drawGerarRelatorioInterface(aluno alunos[], int size){
	char nome_arq[40];
	strcpy(nome_arq, "relatorio");
	gotoxy(centralize("Gerar relatório"), 6); printf("Gerar relatório");
	gotoxy(10, 8); printf("Nome do arquivo: ");
	do{
		readStringVariable(nome_arq, 27, 8, 50, 8, 1);
	}while(stricmp(nome_arq, "\0") == 0);
	
	removeToast();
	
	if(stricmp(nome_arq, "0") !=0){
		if(generateRelatorio(alunos, size, nome_arq))
			showToast("Relatório criado com sucesso");
		else
			showToast("Erro no relatório");
	}
}

void drawAutoSeederInterface(aluno alunos[], int &size){
	gotoxy(centralize("AutoSeed"), 6); printf("AutoSeed");
	int qtd;
	
	gotoxy(10, 8); printf("Quantidade: ");
	gotoxy(22, 9); printf("(Máx: %d)", TF-size);
	do{
		qtd = readIntVariable(22, 8, 40, 8);
	}while(qtd+size > TF);
	
	autoSeed(alunos, size, qtd);
}
