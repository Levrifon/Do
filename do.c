#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <stdlib.h>
#include "makeargv.h" 

#define AND 0
#define OR 1
#define CC 2
#define KILL 3
/*pineuche!!!!!!*/
int main(int argc, char* argv[]) {
	int status;
	int option_index = 0;
	int option_or_and = -1;
	int idx = 0;
	int nb_son = 0;
	int pid;
	int ok = 0;
	char **cmdargv;
	struct option long_options[] = {
        	{"and",no_argument,0,  0 },
                {"or",no_argument,0,  0 },
                {"cc",no_argument,0,  0 },
                {"kill",no_argument,0,  0 },
		{0,0,0,0}
        };

	while (getopt_long(argc, argv,"", long_options, &option_index) != -1) {
		switch (option_index) {	
			case AND: /*long_options[0] == and*/
			option_or_and = 1;
			break;
			case OR:
			option_or_and = 0;
			break;
			case CC:
			printf("coucou3\n");
			break;
			case KILL:
			printf("coucou4\n");
			break;
		}
	}
	while(argv[optind+idx] != NULL) {
		switch(fork()) {
			case -1 :
				assert(0);
			case 0:
				argv[optind+idx +1] = NULL;
				assert(makeargv(argv[optind+idx], " \t", &cmdargv)>0);
				execvp(cmdargv[0],cmdargv);
				assert(0);
			default:
				nb_son++;
				break;		
		}
		idx++;
	}
	if(option_or_and == 1){
		ok = 1;
	} else {
		ok = 0;
	}
	for(idx=0; idx < nb_son ; idx++) {
		assert((pid = wait(&status)) != -1);
		if(WIFEXITED(status)){
			if(option_or_and == 1){
				ok = ok && (WEXITSTATUS(status) == EXIT_SUCCESS);
			} else {
				ok = ok || (WEXITSTATUS(status) == EXIT_SUCCESS);
			}
		}
	}

	printf("ok : %d \n", ok);
	return 0;
}
