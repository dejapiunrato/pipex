#include "pipex.h"

void	exit_error(int error)
{
	if (error == 1)
	{
		perror("Eror 1:\n Format error: ./pipex file1 cmd1 cmd2 file2\n");
		exit (0);
	}
	else if (error == 2)
	{
		perror("Error 2:\n Failed to create pipe\n");
		exit (-1);
	}
	else if (error = 3)
	{
		perror("Error 3:\n Failed to create fork\n");
		exit (-1);
	}
	else if (error = 4)
	{
		perror("Error 4:\n Failed to open file\n");
		exit (-1);
	}
	exit(0);
}

void	exec(char *cmd, char *env)
{
	
}

int	open_file(char *file, int read_write)
{
	int	fd;

	if (read_write == 0)
		fd = open(file, O_RDONLY, 0777);
	else if (read_write == 1)
		fd = open(file, O_WRONLY, 0777);
	if (fd == -1)
		exit_error(4);
	return (fd);
}

void	create_child(char **argv, int *pipe_fd, char **envp)
{
	int	fd;

	fd = open_file(argv[1], 0);
	dup2(fd, 0);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[1]);
	exec(argv[2], envp);
}

void	create_parent(char **argv, int *pipe_fd, char **envp)
{
	int	fd;

	fd = open_file(argv[4], 1);
	dup2(fd, 1);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[1]);
	exec(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (argc != 5)
		exit_error(1);
	if (pipe(pipe_fd) == -1)
		exit_error(2);
	pid =  fork();
	if (pid == -1)
		exit_error(3);
	if (!pid)
		create_child(argv, pipe_fd, envp);
	create_parent(argv, pipe_fd, envp);
	return (0);
}