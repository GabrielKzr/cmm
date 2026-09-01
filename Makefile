CC = gcc
FLEX = flex

CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L

INCLUDES = -I syntactic

BUILD = build
TARGET = $(BUILD)/lexer

# sources
LEXER = lexer/lexer.flex
LEXER_C = $(BUILD)/lex.yy.c

SOURCES = \
	$(LEXER_C) \
	main.c \
	syntactic/asdr_sample.c \
	#lexer/lexer.c \
	#lists/list.c 

# test
TEST = tests/main2.cmm


# lexer generation
$(LEXER_C): $(LEXER)
	mkdir -p $(BUILD)
	$(FLEX) -o $@ $<


# build
$(TARGET): $(SOURCES)
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o $@


all: $(TARGET)


# run lexer
test: $(TARGET)
	@./$(TARGET) < $(TEST)


# clean
clean:
	rm -rf $(BUILD)