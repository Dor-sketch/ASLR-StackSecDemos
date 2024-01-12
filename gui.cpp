#include "BaseUser.h"
#include "utils.h"
#include <cstring>
#include <fstream>
#include <gtk/gtk.h>
#include <iostream>


class Gui {
  GtkWidget *window;
  GtkWidget *buttonSubmit;
  GtkWidget *buttonDemo;
  GtkWidget *label;
  GtkWidget *entry;
  GtkWidget *vbox;

public:
  Gui() : user(new BaseUser("user", "abc")) {
    gtk_init(NULL, NULL);
    setupWindow();
    setupWidgets();
    connectSignals();
    gtk_widget_show_all(window);
    gtk_main();
  }

  ~Gui() { delete user; }
  BaseUser mutable *user;

  void setupWindow() {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Overflow Demo");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  }

  void setupWidgets() {
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    label = gtk_label_new("Enter password:");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    buttonSubmit = gtk_button_new_with_label("Submit");
    gtk_box_pack_start(GTK_BOX(vbox), buttonSubmit, FALSE, FALSE, 0);

    buttonDemo = gtk_button_new_with_label("Don't click me!");
    gtk_box_pack_start(GTK_BOX(vbox), buttonDemo, FALSE, FALSE, 0);
  }

  void connectSignals() {
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);
    g_signal_connect(G_OBJECT(buttonSubmit), "clicked",
                     G_CALLBACK(button_clicked), this);
    g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(entry_activate),
                     this);
    g_signal_connect(G_OBJECT(buttonDemo), "clicked", G_CALLBACK(runDemo),
                     NULL);
  }

  static void destroy(GtkWidget *widget, gpointer data) { gtk_main_quit(); }

  static void button_clicked(GtkWidget *widget, gpointer data) {
    Gui *gui = (Gui *)data;
    const char *pw = gtk_entry_get_text(GTK_ENTRY(gui->entry));
    gui->user->checkAccess(pw);
  }

  static void entry_activate(GtkWidget *widget, gpointer data) {
    button_clicked(widget, data); // Reuse button_clicked function
  }

  static void runDemo(GtkWidget *widget, gpointer data) {
    const std::string random_3_chars = std::to_string(rand() % 1000);
    BaseUser base("John", random_3_chars.c_str());
    char pw[PW_BUFFER_SIZE];
    BaseUser *b = &base;
    int *p = reinterpret_cast<int *>(b);

    std::ofstream outfile("pass.bin", std::ios::out | std::ios::binary);
    if (outfile.is_open()) {
      for (int i = 0; i < sizeof(pw); i++) {
        char c = 'A';
        outfile.write(&c, sizeof(c));
      }

      // Adjust the address based on system architecture.
      int address = p[0] - MEM_ADDRESS_SIZE;
      outfile.write(reinterpret_cast<const char *>(&address), sizeof(address));
      outfile.close();
      std::cout << "Overflow file 'pass.bin' created." << std::endl;
    } else {
      std::cout << "Unable to open file." << std::endl;
    }
    std::ifstream infile("pass.bin", std::ios::in | std::ios::binary);

    if (infile.is_open()) {
      infile.seekg(0, std::ios::end);
      int length = infile.tellg();
      infile.seekg(0, std::ios::beg);

      char *buffer = new char[length];
      infile.read(buffer, length);

      std::cout << "Overflow buffer was generated: ";
      for (int i = 0; i < length; i++) {
        std::cout << std::hex << static_cast<int>(buffer[i]) << " ";
      }
      std::cout << std::endl;

      // input the file into the entry with unsafe gets
      // this is the overflow

      unsafeCopy(pw, buffer);
      std::cout << "Password: [Applying overflow using unsafeCopy...]\n";

      b->checkAccess(pw);
    }
  }
};

int main(int argc, char **argv) {
  Gui gui;
  return 0;
}
