# pipex
Este proyecto consiste en simular el funcionamiento de un pipe de linux, es decir, enlazar la salida de un comando con la entrada del siguiente.
El funcionamiento tendrá que ser igual esta sintaxis de shell.
```shell
<infile cmd1 -options | comd2 -options >outfile
```
pipex se ejecutará de la siguiente forma
```shell
./pipex infile "cmd1 -optionns" "cmd2 -options" oufile
```

Para estar seguras de que el funcionamiento es el indicado habrá que tener en cuenta varias cosas como el tiempo de ejecución, códigos de error en la salida, _file descriptors_ que se queden abiertos, _leaks_ y por supuesto que la salida sea la esperada.

## Funciones permitidas
Solo aparecen explicadas las funciones que se usan en mi proyecto, que no son todas.

```C
# include <fcntl.h>      // open, access
# include <unistd.h>     // close, read, write, pipe, dup, dup2, execve, fork
# include <stdlib.h>     // malloc, free, exit, EXIT_FAILURE
# include <string.h>     // strerror
# include <stdio.h>      // perror
# include <sys/wait.h>   // wait, waitpid
```

### open()
Esta función sirve para abrir un archivo, o crearlo. En caso de que la apertura sea exitosa devuelve un _file descriptor_ (un entero positivo) y si falla devuelve un entero negativo y establece `errno` con el código de error correspondiente.
```C
int	open(const char *pathname, int flags, mode_t mode);
```
- `pathname` Ruta del archivo.
- `flags` Indica cómo se abrirá el archivo.
	Modos básicos de apertura:
	1. `O_RDONLY` Solo lectura.
	2. `O_WRONLY` Solo escritura.
	3. `O_RDWR` Lectura y escritura.
	
	Opcione adicionales:
	1. `O_CREATE` Crea el archivo si no existe, requiere el uso de `mode`.
	2. `O_EXCL` Si se usa junto a `O_CREATE` devuelve error si el archivo ya existe.
	3. `O_TRUNC` Borra el contenido si el archivo ya existe.
	4. `O_APEND` Escribe siempre al final del archivo.
	5. `O_NONBLOCK` Abre el archivo en modo no bloqueante.
- `mode` Es opcional, se usa solo junto a `O_CREATE` y define los permisos del archivo.

Ejemplo de uso:
```C
#include <fcntl.h>		// open
#include <stdio.h>		// perror
#include <unistd.h>		// write y close

int	main(void)
{
	int	fd;
	char	*mensaje;

	fd = open("archivo.txt", O_WRONLY | O_CREATE | O_TRUNC, 0777);
	if (fd < 0)
	{
		perror(NULL);	// Esto se explica más abajo.
		exit(1);		// Salir con el código de error correspondiente.
	}
	write(fd, mensaje, sizeof(mensaje) - 1);
	close(fd); 			// Esto se explica más abajo.
	return (0);
}
```
Abrir `archivo.txt` en modo lectura, si no existe, lo y si ya existe borra el contenido. `0777` idica que tanto el propietario, como el grupo como el resto de usuarios tienen todos los permiso para ese arcivo.

### acces()
`acces()` se usa para comprobar qué permisos de acceso tiene un archivo o directorio. Devuelve `0` si el archivo o directorio tienen el permiso de consulta y `-1` si no tiene el permiso o si hay un error, como que el archivo no exista, al devolver `-1` también `errno` se establece con el número de error correspondiente.
```C
int	acces(const char *pathname, int mode);
```
- `pathname` Ruta del archivo o directorio que se quiere abrir.
- `mode` Tipo de permiso que se quiere verificar.

	| Modo       | Valor | Descripción                        |
	|:----------:|:-----:|:-----------------------------------|
	| `F_OK`     | `0`   | Comprueba si el archivo existe.  |
	| `R_OK`     | `4`   | Verifica si el archivo tiene permiso de lectura.  |
	| `W_OK`     | `2`   | Verifica si el archivo tiene permiso de escritura. |
	| `X_OK`     | `1`   | Verifica si el archivo tiene permiso de ejecución. |

Ejemplo de uso:
```C
#include <unistd.h>	// acces
#include <stdio.h>	// printf

int	main(void)
{
	const char	*file = "archivo.txt";
	
	if (!acces(file, F_OK))
	{
		printf("El archivo existe\n");
		if (!acces(file, R_OK))
			printf("El archivo tiene permiso de lectura\n");
		else
			printf("El archivo no tiene permiso de lecutra\n");
		if (!acces(file, W_OK))
			printf("El archivo tiene permiso de escritura\n");
		else
			printf("El archivo no tiene permiso de escritura\n");
		if (!acces(file, X_OK))
			printf("El archivo tiene permiso de ejecución\n");
		else
			printf("El archivo no tiene permiso de ejecución\n");
	}
	return (0);
}
```
Comprobar qué permisos tiene `file`.

### close()
`close()` cierra libera el _file descriptor_ indicado para que el sistema pueda utilizar el número que deja libre para futuros archivos o recursos. Devuelve `0` si se cerró correctamente y `-1` en caso de error, estableciendo el código asociado a dicho error en `errno`. 

```C
int	close(int fd);
```
- `fd` _File descriptor_ que se desea cerrar.

	- Si `fd` está asociado a un archivo abierto, se vacía el _buffer_ (si hace falta) y se liberan los recursos asignados a ese archivo.
	- Si hay varios descriptores apuntando al mismo archivo, por ejemplo, después de usar `dup()`, solo se cierra el _file descriptor_ indicaco, sin alterar los demás.

Ejemplo de uso:
```C
#include <fcntl.h> 	// open
#include <stdio.h> 	// perror y printf
#include <unistd.h>	// close

int	main(void)
{
	int	fd;

	fd = open("archivo.txt", O_RDONLY)
	if (fd < 0)
	{
		perror(NULL); // Esto se explica más abajo.
		return (1);
	}
	printf("archivo abierto con el descriptor %d", fd);
	if (close(fd) < 0)
	{
		perror(NULL);
		return (1);
	}
	printf("Archivo cerrado sin problemas");
	return (0);
}
```
Abrir y cerrar `archivo.txt`.

### pipe()
Sirve para crear un _pipe_ o conducto de comunicación entre dos procesos, devuelve `0`si tuvo éxito y `-1` no, estableciendo el código asociado a dicho error en `errno`.
```C
int	pipe(int pipe_fd[2]);
```
- `pipe_fd` Es un array de enteros que la función rellena con _file descriptors_.

	- `pipe_fd[0]` Descriptor para lectura.
	- `pipe_fd[1]` Descriptor para escritura.d

Ejemplo de uso:
```C
#include <unistd.h> // pipe
#include <stdio.h>	// perror y printf
#include <stdlib.h> // exit y fork
#include <string.h>	// strlen

int	main(void)
{
	int		pipe_fd[2];
	pid_t	pid;
	char	mensaje_hijo[100];
	char	mensaje_padre;

	if (pipe(pipe_fd) < 0)
	{
		perror(NULL);
		exit(EXIT_FAILURE);
	}
}
```