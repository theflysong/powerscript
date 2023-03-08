#ifndef __LEXER_LEX_HEADER__
#define __LEXER_LEX_HEADER__

#include <lexer/lexer.h>

token_t *__lexer_lex(char *buffer, char **end_pos, void *trie);

#endif