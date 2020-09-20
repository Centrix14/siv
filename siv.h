#ifndef __SIMPLE_IMAGE_VIEWER_HEADER_FILE__
#define __SIMPLE_IMAGE_VIEWER_HEADER_FILE__

void zoom_inc_bttn_click(GtkWidget *bttn, gpointer data);
void zoom_dec_bttn_click(GtkWidget *bttn, gpointer data);
void next_bttn_click(GtkWidget *bttn, GtkWidget *window);
void prev_bttn_click(GtkWidget *bttn, GtkWidget *window);
void shell_bttn_click(GtkWidget *bttn, GtkWidget *parent);

void key_press(GtkWidget *window, GdkEventKey *event, gpointer data);

void send_bttn_click(GtkWidget *bttn, GtkWidget *entry);
void set_window(GtkWidget *new_window);

#endif
