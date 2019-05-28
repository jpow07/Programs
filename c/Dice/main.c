#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>


#define COLORTEXT(r,g,b) \
       	g_print("\033[38;2;%d;%d;%dm", r, g, b) \

#define DEFAULTCOLOR g_print("\033[39m")


GdkPixbuf *create_pixbuf(const gchar *filename) {

	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);

	if (!pixbuf) {
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}

	return pixbuf;
}

void print_msg(GtkWidget *widget, gpointer window) {
	g_print("Dice Rolled\n");
	int numberOfDice  = 4;//atoi(argv[1]);
	int numberOfFaces = 6;//atoi(argv[2]);
	int diceSum = 0;
	srand(time(NULL) * time(NULL));
	for(int i = 0; i < numberOfDice; i++) {

		int diceValue = rand() % numberOfFaces;
		COLORTEXT(50 +rand() % 180, 50 + rand() % 180, 50 + rand() % 180 );
		g_print("Dice %d:\t\t%d\n", i + 1, diceValue);	
		DEFAULTCOLOR;
		diceSum += diceValue;	
	}

	COLORTEXT(rand() % 255, rand() % 255, rand() % 255 );
	g_print("\nDice Total:\t%d\n", diceSum);
	DEFAULTCOLOR;
}


int main(int argc, char *argv[]) {

	GtkWidget *window;

	gtk_init(&argc, &argv);

	g_print("GTK+ version: %d.%d.%d\n", gtk_major_version, 
			gtk_minor_version, gtk_micro_version);

	g_print("GLib version: %d.%d.%d\n", glib_major_version, 
			glib_minor_version, glib_micro_version);


	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Dice Roll");
	gtk_window_set_default_size(GTK_WINDOW(window), 500, 350);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	GdkPixbuf *icon;
	icon = create_pixbuf("web.png");
	gtk_window_set_icon(GTK_WINDOW(window), icon);

	GtkWidget *button;
	GtkWidget *halign;

	button = gtk_button_new_with_mnemonic("_Roll");
	gtk_widget_set_tooltip_text(button, "Roll a Dice");

	g_signal_connect(button, "clicked", G_CALLBACK(print_msg), NULL);

	halign = gtk_alignment_new(0, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(halign), button);
	gtk_container_add(GTK_CONTAINER(window), halign);


	gtk_widget_show_all(window);
	
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	g_object_unref(icon);

	gtk_main();




	return 0;
}
