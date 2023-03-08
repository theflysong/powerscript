#ifndef __ERROR_HEADER__
#define __ERROR_HEADER__

#include <utils.h>

// Warn Format
void warnf(const char *callee, const char *type, const char *fmt, ...);
// Error Format
void errorf(const char *callee, const char *type, const char *fmt, ...);

// Kinds of error

// Call failed macros
#define CALL_FAILED(func, msg) \
    "%s failed, msg: `%s`", \
    func, msg

#define STD_CALL_FAILED_TYPE "StandardCallingFailed"
#define STD_CALL_FAILED(func) \
    STD_CALL_FAILED_TYPE, \
    CALL_FAILED(func, strerror(errno))

// general
#define ILLEGAL_ARGUMENT_TYPE "IllegalArgument"
#define ILLEGAL_ARGUMENT(argument) \
    ILLEGAL_ARGUMENT_TYPE, \
    "argument %s is illegal", \
    argument

#define ILLEGAL_STATE_TYPE "IllegalStateError"
#define ILLEGAL_STATE(state) \
    ILLEGAL_STATE_TYPE, \
    "state %s is illegal", \
    state

// file
#define FILE_ERROR_TYPE "FileError"

#define FILE_NOT_EXIST_TYPE "FileNotExistError"
#define FILE_NOT_EXIST(file) \
    FILE_NOT_EXIST_TYPE, \
    "File %s doesn't exist", \
    file

// lexer
#define LEXER_ERROR_TYPE "LexerError"

#define LEXER_UNEXPECTED_CHARACTER_TYPE "UnexpectedCharacter"
#define LEXER_UNEXPECTED_CHARACTER(ch) \
    LEXER_UNEXPECTED_CHARACTER_TYPE, \
    "Unexpected character '%c'(ascii: %d)", \
    ch, ch

#endif