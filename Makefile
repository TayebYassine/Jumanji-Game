# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm

# Output executable
TARGET = jumanji

# Source files
SOURCES = main.c \
          constants.c \
          globals.c \
          graphics.c \
          audio.c \
          resources.c \
          player_data.c \
          menus.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Header files (for dependency tracking)
HEADERS = types.h \
          constants.h \
          globals.h \
          graphics.h \
          audio.h \
          resources.h \
          player_data.h \
          menus.h

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build successful! Run with: ./$(TARGET)"

# Compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete"

# Rebuild everything
rebuild: clean all

# Run the game
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
