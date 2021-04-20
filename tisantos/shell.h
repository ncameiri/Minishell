/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:59:14 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/20 14:11:48 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "./get_next_line/get_next_line.h"
# include "./libft/libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>


# define LSH_RL_BUFSIZE 1024
# define LSH_TOK_BUFSIZE 64
# define SHELL_DELIMITERS " \t\r\n\a"

#endif
