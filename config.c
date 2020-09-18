#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"

#define DELIM " =\n"

option option_list[LEN];

void open_config(char *name) {
	FILE *config_file = fopen(name, "r");

	if (!config_file) {
		perror(__func__);
		return ;
	}

	read_config(config_file);

	fclose(config_file);
}

void read_config(FILE *config_file) {
	char str[256] = "\0", *option_name = NULL, *option_value = NULL;

	fgets(str, 256, config_file);
	while (!feof(config_file)) {
		option_name = strtok(str, DELIM);
		option_value = strtok(NULL, DELIM);

		add_option(option_name, option_value);

		fgets(str, 256, config_file);
	}
}

static int indx = 0;

int is_there_such_an_option(char *option_name) {
	int i = 0;

	for (; i < indx; i++)
		if (option_list[i].name && !strcmp(option_list[i].name, option_name))
			return i;
	return -1;
}

void add_option(char *option_name, char *option_value) {
	int is_such_option = 0, value_len = 0, name_len = 0;

	name_len = strlen(option_name)+1;
	value_len = strlen(option_value)+1;

	is_such_option = is_there_such_an_option(option_name);
	if (is_such_option > -1) {
		option_list[is_such_option].value = malloc(value_len);
		if (!option_list[is_such_option].value) {
			perror(__func__);
			return ;
		}

		strcpy(option_list[is_such_option].value, option_value);
	}
	else {
		option_list[indx].name = malloc(name_len);
		option_list[indx].value = malloc(value_len);

		if (!option_list[indx].name || !option_list[indx].value) {
			perror(__func__);
			return ;
		}

		strcpy(option_list[indx].name, option_name);
		strcpy(option_list[indx].value, option_value);

		indx++;
	}
}

void close_config() {
	for (int i = 0; i < indx; i++) {
		if (option_list[i].name) {
			free(option_list[i].name);
			free(option_list[i].value);
		}
	}
}

char *get_option_by_name(char *option_name) {
	for (int i = 0; i < indx; i++)
		if (option_list[i].name && !strcmp(option_list[i].name, option_name))
			return option_list[i].value;
	return NULL;
}

char *get_option_by_index(int i) {
	if (option_list[i].value)
		return option_list[i].value;
	return NULL;
}
