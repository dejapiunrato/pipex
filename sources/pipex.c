/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psevilla <psevilla@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:09:40 by psevilla          #+#    #+#             */
/*   Updated: 2025/01/29 23:40:04 by psevilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute(char *cmd, char **envp)
{
	char	**args;
	char	**path;
	int		i;
	char	*tmp;

	args = ft_split(cmd, ' ');
	path = get_path(envp);
	i = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], args[0]);
		if (access(tmp, X_OK) == 0)
			execve(tmp, args, envp);
		free(tmp);
		i++;
	}
	free_array(args);
	free_array(path);
	exit_error(5);
}

void	create_child(char **argv, int *pipe_fd, char **envp)
{
	int	fd;

	fd = open_file(argv[1], 0);
	dup2(fd, 0);
	dup2(pipe_fd[1], 1);
	close(pipe_fd[0]);
	execute(argv[2], envp);
}

void	create_parent(char **argv, int *pipe_fd, char **envp)
{
	int	fd;

	fd = open_file(argv[4], 1);
	dup2(fd, 1);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[1]);
	execute(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (argc != 5)
		exit_error(1);
	if (pipe(pipe_fd) == -1)
		exit_error(2);
	pid = fork();
	if (pid == -1)
		exit_error(3);
	if (!pid)
		create_child(argv, pipe_fd, envp);
	create_parent(argv, pipe_fd, envp);
	return (0);
}
