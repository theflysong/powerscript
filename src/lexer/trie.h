#ifndef __LEXER_TRIE_HEADER__
#define __LEXER_TRIE_HEADER__

#include <lexer/lexer.h>

// 64 = 26 * 2 + 10 + 1 + 1
//      A    a   0    _ other
#define ALPHABET_SIZE (64)

typedef struct __keyword_trie__ {
    token_type type;
    struct __keyword_trie__ *child[ALPHABET_SIZE];
} keyword_trie_t;

keyword_trie_t *create_trie_node(void);
void insert_trie(keyword_trie_t *par, const char *str, token_type type);
void free_trie(keyword_trie_t *node);
token_type lookup_trie(keyword_trie_t *trie, const char *str);

#endif