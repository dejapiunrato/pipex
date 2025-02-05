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
#include <fcntl.h>	// open
#include <stdio.h>	// perror
#include <unistd.h>	// write y close

int	fd;
char	*mensaje;

fd = open("archivo.txt", O_WRONLY | O_CREATE | O_TRUNC, 0777);
if (fd < 0)
{
	perror(NULL);	// Esto se explica más abajo.
	exit(1);	// Salir con el código de error correspondiente.
}
write(fd, mensaje, sizeof(mensaje) - 1);
close(fd); 		// Esto se explica más abajo.
return (0);
```
Abrir `archivo.txt` en modo lectura, si no existe, lo y si ya existe borra el contenido. `0777` idica que tanto el propietario, como el grupo como el resto de usuarios tienen todos los permiso para ese arcivo.
