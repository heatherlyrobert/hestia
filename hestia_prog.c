/*===============================[[ beg-code ]]===============================*/
#include    "hestia.h"



char      verstring    [500];



/*====================------------------------------------====================*/
/*===----                         support functions                    ----===*/
/*====================------------------------------------====================*/
static void      o___SUPPORT_________________o (void) {;}

char*        /*--: return versioning information ---------[ ------ [ ------ ]-*/
PROG_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strcpy (t, "[tcc built  ]");
#elif  __GNUC__  > 0
   strcpy (t, "[gnu gcc    ]");
#elif  __CBANG__  > 0
   strcpy (t, "[cbang      ]");
#elif  __HEPH__  > 0
   strcpy (t, "[hephaestus ]");
#else
   strcpy (t, "[unknown    ]");
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}

char             /* [------] display usage help information ------------------*/
PROG_about         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char        t           [LEN_RECD];
   int         x_len       =    0;
   /*---(display)----------+-----+-----+-*/
   printf("\n");
   printf("focus    : %s\n", P_FOCUS);
   printf("niche    : %s\n", P_NICHE);
   printf("purpose  : %s\n", P_PURPOSE);
   printf("\n");
   printf("namesake : %s\n", P_NAMESAKE);
   printf("heritage : %s\n", P_HERITAGE);
   printf("imagery  : %s\n", P_IMAGERY);
   printf("\n");
   printf("system   : %s\n", P_SYSTEM);
   printf("language : %s\n", P_LANGUAGE);
   printf("codesize : %s\n", P_CODESIZE);
   printf("\n");
   printf("author   : %s\n", P_AUTHOR);
   printf("created  : %s\n", P_CREATED);
   printf("depends  : %s\n", P_DEPENDS);
   printf("\n");
   printf("ver num  : %s\n", P_VERNUM);
   printf("ver txt  : %s\n", P_VERTXT);
   printf("\n");
   printf("usage    : %s\n", P_USAGE);
   printf("debug    : %s\n", P_DEBUG);
   printf("\n");
   strcpy (t, P_SUMMARY);
   x_len = strlen (t);
   for (i = 0; i < x_len; ++i)   if (t [i] == '¦')  t [i] = '\n';
   printf ("%s\n", t);
   printf("priority : %s\n", P_PRIORITY);
   printf("saying   : %s\n", P_SAYING);
   printf("\n");
   printf("alterns  : %s\n", P_ALTERNS);
   printf("reminder : %s\n", P_REMINDER);
   printf("\n");
   exit (0);
}



/*====================------------------------------------====================*/
/*===----                        program startup                       ----===*/
/*====================------------------------------------====================*/
static void      o___STARTUP_________________o (void) {;}

char         /*--> before logging starts ------------[ ------ [abc.de.fghijk]-*/
PROG_preinit            (void)
{
   return 0;
}

char         /*--> before argument processing -------[ ------ [abc.de.fghijk]-*/
PROG_init          (void)  
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(logger)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_TOPS   yLOG_info    ("namesake"  , P_NAMESAKE);
   DEBUG_TOPS   yLOG_info    ("heritage"  , P_HERITAGE);
   DEBUG_TOPS   yLOG_info    ("imagery"   , P_IMAGERY);
   DEBUG_TOPS   yLOG_info    ("purpose"   , P_PURPOSE);
   DEBUG_TOPS   yLOG_info    ("version"   , PROG_version    ());
   DEBUG_TOPS   yLOG_info    ("yEXEC"     , yEXEC_version   ());
   DEBUG_TOPS   yLOG_info    ("yLOG"      , yLOG_version    ());
   DEBUG_TOPS   yLOG_info    ("yPARSE"    , yPARSE_version  ());
   DEBUG_TOPS   yLOG_info    ("ySEC"      , ySEC_version    ());
   DEBUG_TOPS   yLOG_info    ("ySTR"      , ySTR_version    ());
   DEBUG_TOPS   yLOG_info    ("yURG"      , yURG_version    ());
   /*---(call whoami)--------------------*/
   rc = yEXEC_whoami (&my.pid, &my.ppid, &my.uid, NULL, &my.who, 'n');
   DEBUG_TOPS   yLOG_value   ("whoami"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_TOPS   yLOG_value   ("pid"       , my.pid);
   DEBUG_TOPS   yLOG_value   ("ppid"      , my.ppid);
   DEBUG_TOPS   yLOG_value   ("uid"       , my.uid);
   DEBUG_TOPS   yLOG_info    ("who"       , my.who);
   /*---(initialize)---------------------*/
   rc = tty_init ();
   DEBUG_ARGS   yLOG_info    ("yPARSE"   ,"initializing");
   rc = yPARSE_init  ('-', NULL, '-');
   rc = yPARSE_delimiters  ("");
   /*---(complete)-----------------------*/
   DEBUG_TOPS  yLOG_exit   (__FUNCTION__);
   return 0;
}

