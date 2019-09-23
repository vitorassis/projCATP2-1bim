#include <stdio.h>
#include <windows.h>

#define TF 400

#include "structs.h"
#include "interface.h"


int main(void){
	setlocale(LC_ALL, "Portuguese");
	aluno alunos[TF];
	int coord;
	int TL=0;
	drawCanvas();
	do{
		coord = showMainMenu(TL);
		clearCanvas();
		switch(coord){
			case OPC_INCLUIR:
				drawNotasInterface(alunos, TL);
				break;
			case OPC_CONSULTAR:
				drawConsultarInterface(alunos, TL);
				break;
			case OPC_ALTERAR:
				break;
			case OPC_EXCLUIR:
				break;
			case OPC_EXIBIR:
				break;
			case OPC_SAIR:
				clearScreen();
				system("exit");
		}
		clearCanvas();
	}while(coord != OPC_SAIR);
	
}
