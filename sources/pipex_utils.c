/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psevilla <psevilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 23:40:11 by psevilla          #+#    #+#             */
/*   Updated: 2025/02/05 00:06:08 by psevilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	open_file(char *file, int read_write)
{
	int	fd;

	fd = 0;
	if (read_write == 0)
		fd = open(file, O_RDONLY, 0777);
	else if (read_write == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		perror(NULL);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	free_array(char	**array)
{
	int	i;

	i = 0;
	while (array[i])
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
	path = NULL;
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
	i = -1;
	while (dir[++i])
	{
		tmp = ft_strjoin(dir[i], "/");
		free(dir[i]);
		dir[i] = tmp;
	}
	return (dir);
}
