/*===============================[[ beg-code ]]===============================*/
#include    "hestia.h"


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
tPOLL       g_polls     [MAX_TTYS];


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
      /*---(reset run data)--------------*/
      g_ttys [i].rpid   = -1;
      g_ttys [i].active = TTY_UNUSED;
      /*> tty_close (i);                                                              <*/
      /*> tty_open  (i);                                                              <*/
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
   /*---(walk thru ttys)-----------------*/
   for (i = 0; i < MAX_TTYS; ++i) {
      DEBUG_LOOP   yLOG_value   ("tty#"      , i);
      /*---(check events)----------------*/
      DEBUG_LOOP   yLOG_value   (".revents"  , g_polls [i].revents);
      if (g_polls [i].revents  == 0)            continue;
      /*---(filter)----------------------*/
      DEBUG_LOOP   yLOG_char    (".allowed"  , g_ttys  [i].allowed);
      if (g_ttys  [i].allowed  != TTY_ALLOWED)  continue;
      DEBUG_LOOP   yLOG_char    (".watched"  , g_ttys  [i].watched);
      if (g_ttys  [i].watched  != TTY_WATCHED)  continue;
      DEBUG_LOOP   yLOG_char    (".active"   , g_ttys  [i].active);
      if (g_ttys  [i].active   == TTY_ACTIVE)   continue;
      /*---(prepare)---------------------*/
      DEBUG_LOOP   yLOG_info    (".device"   , g_ttys [i].device);
      g_ttys [i].active  = TTY_ACTIVE;
      ++g_ttys [i].attempts;
      /*---(save stdin,out,err)----------*/
      DEBUG_LOOP   yLOG_note    ("save off stdin, stdout, stderr");
      dup2 (0, x_stdin);
      dup2 (1, x_stdout);
      dup2 (2, x_stderr);
      /*---(set to device)---------------*/
      DEBUG_LOOP   yLOG_note    ("set stdin, stdout, stderr to device");
      dup2 (g_polls [i].fd, 0);
      dup2 (g_polls [i].fd, 1);
      dup2 (g_polls [i].fd, 2);
      /*---(launch)----------------------*/
      if (my.user_mode == MODE_DAEMON) {
         sprintf (x_cmd, "/sbin/hearth --external %-12.12s --cluster %d --hostname %s", g_ttys[i].device, g_ttys[i].cluster, g_ttys [i].host_name);
      } else {
         sprintf (x_cmd, "/bin/sleep 2");
      }
      DEBUG_LOOP   yLOG_info    ("x_cmd"     , x_cmd);
      g_ttys [i].rpid = yEXEC_run (g_ttys [i].device, "root", x_cmd, YEXEC_BASH, YEXEC_FULL, YEXEC_FORK, EXEC_FILE);
      DEBUG_LOOP   yLOG_value   (".rpid"     , g_ttys [i].rpid);
      if (g_ttys [i].rpid < 0) {
         DEBUG_LOOP   yLOG_note    ("LAUNCH FAILED");
      }
      /*---(restore stdin,out,err)-------*/
      DEBUG_LOOP   yLOG_note    ("restore stdin, stdout, stderr");
      dup2 (x_stdin , 0);
      dup2 (x_stdout, 1);
      dup2 (x_stderr, 2);
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
