#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

static GtkWidget *entry1, *entry2;

static void show_result(GtkWidget *parent, double result) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%g", result);
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Result: %s", buf);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void show_error(GtkWidget *parent, const char *msg) {
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK,
        "%s", msg);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void on_button_clicked(GtkWidget *btn, gpointer user_data) {
    GtkWidget *window = GTK_WIDGET(user_data);
    const char *op = gtk_button_get_label(GTK_BUTTON(btn));

    const char *s1 = gtk_entry_get_text(GTK_ENTRY(entry1));
    const char *s2 = gtk_entry_get_text(GTK_ENTRY(entry2));

    double n1, n2;
    if (sscanf(s1, "%lf", &n1) != 1 || sscanf(s2, "%lf", &n2) != 1) {
        show_error(window, "Please input valid numbers");
        return;
    }

    double result = 0;
    if (op[0] == '+') result = n1 + n2;
    else if (op[0] == '-') result = n1 - n2;
    else if (op[0] == '*') result = n1 * n2;
    else if (op[0] == '/') {
        if (n2 == 0) {
            show_error(window, "Cannot divide by zero");
            return;
        }
        result = n1 / n2;
    }

    show_result(window, result);
}

static void apply_css(GtkWidget *widget, const char *css) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider(
        gtk_widget_get_style_context(widget),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    /* Window */
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "My Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 225, 250);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Background color via CSS */
    apply_css(window,
        "window { background-color: #0078D7; }"
        "label  { color: white; }"
        "button { color: white; background: #0078D7; border: 1px solid white; }"
        "entry  { background: white; color: black; }");

    /* Layout */
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    /* Label */
    GtkWidget *label = gtk_label_new("Please input two numbers");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    /* Entry 1 */
    entry1 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry1, FALSE, FALSE, 0);

    /* Entry 2 */
    entry2 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry2, FALSE, FALSE, 0);

    /* Buttons row */
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    const char *ops[] = {"+", "-", "*", "/"};
    for (int i = 0; i < 4; i++) {
        GtkWidget *btn = gtk_button_new_with_label(ops[i]);
        gtk_widget_set_size_request(btn, 40, 40);
        g_signal_connect(btn, "clicked", G_CALLBACK(on_button_clicked), window);
        gtk_box_pack_start(GTK_BOX(hbox), btn, TRUE, TRUE, 0);
    }

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
