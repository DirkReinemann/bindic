#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

const char *GLADE_FILE = "bindic.glade";
const char *CSS_FILE = "bindic.css";
const char *CONFIG_DIR = "/usr/share/bindic";
const int TIME = 500;

guint timeout = 0;

gboolean on_window_destroy()
{
    g_source_remove(timeout);
    gtk_main_quit();
    return FALSE;
}

char *get_config_file(const char *filename)
{
    char *result = malloc(0 * sizeof(char));
    size_t sfilename = strlen(filename);

    if (access(filename, R_OK) != -1) {
        result = (char *)realloc(result, (sfilename + 1) * sizeof(char));
        strncpy(result, filename, sfilename);
        result[sfilename] = '\0';
    } else {
        size_t sconfigdir = strlen(CONFIG_DIR);
        size_t sfilepath = sconfigdir + sfilename + 1;
        char filepath[sfilepath + 1];
        strncpy(filepath, CONFIG_DIR, sconfigdir);
        strncpy(filepath + sconfigdir, "/", 1);
        strncpy(filepath + sconfigdir + 1, filename, sfilename);
        filepath[sfilepath] = '\0';

        if (access(filepath, R_OK) != -1) {
            result = (char *)realloc(result, (sfilepath + 1) * sizeof(char));
            strncpy(result, filepath, sfilepath);
            result[sfilepath] = '\0';
        }
    }
    return result;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage: %s [VALUE] [MAX]\n", argv[0]);
        return 1;
    }

    char *glade_file = get_config_file(GLADE_FILE);
    char *css_file = get_config_file(CSS_FILE);

    if (glade_file[0] == '\0' || css_file[0] == '\0') {
        printf("The glade file '%s' or the css file '%s' couldn't be found.\n", GLADE_FILE, CSS_FILE);
        return 2;
    }

    gdouble value = (gdouble)atol(argv[1]);
    gdouble max = (gdouble)atol(argv[2]);

    if (value > max) {
        printf("The actual value '%0.f' is bigger than the maximum value '%0.f'\n.", value, max);
        return 3;
    }

    gchar text[10];
    g_snprintf(text, 9, "%0.f", value);

    gtk_init(NULL, NULL);

    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, glade_file, NULL);
    gtk_builder_connect_signals(builder, NULL);

    GtkCssProvider *provider = gtk_css_provider_get_default();
    gtk_css_provider_load_from_path(provider, css_file, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);


    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkLevelBar *bar = GTK_LEVEL_BAR(gtk_builder_get_object(builder, "bar"));
    GtkLabel *label = GTK_LABEL(gtk_builder_get_object(builder, "label"));

    gtk_level_bar_set_value(bar, value);
    gtk_level_bar_set_max_value(bar, max);
    gtk_label_set_text(label, text);

    timeout = g_timeout_add(TIME, on_window_destroy, NULL);

    gtk_widget_set_opacity(GTK_WIDGET(window), 0.1);

    free(glade_file);
    free(css_file);
    g_object_unref(builder);
    gtk_widget_show(GTK_WIDGET(window));
    gtk_main();

    return 0;
}
