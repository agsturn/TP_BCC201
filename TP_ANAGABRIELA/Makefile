#Ana Gabriela -25.1.4119

# Nome do executável
TARGET = jogo

# Compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall -Wextra -g

# Lista automaticamente todos os arquivos .c da pasta
SRCS = $(wildcard *.c)

# Gera automaticamente a lista de .o correspondentes
OBJS = $(SRCS:.c=.o)

# Regra principal
all: $(TARGET)

# Como gerar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regra genérica para compilar .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rodar o programa (compila antes se precisar)
run: $(TARGET)
	./$(TARGET)     # Linux/Mac
	# $(TARGET).exe  # Windows (descomente esta linha e comente a de cima)

# Limpar arquivos gerados
clean:
	rm -f $(OBJS) $(TARGET)

# Limpar e recompilar do zero
rebuild: clean all

