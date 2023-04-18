.SUFFIXES:

OBJ_DIR := obj
BIN_DIR := bin

CSUFFIX := .c
C_FILES := $(wildcard *$(CSUFFIX))
O_FILES := $(addprefix $(OBJ_DIR)/,$(notdir $(C_FILES:$(CSUFFIX)=.o)))
JOBS := $(notdir $(C_FILES:$(CSUFFIX)=))
TARGETS := $(addprefix $(BIN_DIR)/,$(JOBS))

.phony: all clean list bin obj $(JOBS)

all: $(JOBS)

bin:
	mkdir bin
obj:
	mkdir obj

list:
	@echo $(JOBS)

$(JOBS): $(BIN_DIR)
	gcc -Wall -O3 -DNDEBUG -o $(patsubst %,$(BIN_DIR)/%,$@) $(patsubst %,%$(CSUFFIX),$@)



#obj/person.o: person.c
#	gcc -Wall -c -O3 -DNDEBUG -o obj/person.o person.c
#
#bin/person: obj/person.o
#	gcc -o bin/person obj/person.o

