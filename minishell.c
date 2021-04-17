#include<stdio.h>
#include "get_next_line.h"

int main(int argc, char** aa, char ** env,char **ou)
{	
	char *line;
	



	int i=4;// exclui as 4 primeiras letras que sao o nome da variavel 
	while(env[6][i]!=0)
    printf("%c",env[6][i++]);// Mostra a localizacao onde o a.out e corrido (pwd actual)
	
	
	printf("\n");
	

	while(get_next_line(0,&line) > 0)// stdin file descriptor
		{
			printf("%s\n",line);
			if(!strcmp(line,"sair"))
				break;
		}
	
	//teste

//33

}
