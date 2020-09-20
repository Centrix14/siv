#ifndef __SIMPLE_IMAGE_VIEWER_HEADER_FILE__
#define __SIMPLE_IMAGE_VIEWER_HEADER_FILE__

void zoom_inc_bttn_click(GtkWidget *bttn, gpointer data);
void zoom_dec_bttn_click(GtkWidget *bttn, gpointer data);
void next_bttn_click(GtkWidget *bttn, GtkWidget *window);
void prev_bttn_click(GtkWidget *bttn, GtkWidget *window);

void key_press(GtkWidget *window, GdkEventKey *event, gpointer data);

#endif
