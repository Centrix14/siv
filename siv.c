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
			  *zoom_dec_bttn = NULL;
	char *image_name = NULL;

	// init parametrs and open config
	sev_init_parametrs();
	open_config("c");
	 
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
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

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

	g_signal_connect(G_OBJECT(zoom_inc_bttn), "clicked", G_CALLBACK(zoom_inc_bttn_click), NULL);
	g_signal_connect(G_OBJECT(zoom_dec_bttn), "clicked", G_CALLBACK(zoom_dec_bttn_click), NULL);
	g_signal_connect(G_OBJECT(next_bttn), "clicked", G_CALLBACK(next_bttn_click), window);
	g_signal_connect(G_OBJECT(prev_bttn), "clicked", G_CALLBACK(prev_bttn_click), window);

	// init button panels
	bttn_panel_left = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	bttn_panel_right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	// pack srolled window
	gtk_container_add(GTK_CONTAINER(scroll_window), image);

	// pack button panels
	gtk_box_pack_start(GTK_BOX(bttn_panel_left), zoom_dec_bttn, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(bttn_panel_left), prev_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(bttn_panel_right), zoom_inc_bttn, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(bttn_panel_right), next_bttn, TRUE, TRUE, 5);

	// init main_box
	main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);

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

	w -= zoom_index;
	h -= zoom_index;

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
