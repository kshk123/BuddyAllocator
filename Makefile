#define dir for source files
SRC_DIR = ./src

#define dir for target *.o files, etc
OBJ_DIR = ./bin

#define any directories containing header files other than /usr/include
INCLUDES = -I ./inc

#define libraries to be included
#LIBS = -lstdc++

#define the C compiler to use
CC = gcc

#define any compile-time flags
CFLAGS = -Wall -Werror -g

# define prerequisites (source files)
SOURCES =       $(SRC_DIR)/BuddyAllocator.c

SRCS = $(notdir $(SOURCES))

# define the executable file 
MAIN = buddy

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o, $(filter %.c, $(SRCS)))

all:    $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	@-rm -rf $(OBJ_DIR)/*
	@-rm -r $(MAIN)



