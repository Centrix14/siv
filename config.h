#ifndef __CONFIGURATION_LIBRARY_HEADER_FILE_INCLUDED__
#define __CONFIGURATION_LIBRARY_HEADER_FILE_INCLUDED__

#define LEN 256

typedef struct {
	char *name;
	char *value;
} option;

void open_config(char *name);
void read_config(FILE *config_file);
void add_option(char *option_name, char *option_value);
void close_config();

char *get_option_by_name(char *option_name);
char *get_option_by_index(int i);

#endif
