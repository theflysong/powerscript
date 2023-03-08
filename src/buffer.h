#ifndef __BUFFER_HEADER__
#define __BUFFER_HEADER__

// Create a Buffer for later progress
char *create_buffer(int size);

// Create a Buffer from the file for the later progress
char *create_buffer_from_file(const char *file_name);

// Free the buffer
void free_buffer(char *buffer);

#endif