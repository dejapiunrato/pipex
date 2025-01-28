# Variables
NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I includes
SRCS_DIR = sources
OBJS_DIR = objects
SRCS = $(shell find $(SRCS_DIR) -name '*.c')
OBJS = $(patsubst $(SRCS_DIR)/%.c, $(OBJS_DIR)/%.o, $(SRCS))
RM = rm -rf

# Regla principal
all: $(NAME)

# Regla para compilar el ejecutable
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Regla para compilar los archivos .c en archivos .o
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Crear el directorio de objetos si no existe
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# Regla para limpiar los archivos objeto
clean:
	@$(RM) $(OBJS)
	@$(RM) -r $(OBJS_DIR)

# Regla para limpiar los archivos objeto y el ejecutable
fclean: clean
	@$(RM) $(NAME)

# Regla para recompilar todo
re: fclean all