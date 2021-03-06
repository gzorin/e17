#include "elsa.h"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "Ecore_Getopt.h"
#include <xcb/xcb.h>

#define ELSA_DISPLAY ":0.0"


static Eina_Bool _open_log();
static int _elsa_main(const char *dname);
static void _remove_lock();
static void _signal_cb();
static void _signal_log();
static Eina_Bool _elsa_client_del(void *data, int type, void *event);

static Eina_Bool _testing = 0;
static Eina_Bool _xephyr = 0;
static Ecore_Exe *_elsa_client = NULL;



static void
_signal_cb(int sig)
{
   fprintf(stderr, PACKAGE": signal %d received\n", sig);
   //FIXME  if I don't have main loop at this time ?
   if (_elsa_client) ecore_exe_terminate(_elsa_client);
   /*
   elsa_session_shutdown();
   elsa_xserver_shutdown();
   exit(1);
   */
}

static void
_signal_log(int sig)
{
   fprintf(stderr, PACKAGE": signal %d received reopen the log file\n", sig);
   elsa_close_log();
   _open_log();
}

static Eina_Bool
_get_lock()
{
   FILE *f;
   char buf[128];
   int my_pid;

   my_pid = getpid();
   f = fopen(elsa_config->lockfile, "r");
   if (!f)
     {
        /* No lockfile, so create one */
        f = fopen(elsa_config->lockfile, "w");
        if (!f)
          {
             fprintf(stderr, PACKAGE": Couldn't create lockfile %s!\n",
                     elsa_config->lockfile);
             return (EINA_FALSE);
          }
        snprintf(buf, sizeof(buf), "%d\n", my_pid);
        if (!fwrite(buf, strlen(buf), 1, f))
          {
             fclose(f);
             fprintf(stderr, PACKAGE": Couldn't write the lockfile\n");
             return EINA_FALSE;
          }
        fclose(f);
     }
   else
     {
        int pid = 0;
        /* read the lockfile */
        if (fgets(buf, sizeof(buf), f))
          pid = atoi(buf);
        fclose(f);
        if (pid == my_pid)
          return EINA_TRUE;
        fprintf(stderr, "A lock file are present another instance are present ?\n");
        return EINA_FALSE;
     }

   return EINA_TRUE;
}

static void
_update_lock()
{
   FILE *f;
   char buf[128];
   f = fopen(elsa_config->lockfile, "w");
   snprintf(buf, sizeof(buf), "%d\n", getpid());
   if (!fwrite(buf, strlen(buf), 1, f))
     fprintf(stderr, PACKAGE": Coudn't update lockfile\n");
   fclose(f);
}

static void
_remove_lock()
{
   remove(elsa_config->lockfile);
}

static Eina_Bool
_open_log()
{
   FILE *elog;
   if (_testing) return EINA_TRUE;
   elog = fopen(elsa_config->logfile, "a");
   if (!elog)
     {
        fprintf(stderr, PACKAGE": could not open logfile %s!!!\n",
                elsa_config->logfile);
        return EINA_FALSE;
     }
   fclose(elog);
   if (!freopen(elsa_config->logfile, "a", stdout))
     fprintf(stderr, PACKAGE": Error on reopen stdout\n");
   setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
   if (!freopen(elsa_config->logfile, "a", stderr))
     fprintf(stderr, PACKAGE": Error on reopen stderr\n");
   setvbuf(stderr, NULL, _IONBF, BUFSIZ);
   return EINA_TRUE;
}

void
elsa_close_log()
{
   if (!_testing)
   {
      fclose(stderr);
      fclose(stdout);
   }
}

static void
_elsa_wait()
{
   execl(PACKAGE_BIN_DIR"/elsa_wait", "/usr/sbin/elsa", NULL);
   fprintf(stderr, PACKAGE": HUM HUM HUM ...\n\n\n");
}

static int
_elsa_main(const char *dname)
{
   if (!elsa_config->autologin)
     {
        if (!_elsa_client)
          {
             char buf[PATH_MAX];
             ecore_event_handler_add(ECORE_EXE_EVENT_DEL,
                                     _elsa_client_del, NULL);
             fprintf(stderr, PACKAGE": Exec elsa_client\n");
             snprintf(buf, sizeof(buf),
                      PACKAGE_BIN_DIR"/elsa_client -d %s -t %s",
                      dname, elsa_config->theme);
             _elsa_client = ecore_exe_run(buf, NULL);
          }
     }
   else
     ecore_main_loop_quit();
   return 0;
}

static Eina_Bool
_elsa_client_del(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Exe_Event_Del *ev;

   ev = event;
   if (ev->exe != _elsa_client)
     return ECORE_CALLBACK_PASS_ON;
   ecore_main_loop_quit();
   _elsa_client = NULL;
   fprintf(stderr, PACKAGE": client have terminated\n");

   return ECORE_CALLBACK_DONE;
}


static const Ecore_Getopt options =
{
   PACKAGE,
   "%prog [options]",
   VERSION,
   "(C) 2011 Enlightenment, see AUTHORS",
   "GPL, see COPYING",
   "Elsa is a login manager, written using core efl libraries",
   EINA_TRUE,
   {
      ECORE_GETOPT_STORE_TRUE('n', "nodaemon", "Don't daemonize."),
      ECORE_GETOPT_STORE_TRUE('t', "test", "run in test mode."),
      ECORE_GETOPT_STORE_TRUE('x', "xephyr", "run in test mode and use Xephyr."),
      ECORE_GETOPT_HELP ('h', "help"),
      ECORE_GETOPT_VERSION('V', "version"),
      ECORE_GETOPT_COPYRIGHT('R', "copyright"),
      ECORE_GETOPT_LICENSE('L', "license"),
      ECORE_GETOPT_SENTINEL
   }
};

