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



/*====================------------------------------------====================*/
/*===----                        program startup                       ----===*/
/*====================------------------------------------====================*/
static void      o___STARTUP_________________o (void) {;}

char
PROG_defaults      (void)
{
   my.user_mode = MODE_DAEMON;
   DEBUG_PROG   yLOG_note    ("setting file names");
   snprintf (my.n_conf        , LEN_PATH, "%s%s", DIR_ETC  , FILE_CONF);
   snprintf (my.name_exec     , LEN_PATH, "%s%s", DIR_TMP  , FILE_EXEC);
   snprintf (my.name_status   , LEN_PATH, "%s%s", DIR_TMP  , FILE_STATUS);
   snprintf (my.name_heartbeat, LEN_PATH, "%s%s", DIR_RUN  , FILE_HEARTBEAT);
   my.wait_dsec = 10;
}

char         /*--> before argument processing -------[ ------ [abc.de.fghijk]-*/
PROG_init          (void)  
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(logger)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   DEBUG_PROG   yLOG_info    ("namesake"  , P_NAMESAKE);
   DEBUG_PROG   yLOG_info    ("heritage"  , P_HERITAGE);
   DEBUG_PROG   yLOG_info    ("imagery"   , P_IMAGERY);
   DEBUG_PROG   yLOG_info    ("purpose"   , P_PURPOSE);
   DEBUG_PROG   yLOG_info    ("version"   , PROG_version    ());
   DEBUG_PROG   yLOG_info    ("yEXEC"     , yEXEC_version   ());
   DEBUG_PROG   yLOG_info    ("yLOG"      , yLOGS_version   ());
   DEBUG_PROG   yLOG_info    ("yPARSE"    , yPARSE_version  ());
   DEBUG_PROG   yLOG_info    ("ySEC"      , ySEC_version    ());
   DEBUG_PROG   yLOG_info    ("ySTR"      , ySTR_version    ());
   DEBUG_PROG   yLOG_info    ("yURG"      , yURG_version    ());
   /*---(defaults)-----------------------*/
   PROG_defaults ();
   srand (time(NULL));
   /*---(call whoami)--------------------*/
   rc = yEXEC_whoami (&my.pid, &my.ppid, &my.uid, NULL, &my.who, 'n');
   DEBUG_PROG   yLOG_value   ("whoami"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG   yLOG_value   ("pid"       , my.pid);
   DEBUG_PROG   yLOG_value   ("ppid"      , my.ppid);
   DEBUG_PROG   yLOG_value   ("uid"       , my.uid);
   DEBUG_PROG   yLOG_info    ("who"       , my.who);
   /*---(initialize)---------------------*/
   rc = tty_init ();
   DEBUG_ARGS   yLOG_info    ("yPARSE"   ,"initializing");
   rc = yPARSE_init  ('-', NULL, '-');
   rc = yPARSE_delimiters  ("");
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit   (__FUNCTION__);
   return 0;
}

#define    TWOARG      if (two_arg == 1)

char         /*--> command line arguments -----------[ ------ [abc.de.fghijk]-*/
PROG_args          (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i           =    0;            /* loop iterator -- arguments          */
   char       *a           = NULL;         /* current argument                    */
   int         x_len       =    0;            /* argument length                     */
   char        two_arg     =    0;
   int         x_max       =    0;
   /*---(begin)--------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   DEBUG_PROG   yLOG_value   ("a_argc"      , a_argc);
   if (a_argc == 1) {
      DEBUG_PROG   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(program name)--------------------------*/
   strlcpy (my.prog_name, a_argv [0], LEN_USER);
   DEBUG_PROG   yLOG_value   ("prog name" , my.prog_name);
   /*---(process)------------------------*/
   for (i = 1; i < a_argc; ++i) {
      a = a_argv [i];
      if (i < a_argc - 1) two_arg = 1;
      else                two_arg = 0;
      x_len = strllen (a, LEN_RECD);
      /*---(skip debugging)--------------*/
      if      (a[0] == '@')                     continue;
      /*---(version)---------------------*/
      if      (strcmp  (a, "--version"      ) == 0) { printf ("%s\n", PROG_version ()); rc = -1; }
      else if (strcmp  (a, "--conf"         ) == 0) { TWOARG  strlfile (a, my.n_conf, a_argv [++i], "conf", LEN_RECD); }
      else if (strcmp  (a, "--daemon"       ) == 0)   my.user_mode = MODE_DAEMON;
      else if (strcmp  (a, "--verify"       ) == 0)   my.user_mode = MODE_VERIFY;
      else if (strcmp  (a, "--park"         ) == 0)   my.user_mode = MODE_PARK;
      else if (strcmp  (a, "--fast"         ) == 0)   my.wait_dsec =   2; /*   0.2 min */
      else if (strcmp  (a, "--norm"         ) == 0)   my.wait_dsec =  10; /*   1   min */
      else if (strcmp  (a, "--slow"         ) == 0)   my.wait_dsec = 100; /*  10   min */
      else if (strncmp (a, "--abcde"     , 7) == 0)   ;
      else if (strncmp (a, "--12345"     , 7) == 0)   ;
      /*---(unknown)---------------------*/
      else    printf("requested action not understood or incomplete\n");
   }
   /*---(max name)-----------------------*/
   rc = yEXEC_maxname (a_argc, a_argv, &x_max);
   strlcpy (a_argv [0], P_ONELINE, x_max);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return rc;
}


