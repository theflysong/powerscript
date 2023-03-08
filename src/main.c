#include <stdio.h>
#include <buffer.h>
#include <lexer/lexer.h>

int main() {
    init_lexer();

    // Create the buffer
    char *buffer = create_buffer_from_file("./example/eg1.pds");
    printf("%s\n", buffer);

    // Create the lexer
    lexer_t *lexer = create_lexer(buffer);
    token_t *token = peek_token(lexer);

    // Get the token, until get the eof
    while (token->type != TOKEN_EOF) {
        printf ("(%d:'%s')\n", token->type, token->content);
        token = next_token(lexer);
    }

    free_lexer(lexer);
    free_buffer(buffer);

    terminate_lexer();

    return 0;
}