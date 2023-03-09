TOOL_PREFIX :=

GCC := $(TOOL_PREFIX)gcc

PLATFORM := windows

SOURCES := main buffer error utils \
	lexer/lexer lexer/lex lexer/trie

OBJECTS := $(foreach src, $(SOURCES), src/$(src).o)

OUTPUT := pdos_script

FLAGS := -I"./src" -std=c99

LINK_FLAGS := -L"./thirdparty/psia/lib/$(PLATFORM)/"

src/%.o : src/%.c
	$(GCC) $(FLAGS) $^ -c -o $@

$(OUTPUT) : $(OBJECTS)
	$(GCC) $(LINK_FLAGS) $^ -lpsia -o $@