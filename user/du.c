#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int option_a = 0; 
int option_s = 0;

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

uint64 du(char *path) {
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512];
    uint64 total_size = 0;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "du: cannot open %s\n", path);
        return 0;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "du: cannot stat %s\n", path);
        close(fd);
        return 0;
    }
    if (st.type == T_FILE) {
        close(fd);
        if (option_a && !option_s) {
            printf("%d\t%s\n", (int)st.size, path);
        }
        return st.size;
    }
    if (st.type == T_DIR) {
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            fmtpath(path, de.name, buf);
            total_size += du(buf);
        }
        close(fd);
        // In tổng dung lượng của thư mục nếu không có -s
        if (!option_s) {
            printf("%d\t%s\n", (int)total_size, path);
        }
        return total_size;
    }
    close(fd);
    return 0;
}

int main(int argc, char *argv[]) {
    char *path = ".";
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            option_a = 1;
        } else if (strcmp(argv[i], "-s") == 0) {
            option_s = 1;
        } else if (argv[i][0] == '-') {
            fprintf(2, "du: invalid option %s\n", argv[i]);
            exit(1);
        } else {
            path = argv[i];
        }
    }
    uint64 total = du(path);
    if (option_s) {
        printf("%d\t%s\n", (int)total, path);
    }

    exit(0);
}