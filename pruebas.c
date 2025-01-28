#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

/* int	main(int argc, char **argv, char **envp)
{
	int i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
} */

int main(int argc, char **argv, char **envp) {
    char resolved_path[PATH_MAX];

    if (realpath(argv[0], resolved_path) != NULL) {
        printf("Executable path: %s\n", resolved_path);
    } else {
        perror("realpath");
    }

    return 0;
}