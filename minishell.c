#include"minishell.h"

/*void	free_array(char **array)
{
	 int count;

	 count = 0; 
	while (array[count])
	{
		free(array[count]);
		count++;
	}
	free(array);
}*/

void read_env_var(char **env, t_mishell *mishell)
{
	int i;
	
	i = 0;
	while(env[i])
		i++;
	mishell->env = malloc(sizeof(char**)*(i+1));
	while(i>=0)
	{	
		mishell->env[i] = env[i];
		i--;
	}

}

int command_recon(char **command, t_mishell *mishell) // Para saber se é um comando válido da shell (1->é valido   0->n e valido)
{ 
	int i;
	int k;
	
    
	i = 0;
	k = 0;
	if(!ft_strncmp(command[0], "ls ",3))
	{
			return (1);
	}
	else if(!ft_strncmp(command[0], "export",7) && !command[1])
	{	
		while(mishell->env[i])
		printf("%s\n",mishell->env[i++]);
		return (1);
	}
	else if(!ft_strncmp(command[0], "pwd",4) && !command[1])
	{
		while(mishell->env[i])
		{
			if(!ft_strncmp(mishell->env[i], "PWD=",4))
			{
				k = 4;
				while(mishell->env[i][k])
				{
					write(1,&mishell->env[i][k],1);
					k++;
				}
				write(1, "\n",1);
			}
			i++;

		}

	}
	return (0);
}

int main(int argc, char** argv, char ** env,char **ou)
{	
	char *line;
	char **splitted;
	//char **env_var;
	t_mishell mishell;
	int i=0;

	read_env_var(env, &mishell);
	write(1,"-->",3);
	while(get_next_line(0,&line) > 0)// stdin file descriptor
	{	
			splitted=ft_split(line, ' ');
			if(!ft_strncmp(splitted[0],"exit",5))
				break;
			command_recon(splitted,&mishell);// Falta verificar se existe alguma coisa em splitted para nao dar segfault
				//exec_com(line);
			
			write(1,"-->",3);
				while(ou[i])
	printf("%s\n",ou[i++]);
		
	}

	/*int i=4;// exclui as 4 primeiras letras que sao o nome da variavel 
	while(env[6][i]!=0)
    printf("%c",env[4][i++]);// Mostra a localizacao onde o a.out e corrido (pwd actual)
	
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
		free(line);*/
	

}
