#include <buffer.h>
#include <utils.h>
#include <error.h>

char *create_buffer(int size) {
    return (char *)malloc(size);
}

long get_file_size(FILE *file) {
    // Get the original offset for later
    long origin_offset = ftell(file);

    if (origin_offset == -1) {
        errorf("BUFFER::GET_FILE_SIZE", STD_CALL_FAILED("long ftell(FILE *)"));
    }

    if (fseek(file, 0, SEEK_SET)) {
        errorf("BUFFER::GET_FILE_SIZE", STD_CALL_FAILED("long ftell(FILE *, long, int)"));
    }

    // Get the start offset, which is generally zero
    long start_offset = ftell(file);

    if (start_offset == -1) {
        errorf("BUFFER::GET_FILE_SIZE", STD_CALL_FAILED("long ftell(FILE *)"));
    }

    if (fseek(file, 0, SEEK_END)) {
        errorf("BUFFER::GET_FILE_SIZE", STD_CALL_FAILED("long ftell(FILE *, long, int)"));
    }

    // Get the current offset, which is generally the size of file
    long current_offset = ftell(file);

    if (current_offset == -1) {
        errorf("BUFFER::GET_FILE_SIZE", STD_CALL_FAILED("long ftell(FILE *)"));
    }

    if(fseek(file, origin_offset, SEEK_SET)) {
        errorf("BUFFER::GET_FILE_SIZE", STD_CALL_FAILED("long ftell(FILE *, long, int)"));
    }

    // Get the size of file, include the EOF
    return current_offset - start_offset + 1;
}

char *create_buffer_from_file(const char *file_name) {
    FILE *file = fopen(file_name, "r");

    // What if file doesn't exist?
    if (file == NULL) {
        errorf("BUFFER::CREATE_BUFFER_FROM_FILE", FILE_NOT_EXIST(file_name));
    }

    long size = get_file_size(file);
    char *buffer = create_buffer(size);
    // Copy data from the file
    fread(buffer, sizeof(char), size - 1, file);
    // EOF
    buffer[size - 1] = '\0';

    fclose(file);

    return buffer;
}

void free_buffer(char *buffer) {
    free(buffer);
}