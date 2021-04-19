#ifndef MINISHELL_H
# define MINISHELL_H
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <signal.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include "get_next_line.h"
#include "libft.h"
typedef struct s_mishell
{
	char			**env;
	//struct s_list	*next;
}				t_mishell;
#endif