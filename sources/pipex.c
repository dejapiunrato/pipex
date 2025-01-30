/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psevilla <psevilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:09:40 by psevilla          #+#    #+#             */
/*   Updated: 2025/01/30 19:42:20 by psevilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*search_correct_path(char *cmd, char **path, char **args, char *tmp)
{
	int	i;

	i = 0;
	if (cmd[0] == '/')
		tmp = ft_strdup(cmd);
	else
	{
		while (path[i])
		{
			tmp = ft_strjoin(path[i], args[0]);
			if (access(tmp, X_OK) == 0)
				break ;
			free(tmp);
			tmp = NULL;
			i++;
		}
	}
	return (tmp);
}

void	execute(char *cmd, char **envp)
{
	char	**args;
	char	**path;
	char	*tmp;

	args = ft_split(cmd, ' ');
	if (!args[0])
	{
		free_array(args);
		exit_error(1);
	}
	path = get_path(envp);
	tmp = NULL;
	tmp = search_correct_path(cmd, path, args, tmp);
	if (tmp)
		execve(tmp, args, envp);
	if (!tmp)
		free(tmp);
	free_array(args);
	free_array(path);
	exit_error(5);
}

void	create_child(char **argv, int *pipe_fd, char **envp)
{
	int	fd;

	fd = open_file(argv[1], 0);
	dup2(fd, 0);
	close(fd);
	dup2(pipe_fd[1], 1);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	execute(argv[2], envp);
}

void	create_parent(char **argv, int *pipe_fd, char **envp)
{
	int	fd;

	fd = open_file(argv[4], 1);
	dup2(fd, 1);
	close(fd);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
	execute(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid[2];

	if (argc != 5)
		exit_error(1);
	if (pipe(pipe_fd) == -1)
		exit_error(2);
	pid[0] = fork();
	if (pid[0] == -1)
		exit_error(0);
	if (!pid[0])
		create_child(argv, pipe_fd, envp);
	close(pipe_fd[1]);
	pid[1] = fork();
	if (pid[1] == -1)
		exit_error(0);
	if (!pid[1])
		create_parent(argv, pipe_fd, envp);
	close(pipe_fd[0]);
	wait(NULL);
	wait(NULL);
	return (0);
}
