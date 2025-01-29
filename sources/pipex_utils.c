#include "pipex.h"

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

void	ft_puterror(char *str)
{
	int	i;

	i = 0;
	while(str[i])
	{
		write(2, &str[i], 1);
		i++;
	}

}

void	exit_error(int error)
{
	if (error == 1)
	{
		ft_puterror("Error 1:\n Format error: ./pipex file1 \"cmd1\" \"cmd2\" file2\n");
		exit (-1);
	}
	else if (error == 2)
	{
		ft_puterror("Error 2:\n Failed to create pipe\n");
		exit (-1);
	}
	else if (error == 3)
	{
		ft_puterror("Error 3:\n Failed to create fork\n");
		exit (-1);
	}
	else if (error == 4)
	{
		ft_puterror("Error 4:\n Failed to open file\n");
		exit (-1);
	}
	exit(0);
}

void	free_array(char	**array)
{
	int	i;

	i = 0;
	while(array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**get_path(char **envp)
{
	int		i;
	char	*path;
	char	**dir;
	char	*tmp;

	i = 0;
	while (envp[i++])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(envp[i] + 5);
			break ;
		}
	}
	dir = ft_split(path, ':');
	free(path);
	i = 0;
	while (dir[i])
	{
		tmp = ft_strjoin(dir[i], "/");
		free(dir[i]);
		dir[i] = tmp;
		i++;
	}
	return (dir);
}
