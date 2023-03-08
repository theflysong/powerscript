#include <lexer/trie.h>
#include <utils.h>
#include <error.h>

static int turn_idx(char ch) {
    if (isupper(ch)) {
        return ch - 'A';
    }
    if (islower(ch)) {
        return ch - 'a' + 26;
    }
    if (isdigit(ch)) {
        return ch - '0' + 52;
    }
    else if (ch == '_') {
        return 62;
    }
    warnf("LEXER::TRIE::TURN_IDX", LEXER_UNEXPECTED_CHARACTER(ch));
    return 63;
}

keyword_trie_t *create_trie_node(void) {
    keyword_trie_t *node = NEW(keyword_trie_t);
    node->type = TOKEN_EOF;
    memset (node->child, 0, sizeof(keyword_trie_t *) * ALPHABET_SIZE);
    return node;
}

void insert_trie(keyword_trie_t *par, const char *str, token_type type) {
    // proper node
    if (*str == '\0') {
        par->type = type;
        return;
    }

    int idx = turn_idx(*str);

    // child node doesn't exist
    if (par->child[idx] == NULL) {
        par->child[idx] = create_trie_node();
    }

    // Recursive insert
    insert_trie(par->child[idx], str + 1, type);
}

void free_trie(keyword_trie_t *node) {
    for (int i = 0 ; i < ALPHABET_SIZE ; i ++) {
        if (node->child[i] != NULL) {
            free_trie(node->child[i]);
        }
    }
    free(node);
}

token_type lookup_trie(keyword_trie_t *trie, const char *str) {
    if (trie == NULL) {
        return TOKEN_EOF;
    }

    if (*str == '\0') {
        return trie->type;
    }
    
    int idx = turn_idx(*str);

    return lookup_trie(trie->child[idx], str + 1);
}