#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE 1024
#define MAX_FILES 10

const char *log_files[MAX_FILES] = {
    "/var/log/alternatives.log",
    "/var/log/apache2/access.log",
    "/var/log/apache2/error.log",
    "/var/log/apache2/other_vhosts_access.log",
    "/var/log/apt/history.log",
    "/var/log/apt/term.log",
    "/var/log/boot.log",
    "/var/log/dpkg.log",
    "/var/log/macchanger.log",
    "/var/log/Xorg.0.log"
};

void print_banner() {
    printf("\033[1;36m"); // Cyan color
    printf("###############################################\n");
    printf("#   GHOSTTRAIL - LOG EDITOR                  #\n");
    printf("#   Developed by Freya Roselle               #\n");
    printf("###############################################\n\n");
    printf("\033[0m"); // Reset color
}

void backup_log(int file_index) {
    char backup_file[256];
    snprintf(backup_file, sizeof(backup_file), "%s.bak", log_files[file_index]);
    FILE *orig = fopen(log_files[file_index], "r");
    FILE *backup = fopen(backup_file, "w");
    if (!orig || !backup) {
        perror("Error creating backup");
        return;
    }
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), orig)) {
        fputs(line, backup);
    }
    fclose(orig);
    fclose(backup);
}

void view_logs(int file_index) {
    FILE *file = fopen(log_files[file_index], "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    char line[MAX_LINE];
    int line_number = 1;
    while (fgets(line, sizeof(line), file)) {
        printf("%d: %s", line_number++, line);
    }
    fclose(file);
}

void delete_log_entry(int file_index, int target_line) {
    backup_log(file_index);
    FILE *file = fopen(log_files[file_index], "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    FILE *temp = fopen("temp.log", "w");
    if (!temp) {
        perror("Error creating temp file");
        fclose(file);
        return;
    }
    char line[MAX_LINE];
    int current_line = 1;
    while (fgets(line, sizeof(line), file)) {
        if (current_line != target_line) {
            fputs(line, temp);
        }
        current_line++;
    }
    fclose(file);
    fclose(temp);
    remove(log_files[file_index]);
    rename("temp.log", log_files[file_index]);
}

void update_log_entry(int file_index, int target_line, const char *new_content) {
    backup_log(file_index);
    FILE *file = fopen(log_files[file_index], "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    FILE *temp = fopen("temp.log", "w");
    if (!temp) {
        perror("Error creating temp file");
        fclose(file);
        return;
    }
    char line[MAX_LINE];
    int current_line = 1;
    while (fgets(line, sizeof(line), file)) {
        if (current_line == target_line) {
            fprintf(temp, "%s\n", new_content);
        } else {
            fputs(line, temp);
        }
        current_line++;
    }
    fclose(file);
    fclose(temp);
    remove(log_files[file_index]);
    rename("temp.log", log_files[file_index]);
}

void search_logs(int file_index, const char *keyword) {
    FILE *file = fopen(log_files[file_index], "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    char line[MAX_LINE];
    int line_number = 1;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, keyword)) {
            printf("%d: %s", line_number, line);
        }
        line_number++;
    }
    fclose(file);
}

int main() {
    print_banner();
    int choice, file_index, line_number;
    char new_content[MAX_LINE], keyword[MAX_LINE];
    
    while (1) {
        printf("Select a log file to modify:\n");
        for (int i = 0; i < MAX_FILES; i++) {
            printf("%d: %s\n", i + 1, log_files[i]);
        }
        printf("Enter choice (0 to exit): ");
        scanf("%d", &file_index);
        if (file_index == 0) break;
        if (file_index < 1 || file_index > MAX_FILES) {
            printf("Invalid selection.\n");
            continue;
        }
        file_index--;
        
        printf("1. View logs\n2. Delete a log entry\n3. Update a log entry\n4. Search logs\nChoose an action: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                view_logs(file_index);
                break;
            case 2:
                printf("Enter line number to delete: ");
                scanf("%d", &line_number);
                delete_log_entry(file_index, line_number);
                break;
            case 3:
                printf("Enter line number to update: ");
                scanf("%d", &line_number);
                printf("Enter new content: ");
                getchar();
                fgets(new_content, MAX_LINE, stdin);
                new_content[strcspn(new_content, "\n")] = 0;
                update_log_entry(file_index, line_number, new_content);
                break;
            case 4:
                printf("Enter keyword to search: ");
                scanf("%s", keyword);
                search_logs(file_index, keyword);
                break;
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}
