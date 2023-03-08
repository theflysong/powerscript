GCC := gcc

SOURCES := main buffer error utils \
	lexer/lexer lexer/lex lexer/trie

OBJECTS := $(foreach src, $(SOURCES), src/$(src).o)

OUTPUT := pdos_script

FLAGS := -I"./src" -std=c99

LINK_FLAGS :=

src/%.o : src/%.c
	$(GCC) $(FLAGS) $^ -c -o $@

$(OUTPUT) : $(OBJECTS)
	$(GCC) $(LINK_FLAGS) $^ -o $@