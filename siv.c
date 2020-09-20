#include <stdio.h>
#include <gtk/gtk.h>
#include <dirent.h>
#include <stdlib.h>

#include "config.h"
#include "siv_parametrs.h"
#include "siv.h"

GtkWidget *image = NULL;
GdkPixbuf *pixel_buffer = NULL;

int zoom_index = 5;

DIR *dir = NULL;
struct dirent *entry = NULL;

long int history[256];
int history_index = 0;

int main(int argc, char *argv[]) {
	GtkWidget *window = NULL;
	GtkWidget *main_box = NULL, *bttn_panel_left = NULL, *bttn_panel_right = NULL;
	GtkWidget *scroll_window = NULL;

	GtkWidget *next_bttn = NULL, *prev_bttn = NULL, *zoom_inc_bttn = NULL,
			  *zoom_dec_bttn = NULL, *shell_bttn = NULL;
	char *image_name = "none";

	// init parametrs and open config
	sev_init_parametrs();
	open_config(siv_get_config_file_name());
	siv_set_zoom_index(&zoom_index);
	 
	// open current directory
	dir = opendir(get_option_by_name("pwd"));	
	if (!dir) {
		perror(__func__);
		exit(0);
	}

	gtk_init(&argc, &argv);

	// init window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "siv");
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	//gtk_window_set_default_icon_from_file("gbl.ico", NULL);
	gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);

	// add signals to window
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(key_press), NULL);

	// init scrolled window
	scroll_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window), GTK_POLICY_ALWAYS, GTK_POLICY_AUTOMATIC);

	// init image
	if (argc >= 2) {
		image_name = argv[1];
		gtk_window_set_title(GTK_WINDOW(window), image_name);
	}
	pixel_buffer = gdk_pixbuf_new_from_file(image_name, NULL);
	image = gtk_image_new_from_pixbuf(pixel_buffer);

	// init buttons
	next_bttn = gtk_button_new_with_label(">");
	prev_bttn = gtk_button_new_with_label("<");
	zoom_inc_bttn = gtk_button_new_with_label("+");
	zoom_dec_bttn = gtk_button_new_with_label("-");
	shell_bttn = gtk_button_new_with_label("/");

	g_signal_connect(G_OBJECT(zoom_inc_bttn), "clicked", G_CALLBACK(zoom_inc_bttn_click), NULL);
	g_signal_connect(G_OBJECT(zoom_dec_bttn), "clicked", G_CALLBACK(zoom_dec_bttn_click), NULL);
	g_signal_connect(G_OBJECT(next_bttn), "clicked", G_CALLBACK(next_bttn_click), window);
	g_signal_connect(G_OBJECT(prev_bttn), "clicked", G_CALLBACK(prev_bttn_click), window);
	g_signal_connect(G_OBJECT(shell_bttn), "clicked", G_CALLBACK(shell_bttn_click), window);

	// init button panels
	bttn_panel_left = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	bttn_panel_right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	// pack srolled window
	gtk_container_add(GTK_CONTAINER(scroll_window), image);

	// pack button panels
	if (siv_is_option_true("zoom--button"))
		gtk_box_pack_start(GTK_BOX(bttn_panel_left), zoom_dec_bttn, FALSE, FALSE, 5);
	if (siv_is_option_true("prev-button"))
		gtk_box_pack_start(GTK_BOX(bttn_panel_left), prev_bttn, TRUE, TRUE, 5);
	if (siv_is_option_true("zoom+-button"))
		gtk_box_pack_start(GTK_BOX(bttn_panel_right), zoom_inc_bttn, FALSE, FALSE, 5);
	if (siv_is_option_true("next-button"))
		gtk_box_pack_start(GTK_BOX(bttn_panel_right), next_bttn, TRUE, TRUE, 5);
	if (siv_is_option_true("shell-button"))
		gtk_box_pack_start(GTK_BOX(bttn_panel_left), shell_bttn, FALSE, FALSE, 5);	

	// init main_box
	main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	// pack main box
	gtk_box_pack_start(GTK_BOX(main_box), bttn_panel_left, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(main_box), scroll_window, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(main_box), bttn_panel_right, FALSE, FALSE, 0);

	// pack window
	gtk_container_add(GTK_CONTAINER(window), main_box);
	gtk_widget_show_all(window);

	gtk_main();

	close_config();
	closedir(dir);

	return 0;
}

