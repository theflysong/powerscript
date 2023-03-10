#include <lexer/lex.h>
#include <lexer/trie.h>
#include <error.h>
#include <utils.h>

// States of lexer
typedef enum {
    LX_START, // START state
    LX_PLUS_START,
    LX_PLUS,
    LX_INCREASE,
    LX_MINUS_START,
    LX_MINUS,
    LX_DECREASE,
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
#define SET_START(offset) info->start_pos = info->position + offset
#define SET_END(offset) info->end_pos = info->position + offset
#define SKIP()
#define COMPLETE() info->complete = true

DEF_ONSTATE(LX_START) {
    if (ch == '+') {
        SET_START(0);
        GOTO(LX_PLUS_START);
    }
    else if (ch == '-') {
        SET_START(0);
        GOTO(LX_MINUS_START);
    }
    else if (isdigit(ch)) {
        SET_START(0);
        GOTO(LX_NUMBER_START);
    }
    else if (isalpha(ch) || ch == '_') {
        SET_START(0);
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
        SKIP();
    }
    else {
        errorf("LEXER::LEX::ONSTATE_LX_START", LEXER_UNEXPECTED_CHARACTER(ch));
    }
}

DEF_ONSTATE(LX_PLUS_START) {
    if (ch == '+') { // ++
        GOTO(LX_INCREASE);
        COMPLETE();
        info->buffer ++;
        SET_END(1);
    }
    else { // +
        GOTO(LX_PLUS);
        COMPLETE();
        SET_END(0);
    }
}

DEF_ONSTATE(LX_MINUS_START) {
    if (ch == '-') { // -
        GOTO(LX_DECREASE);
        COMPLETE();
        info->buffer ++;
        SET_END(1);
    }
    else { // -
        GOTO(LX_MINUS);
        COMPLETE();
        SET_END(0);
    }
}

DEF_ONSTATE(LX_NUMBER_START) {
    if (isdigit(ch)) {
        SKIP();
    }
    else {
        GOTO(LX_NUMBER);
        COMPLETE();
        SET_END(0);
    }
}

DEF_ONSTATE(LX_ALPHABET_SEQUENCE_START) {
    if (isalnum(ch) || ch == '_') {
        SKIP();
    }
    else {
        GOTO(LX_ALPHABET_SEQUENCE);
        COMPLETE();
        SET_END(0);
    }
}

token_type to_token_type(lexer_states_t state) {
    switch (state) {
    case LX_PLUS: return PLUS;
    case LX_MINUS: return MINUS;
    case LX_INCREASE: return INCREASE;
    case LX_DECREASE: return DECREASE;
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
        case LX_PLUS_START: ONSTATE(LX_PLUS_START)(&info, ch); break;
        case LX_MINUS_START: ONSTATE(LX_MINUS_START)(&info, ch); break;
        case LX_NUMBER_START: ONSTATE(LX_NUMBER_START)(&info, ch); break;
        case LX_ALPHABET_SEQUENCE_START: ONSTATE(LX_ALPHABET_SEQUENCE_START)(&info, ch); break;
        default: {
            errorf(ILLEGAL_STATE("unknown state"));
        }
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