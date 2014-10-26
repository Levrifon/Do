#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <stdlib.h>
#include "makeargv.h" 
#include <signal.h>

#define AND 0
#define OR 1
#define CC 2
#define KILL 3

void kill_son(pid_t tab_pid[], int size){
	int i;
	for(i=0 ; i<size ; i++){
		if(tab_pid[i] != (pid_t) NULL){
			kill(tab_pid[i], SIGKILL);
		}
	}
}


int main(int argc, char* argv[]) {
	int status;
	int option_index = 0;
	int option_or_and = -1;
	int option_cc = 0;
	int option_kill = 0;
	int idx = 0;
	int idx_pid = 0;
	int nb_son = 0;
	int ok = 0;
	pid_t * tab_pid;
	pid_t son_pid;
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
			option_cc = 1;
			break;
			case KILL:
			if(option_cc != 1){
				perror("argument kill ne peut pas etre invoquer sans cc");
				exit(EXIT_FAILURE);
			}
			option_kill = 1;
			break;
		}
	}
	tab_pid = malloc((argc - optind)*sizeof(pid_t));
	while(argv[optind+idx] != NULL) {
		switch(son_pid = fork()) {
			case -1 :
				assert(0);
			case 0:
				argv[optind+idx +1] = NULL;
				assert(makeargv(argv[optind+idx], " \t", &cmdargv)>0);
				execvp(cmdargv[0],cmdargv);
				assert(0);
			default:
				if(option_kill == 1){
					tab_pid[idx] = son_pid;
				}
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
		assert((son_pid = wait(&status)) != -1);
		if(option_kill == 1){
			while(tab_pid[idx_pid] != son_pid){
				idx_pid ++;
			}
			tab_pid[idx_pid] =(pid_t) NULL;
		}
		if(WIFEXITED(status)){
			if(option_or_and == 1){
				ok = ok && (WEXITSTATUS(status) == EXIT_SUCCESS);
				if(option_cc == 1 && ok == 0){ /*si un processus renvoie 0, on est sur d'avoir raté donc on sort*/
					printf("ok : 0\n");
					if(option_kill == 1)
						kill_son(tab_pid, nb_son);
					return 0;
				}
			} else {
				ok = ok || (WEXITSTATUS(status) == EXIT_SUCCESS);
				if(option_cc == 1 && ok == 1){/*si un processus renvoie 1, on est sur d'avoir réussi donc on sort*/
					printf("ok : 1\n");
					if(option_kill == 1)
						kill_son(tab_pid, nb_son);
					return 1;
				}
			}
		}
	}

	printf("ok : %d \n", ok);
	return 0;
}
