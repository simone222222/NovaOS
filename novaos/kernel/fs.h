#ifndef FS_H
#define FS_H

#include <stdint.h>

#define MAX_FILENAME_LEN 32
#define MAX_FILES 100
#define MAX_FILE_SIZE 1024
#define MAX_PATH_LEN 256

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
    char current_directory[MAX_PATH_LEN];  // Current working directory
};

// Filesystem functions
void fs_init(void);
int fs_create_file(const char* name, const char* content);
int fs_create_directory(const char* name);
int fs_list_directory(const char* path);
int fs_read_file(const char* name, char* buffer, uint32_t max_size);
struct file_entry* fs_find_file(const char* name);
int fs_change_directory(const char* path);
const char* fs_get_current_directory(void);
char* fs_get_full_path(const char* name);

#endif // FS_H 