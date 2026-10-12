#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// Hàm nối đường dẫn: parent + "/" + child
char* fmtpath(char *parent, char *child, char *buf) {
    char *p;
    strcpy(buf, parent);
    p = buf + strlen(buf);
    if (p > buf && *(p - 1) != '/') {
        *p++ = '/';
    }
    strcpy(p, child);
    return buf;
}

void tree(char *path, int level, int maxDepth, int onlyDir) {
    if (maxDepth != -1 && level > maxDepth)
        return;

    int fd;
    struct stat st;
    struct dirent de;
    char buf[512];

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "tree: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "tree: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type != T_DIR) {
        close(fd);
        return;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
            continue;

        // Bỏ qua thư mục . và .. 
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;

        fmtpath(path, de.name, buf);

        struct stat st_child;
        if (stat(buf, &st_child) < 0)
            continue;

        if (onlyDir && st_child.type != T_DIR)
            continue;

        for (int i = 0; i < level - 1; i++) {
            printf("|   ");
        }

        printf("|-- %s\n", de.name);

        if (st_child.type == T_DIR) {
            tree(buf, level + 1, maxDepth, onlyDir);
        }
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    char *path = ".";
    int maxDepth = -1; 
    int onlyDir = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            onlyDir = 1;
        } else if (strcmp(argv[i], "-L") == 0) {
            if (i + 1 < argc) {
                maxDepth = atoi(argv[i + 1]);
                i++;
            } else {
                fprintf(2, "tree: missing argument for -L\n");
                exit(1);
            }
        } else if (argv[i][0] == '-') {
            fprintf(2, "tree: invalid option %s\n", argv[i]);
            exit(1);
        } else {
            path = argv[i];
        }
    }

    printf("%s\n", path);
    tree(path, 1, maxDepth, onlyDir);
    exit(0);
}