int
main (int argc, char ** argv)
{
   int args;
   int pid;
   char *dname = strdup(ELSA_DISPLAY);
   char *elsa_user = NULL;
   unsigned char nodaemon = 0;
   unsigned char quit_option = 0;
   Ecore_Getopt_Value values[] =
     {
        ECORE_GETOPT_VALUE_BOOL(nodaemon),
        ECORE_GETOPT_VALUE_BOOL(_testing),
        ECORE_GETOPT_VALUE_BOOL(_xephyr),
        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_NONE
     };

   args = ecore_getopt_parse(&options, values, argc, argv);
   if (args < 0)
     {
        fprintf(stderr, PACKAGE": ERROR: could not parse options.\n");
        return -1;
     }

   if (quit_option)
     return 0;

   if(getuid() != 0 && !_testing)
     {
        fprintf(stderr, "Only root can run this program\n");
        return 1;
     }

   if (_testing)
     nodaemon = EINA_TRUE;

   eet_init();
   elsa_config_init();
   if (!_testing && !_get_lock())
     {
        exit(1);
     }

   if (!nodaemon && elsa_config->daemonize)
     {
        if (daemon(0, 1) == -1)
          {
             fprintf(stderr, PACKAGE": Error on daemonize !");
             quit_option = EINA_TRUE;
          }
        _update_lock();
        int fd;
        if ((fd = open("/dev/null", O_RDONLY)))
          {
             dup2(fd, 0);
             close(fd);
          }
     }

   if (!_open_log())
     {
        fprintf(stderr, PACKAGE": Can't open log file !!!!\n");
        quit_option = EINA_TRUE;
     }

   if (quit_option)
     {
        elsa_config_shutdown();
        elsa_close_log();
        exit(1);
     }

   elsa_user = getenv("ELSA_USER");
#ifdef HAVE_PAM
   elsa_pam_init(PACKAGE, dname, elsa_user);
#endif
   if (elsa_user)
     {
        char *quit;
        elsa_session_end(elsa_user);
        sleep(2);
        elsa_xserver_end();
        quit = getenv("ELSA_QUIT");
        if (quit)
          {
             unsetenv("ELSA_QUIT");
             _remove_lock();
             elsa_config_shutdown();
             fprintf(stderr, PACKAGE": Good bye\n");
             exit(0);
          }
        sleep(3);
#ifdef HAVE_PAM
        elsa_pam_init(PACKAGE, dname, NULL);
#endif
     }
   else
     fprintf(stderr, "\n");
   fprintf(stderr, PACKAGE": Welcome\n");
   ecore_init();
   /* Initialise event handler */

   signal(SIGQUIT, _signal_cb);
   signal(SIGTERM, _signal_cb);
   signal(SIGKILL, _signal_cb);
   signal(SIGINT, _signal_cb);
   signal(SIGHUP, _signal_cb);
   signal(SIGPIPE, _signal_cb);
   signal(SIGALRM, _signal_cb);
   signal(SIGUSR2, _signal_log);

   fprintf(stderr, PACKAGE": session init\n");
   elsa_session_init(elsa_config->command.xauth_file);
   fprintf(stderr, PACKAGE": xserver init\n");
   pid = elsa_xserver_init(_elsa_main, dname);

   if (elsa_config->autologin && !elsa_user)
     {
        xcb_connection_t *disp = NULL;
        disp = xcb_connect(dname, NULL);
        ecore_main_loop_begin();
#ifdef HAVE_PAM
        elsa_pam_item_set(ELSA_PAM_ITEM_USER, elsa_config->userlogin);
#endif
        elsa_session_login(elsa_config->command.session_login, EINA_FALSE);
        sleep(30);
        xcb_disconnect(disp);
     }
   else
     {
        fprintf(stderr, PACKAGE": action init\n");
        elsa_action_init();
        fprintf(stderr, PACKAGE": history init\n");
        elsa_history_init();
        fprintf(stderr, PACKAGE": server init\n");
        elsa_server_init();
        fprintf(stderr, PACKAGE": starting main loop\n");
        ecore_main_loop_begin();
        fprintf(stderr, PACKAGE": main loop end\n");
        elsa_server_shutdown();
        fprintf(stderr, PACKAGE": server shutdown\n");
        elsa_history_shutdown();
        fprintf(stderr, PACKAGE": history shutdown\n");
        elsa_action_shutdown();
        fprintf(stderr, PACKAGE": action shutdown\n");
     }
   elsa_xserver_shutdown();
   fprintf(stderr, PACKAGE": xserver shutdown\n");
#ifdef HAVE_PAM
   elsa_pam_shutdown();
   fprintf(stderr, PACKAGE": pam shutdown\n");
#endif
   ecore_shutdown();
   elsa_config_shutdown();
   fprintf(stderr, PACKAGE": config shutdown\n");
   elsa_session_shutdown();
   fprintf(stderr, PACKAGE": session shutdown\n");
   eet_shutdown();
   free(dname);
   if (elsa_session_logged_get())
     {
        fprintf(stderr, PACKAGE": close log\n");
        elsa_close_log();
        fprintf(stderr, PACKAGE": wait session \n");
        _elsa_wait();
     }
   fprintf(stderr, PACKAGE": ending xserver\n");
   kill(pid, SIGTERM);
   elsa_xserver_end();
   fprintf(stderr, PACKAGE": close log\n");
   elsa_close_log();
   return 0;
}

