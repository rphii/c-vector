ifeq ($(OS),Windows_NT)
ifeq ($(XSUFFIX),)
	XSUFFIX := .exe
endif
endif

GIT_VERSION := "$(shell git describe --abbrev=4 --dirty --always --tags)"
CC	    := tcc #clang #gcc
LDFLAGS := -fsanitize=address \
		   #-rdynamic -pg \

CFLAGS  := -Wall -Wextra \
		   -I"../include/" \
		   -fsanitize=address \
		   #-rdynamic -pg \
		   -O3 -march=native \

BIN_DIR := bin
OBJ_DIR := obj
CSUFFIX := .c
JOBS    := test
TARGETS := $(addprefix $(BIN_DIR)/,$(addsuffix $(XSUFFIX),$(JOBS)))
.phony: all clean list bin obj $(JOBS) $(TARGETS)

### LINKING FUNCTION : $(call link,LDFLAGS,EXECUTABLE,PATH/TO/FILE.o PATH/TO/ANOTHERFILE.o)
# arguments
# 1: [IN]  linker options (LDFLAGS)
# 2: [OUT] executable output
# 3: [IN]  path/to/file.o path/to/anotherfile.o
define link
	@echo link	: $@ $^
	@$(CC) $1 -o $2 $3
endef

### COMPILATION FUNCTION : $(call compile,CFLAGS,OBJ_DIR,PATH/FILE.o)
# arguments
# 1: [IN]  compile options (CFLAGS)
# 2: [OUT] object directory
# 3: [IN]  src/folder
define compile_job
$2/%.o: $3/%$(CSUFFIX) Makefile | $2
	@echo compile : $$<
	@$$(CC) $1 -DVERSION=\"$$(GIT_VERSION)\" -c -MMD -o $$(addprefix $2/,$$(notdir $$(basename $$<)).o) $$<
endef

### CREATE BINARY FUNCTION : $(call CFLAGS,LDFLAGS,OBJ_DIR,BIN_PATH,PATH/TO/FILE.c PATH/TO/ANOTHERFILE.c)
# arguments
# 1: [IN]  compile options
# 2: [IN]  linker options
# 3: [IN]  object directory
# 4: [OUT] binary path
# 5: [IN]  source files
define binary
BIN_DIR := $$(patsubst %/,%,$$(dir $4))
OBJ_DIR := $3
C_FILES := $5
C_DIRS  := $$(patsubst %/,%,$$(dir $$(C_FILES)))
O_FILES := $$(addprefix $$(OBJ_DIR)/,$$(addsuffix .o,$$(notdir $$(basename $$(C_FILES)))))
D_FILES := $$(addprefix $$(OBJ_DIR)/,$$(addsuffix .d,$$(notdir $$(basename $$(C_FILES)))))

-include $$(D_FILES)

ifeq ($$(RULE_$$(BIN_DIR)),)
RULE_$$(BIN_DIR) := 1
$$(BIN_DIR):
	mkdir -p $$(BIN_DIR)
endif

ifeq ($$(RULE_$$(OBJ_DIR)),)
RULE_$$(OBJ_DIR) := 1
$$(OBJ_DIR):
	mkdir -p $$(OBJ_DIR)
endif

$$(foreach SRC,$$(C_DIRS),$$(eval $$(call compile_job,$1,$$(OBJ_DIR),$$(SRC))))

$4$$(XSUFFIX): $$(O_FILES) | $$(BIN_DIR)
	$$(call link,$2,$$@,$$^)

endef

all: $(TARGETS)
$(eval $(call binary,$(CFLAGS),$(LDFLAGS),$(OBJ_DIR),bin/test,src/main.c src/unity.c \
	src/vu8.c src/vu16.c src/vu32.c src/vu64.c \
	src/rvu8.c src/rvu16.c src/rvu32.c src/rvu64.c \
	src/vi8.c src/vi16.c src/vi32.c src/vi64.c \
	src/rvi8.c src/rvi16.c src/rvi32.c src/rvi64.c \
	str/str.c str/vstr.c src/rvstr.c \
	))

#$(BIN_DIR):
#	mkdir $(BIN_DIR)
#
#$(OBJ_DIR):
#	mkdir $(OBJ_DIR)

list:
	@echo possible binaries
	@$(foreach TARGET,$(TARGETS),echo "- $(TARGET)";)

################################################################################
### Cleaning ###################################################################
################################################################################
ifeq ($(OS),Windows_NT)
# Cleaning rules for Windows OS (no clue if this works)
clean:
	@del /q $(OBJ_DIR), $(TARGETS), $(BIN_DIR)
	@rmdir $(OBJ_DIR)
	@rmdir $(BIN_DIR)
else
# Cleaning rules for Unix-based OS
clean:
	@rm -rf $(OBJ_DIR) $(TARGETS) $(BIN_DIR)
endif

ifeq ($(OS),Windows_NT)
ifeq ($(XSUFFIX),)
	XSUFFIX := .exe
endif
endif

