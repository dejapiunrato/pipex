/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psevilla <psevilla@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:09:44 by psevilla          #+#    #+#             */
/*   Updated: 2025/01/29 17:54:58 by psevilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	*ft_calloc(size_t n, size_t size)
{
	size_t	n_size;
	size_t	i;	
	void	*ptr;

	n_size = n * size;
	ptr = malloc(n_size);
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < n_size)
		((char *)ptr)[i++] = 0;
	return (ptr);
}

static int	count_words(char const *s, char c)
{
	int	in_word;
	int	j;

	in_word = 0;
	j = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			j++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (j);
}

static char	**free_memory(char **str, int word)
{
	while (word--)
		free(str[word]);
	free(str);
	return (NULL);
}

static char	*str_creator(char const *s, char c, int start)
{
	char	*str;
	int		i;
	int		j;

	j = start;
	while (s[j] && s[j] != c)
		j++;
	i = 0;
	str = (char *)malloc(sizeof(char) * (j - start + 1));
	if (!str)
		return (NULL);
	while (start < j)
		str[i++] = s[start++];
	str[i] = '\0';
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		i;
	int		word;

	i = 0;
	word = 0;
	str = (char **)ft_calloc((count_words(s, c) + 1), sizeof(char *));
	if (!str)
		return (NULL);
	while (s[i])
	{
		if (s[i] != c)
		{
			str[word++] = str_creator(s, c, i);
			if (!str[word - 1])
				return (free_memory(str, word - 1));
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (str);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
