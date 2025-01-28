#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>      // open, access
# include <unistd.h>     // close, read, write, pipe, dup, dup2, execve, fork
# include <stdlib.h>     // malloc, free, exit
# include <string.h>     // strerror
# include <stdio.h>      // perror
# include <sys/wait.h>   // wait, waitpid

//Funciones en str_utils_1.c
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);

//Funciones en str_utils_2.c
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_strcmp(char *s1, char *s2);


#endif