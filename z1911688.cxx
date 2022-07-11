/*
CSCI 330 - ASSIGNMENT 07
z1911688.cxx

NAME: Leonart Jaos
Z-ID: Z1911688
PROFESSOR: Raimund EGE
DATE DUE: NOVEMBER 11, 2020

PURPOSE: This program allows you to add messsages to a file with no
permissions.

Usage: seclog [-c] out_file message_string

*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

bool checkPerms(mode_t st_mode){
	if(st_mode & S_IRUSR) return false;
	else if(st_mode & S_IWUSR) return false;
	else if(st_mode & S_IXUSR) return false;
	else if(st_mode & S_IRGRP) return false;
	else if(st_mode & S_IWGRP) return false;
	else if(st_mode & S_IXGRP) return false;
	else if(st_mode & S_IROTH) return false;
	else if(st_mode & S_IWOTH) return false;
	else if(st_mode & S_IXOTH) return false;
	else return true;
	}

int main(int argc, char* argv[]){
	bool check = false;
	int rs, ofd, fd;
	struct stat buffer;
	// must have at least 3 fields in command line
	if(argc < 3){
		cerr << "Usage: seclog [-c] out_file message_string\n";
		cerr << "\twhere the message_string is appended to file out_file.\n";
		cerr << "\tThe -c option clears the file before the message is appended\n";
		exit(EXIT_FAILURE);
	}
	// file checking if option field is selected
	// opens/creates file and checks permissions
	if((strcmp(argv[1],"-c")) == 0){
		// argv[2] must be filename
		check = true;
		rs = stat(argv[2], &buffer);
		if(rs == -1){
			perror(argv[2]);
			exit(EXIT_FAILURE);
		}
		// checks if the file exists
		if(S_ISREG(buffer.st_mode)){
			// check permissions
			if(!(checkPerms(buffer.st_mode))){
				cerr << "log is not secure. Ignoring.\n";
				exit(EXIT_FAILURE);
				}
		}
		// file doesn't exist, make one
		else{
			ofd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 00200);
			if(ofd == -1){
				perror(argv[1]);
				exit(EXIT_FAILURE);
			}
			close(ofd);
		}		
	}
	// file checking if clear option isn't in field 
	else{
		rs = stat(argv[1], &buffer);
		//if(rs == -1){
			//perror(argv[1]);
			//exit(EXIT_FAILURE);
	//	}
		if(S_ISREG(buffer.st_mode)){
			// check permissions
			if(!(checkPerms(buffer.st_mode))){
				cerr << "log is not secure. Ignoring.\n";
				exit(EXIT_FAILURE);
			}
		}
		else{
			// create file
			ofd = creat(argv[1],S_IWUSR);
			if(ofd == -1){
				perror(argv[1]);
				exit(EXIT_FAILURE);
			}
			close(ofd);
		}
	}
	// at this point all files are checked for permissions and created
	// if they hadn't already existed
	// case if -c flag was chosen
	if(check){
		// open file with write and truncate
		system((string("chmod 755 ") + argv[2]).c_str());
		fd = open(argv[2], O_WRONLY | O_TRUNC);
		if(fd == -1){
			perror(argv[2]);
			exit(EXIT_FAILURE);
		}
		// check if argv[3] is filled and write
		if(argc < 4){
			// does nothing	
		}
		// fills with argv[3]
		else{
			write(fd, argv[3], strlen(argv[3]));
			if(fd == -1){
				perror(argv[3]);
				exit(fd);
			}
			write(fd, "\n", strlen("\n"));
			if(fd == -1){
				perror(argv[3]);
				exit(fd);
			}
		}
		// clear write permission
		system((string("chmod 000 ") + argv[2]).c_str());
		stat(argv[2], &buffer);
		if(!(checkPerms(buffer.st_mode))){
			cerr << "CHMOD ERROR\n";
			exit(EXIT_FAILURE);		
		}
		close(fd);
		// close file
	}
	else{
		// open file with write and append
		system((string("chmod 755 ") + argv[1]).c_str());
		fd = open(argv[1], O_WRONLY | O_APPEND);
		if(fd == -1){
			perror(argv[1]);
			exit(EXIT_FAILURE);
		}
		// write with argv[2]
		write(fd, argv[2], strlen(argv[2]));
		if(fd == -1){
			perror(argv[2]);				
			exit(fd);
		}
		write(fd, "\n", strlen("\n"));
		if(fd == -1){
			perror(argv[2]);
			exit(fd);
		}
		// clear write permission
		system((string("chmod 000 ") + argv[1]).c_str());
		stat(argv[1], &buffer);
		if(!(checkPerms(buffer.st_mode))){
			cerr << "CHMOD ERROR\n";
			exit(EXIT_FAILURE);		
		}
		// close file
		close(fd);
	}
return 0;
}