char         /*--> command line arguments -----------[ ------ [abc.de.fghijk]-*/
PROG_args          (int a_argc, char *a_argv[])
{
   /*---(begin)------------+-----------+-*/
   DEBUG_PROG   yLOG_enter  (__FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   /*---(allocation)---------------------*/
   /*> ntty   = a_argc - 1;                                                             <* 
    *> yLOG_value  ("ttys"      , ntty);                                              <* 
    *> ttys      = (tTTY  *) malloc (sizeof (tTTY ) * ntty);                          <* 
    *> yLOG_note   ("allocated ttys");                                                <* 
    *> polling   = (tPOLL *) malloc (sizeof (tPOLL) * ntty);                          <* 
    *> yLOG_note   ("allocated polling");                                             <*/
   srand (time(NULL));
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit   (__FUNCTION__);
   return 0;
}

char         /*--> before normal start --------------[ ------ [abc.de.fghijk]-*/
PROG_begin         (void)
{
   /*---(begin)------------+-----------+-*/
   DEBUG_PROG   yLOG_enter  (__FUNCTION__);
   /*---(set file names)-----------------*/
   DEBUG_PROG   yLOG_note    ("setting file names");
   snprintf (my.name_conf     , LEN_PATH, "%s%s", DIR_ETC  , FILE_CONF);
   snprintf (my.name_exec     , LEN_PATH, "%s%s", DIR_TMP  , FILE_EXEC);
   snprintf (my.name_status   , LEN_PATH, "%s%s", DIR_TMP  , FILE_STATUS);
   snprintf (my.name_heartbeat, LEN_PATH, "%s%s", DIR_RUN  , FILE_HEARTBEAT);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit   (__FUNCTION__);
   return   0;
}

char         /*--> before start, not testing --------[ ------ [abc.de.fghijk]-*/
PROG_final         (void)
{
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         runtime support                      ----===*/
/*====================------------------------------------====================*/
static void      o___RUNTIME_________________o (void) {;}


/*> void             /+ [------] receive signals ---------------------------------+/   <* 
 *> PROG_signal        (int a_signal, siginfo_t *a_info, void *a_nada)                 <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-----------+-----------+-+/                                        <* 
 *>    int         status      = 0;                                                    <* 
 *>    int         xlink       = 0;                                                    <* 
 *>    int         errsave     = 0;                                                    <* 
 *>    int         i           = 0;                                                    <* 
 *>    switch (a_signal) {                                                             <* 
 *>    case  SIGCHLD:                                                                  <* 
 *>       break;                                                                       <* 
 *>    case  SIGHUP:                                                                   <* 
 *>       PROG_end ();                                                                 <* 
 *>       execvp("/sbin/hestia", NULL);                                                <* 
 *>       break;                                                                       <* 
 *>    case  SIGUSR1:                                                                  <* 
 *>    case  SIGUSR2:                                                                  <* 
 *>    case  SIGTERM:                                                                  <* 
 *>    case  SIGSEGV:                                                                  <* 
 *>       /+---(loop through ttys)--------------+/                                     <* 
 *>       for (i = 0; i < ntty; ++i) {                                                 <* 
 *>          if (g_ttys [i].allowed != 'y') continue;                                  <* 
 *>          if (g_ttys [i].watched != 'y') continue;                                  <* 
 *>          if (g_ttys [i].active  == 'y') continue;                                  <* 
 *>          ySEC_getty_off (g_ttys [i].full + 5, getpid());                           <* 
 *>       }                                                                            <* 
 *>       break;                                                                       <* 
 *>    default:                                                                        <* 
 *>       break;                                                                       <* 
 *>    }                                                                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return;                                                                         <* 
 *> }                                                                                  <*/

void             /* [------] receive signals ---------------------------------*/
PROG_comm          (int a_signal, siginfo_t *a_info, void *a_nada)
{
   /*---(catch)--------------------------*/
   switch (a_signal) {
   case  SIGHUP:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGHUP MEANS REFRESH");
      break;
   case  SIGUSR1:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGUSR1 MEANS ...");
      break;
   case  SIGUSR2:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGUSR2 MEANS ...");
      break;
   case  SIGALRM:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGALRM MEANS ...");
      break;
   case  SIGTERM:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGTERM means terminate daemon");
      yEXEC_term    ("EXITING", 99);
      break;
   case  SIGSEGV:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGSEGV means daemon blew up");
      yEXEC_term    ("EXITING", 99);
      break;
   case  SIGABRT:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGABRT means daemon blew up");
      yEXEC_term    ("EXITING", 99);
      break;
   default      :
      DEBUG_PROG  yLOG_info     ("SIGNAL", "unknown signal recieved");
      break;
   }
   /*---(complete)-----------------------*/
   return;
}

char         /*--> daemonize the program -------------------------------------*/
PROG_daemon        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_running   =    0;
   int         x_uid       =    0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(check for other)----------------*/
   x_running = yEXEC_find ("hestia", NULL);
   DEBUG_ENVI   yLOG_value   ("x_running" , x_running);
   --rce;  if (x_running > 1) {
      printf ("hestia already running in daemon mode\n");
      DEBUG_ENVI   yLOG_note    ("hestia already running in daemon mode");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for user)-----------------*/
   DEBUG_ENVI   yLOG_value   ("my.uid"    , my.uid);
   --rce;  if (my.uid != 0) {
      printf ("only root can run hestia in daemon mode\n");
      DEBUG_ENVI   yLOG_note    ("only root can run hestia in daemon mode");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(fork off and die)---------------*/
   DEBUG_ENVI   yLOG_value   ("logger"    , yURG_debug.logger);
   rc = yEXEC_daemon (yURG_debug.logger, &my.pid);
   DEBUG_ENVI   yLOG_value   ("daemon"    , rc);
   --rce;  if (rc < 0) {
      printf ("hestia could not be daemonized\n");
      DEBUG_ENVI   yLOG_note    ("hestia could not be daemonized");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(signals)-------------------------------*/
   yEXEC_signal (YEXEC_SOFT, YEXEC_YES, YEXEC_YES, PROG_comm);
   DEBUG_ENVI   yLOG_value   ("signals"   , rc);
   --rce;  if (rc < 0) {
      printf ("hestia sigals could not be set properly\n");
      DEBUG_ENVI   yLOG_note    ("hestia signals cound not be set properly");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)------------------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     program wrapup                           ----===*/
/*====================------------------------------------====================*/
static void      o___WRAPUP__________________o (void) {;}

char         /*--> wrap-up program ------------------[ ------ [abc.de.fghijk]-*/
PROG_end           (void)
{
   /*---(begin)------------+-----------+-*/
   yLOG_enter  (__FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   /*---(complete)-----------------------*/
   yLOG_exit   (__FUNCTION__);
   yLOG_end    ();
   return   0;
}



/*====================------------------------------------====================*/
/*===----                   helpers for unit testing                   ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char       /*----: set up program test file locations ------------------------*/
prog__unit_files   (void)
{
   char        x_cmd       [LEN_RECD];
   /*> chdir    ("/tmp");                                                             <* 
    *> sprintf  (x_cmd, "rm -fr %s* > /dev/null", DIR_UNIT);                          <* 
    *> system   (x_cmd);                                                              <* 
    *> rmdir    (DIR_UNIT);                                                           <* 
    *> sprintf  (x_cmd, "mkdir %s   > /dev/null", DIR_UNIT);                          <* 
    *> system   (x_cmd);                                                              <*/
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_quiet   (void)
{
   int         x_argc      = 1;
   char       *x_argv [1]  = { "hestia" };
   PROG_preinit   ();
   yURG_logger    (x_argc, x_argv);
   yURG_urgs      (x_argc, x_argv);
   PROG_init      ();
   prog__unit_files ();
   PROG_args      (x_argc, x_argv);
   PROG_begin     ();
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_loud    (void)
{
   int         x_argc      = 4;
   char       *x_argv [4]  = { "hestia_unit", "@@kitchen", "@@yparse", "@@yexec"  };
   PROG_preinit   ();
   yURG_logger    (x_argc, x_argv);
   yURG_urgs      (x_argc, x_argv);
   PROG_init      ();
   prog__unit_files ();
   PROG_args      (x_argc, x_argv);
   PROG_begin     ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
prog__unit_end     (void)
{
   char        x_cmd       [LEN_RECD];
   /*> chdir    ("/tmp");                                                             <* 
    *> sprintf  (x_cmd, "rm -fr %s* > /dev/null", DIR_UNIT);                          <* 
    *> system   (x_cmd);                                                              <* 
    *> rmdir    (DIR_UNIT);                                                           <*/
   PROG_end       ();
   return 0;
}

