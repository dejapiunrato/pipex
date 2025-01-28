#include "pipex.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	char	*str;

	i = 0;
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (NULL);
	while (*s1)
		str[i++] = *s1++;
	while (*s2)
		str[i++] = *s2++;
	str[i] = '\0';
	return (str);
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