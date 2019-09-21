#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <string.h>

#include "structs.h"
#include "interface.h"

#define TF 400

int main(void){
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
