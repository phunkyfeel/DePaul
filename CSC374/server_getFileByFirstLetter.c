/*  HAD TO CHANGE INCLUDE OF WAIT.H TO SYS/WAIT.H BECAUSE MY COMPUTER COULD NOT FIND THE DIRECTORY OTHERWISE   */

//server_getFileByFirstLetter.c
//
//Clayton Cohn
//CSC374
//Prof Joe
//26 May 2016

#include	"getFileByFirstLetter.h"
#include	<signal.h>		// For sigaction()
#include	<sys/wait.h>	// For waitpid()
#include	<dirent.h>		// For opendir(), readdir(), closedir()
#include    <unistd.h>      // Had to include for R_OK permission

const int LO_LEGAL_PORT = 1025;
const int HI_LEGAL_PORT = 65535;
const int ERROR_FD = -1;
const int NUM_CLIENTS_TO_SERVE = 4;
typedef int BOOL;

//  PURPOSE:  To get zombie children.  Ignores 'sig' (which will be 'SIGCHLD').
//	No return value.
void sigChildHandler(int sig) {
	//  I.  Application validity check:

	//  II.  Get all zombie children:
	//  YOUR CODE HERE
    
	 int      status;
     pid_t    pid;

	 while ( (pid = waitpid(-1, &status, WNOHANG)) > 0) printf("Zombie child %d finished.\n", pid);
	 return;
    
	//  III.  Finished:
}

//  PURPOSE:  To attempt to create and return a file-descriptor for listening
//	to the OS telling this server when a client process has connect()-ed
//	to 'port'.  Returns that file-descriptor, or 'ERROR_FD' on failure.
int getServerFileDescriptor(int port, const char* progName) {
    //  I.  Application validity check:
    if  (progName == NULL) {
        fprintf(stderr,"Error: NULL ptr to file descriptor.\n");
        exit(EXIT_FAILURE);
    }
    
    //  II.  Attempt to get socket file descriptor and bind it to 'port':
    //  YOUR CODE HERE
    
    int socketDesc = socket(AF_INET, /*AF_INET domain*/ SOCK_STREAM, /*Reliable TCP*/ 0); //Create socket
    struct sockaddr_in socketInf;  //Make sockaddr_in struct
    memset(&socketInf,'\0',sizeof(socketInf));    //Populate with zeroes
    socketInf.sin_family = AF_INET;     //Using TCP/IP
    socketInf.sin_port = htons(port);   //Let port know in network endian
    socketInf.sin_addr.s_addr = INADDR_ANY; //Allow machine to connect
    int status = bind(socketDesc, (struct sockaddr*)&socketInf, sizeof(socketInf)); //Bind socket with port
    
    if  (status < 0) {
        fprintf(stderr,"Error: Could not bind to port %d\n",port);
        exit(EXIT_FAILURE);
    }
    
    listen(socketDesc, 5);
    
    //  III.  Finished:
    
    return(socketDesc);
}

//  PURPOSE:  To install 'sigChildHandler()' as the signal simple handler for
//	the 'SIGCHLD' signal.  Tells OS to restarts system calls if receives
//	'SIGCHLD'.
void installSigChildHandler() {
    //  I.  Application validity check:

    //  II.  Install 'sigChildHandler()' as the 'SIGCHLD' handler:
    //  YOUR CODE HERE
    
    struct sigaction actn;                           //Make new action struct
    memset(&actn,'\0',sizeof(struct sigaction));
    sigemptyset(&actn.sa_mask);
    actn.sa_flags = SA_NOCLDSTOP | SA_RESTART;;
    actn.sa_handler = sigChildHandler;
    sigaction(SIGCHLD,&actn,NULL);  //Install 'sigChildHandler()' as 'SIGCHLD' handler
    
    //  III.  Finished:
}

//  PURPOSE:  To ask the user which port to attempt to monopolize, and to return
//	entered port number.
int getPort() {
	//  I.  Application validity check:

	//  II.  Get port number
	int port;

	do {
		char buffer[BUFFER_LEN];
		printf("Please enter port number to monopolize [%d-%d]: ", LO_LEGAL_PORT, HI_LEGAL_PORT);
		fgets(buffer, BUFFER_LEN, stdin);
		port = strtol(buffer, NULL, 10);
	} while ((port < LO_LEGAL_PORT) || (port > HI_LEGAL_PORT));

	//  III.  Finished:
	return (port);
}

