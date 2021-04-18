#include<stdio.h>
#include "get_next_line.h"
#include "libft.h"


void	free_array(char **array)
{
	 int count;

	 count = 0; 
	while (array[count])
	{
		free(array[count]);
		count++;
	}
	free(array);
}

int command_recon(char **command) // Para saber se é um comando válido da shell (1->é valido   0->n e valido)
{
	if(!ft_strncmp(command[0], "ls ",3))
		return (1);
	else if(!ft_strncmp(command[0], "export",7) && !command[1])
	{	printf("export command\n");
		return (1);
	}
	return (0);
}

int main(int argc, char** aa, char ** env,char **ou)
{	
	char *line;
	char **splitted;
	



	int i=4;// exclui as 4 primeiras letras que sao o nome da variavel 
	while(env[6][i]!=0)
    printf("%c",env[4][i++]);// Mostra a localizacao onde o a.out e corrido (pwd actual)
	
	
	printf("\n");
	
write(1,"-->",3);
	while(get_next_line(0,&line) > 0)// stdin file descriptor
		{	
			splitted=ft_split(line, ' ');
			if(!ft_strncmp(splitted[0],"exit",5))
				break;
			else if(command_recon(splitted))// Falata verificar se existe alguma coisa em splitted para nao dar segfault
				//exec_com(line);
			
			write(1,"-->",3);
			free(line);
			free_array(splitted);
		
		}
		free_array(splitted);
		free(line);
	

}
