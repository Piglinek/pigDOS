#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

typedef enum {
    CMD_UNKNOWN,
    CMD_DIR,
    CMD_COPY,
    CMD_REN,
    CMD_RENAME,
    CMD_DEL,
    CMD_ERASE,
    CMD_TYPE,
    CMD_DATE,
    CMD_TIME,
    CMD_CLS,
    CMD_VER,
    CMD_VOL,
    CMD_MKDIR,
    CMD_RMDIR,
    CMD_CD,
    CMD_EXIT
} Command;

Command parse_command(const char* input) {
    if (strcmp(input, "dir") == 0) return CMD_DIR;
    if (strcmp(input, "copy") == 0) return CMD_COPY;
    if (strcmp(input, "ren") == 0) return CMD_REN;
    if (strcmp(input, "rename") == 0) return CMD_RENAME;
    if (strcmp(input, "del") == 0) return CMD_DEL;
    if (strcmp(input, "erase") == 0) return CMD_ERASE;
    if (strcmp(input, "type") == 0) return CMD_TYPE;
    if (strcmp(input, "date") == 0) return CMD_DATE;
    if (strcmp(input, "time") == 0) return CMD_TIME;
    if (strcmp(input, "cls") == 0) return CMD_CLS;
    if (strcmp(input, "ver") == 0) return CMD_VER;
    if (strcmp(input, "vol") == 0) return CMD_VOL;
    if (strcmp(input, "mkdir") == 0) return CMD_MKDIR;
    if (strcmp(input, "rmdir") == 0) return CMD_RMDIR;
    if (strcmp(input, "cd") == 0) return CMD_CD;
    if (strcmp(input, "exit") == 0) return CMD_EXIT;
    return CMD_UNKNOWN;
}

// --- Command implementations ---

void cmd_dir(const char* path) {
    const char* dir_path = (*path) ? path : ".";
    DIR* d = opendir(dir_path);
    if (!d) { printf("Directory not found: %s\n", dir_path); return; }

    struct dirent* entry;
    while ((entry = readdir(d)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(d);
}

void cmd_type(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) { printf("File not found: %s\n", filename); return; }

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }
    fclose(f);
}

void cmd_del(const char* filename) {
    if (remove(filename) == 0) {
        printf("Deleted: %s\n", filename);
    } else {
        printf("Failed to delete: %s\n", filename);
    }
}

void cmd_mkdir(const char* path) {
    if (mkdir(path, 0777) == 0) {
        printf("Directory created: %s\n", path);
    } else {
        printf("Failed to create directory: %s\n", path);
    }
}

void cmd_rmdir(const char* path) {
    if (rmdir(path) == 0) {
        printf("Directory removed: %s\n", path);
    } else {
        printf("Failed to remove directory: %s\n", path);
    }
}

void cmd_cd(const char* path) {
    if (chdir(path) == 0) {
        char cwd[512];
        getcwd(cwd, sizeof(cwd));
        printf("Current directory: %s\n", cwd);
    } else {
        printf("Failed to change directory: %s\n", path);
    }
}

void cmd_copy(const char* args) {
    // TODO: implement full copy later, placeholder for now
    printf("[COPY] Args: %s\n", args);
}

void cmd_ren(const char* args) {
    // TODO: implement rename later
    printf("[REN] Args: %s\n", args);
}

// Other placeholders
void cmd_date() { printf("[DATE] Placeholder\n"); }
void cmd_time() { printf("[TIME] Placeholder\n"); }
void cmd_cls() { printf("\033[2J\033[H"); }
void cmd_ver() { printf("[VER] PigOS v0.1\n"); }
void cmd_vol() { printf("[VOL] Volume info placeholder\n"); }

void execute_command(Command cmd, const char* args) {
    switch(cmd) {
        case CMD_DIR: cmd_dir(args); break;
        case CMD_TYPE: cmd_type(args); break;
        case CMD_DEL:
        case CMD_ERASE: cmd_del(args); break;
        case CMD_MKDIR: cmd_mkdir(args); break;
        case CMD_RMDIR: cmd_rmdir(args); break;
        case CMD_CD: cmd_cd(args); break;
        case CMD_COPY: cmd_copy(args); break;
        case CMD_REN:
        case CMD_RENAME: cmd_ren(args); break;
        case CMD_DATE: cmd_date(); break;
        case CMD_TIME: cmd_time(); break;
        case CMD_CLS: cmd_cls(); break;
        case CMD_VER: cmd_ver(); break;
        case CMD_VOL: cmd_vol(); break;
        case CMD_EXIT: exit(0);
        default: printf("Unknown command: %s\n", args); break;
    }
}

int main() {
    char input[256];

    while(1) {
        char cwd[512];
        getcwd(cwd, sizeof(cwd));
        printf("%s> ", cwd);

        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0; // remove newline

        char* space = strchr(input, ' ');
        char* args = "";
        if (space) {
            *space = 0;
            args = space + 1;
        }

        Command cmd = parse_command(input);
        execute_command(cmd, args);
    }

    return 0;
}