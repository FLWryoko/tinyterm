#include <glib.h>
#include <gtk/gtk.h>
#include <vte/vte.h>
#include <pwd.h>
#include <stdlib.h>

GtkWidget *vte ;
GtkWidget *window ;
GtkWidget *main_box;
GtkWidget *box ;
const gchar *pwd;
gchar **env;
gchar **exec;

static const gchar *get_shell(){
 const gchar *shell;
 const gchar *fallback_shell = "/bin/sh";
 shell = g_getenv("SHELL");
 if(shell != NULL && access(shell,X_OK) == 0){
    return shell;
 }else if(access(fallback_shell,X_OK) == 0){
 	 return fallback_shell;
 }else{
 return NULL;
 }
}

static gchar init_exec(){

if(exec == NULL){
vte_terminal_feed(vte,"",-1);
exec = g_malloc(3);
exec[0] = get_shell();
exec[1] = g_path_get_basename(exec[0]);
exec[2] = NULL;
}
}
static void  create_vte(){

 window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

 g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), &window);

 GdkVisual *visual = gdk_screen_get_rgba_visual(gtk_widget_get_screen(GTK_WIDGET(window)));
 if(visual != NULL)
  {
 gtk_widget_set_visual(window, visual);
  }else
  {
  }

 vte = vte_terminal_new();

 if(pwd == NULL){
  pwd = g_get_current_dir();
 }

init_exec();
vte_terminal_spawn_async(vte, VTE_PTY_NO_LASTLOG | VTE_PTY_NO_UTMP | VTE_PTY_NO_WTMP, pwd, exec, env, G_SPAWN_SEARCH_PATH | G_SPAWN_FILE_AND_ARGV_ZERO, NULL, NULL, NULL, 10, NULL, NULL, NULL);

 main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
 gtk_container_add(GTK_CONTAINER(window), main_box);
 gtk_container_add(GTK_CONTAINER(main_box), vte);

 gtk_widget_show_all(window);

}

int main(int argc, char *argv[])
{
 get_shell();	
 gtk_init(&argc, &argv);
 create_vte();
 gtk_main();
}
