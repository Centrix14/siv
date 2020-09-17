#include <stdio.h>
#include <gtk/gtk.h>

#include "config.h"
#include "siv_parametrs.h"

int main(int argc, char *argv[]) {
	GtkWidget *window = NULL;
	GtkWidget *main_box = NULL, *bttn_panel_left = NULL, *bttn_panel_right = NULL;
	GtkWidget *scroll_window = NULL;

	GtkWidget *image = NULL;
	GdkPixbuf *pixel_buffer = NULL;

	GtkWidget *next_bttn = NULL, *prev_bttn = NULL, *zoom_inc_bttn = NULL,
			  *zoom_dec_bttn = NULL;

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
	pixel_buffer = gdk_pixbuf_new_from_file("i.jpg", NULL);
	image = gtk_image_new_from_pixbuf(pixel_buffer);

	// init buttons
	next_bttn = gtk_button_new_with_label(">");
	prev_bttn = gtk_button_new_with_label("<");
	zoom_inc_bttn = gtk_button_new_with_label("+");
	zoom_dec_bttn = gtk_button_new_with_label("-");

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
	return 0;
}
