#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <sys/stat.h>

static gint start_x = 0;
static gint start_y = 0;
int verbose=0;
const char fn_pid[]="/tmp/whisper-auto.pid";
const char fn_pos[]="/tmp/whisper-auto-pos.txt";
GtkWidget *button_status, *label_status;

const char stat_rdy[]="Ready";
const char stat_rec[]="REC";
const char stat_whi[]="Whisper";

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Button clicked: %s\n", (char *)data);
}
static gboolean on_button_press(GtkWidget* widget, GdkEventButton* event, GtkWindow* window) {
    g_print("Button press\n");
    if (event->button == 1) { // LMB
        start_x = event->x_root;
        start_y = event->y_root;
        g_print("Button press at coordinates: (%f, %f)\n", event->x, event->y);
    }
    return FALSE; // propagate event?
}
static gboolean on_button_release(GtkWidget* widget, GdkEventButton* event, GtkWindow* window) {
    g_print("Button release at coordinates: (%f, %f)\n", event->x, event->y);
    gint x, y;
    gtk_window_get_position(window, &x, &y);
    FILE *f = fopen(fn_pos, "w");
    if (f == NULL) { g_print("Error opening file!\n"); return FALSE; }
    fprintf(f, "%d %d\n", x, y);
    fclose(f);
    return FALSE; // propagate event?
}
static gboolean on_motion_notify(GtkWidget* widget, GdkEventMotion* event, GtkWindow* window) {
    if (verbose>1) g_print("Motion notification\n");
    if (event->state & GDK_BUTTON1_MASK) { // LMB
        gint x, y;
        gtk_window_get_position(window, &x, &y);
        gtk_window_move(window, x + event->x_root - start_x, y + event->y_root - start_y);
        start_x = event->x_root;
        start_y = event->y_root;
    }
    return FALSE; // propagate event?
}
void usage() {
	g_print("Usage: whisper-buttons [-v] [-h]\n");
}
void setup_pid_file() {
	FILE* pid_file = fopen(fn_pid, "w");
	if (pid_file != NULL) {
		fprintf(pid_file, "%d\n", getpid());
		fclose(pid_file);
		chmod(fn_pid, S_IRUSR | S_IWUSR);
	} else {
		perror("Error opening PID file");
		exit(EXIT_FAILURE);
	}
}
void set_window_position_from_file(GtkWindow* window, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) { g_print("No location file: %s\n", fn_pos); return; }
    gint x, y;
    if (fscanf(f, "%d %d", &x, &y) == 2) // exactly two values
        gtk_window_move(window, x, y);
    else g_print("Error reading coordinates from file!\n");
    fclose(f);
}

void set_widget_style(GtkWidget *widget, gchar *style) {
    GtkCssProvider *provider = gtk_css_provider_new();

    gtk_css_provider_load_from_data(provider, style, -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget),
                                   GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}
void set_widget_child_style(GtkWidget *parent, gchar *style) {
	GtkWidget *widget = gtk_bin_get_child(GTK_BIN(parent));
	if (widget) set_widget_style(widget, style);
}
static void handle_signals(int sig) {
    g_print("Received signal %d\n", sig);
    switch (sig) {
        case SIGUSR1:
			gtk_button_set_label(
					GTK_BUTTON(button_status), stat_rec);
			label_status = gtk_bin_get_child(
					GTK_BIN(button_status));
			gtk_widget_set_name(label_status, "label_status");
            set_widget_style(button_status, 
				"#button_status { "
					"font-size: 10px; "
					"background: rgba(255, 255, 0, 0.8); "
					"border-radius: 10px; "
					"border: 2px solid red; "
				"}");
            set_widget_style(label_status, 
				"#label_status { "
					"font-weight: bold; "
					"font-size: 14px; "
					"color: black; "
				"}");
            break;
        case SIGUSR2:
			gtk_button_set_label(GTK_BUTTON(button_status), "REC");
			label_status = gtk_bin_get_child(GTK_BIN(button_status));
			gtk_widget_set_name(label_status, "label_status");
            set_widget_style(button_status, 
				"#button_status { "
					"font-size: 10px; "
					"background: rgba(255, 255, 0, 0.8); "
					"border-radius: 10px; "
					"border: 2px solid red; "
				"}");
            set_widget_style(label_status, 
				"#label_status { "
					"font-weight: bold; "
					"font-size: 14px; "
					"color: black; "
				"}");
            break;
    }
}

int main(int argc, char *argv[]) {
	for (int i=0; i<argc; i++) {
		if (!strcmp(argv[i], "-v")) verbose++;
		else if (!strcmp(argv[i], "-h")) {
			usage(); exit(0);
		}
	}
    GtkWidget *window, *vbox, *hbox;
	GtkWidget *button_close, *button1, *button2;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    set_window_position_from_file(GTK_WINDOW(window), fn_pos);
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    gtk_window_stick(GTK_WINDOW(window));
    gtk_widget_set_app_paintable(window, TRUE);
    GdkScreen *screen = gtk_widget_get_screen(window);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
    if (visual != NULL && gdk_screen_is_composited(screen)) {
        gtk_widget_set_visual(window, visual);
    }

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

	// STATUS | CLOSE
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

	button_status = gtk_button_new_with_label(stat_rdy);
	gtk_widget_set_name(button_status, "button_status");
	label_status = gtk_bin_get_child(GTK_BIN(button_status));
	gtk_widget_set_name(label_status, "label_status");

	gtk_widget_set_events(button_status, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);
	g_signal_connect(button_status, "button-press-event", G_CALLBACK(on_button_press), window);
	g_signal_connect(button_status, "motion-notify-event", G_CALLBACK(on_motion_notify), window);
	g_signal_connect(button_status, "button-release-event", G_CALLBACK(on_button_release), window);
	gtk_box_pack_start(GTK_BOX(hbox), button_status, TRUE, TRUE, 0);
	// Disable click events on status
	g_signal_handlers_block_by_func(button_status, on_button_clicked, "Status");

	button_close = gtk_button_new_with_label("☒");
	gtk_widget_set_name(button_close, "button_close");
    g_signal_connect(button_close, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), button_close, TRUE, TRUE, 0);

	// Apply CSS
    GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(provider,
		"#button_status, #button_status:hover, #button_status:active, #button_status:focus { "
			"font-size: 13px; "
			"background: rgba(48, 159, 16, 0.8); "
			"border-radius: 10px; "
		"} "
		"#label_status { "
			"color: white; "
			"font-weight: bold; "
		"} "
		"#button_close { "
			"font-size: 17px; "
			"font-weight: bold; "
			"background: rgba(48, 48, 48, 0.6); "
			"border-radius: 10px; "
		"} "
		"button { "
			"border-width: 0px; "
			"font-size: 16px; "
			"background: rgba(48, 48, 48, 0.8); "
			"border-radius: 10px; "
		"}", -1, NULL);
    gtk_style_context_add_provider_for_screen(
    	gdk_screen_get_default(),
		GTK_STYLE_PROVIDER(provider),
		GTK_STYLE_PROVIDER_PRIORITY_USER);

	// OTHER BUTTONS

    button1 = gtk_button_new_with_label("Record");
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button_clicked), "Record");
    gtk_box_pack_start(GTK_BOX(vbox), button1, TRUE, TRUE, 0);

    button2 = gtk_button_new_with_label("Stop");
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button_clicked), "Stop");
    gtk_box_pack_start(GTK_BOX(vbox), button2, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);

	setup_pid_file();
    signal(SIGUSR1, handle_signals);
    signal(SIGUSR2, handle_signals);

    gtk_main();

    return 0;
}
