#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

char *options[] = {"left-button", "right-button", "inc-button", "dec-button", NULL};
char *default_vals[] = {"t", "t", "t", "t"};

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

