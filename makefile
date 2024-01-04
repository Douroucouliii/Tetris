CC := gcc
SRC_DIR ?= src
OBJ_DIR ?= obj
DOC_DIR ?= doc
DEBUG ?= 1

ifeq '$(DEBUG)' '1'
CFLAGS ?= -Wall -MMD -g
else
CFLAGS ?= -Wall -MMD -O3 -DNDEBUG
endif

LDFLAGS=-lncurses -lSDL2 -lSDL2_ttf -lSDL2_mixer
CFLAGS += -ISDL2/include -ISDL2_ttf/include -ISDL2_mixer/include

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

DEPS := $(OBJS:.o=.d)

TARGET ?= exec

.PHONY: clean mrpropre doc

all: createRep $(TARGET)

createRep:
	@mkdir -p $(OBJ_DIR)/$(SRC_DIR)

$(TARGET): createRep $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

doc:
	@mkdir -p $(DOC_DIR)
	@if [ -e Doxyfile ]; then \
		doxygen ./Doxyfile; \
	else \
		echo "Erreur : Doxyfile n'existe pas";\
	fi

clean:
	rm -rf $(OBJ_DIR)

mrpropre: clean
	rm -f $(TARGET)

-include $(DEPS)