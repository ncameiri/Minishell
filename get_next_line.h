/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncameiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 12:16:28 by ncameiri          #+#    #+#             */
/*   Updated: 2021/02/17 12:16:31 by ncameiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

typedef	struct	s_vars
{
	int		ret;
	char	*bu;
}				t_vars;
int				get_next_line(int fd, char **line);

size_t			ft_strlen(const char *s);

char			*ft_strjoin(char *s1, char const *s2);

int				get_line_break(char *final);

char			*ft_strdup(const char *s);

char			*remove_after_line_break(char *final);

char			*new_line_return(char *file_content, char **ret);

int				ret_func(char **f_content, char **line);

int				srch_break(char *search);

int				ret_invalid(char **line);
#endif