void zoom_inc_bttn_click(GtkWidget *bttn, gpointer data) {
	int w = 0, h = 0;

	w = gdk_pixbuf_get_width(pixel_buffer);
	h = gdk_pixbuf_get_height(pixel_buffer);

	w += zoom_index;
	h += zoom_index;

	pixel_buffer = gdk_pixbuf_scale_simple(pixel_buffer, w, h, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixel_buffer);
}

void zoom_dec_bttn_click(GtkWidget *bttn, gpointer data) {
	int w = 0, h = 0;

	w = gdk_pixbuf_get_width(pixel_buffer);
	h = gdk_pixbuf_get_height(pixel_buffer);

	if (w - zoom_index > 0 && h - zoom_index > 0) {
		w -= zoom_index;
		h -= zoom_index;
	}

	pixel_buffer = gdk_pixbuf_scale_simple(pixel_buffer, w, h, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixel_buffer);
}

int is_image_file(char *name) {
	char *formats[] = {".jpg", ".png", ".bmp", ".ico", ".jpeg", NULL};

	for (int i = 0; formats[i]; i++)
		if (strstr(name, formats[i]))
			return i+1;
	return 0;
}

void next_bttn_click(GtkWidget *bttn, GtkWidget *window) {
	long int pos = telldir(dir);

	entry = readdir(dir);
	while (entry && !is_image_file(entry->d_name))
		entry = readdir(dir);
	if (!entry)
		return ;
	
	if (pos > 0)
		pos--;
	else
		pos = telldir(dir)-1;
	history[history_index++] = pos;

	gtk_image_clear(GTK_IMAGE(image));
	g_clear_object(&pixel_buffer);

	pixel_buffer = gdk_pixbuf_new_from_file(entry->d_name, NULL);
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixel_buffer);

	gtk_window_set_title(GTK_WINDOW(window), entry->d_name);
}

void prev_bttn_click(GtkWidget *bttn, GtkWidget *window) {
	if (history_index > 0)
		history_index--;
	else
		return ;

	seekdir(dir, history[history_index]);
	entry = readdir(dir);
	if (!entry)
		return ;

	gtk_image_clear(GTK_IMAGE(image));
	g_clear_object(&pixel_buffer);

	pixel_buffer = gdk_pixbuf_new_from_file(entry->d_name, NULL);
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixel_buffer);

	gtk_window_set_title(GTK_WINDOW(window), entry->d_name);
}

void shell_bttn_click(GtkWidget *bttn, GtkWidget *parent) {
	GtkWidget *dialog = NULL, *content = NULL;
	GtkWidget *main_box = NULL, *send_box = NULL;
	GtkWidget *info_label = NULL, *command_entry = NULL, *send_bttn = NULL;

	// init dialog
	dialog = gtk_dialog_new_with_buttons("shell", GTK_WINDOW(parent), (GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// init widgets
	info_label = gtk_label_new("Enter the command to process the file");
	command_entry = gtk_entry_new();
	send_bttn = gtk_button_new_with_label("send");

	// add signal to send_bttn
	g_signal_connect(G_OBJECT(send_bttn), "clicked", G_CALLBACK(send_bttn_click), command_entry);

	// set target window
	set_window(parent);

	// init & pack send box
	send_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(send_box), command_entry, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(send_box), send_bttn, FALSE, FALSE, 5);

	// init & pack main box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(main_box), info_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), send_box, TRUE, TRUE, 5);

	gtk_container_add(GTK_CONTAINER(content), main_box);
	gtk_widget_show_all(dialog);
}

void key_press(GtkWidget *window, GdkEventKey *event, gpointer data) {
	switch (event->keyval) {
		case GDK_KEY_Left:
			prev_bttn_click(NULL, window);
		break;

		case GDK_KEY_Right:
			next_bttn_click(NULL, window);
		break;

		case GDK_KEY_plus:
			zoom_inc_bttn_click(NULL, NULL);
		break;

		case GDK_KEY_minus:
			zoom_dec_bttn_click(NULL, NULL);
		break;

		case GDK_KEY_Escape:
			gtk_main_quit();
		break;

		case GDK_KEY_slash:
			shell_bttn_click(NULL, window);
		break;
	}
}