/*345678901-12345678901-123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*> hestia-polyolbos (full of blessings) getty terminal daemon                        <* 
 *> /sbin/hestia --loadsharing --aggressive --hardened --123456                       <*/

/*> khronos-anileis (merciless time) primary batch-automation daemon                  <* 
 *> khronos --daemon --abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrs                  <*/

char         /*--> before normal start --------------[ ------ [abc.de.fghijk]-*/
PROG_begin         (void)
{
   /*---(begin)------------+-----------+-*/
   DEBUG_PROG   yLOG_enter  (__FUNCTION__);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit   (__FUNCTION__);
   return   0;
}

char         /*--> before start, not testing --------[ ------ [abc.de.fghijk]-*/
PROG_final         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(logger)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   rc = conf_driver ();
   DEBUG_PROG   yLOG_value   ("conf"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr  (__FUNCTION__, rce);
      return rce;
   }
   rc = tty_review  ();
   DEBUG_PROG   yLOG_value   ("review"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit   (__FUNCTION__);
   return   0;
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
PROG_comm          (int a_signal, siginfo_t *a_info, char *a_name, char *a_desc)
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
   x_running  = yEXEC_find ("hestia", NULL);
   x_running += yEXEC_find ("hestia_debug", NULL);
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
   DEBUG_ENVI   yLOG_value   ("logger"    , yLOGS_lognum ());
   rc = yEXEC_daemon (yLOGS_lognum (), &my.pid);
   DEBUG_ENVI   yLOG_value   ("daemon"    , rc);
   --rce;  if (rc < 0) {
      printf ("hestia could not be daemonized\n");
      DEBUG_ENVI   yLOG_note    ("hestia could not be daemonized");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(signals)-------------------------------*/
   yEXEC_signal (YEXEC_SOFT, YEXEC_YES, YEXEC_NO, PROG_comm, "stdsig");
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
   /*---(actions)------------------------*/
   tty_wrap ();
   /*---(complete)-----------------------*/
   yLOG_exit   (__FUNCTION__);
   yLOGS_end   ();
   return   0;
}



/*====================------------------------------------====================*/
/*===----                   helpers for unit testing                   ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char       /*----: set up program test file locations ------------------------*/
prog__unit_files   (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_cmd       [LEN_RECD];
   /*---(change file locations)----------*/
   snprintf (my.n_conf        , LEN_PATH, "%s%s", DIR_UNIT , FILE_CONF);
   snprintf (my.name_exec     , LEN_PATH, "%s%s", DIR_UNIT , FILE_EXEC);
   snprintf (my.name_status   , LEN_PATH, "%s%s", DIR_UNIT , FILE_STATUS);
   snprintf (my.name_heartbeat, LEN_PATH, "%s%s", DIR_UNIT , FILE_HEARTBEAT);
   /*---(wipe)---------------------------*/
   chdir    ("/tmp");
   sprintf  (x_cmd, "rm -fr %s* > /dev/null", DIR_UNIT);
   system   (x_cmd);
   /*---(set up)-------------------------*/
   rmdir    (DIR_UNIT);
   sprintf  (x_cmd, "mkdir %s   > /dev/null", DIR_UNIT);
   system   (x_cmd);
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_quiet   (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_argc      = 1;
   char       *x_argv [1]  = { "hestia" };
   /*---(run)----------------------------*/
   yURG_logger    (x_argc, x_argv);
   yURG_urgs      (x_argc, x_argv);
   PROG_init      ();
   PROG_args      (x_argc, x_argv);
   PROG_begin     ();
   my.user_mode = MODE_UNIT;
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_loud    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_argc      = 4;
   char       *x_argv [4]  = { "hestia_unit", "@@kitchen", "@@yparse", "@@yexec"  };
   /*---(run)----------------------------*/
   yURG_logger    (x_argc, x_argv);
   yURG_urgs      (x_argc, x_argv);
   PROG_init      ();
   PROG_args      (x_argc, x_argv);
   PROG_begin     ();
   my.user_mode = MODE_UNIT;
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
prog__unit_end     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_cmd       [LEN_RECD];
   /*---(run)----------------------------*/
   PROG_end       ();
   /*---(wipe)---------------------------*/
   chdir    ("/tmp");
   sprintf  (x_cmd, "rm -fr %s* > /dev/null", DIR_UNIT);
   system   (x_cmd);
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*            /*--> unit test accessor ------------------------------*/
prog__unit              (char *a_question, int n)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         c           =    0;
   char        x_recd      [LEN_RECD]  = "";
   char        t           [LEN_DESC]  = "";
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "PROG             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "conf"          )  == 0) {
      sprintf (t, "%2d[%.20s]", strlen (my.n_conf), my.n_conf);
      c = yEXEC_file_verify (my.n_conf, n, x_recd);
      snprintf (unit_answer, LEN_RECD, "PROG conf        : %3d  %-24.24s  %2d[%.45s]", c, t, strlen (x_recd), x_recd);
   }
   else if (strcmp (a_question, "speed"         )  == 0) {
      snprintf (unit_answer, LEN_RECD, "PROG speed       : %3d wait", my.wait_dsec);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

