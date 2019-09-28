#include <stdio.h>
#include <windows.h>

#define TF 1000

#include "structs.h"
#include "interface.h"


int main(void){
	srand(time(NULL));
	setlocale(LC_ALL, "Portuguese");
	aluno alunos[TF];
	int coord;
	int TL=0;
	drawCanvas();
	do{
		coord = showMainMenu(TL);
		clearCanvas();
		removeToast();
		switch(coord){
			case OPC_INCLUIR:
				drawNotasInterface(alunos, TL);
				break;
			case OPC_CONSULTAR:
				drawConsultarInterface(alunos, TL);
				break;
			case OPC_REMOVERPORCATEGORIA:
				drawRemoverPorCategoriaInterface(alunos, TL);
				break;
			case OPC_GERARRELAORIO:
				drawGerarRelatorioInterface(alunos, TL);
				break;
			case OPC_AUTOSEEDER:
				drawAutoSeederInterface(alunos, TL);
				break;
			case OPC_SAIR:
				clearScreen();
				system("exit");
		}
		clearCanvas();
	}while(coord != OPC_SAIR);
	
}
