#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

GtkWidget *target_window = NULL;

void send_bttn_click(GtkWidget *bttn, GtkWidget *entry) {
	char cmd[256] = "";

	sprintf(cmd, "%s %s", gtk_entry_get_text(GTK_ENTRY(entry)), gtk_window_get_title(GTK_WINDOW(target_window)));
	system(cmd);
}

void set_window(GtkWidget *new_window) {
	target_window = new_window;
}
