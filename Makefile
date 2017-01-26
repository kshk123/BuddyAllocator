#define dir for source files
SRC_DIR = ./src

#define dir for target *.o files, etc
OBJ_DIR = ./bin

#define any directories containing header files other than /usr/include
INCLUDES = -I ./inc

#define libraries to be included
#LIBS = -lstdc++

#define any compile-time flags for debug executable
DCFLAGS = -Wall -Werror -g

#define any linking-time flags for debug executable
DLFLAGS = 

#define any compile-time flags for release shared object
RCFLAGS = -fPIC -Wall -Werror -O3

#define any linking-time flags for release shared object
RLFLAGS = -shared

# define prerequisites (source files for debug executable)
DSOURCES =       $(SRC_DIR)/BuddyAllocator.c\
					$(SRC_DIR)/BuddyAllocatorTest.c

# define prerequisites (source files for release shared object)
RSOURCES =       $(SRC_DIR)/BuddyAllocator.c

DSRCS = $(notdir $(DSOURCES))

RSRCS = $(notdir $(RSOURCES))

# define the test executable file 
TESTEXE = buddyTest

# define the test executable file 
RELEASELIB = libBuddy.so

DOBJS = $(patsubst %.c,$(OBJ_DIR)/%.o, $(filter %.c, $(DSRCS)))

ROBJS = $(patsubst %.c,$(OBJ_DIR)/%.o, $(filter %.c, $(RSRCS)))

all:    $(TESTEXE) $(RELEASELIB)

test:   $(TESTEXE)

release: $(RELEASELIB)

$(TESTEXE): $(DOBJS)
	$(CC) $(DCFLAGS) $(INCLUDES) $(LIBS) -o $(TESTEXE) $(DOBJS) $(DLFLAGS) $(LIBS)

$(RELEASELIB): $(ROBJS)
	$(CC) $(RCFLAGS) $(INCLUDES) $(LIBS) -o $(RELEASELIB) $(ROBJS) $(RLFLAGS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(DCFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	@-rm -rf $(OBJ_DIR)/*
	@-rm -r $(TESTEXE) $(RELEASELIB)



