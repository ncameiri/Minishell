/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 18:09:40 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/28 18:17:44 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

int	only_spaces(char *line)
{
	int i;
	int a;

	i = 0;
	a = 0;
	while (line[i] != '\0')
	{
		if (line[i] != ' ')
			a++;
		i++;
	}
	if (a > 0)
		return (0);
	else
		return (1);
}

int	*add_int_to_arr(int *array, int location, int count)
{
	int a;
	int i;
	int *temp;

	i = 0;
	a = 0;

	while (i < count)
		i++;
	temp = malloc(sizeof(int) * (i + 2));
	if (temp == NULL)
		return (NULL);
	while(a < i)
	{
		temp[a] = array[a];
		a++;
	}
	temp[a++] = location;
	temp[a] = '\0';

	free(array);

	return (temp);
}

char	**add_str_to_arrarr(char **array, char *string)
{
	int a;
	int i;
	char **temp;

	i = 0;
	a = 0;
	while (array[i] != NULL)
		i++;
	temp = malloc(sizeof(char *) * (i + 2));
	if (temp == NULL)
		return (NULL);
	while(a < i)
	{
		temp[a] = ft_strdup(array[a]);
		a++;
	}
	temp[a++] = ft_strdup(string);
	temp[a] = NULL;
	free_array(array);
	return (temp);
}

void debug(int a)
{
	t_linklis *ptr;

	int i = 0;
	printf("\n------------COMMAND TABLE [%i]------------\n", a);
	printf("\n%s\n", mini_sh.cmd_tables[a]);
	ptr = mini_sh.ls_start;
	while(ptr)
	{
		printf("\n");
		printf("[ %s]\n", ptr->pre_split);


		if (ptr->type == 1)
			printf("Type %d = | \n", ptr->type);
		else if (ptr->type == 2)
			printf("Type %d = >> \n", ptr->type);
		else if (ptr->type == 3)
			printf("Type %d = > \n", ptr->type);
		else if (ptr->type == 4)
			printf("Type %d = < \n", ptr->type);
		else if (ptr->type == 8)
			printf("Type %d = last arg \n", ptr->type);

		printf("Builtin: %i\n", ptr->builtin);

		while(ptr->content[i] != NULL)
		{
			printf("arg[%i] = %s\n", i, ptr->content[i]);
			i++;
		}
		printf("arg[%i] = %s\n", i, ptr->content[i]);

		i = 0;
		ptr=ptr->next;
	}
}
