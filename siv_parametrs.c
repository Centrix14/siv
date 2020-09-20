#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#ifdef WIN32
	#define DELIM "\\"
#else
	#define DELIM "/"
#endif

char *options[] = {"prev-button", "next-button", "zoom+-button", "zoom--button", NULL};
char *default_vals[] = {"t", "t", "t", "t"};

char file[256] = "";

void add_pwd_param() {
	add_option("pwd", getenv("PWD"));
}

void sev_init_parametrs() {
	for (int i = 0; options[i]; i++)
		add_option(options[i], default_vals[i]);
	add_pwd_param();
}

void sev_print_parametrs() {
	for (int i = 0; get_option_by_index(i); i++)
		puts(get_option_by_index(i));
}

char *siv_get_config_file_name() {
	sprintf(file, "%s%s.sivrc", getenv("HOME"), DELIM);	

	return file;
}

int siv_is_option_true(char *name) {
	char *option = get_option_by_name(name);

	if (!strcmp(option, "t"))
		return 1;
	return 0;
}
