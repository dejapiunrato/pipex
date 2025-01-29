/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psevilla <psevilla@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:09:47 by psevilla          #+#    #+#             */
/*   Updated: 2025/01/29 23:43:40 by psevilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		i++;
	}
	if ((s1[i] == 0 || s2[i] == 0) && i < n)
		return (s1[i] - s2[i]);
	return (0);
}

char	*ft_strdup(const char *s)
{
	size_t	size;
	char	*d;
	char	*aux;

	size = ft_strlen(s) + 1;
	d = malloc(size);
	if (d == NULL)
		return (NULL);
	aux = d;
	while (*s)
		*d++ = *s++;
	*d = '\0';
	return (aux);
}

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
