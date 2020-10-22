#include "./razor_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *rz_LoadFile(const char *filename)
{
    FILE *file = fopen(filename, "rt");

    if (file == NULL) {
        return NULL;
    }

    size_t file_size;

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *string = malloc((file_size + 1) * sizeof * string);
    int string_position = 0;

#define RZ_LOAD_FILE_BUFFER_SIZE 1024

    size_t chars_read;
    char buffer[RZ_LOAD_FILE_BUFFER_SIZE];

    do {
        chars_read = fread(buffer, sizeof * buffer, RZ_LOAD_FILE_BUFFER_SIZE, file);
        memcpy(&string[string_position], buffer, chars_read * sizeof(char));
        string_position += chars_read;
    }
    while (chars_read > 0);

#undef RZ_LOAD_FILE_BUFFER_SIZE

    string[string_position] = '\0';

    return string;
}

