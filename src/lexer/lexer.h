#ifndef __LEXER_LEXER_HEADER__
#define __LEXER_LEXER_HEADER__

// Kinds of token
typedef enum {
    TOKEN_EOF = -1,
    NUMBER,
    PLUS,
    MINUS,
    INCREASE,
    DECREASE,
    IDENTIFIER,
    IF,
    ELSE,
    ELIF,
    DO,
    WHILE,
    FOR,
    FOREACH,
    SWITCH,
    CASE,
    DEFAULT,
    BREAK,
    CONTINUE,
    INTERACTION,
    PRINTF,
    SCANF,
    SCANLINE,
    USING,
    LET,
    INT,
    FLOAT,
    CHAR,
    STRING,
    BOOL,
    TOKEN_TRUE,
    TOKEN_FALSE,
    CONST,
    CONSTEXPR,
    FUNC,
    RETURN,
    EXTERN,
    BUILTIN
} token_type;

typedef struct {
    char *content;
    int content_length;
    token_type type;
} token_t;

// the lexer
typedef struct {
    char *buffer;
    char *last_pos;
    token_t *cur_token;
    void *trie;
} lexer_t;

// Create a lexer to peek tokens
lexer_t *create_lexer(char *buffer);

// Initialize the lexer
void init_lexer(void);

// Terminate the lexer
void terminate_lexer(void);

// Peek a token
token_t *peek_token(lexer_t *lexer);

// Get next token
// Will free last token
token_t *next_token(lexer_t *lexer);

// Make a copy of original token
token_t *copy_token(token_t *original);

void free_token(token_t *token);
void free_lexer(lexer_t *lexer);

#endif