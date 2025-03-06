#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]) {
	int ticks;
	if (argc != 2){
		printf("Usage: ticks\n");
		exit(1);
	}

	if (argv[1][0] == '-'){
		printf("Invalid interval %s\n", argv[1]);
        exit(1);
	}
	
	ticks = atoi(argv[1]);

    sleep(ticks);
	exit(0);
}