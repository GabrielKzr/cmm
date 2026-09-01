CC = gcc
FLEX = flex

CFLAGS = -Wall -Wextra -std=c11 
CFLAGS_LEXER = -D_POSIX_C_SOURCE=200809L

INCLUDES = -I syntactic

BUILD = build
TARGET = $(BUILD)/lexer

# sources
LEXER = lexer/lexer.flex
LEXER_C = $(BUILD)/lex.yy.c
LEXER_O = $(BUILD)/lex.yy.o

SOURCES = \
	main.c \
	syntactic/asdr_sample.c \

# test
TEST = tests/main2.cmm

all: $(TARGET)

# lexer generation
$(LEXER_C): $(LEXER)
	mkdir -p $(BUILD)
	$(FLEX) -o $@ $<

$(LEXER_O): $(LEXER_C)
	$(CC) $(CFLAGS_LEXER) $(INCLUDES) -Wno-sign-compare -c $< -o $@

$(TARGET): $(LEXER_O) $(SOURCES)
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) $(LEXER_O) $(SOURCES) -o $@

# run lexer
test: $(TARGET)
	@./$(TARGET) < $(TEST)

# clean
clean:
	rm -rf $(BUILD)