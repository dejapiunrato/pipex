/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psevilla <psevilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:09:40 by psevilla          #+#    #+#             */
/*   Updated: 2025/02/05 00:09:34 by psevilla         ###   ########.fr       */
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
		exit(EXIT_FAILURE);
	}
	path = get_path(envp);
	tmp = NULL;
	tmp = search_correct_path(cmd, path, args, tmp);
	if (!tmp)
	{
		free_array(args);
		free_array(path);
		exit(127);
	}
	execve(tmp, args, envp);
	free(tmp);
	free_array(args);
	free_array(path);
	exit(127);
}

void	create_child(char **argv, int *pipe_fd, char **envp)
{
	int	fd;

	close(pipe_fd[0]);
	fd = open_file(argv[1], 0);
	if (fd < 0)
		exit(EXIT_FAILURE);
	dup2(fd, 0);
	close(fd);
	dup2(pipe_fd[1], 1);
	close(pipe_fd[1]);
	execute(argv[2], envp);
}

void	create_parent(char **argv, int *pipe_fd, char **envp)
{
	int	fd;

	fd = open_file(argv[4], 1);
	if (fd < 0)
		exit(EXIT_FAILURE);
	dup2(fd, 1);
	close(fd);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
	execute(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		exit(1);
	if (pipe(pipex.pipe_fd) < 0)
		exit(1);
	pipex.pid[0] = fork();
	if (pipex.pid[0] < 0)
		exit (1);
	if (!pipex.pid[0])
		create_child(argv, pipex.pipe_fd, envp);
	close(pipex.pipe_fd[1]);
	pipex.pid[1] = fork();
	if (pipex.pid[1] < 0)
		exit(1);
	if (!pipex.pid[1])
		create_parent(argv, pipex.pipe_fd, envp);
	close(pipex.pipe_fd[0]);
	pipex.exit_code = 0;
	waitpid(pipex.pid[0], &pipex.status, 0);
	pipex.exit_code = ((pipex.status >> 8) & 0xFF);
	waitpid(pipex.pid[1], &pipex.status, 0);
	pipex.exit_code = ((pipex.status >> 8) & 0xFF);
	return (pipex.exit_code);
}
