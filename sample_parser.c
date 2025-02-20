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

#define INPUT_LENGTH 2048
#define MAX_ARGS		 512


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
	if(input->argc == 1){
		chdir(getenv("HOME"));
	}else{
		if(chdir(input->argv[1]) != 0){
			perror("cd");
		}
	}
}


void check_input(struct command_line *input)
{
	if(input->argv[0] == NULL || input->argv[0][0] == '#' || input->argv[0][0] == '\n'){
		//printf("here");
	}else if(strcmp(input->argv[0], "exit") == 0){
		printf("exit here");
		exit_func();
	}else if(strcmp(input->argv[0], "cd") == 0){
	 	cd_func(input);
	}
	//else if(strcmp(input->argv[0], "status")){
	// 	status_func();
	// }
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