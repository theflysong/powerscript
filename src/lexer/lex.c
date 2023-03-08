#include <lexer/lex.h>
#include <lexer/trie.h>
#include <error.h>
#include <utils.h>

// States of lexer
typedef enum {
    LX_START, // START state
    LX_ADD_START,
    LX_ADD,
    LX_INCREASE,
    LX_NUMBER_START,
    LX_NUMBER,
    LX_ALPHABET_SEQUENCE_START,
    LX_ALPHABET_SEQUENCE
} lexer_states_t;

typedef struct {
    int position;
    int start_pos;
    int end_pos;
    bool complete;
    lexer_states_t state;
    char *buffer;
} lex_info_t;

#define ONSTATE(state) ONSTATE_##state
#define DEF_ONSTATE(state) static void ONSTATE(state)(lex_info_t *info, char ch)
#define GOTO(target) info->state = target

DEF_ONSTATE(LX_START) {
    if (ch == '+') {
        info->start_pos = info->position;
        GOTO(LX_ADD_START);
    }
    else if (isdigit(ch)) {
        info->start_pos = info->position;
        GOTO(LX_NUMBER_START);
    }
    else if (isalpha(ch) || ch == '_') {
        info->start_pos = info->position;
        GOTO(LX_ALPHABET_SEQUENCE_START);
    }
    else if (
        ch == ' '  ||
        ch == '\n' ||
        ch == '\r' ||
        ch == '\t' ||
        ch == '\v' ||
        ch == '\f' ||
        ch == '\0'
    ) {
        // do nothing
    }
    else {
        errorf("LEXER::LEX::ONSTATE_LX_START", LEXER_UNEXPECTED_CHARACTER(ch));
    }
}

DEF_ONSTATE(LX_ADD_START) {
    if (ch == '+') { // ++  
        GOTO(LX_INCREASE);
        info->complete = true;
        info->buffer ++;
        info->end_pos = info->position + 1;
    }
    else { // +
        GOTO(LX_ADD);
        info->complete = true;
        info->end_pos = info->position;
    }
}

DEF_ONSTATE(LX_NUMBER_START) {
    if (isdigit(ch)) {
        // do nothing
    }
    else {
        GOTO(LX_NUMBER);
        info->complete = true;
        info->end_pos = info->position;
    }
}

DEF_ONSTATE(LX_ALPHABET_SEQUENCE_START) {
    if (isalnum(ch) || ch == '_') {
        // do nothing
    }
    else {
        GOTO(LX_ALPHABET_SEQUENCE);
        info->complete = true;
        info->end_pos = info->position;
    }
}

token_type to_token_type(lexer_states_t state) {
    switch (state) {
    case LX_ADD: return ADD;
    case LX_INCREASE: return INCREASE;
    case LX_NUMBER: return NUMBER;
    case LX_ALPHABET_SEQUENCE: return IDENTIFIER;
    }
    return TOKEN_EOF;
}

// lex a token
token_t *__lexer_lex(char *buffer, char **end_pos, void *trie) {
    token_t *token = NEW(token_t);

    // lex infomations
    lex_info_t info = {
        .position = 0,
        .complete = false,
        .start_pos = 0,
        .end_pos = 0,
        .state = LX_START,
        .buffer = buffer
    };

    while (true) {
        char ch = *info.buffer;
        switch(info.state) {
        case LX_START: ONSTATE(LX_START)(&info, ch); break;
        case LX_ADD_START: ONSTATE(LX_ADD_START)(&info, ch); break;
        case LX_NUMBER_START: ONSTATE(LX_NUMBER_START)(&info, ch); break;
        case LX_ALPHABET_SEQUENCE_START: ONSTATE(LX_ALPHABET_SEQUENCE_START)(&info, ch); break;
        }

        if (info.complete || *info.buffer == '\0') {
            break;
        }

        info.buffer ++;
        info.position ++;
    }

    *end_pos = info.buffer;

    token_type ttype = to_token_type(info.state);

    if (ttype == TOKEN_EOF) {
        // end of file
        if (*info.buffer == '\0') {
            token->content = (char *)malloc(4);
            strcpy(token->content, "EOF");
            token->content_length = 3;
            token->type = TOKEN_EOF;
            return token;
        }
        else {
            // ERROR
        }
    }

    // make a token
    token->type = ttype;
    token->content = str_slice(buffer, info.start_pos, info.end_pos);
    if (token->type == IDENTIFIER) {
        token_type stype = lookup_trie((keyword_trie_t *)trie, token->content);
        if (stype != TOKEN_EOF) {
            token->type = stype;
        }
    }
    token->content_length = info.end_pos - info.start_pos;

    return token;
}