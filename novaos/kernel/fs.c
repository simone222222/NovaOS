#include "fs.h"
#include "vga.h"
#include "utils.h"
#include "string.h"  // Our own string utilities

static struct filesystem fs;

void fs_init(void) {
    fs.file_count = 0;
    // Create root directory
    fs_create_directory("/");
}

int fs_create_file(const char* name, const char* content) {
    if (fs.file_count >= MAX_FILES) {
        return -1; // Filesystem full
    }

    // Check if file already exists
    if (fs_find_file(name) != NULL) {
        return -2; // File already exists
    }

    struct file_entry* file = &fs.files[fs.file_count];
    strncpy(file->name, name, MAX_FILENAME_LEN - 1);
    file->name[MAX_FILENAME_LEN - 1] = '\0';
    
    if (content != NULL) {
        strncpy((char*)file->data, content, MAX_FILE_SIZE - 1);
        file->size = strlen(content);
    } else {
        file->size = 0;
    }
    
    file->is_directory = 0;
    fs.file_count++;
    return 0;
}

int fs_create_directory(const char* name) {
    if (fs.file_count >= MAX_FILES) {
        return -1;
    }

    if (fs_find_file(name) != NULL) {
        return -2;
    }

    struct file_entry* dir = &fs.files[fs.file_count];
    strncpy(dir->name, name, MAX_FILENAME_LEN - 1);
    dir->name[MAX_FILENAME_LEN - 1] = '\0';
    dir->size = 0;
    dir->is_directory = 1;
    fs.file_count++;
    return 0;
}

int fs_list_directory(const char* path) {
    vga_write_string("Directory listing of ");
    vga_write_string(path);
    vga_write_string(":\n");
    
    for (uint32_t i = 0; i < fs.file_count; i++) {
        struct file_entry* entry = &fs.files[i];
        if (entry->is_directory) {
            vga_write_string("[DIR] ");
        } else {
            vga_write_string("      ");
        }
        vga_write_string(entry->name);
        vga_write_string("\n");
    }
    return 0;
}

int fs_read_file(const char* name, char* buffer, uint32_t max_size) {
    struct file_entry* file = fs_find_file(name);
    if (file == NULL) {
        return -1; // File not found
    }

    if (file->is_directory) {
        return -2; // Is a directory
    }

    uint32_t copy_size = (file->size < max_size) ? file->size : max_size;
    memcpy(buffer, file->data, copy_size);
    buffer[copy_size] = '\0';
    return copy_size;
}

struct file_entry* fs_find_file(const char* name) {
    for (uint32_t i = 0; i < fs.file_count; i++) {
        if (strcmp(fs.files[i].name, name) == 0) {
            return &fs.files[i];
        }
    }
    return NULL;
} 