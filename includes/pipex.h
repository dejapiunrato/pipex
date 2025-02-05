/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psevilla <psevilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:09:37 by psevilla          #+#    #+#             */
/*   Updated: 2025/02/05 15:28:34 by psevilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>      // open, access
# include <unistd.h>     // close, read, write, pipe, dup, dup2, execve, fork
# include <stdlib.h>     // malloc, free, exit, EXIT_FAILURE
# include <string.h>     // strerror
# include <stdio.h>      // perror
# include <sys/wait.h>   // wait, waitpid

typedef struct s_pipex
{
	pid_t	pid[2];
	int		pipe_fd[2];
	int		status;
	int		exit_code;
}	t_pipex;

//Funciones en pipex_utils.c
int		open_file(char *file, int read_write);
void	free_array(char	**array);
char	**get_path(char **envp);

//Funciones en str_utils_1.c
char	**ft_split(char const *s, char c);

//Funciones en str_utils_2.c
int		ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);

#endif
