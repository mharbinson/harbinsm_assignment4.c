/**
 * A sample program for parsing a command line. If you find it useful,
 * feel free to adapt this code for Assignment 4.
 * Do fix memory leaks and any additional issues you find.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define INPUT_LENGTH 2048
#define MAX_ARGS		 512

int status = 0;

struct command_line
{
	char *argv[MAX_ARGS + 1];
	int argc;
	char *input_file;
	char *output_file;
	bool is_bg;
};

void exit_func()
{
	exit(1);
}

void cd_func(struct command_line *input)
{
	char path[MAX_ARGS];
	getcwd(path,sizeof path);

	if(input->argc == 1){
		chdir(getenv("HOME"));
	}else{
		if(chdir(input->argv[1]) != 0){
			strcat(path,"/");
    		strcat(path,input->argv[1]);
    		chdir(path);
			perror("cd");
		}
	}
	getcwd(path,sizeof path);
	printf("%s", getenv("PWD"));
}

void status_func(int input){
	if(WIFEXITED(status)){
		printf("%d", WEXITSTATUS(status));
	}else if(WIFSIGNALED(status)){
		printf("%d", WTERMSIG(status));
	}
}


void other_commands(struct command_line *input){
	pid_t pid; 
	pid = fork();

	switch(pid){
		case -1:
			perror("fork() failed\n");
			exit(1);
			break;
		case 0:
			printf("This is the child process\n");
			if(execvp(input->argv[0], input->argv) < 0){
				perror("execvp failed");
				break;
			}
			break;
		default:
			printf("This is the parent process\n");
			waitpid(pid, &status, 0);
			break;


	}
	
};


void check_input(struct command_line *input)
{
	if(input->argv[0] == NULL || input->argv[0][0] == '#' || input->argv[0][0] == '\n'){
		//printf("here");
	}else if(strcmp(input->argv[0], "exit") == 0){
		printf("exit here");
		exit_func();
	}else if(strcmp(input->argv[0], "cd") == 0){
	 	cd_func(input);
	}else if(strcmp(input->argv[0], "status") == 0){
	 	status_func(status);	
	}else{
		other_commands(input);
	}
}

struct command_line *parse_input()
{
	char input[INPUT_LENGTH];
	struct command_line *curr_command = (struct command_line *) calloc(1, sizeof(struct command_line));

	// Get input
	printf(": ");
	fflush(stdout);
	fgets(input, INPUT_LENGTH, stdin);

	// Tokenize the input
	char *token = strtok(input, " \n");
	while(token){
		if(!strcmp(token,"<")){
			curr_command->input_file = strdup(strtok(NULL," \n"));
		} else if(!strcmp(token,">")){
			curr_command->output_file = strdup(strtok(NULL," \n"));
		} else if(!strcmp(token,"&")){
			curr_command->is_bg = true;
		} else{
			curr_command->argv[curr_command->argc++] = strdup(token);
		}
		token=strtok(NULL," \n");
	}
	return curr_command;
}

int main()
{
	struct command_line *curr_command;

	while(true)
	{
		curr_command = parse_input();
		// printf("%s",curr_command->argv[0]);
		// printf("%s" ,curr_command->input_file);
		// printf("%s" ,curr_command->output_file);
		// printf("%d",curr_command->is_bg);
		check_input(curr_command);


	}
	return EXIT_SUCCESS;
}