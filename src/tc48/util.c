#include <tc48/util.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

tc48_memory* tc48_load_t48b(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "error: cannot open '%s'\n", path);
        return NULL;
    }

    char magic[4];
    if (fread(magic, 1, 4, f) != 4 || memcmp(magic, "T48B", 4) != 0) {
        fprintf(stderr, "error: invalid T48B magic in '%s'\n", path);
        fclose(f);
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fprintf(stderr, "error: cannot seek '%s'\n", path);
        fclose(f);
        return NULL;
    }

    long file_size = ftell(f);
    if (file_size < 4) {
        fprintf(stderr, "error: file too small: '%s'\n", path);
        fclose(f);
        return NULL;
    }

    size_t tryte_count = (size_t)(file_size - 4) / sizeof(tc48_tryte);
    if (fseek(f, 4, SEEK_SET) != 0) {
        fprintf(stderr, "error: cannot seek '%s'\n", path);
        fclose(f);
        return NULL;
    }

    tc48_memory* mem = tc48_mem_alloc((tc48_word)tryte_count);
    if (!mem) {
        fprintf(stderr, "error: out of memory\n");
        fclose(f);
        return NULL;
    }

    size_t nread = fread(mem->data, sizeof(tc48_tryte), tryte_count, f);
    fclose(f);
    if (nread != tryte_count) {
        fprintf(stderr, "error: short read from '%s'\n", path);
        tc48_mem_free(mem);
        return NULL;
    }

    return mem;
}

