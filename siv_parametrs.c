#include <stdio.h>

#include "config.h"

char *options[] = {"left-button", "right-button", "inc-button", "dec-button", NULL};
char *default_vals[] = {"t", "t", "t", "t"};

void sev_init_parametrs() {
	for (int i = 0; options[i]; i++)
		add_option(options[i], default_vals[i]);
}

void sev_print_parametrs() {
	for (int i = 0; get_option_by_index(i); i++)
		puts(get_option_by_index(i));
}
