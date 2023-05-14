#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <sys/stat.h>
#include "utils.h"

static gint start_x = 0;
static gint start_y = 0;
int verbose=0;
const char fn_pid[]="/tmp/whisper-auto-ui.pid";
const char fn_pos[]="/tmp/whisper-auto-ui-pos.txt";
GtkWidget *btn_status, *label_status;
GtkWidget *btn_close, *btn_start, *btn_stop;
char *error_status=NULL;

const char stat_rdy[]="Ready";
const char stat_rec[]="REC";
const char stat_whi[]="Whisper";

char *path_bin_start="whisper-auto";
char *path_bin_stop="whisper-kill-rec";

char style_rdy_button[] =
			"#button_status { "
				"background: rgba(48, 159, 16, 0.8); "
				"border-radius: 10px; "
				"border: none;"
				"padding-top: .3em; "
				"padding-bottom: .3em; "
			"}";
char style_rdy_label[] =
			"#label_status { "
				"font-weight: bold; "
				"font-size: 13px; "
				"color: white; "
			"}";

void child_watch_cb(GPid pid, gint status, gpointer user_data) {
    // This function is called when the child process terminates
    g_print("Child process ended with status %d\n", status);
    g_spawn_close_pid(pid);  // Close the handle to the child process
}

void exec_script(char *cmd) {
    GError *error = NULL;
    GPid child_pid;
    char *path;
    char *argv[] = {cmd, NULL};
    if (!g_spawn_async(NULL,
                       argv,
                       NULL,
                       G_SPAWN_DO_NOT_REAP_CHILD,
                       NULL,
                       NULL,
                       &child_pid,
                       &error)) {
        g_print("Error starting script: %s\n", error->message);
        g_error_free(error);
    } else {
        // Add a watch function for the child process
        g_child_watch_add(child_pid, child_watch_cb, NULL);
    }
}
void exec_start() { exec_script(path_bin_start); }
void exec_stop() { exec_script(path_bin_stop); }

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    if (widget == btn_start) {
		g_print("  START Button clicked: %s\n", (char *)data);
		exec_start();
	} else if (widget == btn_stop) {
		g_print("  STOP Button clicked: %s\n", (char *)data);
		exec_stop();
	} else if (widget == btn_close) {
		g_print("  CLOSE Button clicked: %s\n", (char *)data);
		unlink(fn_pid);
		gtk_main_quit();
	} else {
		g_print("  Other Button clicked: %s\n", (char *)data);
	}
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
void set_label_status(const char txt[],
		char *style_button,
		char *style_label) {
	gtk_button_set_label(GTK_BUTTON(btn_status), txt);
	// set label makes a new label widget!!!
	label_status = gtk_bin_get_child(GTK_BIN(btn_status));
	gtk_widget_set_name(label_status, "label_status");
	set_widget_style(btn_status, style_button);
	set_widget_style(label_status,  style_label);
}
static void handle_signals(int sig) {
    g_print("Received signal %d\n", sig);
    switch (sig) {
        case SIGUSR1:
        	set_label_status(stat_rec,
				"#button_status { "
					"font-size: 10px; "
					"background: rgba(255, 255, 0, 0.8); "
					"border-radius: 10px; "
					"border: 2px solid red; "
					"padding-top: .3em; "
					"padding-bottom: .3em; "
				"}",
				"#label_status { "
					"font-weight: bold; "
					"font-size: 14px; "
					"color: black; "
				"}");
            break;
        case SIGUSR2:
        	set_label_status(stat_whi,
				"#button_status { "
					"font-size: 10px; "
					"background: rgba(255, 255, 0, 0.8); "
					"border-radius: 10px; "
					"border: none;"
					"padding-top: .3em; "
					"padding-bottom: .3em; "
				"}",
				"#label_status { "
					"font-weight: bold; "
					"font-size: 14px; "
					"color: black; "
				"}");
            break;
        case SIGHUP:
        	set_label_status(stat_rdy,
        		style_rdy_button,
        		style_rdy_label);
            break;
    }
}

void setup() {
	char *new_path;
	if (*path_bin_stop != '/') {
		new_path = locate_script(path_bin_stop);
		if (!new_path) {
			error_status = "No Stop Script!";
			g_print("Error locating script in path: %s\n", path_bin_stop);
		} else {
			path_bin_stop = new_path;
		}
	}
	if (*path_bin_start != '/') {
		new_path = locate_script(path_bin_start);
		if (!new_path) {
			error_status = "No Start Script!";
			g_print("Error locating script in path: %s\n", path_bin_start);
		} else {
			path_bin_start = new_path;
		}
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
    gtk_init(&argc, &argv);
	setup();

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

	btn_status = gtk_button_new_with_label(stat_rdy);
	gtk_widget_set_name(btn_status, "button_status");
	label_status = gtk_bin_get_child(GTK_BIN(btn_status));
	gtk_widget_set_name(label_status, "label_status");

	gtk_widget_set_events(btn_status, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);
	g_signal_connect(btn_status, "button-press-event", G_CALLBACK(on_button_press), window);
	g_signal_connect(btn_status, "motion-notify-event", G_CALLBACK(on_motion_notify), window);
	g_signal_connect(btn_status, "button-release-event", G_CALLBACK(on_button_release), window);
	gtk_box_pack_start(GTK_BOX(hbox), btn_status, TRUE, TRUE, 0);
	// Disable click events on status
	g_signal_handlers_block_by_func(btn_status, on_button_clicked, "Status");

	btn_close = gtk_button_new_with_label("☒");
	gtk_widget_set_name(btn_close, "button_close");
    g_signal_connect(btn_close, "clicked", G_CALLBACK(on_button_clicked), "Close");
    gtk_box_pack_start(GTK_BOX(hbox), btn_close, TRUE, TRUE, 0);

	// Apply CSS
    GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(provider,
		/* "#button_status, #button_status:hover, #button_status:active, #button_status:focus { " */
		/* 	"font-size: 13px; " */
		/* 	"background: rgba(48, 159, 16, 0.8); " */
		/* 	"border-radius: 10px; " */
		/* "} " */
		/* "#label_status { " */
		/* 	"color: white; " */
		/* 	"font-weight: bold; " */
		/* "} " */
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
	if (error_status) {
		set_label_status(error_status,
			style_rdy_button,
			style_rdy_label);
	} else {
		set_label_status(stat_rdy,
			style_rdy_button,
			style_rdy_label);
	}

	// OTHER BUTTONS

    btn_start = gtk_button_new_with_label("Record");
    g_signal_connect(btn_start, "clicked", G_CALLBACK(on_button_clicked), "Record");
    gtk_box_pack_start(GTK_BOX(vbox), btn_start, TRUE, TRUE, 0);

    btn_stop = gtk_button_new_with_label("Stop");
    g_signal_connect(btn_stop, "clicked", G_CALLBACK(on_button_clicked), "Stop");
    gtk_box_pack_start(GTK_BOX(vbox), btn_stop, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);

	setup_pid_file();
    signal(SIGUSR1, handle_signals);
    signal(SIGUSR2, handle_signals);
    signal(SIGHUP, handle_signals);

    gtk_main();

    return 0;
}
