/*===============================[[ beg-code ]]===============================*/
#include    "hestia.h"


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
tPOLL       g_polls     [MAX_TTYS];
tPOLL       test_poll   [1];

/*====================------------------------------------====================*/
/*===----                           drivers                            ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVERS_________________o (void) {;}

long         /*--> set the cron times ----------------------------------------*/
exec_time               (long a_now)
{
   /*---(locals)-----------+-----+-----+-*/
   tTIME      *x_broke     = NULL;
   char        t           [LEN_LABEL];
   /*---(set time)-----------------------*/
   if (a_now > 0)  my.now  = a_now;
   else            my.now  = time (NULL);
   /*---(break it down)------------------*/
   x_broke   = localtime (&my.now);
   /*---(heartbeat)----------------------*/
   strftime (t, 20, "%y.%m.%d.%H.%M.%S", x_broke);
   sprintf  (my.heartbeat, "%s  %-10d  now  %d", t, my.now, my.pid);
   rptg_heartbeat ();
   /*---(complete------------------------*/
   return my.now;
}

int
exec_check              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_return    =    0;
   int         c           =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(walk thru ttys)-----------------*/
   for (i = 0; i < MAX_TTYS; ++i) {
      DEBUG_LOOP   yLOG_value   ("tty#"      , i);
      /*---(filter)----------------------*/
      DEBUG_LOOP   yLOG_value   (".rpid"     , g_ttys  [i].rpid);
      if (g_ttys  [i].rpid <= 0)  continue;
      /*---(prepare)---------------------*/
      DEBUG_LOOP   yLOG_info    (".device"   , g_ttys [i].device);
      /*---(check)-----------------------*/
      rc = yEXEC_check (g_ttys [i].device, g_ttys [i].rpid, &x_return);
      DEBUG_LOOP   yLOG_value   ("check"     , rc);
      if (rc == YEXEC_RUNNING) {
         DEBUG_LOOP   yLOG_note    ("still running, next");
         continue;
      }
      ++g_ttys [i].complete;
      /*---(log results)-----------------*/
      switch (rc) {
      case YEXEC_ERROR   :
         DEBUG_LOOP  yLOG_note    ("error, failed to launch");
         --g_ttys [i].complete;
         ++g_ttys [i].errors;
         break;
      case YEXEC_KILLED  :
         DEBUG_LOOP  yLOG_note    ("terminated early");
         ++g_ttys [i].kills;
         break;
      case YEXEC_NORMAL  :
         DEBUG_LOOP  yLOG_note    ("normal ending");
         break;
      case YEXEC_WARNING :
         DEBUG_LOOP  yLOG_note    ("ended ok, but with warning");
         break;
      case YEXEC_FAILURE :
         DEBUG_LOOP  yLOG_note    ("ended badly");
         ++g_ttys [i].failures;
         break;
      }
      /*---(re-open)------------------------*/
      rc = tty_open     (i);
      DEBUG_LOOP   yLOG_value   ("re-open"   , rc);
      rc = tty_display  (i);
      DEBUG_LOOP   yLOG_value   ("display"   , rc);
      /*---(reset run data)--------------*/
      g_ttys [i].rpid   = -1;
      g_ttys [i].active = TTY_UNUSED;
      ++c;
      DEBUG_LOOP  yLOG_note    ("collected, next");
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return c;
}

