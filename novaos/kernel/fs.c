#include "fs.h"
#include "vga.h"
#include "utils.h"
#include "string.h"  // Our own string utilities

static struct filesystem fs;

void fs_init(void) {
    fs.file_count = 0;
    // Create root directory
    fs_create_directory("/");
    // Set initial current directory to root
    strcpy(fs.current_directory, "/");
}

int fs_create_file(const char* name, const char* content) {
    char* full_path = fs_get_full_path(name);
    if (fs.file_count >= MAX_FILES) {
        return -1; // Filesystem full
    }

    // Check if file already exists
    if (fs_find_file(full_path) != NULL) {
        return -2; // File already exists
    }

    struct file_entry* file = &fs.files[fs.file_count];
    strncpy(file->name, full_path, MAX_FILENAME_LEN - 1);
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
    char* full_path = fs_get_full_path(name);
    if (fs.file_count >= MAX_FILES) {
        return -1;
    }

    if (fs_find_file(full_path) != NULL) {
        return -2;
    }

    struct file_entry* dir = &fs.files[fs.file_count];
    strncpy(dir->name, full_path, MAX_FILENAME_LEN - 1);
    dir->name[MAX_FILENAME_LEN - 1] = '\0';
    dir->size = 0;
    dir->is_directory = 1;
    fs.file_count++;
    return 0;
}

int fs_list_directory(const char* path) {
    char* full_path = fs_get_full_path(path);
    vga_write_string("Directory listing of ");
    vga_write_string(full_path);
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
    char* full_path = fs_get_full_path(name);
    struct file_entry* file = fs_find_file(full_path);
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
    char* full_path = fs_get_full_path(name);
    for (uint32_t i = 0; i < fs.file_count; i++) {
        if (strcmp(fs.files[i].name, full_path) == 0) {
            return &fs.files[i];
        }
    }
    return NULL;
}

int fs_change_directory(const char* path) {
    // Handle absolute paths
    if (path[0] == '/') {
        if (fs_find_file(path) == NULL) {
            return -1; // Directory not found
        }
        strcpy(fs.current_directory, path);
        return 0;
    }
    
    // Handle relative paths
    char new_path[MAX_PATH_LEN];
    if (strcmp(fs.current_directory, "/") == 0) {
        strcpy(new_path, "/");
        strcat(new_path, path);
    } else {
        strcpy(new_path, fs.current_directory);
        strcat(new_path, "/");
        strcat(new_path, path);
    }
    
    if (fs_find_file(new_path) == NULL) {
        return -1; // Directory not found
    }
    
    strcpy(fs.current_directory, new_path);
    return 0;
}

const char* fs_get_current_directory(void) {
    return fs.current_directory;
}

char* fs_get_full_path(const char* name) {
    static char full_path[MAX_PATH_LEN];
    if (name[0] == '/') {
        strcpy(full_path, name);
    } else {
        if (strcmp(fs.current_directory, "/") == 0) {
            strcpy(full_path, "/");
            strcat(full_path, name);
        } else {
            strcpy(full_path, fs.current_directory);
            strcat(full_path, "/");
            strcat(full_path, name);
        }
    }
    return full_path;
} 