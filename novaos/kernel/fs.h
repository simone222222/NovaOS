#ifndef FS_H
#define FS_H

#include <stdint.h>

#define MAX_FILENAME_LEN 32
#define MAX_FILES 100
#define MAX_FILE_SIZE 1024

// File entry structure
struct file_entry {
    char name[MAX_FILENAME_LEN];
    uint8_t data[MAX_FILE_SIZE];
    uint32_t size;
    uint8_t is_directory;
};

// Filesystem structure
struct filesystem {
    struct file_entry files[MAX_FILES];
    uint32_t file_count;
};

// Filesystem functions
void fs_init(void);
int fs_create_file(const char* name, const char* content);
int fs_create_directory(const char* name);
int fs_list_directory(const char* path);
int fs_read_file(const char* name, char* buffer, uint32_t max_size);
struct file_entry* fs_find_file(const char* name);

#endif // FS_H 