char
exec_poll               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   int         x_stdin     = 0;
   int         x_stdout    = 0;
   int         x_stderr    = 0;
   char        x_cmd       [LEN_RECD];
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter   (__FUNCTION__);
   /*---(polling)------------------------*/
   rc = poll (g_polls, MAX_TTYS, 1);
   DEBUG_LOOP   yLOG_value   ("poll"      , rc);
   --rce;  if (rc <  0) {
      DEBUG_LOOP   yLOG_note    ("poll call failed");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (rc ==  0) {
      DEBUG_LOOP   yLOG_note    ("no ttys registered input available");
      DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(walk thru ttys)-----------------*/
   for (i = 0; i < MAX_TTYS; ++i) {
      DEBUG_LOOP   yLOG_complex ("tty#"      , "%2d, %-12.12s, %2d=%2d %2d %2d, %c %c %c %c", i, g_ttys [i].device, g_ttys [i].fd, g_polls [i].fd, g_polls [i].events, g_polls [i].revents, g_ttys [i].valid, g_ttys [i].allowed, g_ttys [i].watched, g_ttys [i].active);
      /*---(check events)----------------*/
      switch (g_polls [i].revents) {
      case    0     :
         DEBUG_LOOP   yLOG_note    ("no events on this tty");
         continue;
         break;
      case POLLNVAL :
         DEBUG_LOOP   yLOG_note    ("fd is not open");
         continue;
         break;
      case POLLHUP  :
         DEBUG_LOOP   yLOG_note    ("requested read when write end is closed");
         continue;
         break;
      case POLLERR  :
         DEBUG_LOOP   yLOG_note    ("requested write when read end is closed");
         continue;
         break;
      }
      /*---(filter)----------------------*/
      if (g_ttys  [i].allowed  != TTY_ALLOWED)  continue;
      if (g_ttys  [i].watched  != TTY_WATCHED)  continue;
      if (g_ttys  [i].active   == TTY_ACTIVE)   continue;
      /*---(prepare)---------------------*/
      g_ttys [i].active  = TTY_ACTIVE;
      ++g_ttys [i].attempts;
      /*---(save stdin,out,err)----------*/
      /*> DEBUG_LOOP   yLOG_note    ("save off stdin, stdout, stderr");               <*/
      /*> dup2 (0, x_stdin);                                                          <* 
       *> dup2 (1, x_stdout);                                                         <* 
       *> dup2 (2, x_stderr);                                                         <*/
      /*---(set to device)---------------*/
      /*> DEBUG_LOOP   yLOG_note    ("set stdin, stdout, stderr to device");          <*/
      /*> dup2 (g_polls [i].fd, 0);                                                   <* 
       *> dup2 (g_polls [i].fd, 1);                                                   <* 
       *> dup2 (g_polls [i].fd, 2);                                                   <*/
      tty_close (i);
      /*---(launch)----------------------*/
      /*> DEBUG_LOOP  yLOG_value   ("input"     , fgetc (stdin));                     <*/
      if (my.user_mode == MODE_DAEMON) {
         sprintf (x_cmd, "/sbin/hearth_debug @@kitchen --language %d --cluster %d --host %d %s", g_ttys[i].language, g_ttys[i].cluster, g_ttys [i].host, g_ttys[i].device);
         /*> sprintf (x_cmd, "/sbin/hearth --language %d --cluster %d --host %d %s", g_ttys[i].language, g_ttys[i].cluster, g_ttys [i].host, g_ttys[i].device);   <*/
      } else {
         sprintf (x_cmd, "/bin/sleep %d", i / 10);
      }
      DEBUG_LOOP   yLOG_info    ("x_cmd"     , x_cmd);
      g_ttys [i].rpid = yEXEC_run (g_ttys [i].device, "root", x_cmd, YEXEC_BASH, YEXEC_FULL, YEXEC_FORK, EXEC_FILE);
      DEBUG_LOOP   yLOG_value   (".rpid"     , g_ttys [i].rpid);
      if (g_ttys [i].rpid < 0) {
         DEBUG_LOOP   yLOG_note    ("LAUNCH FAILED");
      }
      /*---(restore stdin,out,err)-------*/
      /*> DEBUG_LOOP   yLOG_note    ("restore stdin, stdout, stderr");                <*/
      /*> dup2 (x_stdin , 0);                                                         <* 
       *> dup2 (x_stdout, 1);                                                         <* 
       *> dup2 (x_stderr, 2);                                                         <*/
      /*---(wrap)------------------------*/
      DEBUG_LOOP   yLOG_note    ("launch complete");
      ++c;
      /*---(done)------------------------*/
   }
   /*---(summary)------------------------*/
   DEBUG_LOOP   yLOG_value   ("c"         , c);
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
   return c;
}

char
exec_loop              (void)
{
   char        rc          =    0;
   while (1) {
      rc = tty_review ();
      if (rc < 0)  return rc;
      rc = exec_check ();
      if (rc < 0)  return rc;
      rc = exec_poll  ();
      if (rc < 0)  return rc;
      sleep (1);
   }
}


/*> char                                                                                                                                                <* 
 *> poller             (void)                                                                                                                           <* 
 *> {                                                                                                                                                   <* 
 *>    /+---(locals)-----------+-----------+-+/                                                                                                         <* 
 *>    int         rcc         = 0;                                                                                                                     <* 
 *>    int         rci         = 0;                                                                                                                     <* 
 *>    int         i;                                                                                                                                   <* 
 *>    int         count       = 0;                                                                                                                     <* 
 *>    char        printed     = 0;                                                                                                                     <* 
 *>    int         x_stdin     = 0;                                                                                                                     <* 
 *>    int         x_stdout    = 0;                                                                                                                     <* 
 *>    int         x_stderr    = 0;                                                                                                                     <* 
 *>    char        x_cmd       [200];                                                                                                                   <* 
 *>    FILE       *fs          = NULL;                                                                                                                  <* 
 *>    int         x_return    =    0;                                                                                                                  <* 
 *>    /+---(begin)--------------------------+/                                                                                                         <* 
 *>    DEBUG_LOOP   yLOG_enter   (__FUNCTION__);                                                                                                        <* 
 *>    /+---(polling)------------------------+/                                                                                                         <* 
 *>    while (1) {                                                                                                                                      <* 
 *>       /+---(check for input)-------------+/                                                                                                         <* 
 *>       yLOG_value  ("cycle"     , count);                                                                                                            <* 
 *>       /+> rci = poll (g_polls, ntty, 1);                                              <+/                                                           <* 
 *>       if (rci <  0) {                                                                                                                               <* 
 *>          yLOG_info   ("poll"      , "WARN , got an error");                                                                                         <* 
 *>       }                                                                                                                                             <* 
 *>       if (rci >  0) {                                                                                                                               <* 
 *>          for (i = 0; i < MAX_TTYS; ++i) {                                                                                                           <* 
 *>             if (g_polls[i].revents  != 0   &&                                                                                                       <* 
 *>                   g_ttys [i].allowed  == TTY_ALLOWED &&                                                                                             <* 
 *>                   g_ttys [i].watched  == TTY_WATCHED &&                                                                                             <* 
 *>                   g_ttys [i].active   != TTY_ACTIVE) {                                                                                              <* 
 *>                yLOG_info   ("tty"       , g_ttys [i].device);                                                                                       <* 
 *>                g_ttys [i].active  = TTY_ACTIVE;                                                                                                     <* 
 *>                ++g_ttys [i].attempts;                                                                                                               <* 
 *>                /+> write(g_polls[i].fd,"\033c",2);		                                  <+/                                                           <* 
 *>                dup2 (0             , x_stdin);                                                                                                      <* 
 *>                dup2 (1             , x_stdout);                                                                                                     <* 
 *>                dup2 (2             , x_stderr);                                                                                                     <* 
 *>                dup2 (g_polls [i].fd, 0   );                                                                                                         <* 
 *>                dup2 (g_polls [i].fd, 1   );                                                                                                         <* 
 *>                dup2 (g_polls [i].fd, 2   );                                                                                                         <* 
 *>                sprintf (x_cmd, "/sbin/hearth --external %-12.12s --cluster %d --hostname %s", g_ttys[i].full, rand() % 10, g_ttys [i].host_name);   <* 
 *>                g_ttys [i].rpid = yEXEC_run (EXEC_FILE, g_ttys [i].full, "root", x_cmd, YEXEC_BASH, YEXEC_FULL, YEXEC_FORK);                         <* 
 *>                dup2 (x_stdin       , 0   );                                                                                                         <* 
 *>                dup2 (x_stdout      , 1   );                                                                                                         <* 
 *>                dup2 (x_stderr      , 2   );                                                                                                         <* 
 *>             }                                                                                                                                       <* 
 *>          }                                                                                                                                          <* 
 *>       }                                                                                                                                             <* 
 *>       /+---(wait)------------------------+/                                                                                                         <* 
 *>       sleep (2);                                                                                                                                    <* 
 *>       /+---(check devices)---------------+/                                                                                                         <* 
 *>       /+> fs = fopen (TEMP_FILE, "w");                                                <*                                                            <* 
 *>        *> if (fs != NULL)  fprintf (fs, "# hestia (getty) device check file\n");      <*                                                            <* 
 *>        *> if (fs != NULL)  fprintf (fs, "# last updated at %s\n", my.heartbeat);      <*                                                            <* 
 *>        *> system ("lsof -R | grep /dev/tty >> /tmp/hestia_ttys");                     <*                                                            <* 
 *>        *> if (fs != NULL)  fclose (fs);                                               <+/                                                           <* 
 *>       /+---(update status)---------------+/                                                                                                         <* 
 *>       fs = fopen (HSTAT_FILE, "w");                                                                                                                 <* 
 *>       if (fs != NULL)  fprintf (fs, "# hestia (getty) summary file\n");                                                                             <* 
 *>       if (fs != NULL)  fprintf (fs, "# last updated at %s\n", my.heartbeat);                                                                        <* 
 *>       if (fs != NULL)  fprintf (fs, "#-name----  --long-name--------- valid watch activ  rpid  tries  compl  fails\n");                             <* 
 *>       /+---(walk through processes)------+/                                                                                                         <* 
 *>       for (i = 0; i < MAX_TTYS; ++i) {                                                                                                              <* 
 *>          if (g_ttys [i].rpid != 0) {                                                                                                                <* 
 *>             rci = yEXEC_check (g_ttys [i].full, g_ttys [i].rpid, &x_return);                                                                        <* 
 *>             rcc = rci;                                                                                                                              <* 
 *>             yLOG_value  ("check rci"  , rci);                                                                                                       <* 
 *>             yLOG_value  ("check rcc"  , rcc);                                                                                                       <* 
 *>             if (rcc == 0) {                                                                                                                         <* 
 *>                yLOG_info   ("completed" , g_ttys [i].full);                                                                                         <* 
*>                ++g_ttys [i].complete;                                                                                                               <* 
*>                g_ttys [i].active  = '-';                                                                                                            <* 
*>                g_ttys [i].rpid    =  0 ;                                                                                                            <* 
*>                close(g_ttys [i].full);                                                                                                              <* 
*>                tty_open (i);                                                                                                                        <* 
*>             }                                                                                                                                       <* 
*>             if (rcc <  0) {                                                                                                                         <* 
   *>                yLOG_info   ("failed"    , g_ttys [i].full);                                                                                         <* 
      *>                ++g_ttys [i].failure;                                                                                                                <* 
      *>                g_ttys [i].active  = TTY_UNUSED;                                                                                                     <* 
      *>                g_ttys [i].rpid    =  0 ;                                                                                                            <* 
      *>                close(g_ttys [i].full);                                                                                                              <* 
      *>                tty_open (i);                                                                                                                        <* 
      *>             }                                                                                                                                       <* 
      *>          } else if (g_ttys [i].watched == 'B') {                                                                                                    <* 
         *>             rcc = tty_parse (g_ttys [i].full);                                                                                                      <* 
            *>             if (rcc == -1) {                                                                                                                        <* 
               *>                g_ttys [i].watched = TTY_WATCHED;                                                                                                    <* 
                  *>                g_ttys [i].active  = TTY_UNUSED;                                                                                                     <* 
                  *>                g_ttys [i].rpid    =  0 ;                                                                                                            <* 
                  *>                tty_open (i);                                                                                                                        <* 
                  *>             }                                                                                                                                       <* 
                  *>          }                                                                                                                                          <* 
                  *>          if (fs != NULL) {                                                                                                                          <* 
                     *>             fprintf (fs, "%-10.10s  %-20.20s   %c     %c     %c   %5d  %5d  %5d  %5d\n",                                                            <* 
                           *>                   g_ttys [i].name, g_ttys [i].full,                                                                                                 <* 
                           *>                   g_ttys [i].allowed, g_ttys [i].watched, g_ttys [i].active,                                                                        <* 
                           *>                   g_ttys [i].rpid, g_ttys [i].attempts, g_ttys [i].complete, g_ttys [i].failure);                                                   <* 
                        *>          }                                                                                                                                          <* 
                        *>       }                                                                                                                                             <* 
                        *>       if (fs != NULL)  fclose (fs);                                                                                                                 <* 
                        *>       /+---(trim log)--------------------+/                                                                                                         <* 
                        *>       if ((count %  30) == 0)  yLOG_sync();                                                                                                         <* 
                        *>       /+---(prepare for next cycle)------+/                                                                                                         <* 
                        *>       ++count;                                                                                                                                      <* 
                        *>    }                                                                                                                                                <* 
                        *>    /+---(complete)-----------------------+/                                                                                                         <* 
                        *>    DEBUG_LOOP   yLOG_exit    (__FUNCTION__);                                                                                                        <* 
                        *>    return   0;                                                                                                                                      <* 
                        *> }                                                                                                                                                   <*/




                        /*====================------------------------------------====================*/
                        /*===----                      unit test accessor                      ----===*/
                        /*====================------------------------------------====================*/
                        static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
exec__unit              (char *a_question, int a_num)
{
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "EXEC             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "time"    )        == 0) {
      snprintf (unit_answer, LEN_RECD, "EXEC time        : %-10d", my.now);
   }
   else if (strcmp (a_question, "pid"     )        == 0) {
      snprintf (unit_answer, LEN_RECD, "EXEC pid         : %d", my.pid);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char
exec__unit_ping         (int a_tty)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   int         x_fd        =   -1;
   char        x_msg       [LEN_RECD];
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*> sprintf (x_msg, "echo \"ping\n\" > /dev/tty%d", a_tty);                        <* 
    *> system (x_msg);                                                                <*/
   /*---(open)---------------------------*/
   f = fopen (g_ttys [a_tty].device, "rt");
   DEBUG_LOOP   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strcpy (x_msg, "ping\n");
   for (i = strlen (x_msg) - 1; i >= 0; --i) {
      ungetc (x_msg [i], f);
   }
   /*> x_fd = open (g_ttys [a_tty].device, O_WRONLY | O_NOCTTY);                      <* 
    *> DEBUG_LOOP   yLOG_value   ("x_fd"      , x_fd);                                <* 
    *> --rce;  if (x_fd <  0) {                                                       <* 
    *>    DEBUG_LOOP   yLOG_note    ("can not get control/open device");              <* 
    *>    DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(clear the screen)---------------*/
   /*> DEBUG_LOOP   yLOG_note    ("clear and write login message");                   <*/
   /*> sprintf (x_msg, "ping");                                                       <* 
    *> write   (x_fd, x_msg, strlen (x_msg));                                         <*/
   /*> fprintf (f, "ping\n");                                                         <*/
   /*> sprintf (x_msg, "p");                                                          <* 
    *> write (g_ttys [a_tty].fd, x_msg, strlen (x_msg));                              <*/
   /*---(close)--------------------------*/
   /*> rc = close (x_fd);                                                             <*/
   rc = fclose (f);
   DEBUG_LOOP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*> int         x_stdin     = 0;                                                   <* 
    *> int         x_stdout    = 0;                                                   <* 
    *> int         x_stderr    = 0;                                                   <* 
    *> /+---(save stdin,out,err)----------+/                                          <* 
    *> DEBUG_LOOP   yLOG_note    ("save off stdin, stdout, stderr");                  <* 
    *> dup2 (0, x_stdin);                                                             <* 
    *> dup2 (1, x_stdout);                                                            <* 
    *> dup2 (2, x_stderr);                                                            <* 
    *> /+---(set to device)---------------+/                                          <* 
    *> DEBUG_LOOP   yLOG_note    ("set stdin, stdout, stderr to device");             <* 
    *> dup2 (g_polls [a_tty].fd, 0);                                                  <* 
    *> dup2 (g_polls [a_tty].fd, 1);                                                  <* 
    *> dup2 (g_polls [a_tty].fd, 2);                                                  <* 
    *> /+---(launch)----------------------+/                                          <* 
    *> printf ("ping\n");                                                             <* 
    *> /+---(restore stdin,out,err)-------+/                                          <* 
    *> DEBUG_LOOP   yLOG_note    ("restore stdin, stdout, stderr");                   <* 
    *> dup2 (x_stdin , 0);                                                            <* 
    *> dup2 (x_stdout, 1);                                                            <* 
    *> dup2 (x_stderr, 2);                                                            <*/
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return 0;
}

