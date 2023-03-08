#include <lexer/lexer.h>
#include <lexer/lex.h>
#include <lexer/trie.h>
#include <utils.h>
#include <error.h>

static keyword_trie_t *root;

void init_lexer(void) {
    root = create_trie_node();

    insert_trie(root, "if", IF);
    insert_trie(root, "else", ELSE);
    insert_trie(root, "elif", ELIF);
    insert_trie(root, "do", DO);
    insert_trie(root, "while", WHILE);
    insert_trie(root, "for", FOR);
    insert_trie(root, "foreach", FOREACH);
    insert_trie(root, "switch", SWITCH);
    insert_trie(root, "case", CASE);
    insert_trie(root, "default", DEFAULT);
    insert_trie(root, "break", BREAK);
    insert_trie(root, "func", FUNC);
    insert_trie(root, "return", RETURN);
    insert_trie(root, "interaction", INTERACTION);
    insert_trie(root, "printf", PRINTF);
    insert_trie(root, "scanf", SCANF);
    insert_trie(root, "using", USING);
    insert_trie(root, "let", LET);
    insert_trie(root, "int", INT);
    insert_trie(root, "float", FLOAT);
    insert_trie(root, "char", CHAR);
    insert_trie(root, "string", STRING);
    insert_trie(root, "bool", BOOL);
    insert_trie(root, "const", CONST);
    insert_trie(root, "immediate", IMMEDIATE);
}

void terminate_lexer(void) {
    free_trie(root);
}

lexer_t *create_lexer(char *buffer) {
    lexer_t *lexer = NEW(lexer_t);

    lexer->buffer = buffer;
    lexer->last_pos = buffer;
    lexer->trie = root;
    lexer->cur_token = __lexer_lex(lexer->last_pos, &lexer->last_pos, lexer->trie);

    return lexer;
}

token_t *peek_token(lexer_t *lexer) {
    return lexer->cur_token;
}

// Will free last token
token_t *next_token(lexer_t *lexer) {
    free(lexer->cur_token);
    lexer->cur_token = __lexer_lex(lexer->last_pos, &lexer->last_pos, lexer->trie);
    return lexer->cur_token;
}

token_t *copy_token(token_t *original) {
    token_t *token = NEW(token_t);

    // copy the content
    token->content = (char *)malloc(original->content_length + 1);
    strcpy(token->content, original->content);

    token->content_length = original->content_length;
    token->type = original->type;

    return token;
}

void free_token(token_t *token) {
    free(token->content);
    free(token);
}

void free_lexer(lexer_t *lexer) {
    free_token(lexer->cur_token);
    free(lexer);
}