//  PURPOSE:  To do the work of handling the client.  Communication with the
//	client take place using file-descriptor 'fd'.  No return value.
void handleClient(int fd) {
	//  I.  Application validity check:

	if (fd < 0) {
		fprintf(stderr, "Error: Illegal file descriptor.\n");
		exit(EXIT_FAILURE);
	}

	//  II.  Handle the client:
	//  YOUR CODE HERE

	DIR* dir = NULL;                            //Create directory
	FILE* fPtr = NULL;                          //Create file pointer
	struct dirent *dirEntry = NULL;             //Create directory entry
    char buffer[BUFFER_LEN + 1];                //Create buffer

	uint32_t fSize = 0;
	uint32_t response = 0;
    
	struct stat fStat;
	BOOL	foundFile = 0;
	memset(buffer, 0, sizeof(buffer));
	memset(&fStat, 0, sizeof(fStat));

	printf("\n");
	read(fd, buffer, 1);                        //Get char from client
	dir = opendir(".");
	if(NULL == dir) {
		response = htonl(CANT_READ_DIR_CODE);   //Send CANT_READ_DIR_CODE back to client
		write(fd, &response, sizeof(response));
		printf("Cannot read directory.\n");
		return;
	}
    
	BOOL regFile = 0;
    
	while((dirEntry = readdir(dir)) != NULL ) { //Read directory
		stat(dirEntry->d_name, &fStat);
		if(dirEntry->d_name[0] == buffer[0]) {
			foundFile = 1;
			if(S_ISREG(fStat.st_mode)) {
				regFile = 1;
				break;
			}
		}
	}

	if(foundFile) {                             //Check file permissions
		if(regFile) {
			foundFile = 0;
			int readPerm = access(dirEntry->d_name, R_OK);
			if(readPerm == -1) {
				response = htonl(CANT_READ_FILE_CODE);  //Send CANT_READ_FILE_CODE to client
				write(fd, &response, sizeof(response));
				printf("Cannot read file %s\n.", dirEntry->d_name);
				return;
			} else foundFile = 1;
		} else {
            response = htonl(CANT_READ_DIR_CODE);   //Send CANT_READ_DIR_CODE back to client
			write(fd, &response, sizeof(response));
			printf("%c is the right character but it's not a file.\n",buffer[0]);
			printf("No matching file.\n");
			return;
		}
	} else {
		response = htonl(NO_MATCH_CODE);            //Send NO_MATCH_CODE to client
		write(fd, &response, sizeof(response));
		printf("No file begins with %c:\n",buffer[0]);
		printf("No matching file.\n");
		return;
	}

	if(foundFile) {                                 //All good, send file to client
		uint32_t ffd = -1;
		fPtr = fopen(dirEntry->d_name, "r");        //Find file in current directory
		ffd = fileno(fPtr);
		fseek(fPtr, 0L, SEEK_END);
		fSize = ftell(fPtr);
		fclose(fPtr);
		fPtr = fopen(dirEntry->d_name, "r");
		ffd = fileno(fPtr);
		printf("Opening %s and sending to client.\n",dirEntry->d_name);
		printf("Sending %d bytes to %s.\n", fSize, dirEntry->d_name);

		fSize = htonl(fSize);
		write(fd, &fSize, sizeof(fSize));
		fSize = ntohl(fSize);

		uint32_t totalBytesSent = 0;

		int numBytesRead = 0;
		int writtenBytes = 0;
		while(totalBytesSent < fSize) {
			writtenBytes = 0;
			numBytesRead = read(ffd, buffer, BUFFER_LEN);
			while(writtenBytes < numBytesRead) writtenBytes += write(fd, buffer + writtenBytes, numBytesRead - writtenBytes);
			buffer[writtenBytes] = '\0';
			printf("%s", buffer);
			totalBytesSent += numBytesRead;
		}
	}
	//  III.  Finished:
}

//  PURPOSE:  To serve the clients using file-descriptor 'listenFd' to tell
//	when a client has connected.  Each client is handled by its own child
//	process.  Both the parent and the child processes close unnecesary
//	file-descriptorors.  Serves 'NUM_CLIENTS_TO_SERVE' clients, then quits.
//	No return value.
void doServer(int listenFd) {
	//  I.  Application validity check:
	if (listenFd < 0) {
		fprintf(stderr, "Error: Illegal file descriptor.\n");
		exit(EXIT_FAILURE);
	}
    
	//  II.  Do the work of the server:
    listen(listenFd, NUM_CLIENTS_TO_SERVE);
	int i, clientLen;
	pid_t zombieChildProcess;
	int clientFd;
    
    for  (i = 0;  i < NUM_CLIENTS_TO_SERVE;  i++) {
        
		//  YOUR CODE HERE
        
		if ((clientFd = accept(listenFd, NULL, NULL)) < 0) {
			if (errno == EINTR) continue;
			else perror("Could not accept request from client.\n");
		} else {
			if ((zombieChildProcess = fork()) == 0) {
				handleClient(clientFd);
				close(clientFd);
				exit(0);
			}
			close(clientFd);
		}
	}

	//  III.  Finished:
}

//  PURPOSE:  To oversee the main work of the server.  Ignores 'argc' but
//	uses 'argv[0]' as the name of the program.  Returns 'EXIT_SUCCESS' to
//	OS on success or 'EXIT_FAILURE' otherwise.
int main(int argc, char* argv[]) {
	//  I.  Application validity check:

	//  II.  Do server:
	int port = getPort();
	int socketFd = getServerFileDescriptor(port, argv[0]);

	installSigChildHandler();
	doServer(socketFd);
	printf("\n");
	//  III.  Finished:
	return (EXIT_SUCCESS);
}
