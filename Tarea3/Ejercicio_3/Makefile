ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

SOURCE = pi.c
CC = gcc
TARGET = pi
FLAGS = -lpthread -lm


all: build run

build:
	@$(CC) $(SOURCE) -o $(TARGET) $(FLAGS)
	@echo Ejecutable creado.

run:
	@echo Ejecuntando ."\n"
	@./$(TARGET)



clean:
	@\rm -f $(TARGET) *.o
	@echo Archivos limpiados.
