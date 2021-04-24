/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 18:09:40 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/24 01:10:46 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

char **ft_aloc_env(char **env)
{
	int i;
	int no_envs;
	char **ret;

	i = 0;
	no_envs = 0;
	while (env[i++])
		no_envs++;
	ret = malloc(sizeof(char*)*(no_envs+1));
	if(!ret)
		return NULL;
	i = 0;
	while (i < no_envs)
	{
		ret[i] = ft_strdup(env[i]);
		i++;
	}
	ret[i] = NULL;
	return (ret);
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

void	free_global(char *f1, char *f2, char *f3, char *f4)
{
	if (ft_strcmp(f1, "line") || ft_strcmp(f2, "line")
		|| ft_strcmp(f3, "line") || ft_strcmp(f4, "line"))
		free(mini_sh.line);
	if (ft_strcmp(f1, "history") || ft_strcmp(f2, "history")
		|| ft_strcmp(f3, "history") || ft_strcmp(f4, "history"))
	{
		free_array(mini_sh.history);
		mini_sh.history = NULL;
	}
}
