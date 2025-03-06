#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"  

#define is_blank(chr) (chr == ' ' || chr == '\t')


void execute_command(char *v[], int argc) {
    if (fork() == 0) {
        exec(v[0], v);
        fprintf(2, "exec failed\n");
        exit(1);
    }
    wait(0);
}


int read_input(char *buf, char *v[], int argc) {
    char ch;
    int blanks = 0;
    int offset = 0;
    char *p = buf;
    int c = argc - 1; // Bat dau tu vi tri dau tien cua argv

    while (read(0, &ch, 1) > 0) {
        if (is_blank(ch)) {
            blanks++;
            continue;
        }

        if (blanks) {
            buf[offset++] = 0;
            v[c++] = p;
            p = buf + offset;
            blanks = 0;
        }

        if (ch != '\n') {
            buf[offset++] = ch;
        } else {
            buf[offset++] = 0;
            v[c++] = p;
            p = buf + offset;
            execute_command(v, c);
            c = argc - 1; // Dat lai so luong doi so
        }
    }

    if (offset > 0) {  // Xu ly dong cuoi neu khong xuong dong
        buf[offset++] = 0;
        v[c++] = p;
        execute_command(v, c);
    }

    return c; 
}

int main(int argc, char *argv[]) {
    char buf[2048];
    char *v[MAXARG];

    if (argc <= 1) {
        fprintf(2, "usage: xargs <command> [argv...]\n");
        exit(1);
    }


    for (int i = 1; i < argc; i++) 
        v[i - 1] = argv[i];
    

    read_input(buf, v, argc);

    exit(0);
}