#include <gtk/gtk.h>
#include <gdk/gdk.h>

// Other code...

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *button1;
    GtkWidget *button2;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_app_paintable(window, TRUE);
    GdkScreen *screen = gtk_widget_get_screen(window);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
    if (visual != NULL && gdk_screen_is_composited(screen)) {
        gtk_widget_set_visual(window, visual);
    }

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    label = gtk_label_new("Status");
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

    button1 = gtk_button_new_with_label("Record");
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button_clicked), "Record");
    gtk_box_pack_start(GTK_BOX(vbox), button1, TRUE, TRUE, 0);

    button2 = gtk_button_new_with_label("Stop");
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button_clicked), "Stop");
    gtk_box_pack_start(GTK_BOX(vbox), button2, TRUE, TRUE, 0);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
