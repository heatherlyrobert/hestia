/*===============================[[ beg-code ]]===============================*/
#include    "hestia.h"


tTTY        g_ttys      [MAX_TTYS];
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        s_active    [LEN_HUND] = "------------------------------------------------------------------------------------------";



/*====================------------------------------------====================*/
/*===----                              tty                             ----===*/
/*====================------------------------------------====================*/
static void      o___TTY_____________________o (void) {;}

char         /*--> validate a tty device file -------[ ------ [abc.de.fghijk]-*/
tty_valid               (char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSTAT       s;
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_info    ("a_name"    , a_name);
   /*---(check device file)--------------*/
   rc = stat (a_name, &s);
   DEBUG_LOOP   yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_LOOP   yLOG_note    ("can not find device file");
      DEBUG_LOOP   yLOG_exitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (S_ISDIR (s.st_mode))  {
      DEBUG_LOOP   yLOG_note    ("can not use a directory");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (S_ISLNK (s.st_mode))  {
      DEBUG_LOOP   yLOG_note    ("can not use a symlink");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (S_ISREG (s.st_mode))  {
      DEBUG_LOOP   yLOG_note    ("can not use regular files");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (!S_ISCHR (s.st_mode))  {
      DEBUG_LOOP   yLOG_note    ("can only use character devices");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(confirmation)-------------------*/
   DEBUG_LOOP   yLOG_note   ("successfully found character device file");
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> initialize the tty data ----------[ ------ [abc.de.fghijk]-*/
tty_init                (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(initialize ttys)----------------*/
   DEBUG_PROG   yLOG_value   ("MAX_TTY"   , MAX_TTYS);
   for (i = 0; i < MAX_TTYS; ++i) {
      DEBUG_PROG   yLOG_value   ("tty#"      , i);
      /*---(device)----------------------*/
      sprintf (g_ttys [i].name  , "tty%d"     , i);
      sprintf (g_ttys [i].device, "/dev/tty%d", i);
      /*---(host)------------------------*/
      g_ttys [i].language = ySTR_language ();
      g_ttys [i].cluster  = ySTR_cluster  ();
      g_ttys [i].host     = ySTR_host     ();
      /*---(flags)-----------------------*/
      g_ttys [i].valid    = TTY_INVALID;
      g_ttys [i].allowed  = TTY_BLOCKED;
      g_ttys [i].watched  = TTY_IGNORED;
      g_ttys [i].active   = TTY_UNUSED;
      /*---(working)---------------------*/
      g_ttys [i].style    = TTY_UNUSED;
      g_ttys [i].fd       = -1;
      g_ttys [i].rpid     = -1;
      /*---(stats)-----------------------*/
      g_ttys [i].attempts = 0;
      g_ttys [i].complete = 0;
      g_ttys [i].failures = 0;
      /*---(validate)--------------------*/
      rc = tty_valid (g_ttys [i].device);
      if (rc >= 0)   g_ttys [i].valid    = TTY_VALID;
      /*> DEBUG_PROG   yLOG_complex ("g_ttys"    , "%2d %-12.12s %c %c %c %c", i, g_ttys [i].device, g_ttys [i].valid, g_ttys [i].allowed, g_ttys [i].watched, g_ttys [i].active);   <*/
      /*---(polling)---------------------*/
      g_polls [i].fd      = -1;
      g_polls [i].events  = POLLIN;
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> shutdown the tty data ------------[ ------ [abc.de.fghijk]-*/
tty_wrap                (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(initialize ttys)----------------*/
   DEBUG_PROG   yLOG_value   ("MAX_TTY"   , MAX_TTYS);
   for (i = 0; i < MAX_TTYS; ++i) {
      DEBUG_PROG   yLOG_value   ("tty#"      , i);
      DEBUG_PROG   yLOG_value   (".fd"       , g_ttys [i].fd);
      if (g_ttys [i].fd <= 0)  continue;
      tty_close (i);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> update and open a tty device -----[ ------ [abc.de.fghijk]-*/
tty_open                (int a_tty)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_fd        =   -1;
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_value   ("a_tty"     , a_tty);
   DEBUG_PROG   yLOG_value   ("MAX_TTY"   , MAX_TTYS);
   --rce;  if (a_tty < 0 || a_tty >= MAX_TTYS) {
      DEBUG_LOOP   yLOG_note    ("a_tty must be 0 to MAX_TTYS");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_info    (".device"   , g_ttys [a_tty].device);
   /*---(check if watched)---------------*/
   DEBUG_LOOP   yLOG_char    (".allowed"  , g_ttys [a_tty].allowed);
   --rce;  if (g_ttys [a_tty].allowed != TTY_ALLOWED) {
      DEBUG_LOOP   yLOG_note    ("a_tty is not allowed to be used");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check if already active)--------*/
   DEBUG_LOOP   yLOG_char    (".active"   , g_ttys [a_tty].active);
   --rce;  if (g_ttys [a_tty].active != TTY_UNUSED) {
      DEBUG_LOOP   yLOG_note    ("a_tty is not unused (can not use)");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check existing status)----------*/
   DEBUG_LOOP   yLOG_value   (".fd"       , g_ttys [a_tty].fd);
   --rce;  if (g_ttys [a_tty].fd >= 0) {
      DEBUG_LOOP   yLOG_note    ("a_tty already open (nothing to do)");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(change ownership)---------------*/
   rc = chown (g_ttys [a_tty].device, 0, 0);
   DEBUG_LOOP   yLOG_value   ("chown"     , rc);
   --rce;  if (rc != 0) {
      DEBUG_LOOP   yLOG_note    ("can not change ownership to root only");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(change permissions)-------------*/
   rc = chmod (g_ttys [a_tty].device, 0600);
   DEBUG_LOOP   yLOG_value   ("chmod"     , rc);
   --rce;  if (rc != 0) {
      DEBUG_LOOP   yLOG_note    ("can not change permissions to 0600");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   x_fd = open (g_ttys [a_tty].device, O_RDWR | O_NOCTTY | O_NONBLOCK);
   DEBUG_LOOP   yLOG_value   ("x_fd"      , x_fd);
   --rce;  if (x_fd <  0) {
      DEBUG_LOOP   yLOG_note    ("can not get control/open device");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(test terminal)------------------*/
   DEBUG_LOOP   yLOG_value   ("isatty"    , isatty (x_fd));
   --rce;  if (!isatty (x_fd)) {
      DEBUG_LOOP   yLOG_note    ("not a terminal device");
      close (x_fd);
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save file descriptor)-----------*/
   g_ttys  [a_tty].fd      = x_fd;
   g_polls [a_tty].fd      = x_fd;
   g_ttys  [a_tty].watched = TTY_WATCHED;
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
   return x_fd;
}

char         /*--> close a tty device ---------------[ ------ [abc.de.fghijk]-*/
tty_close               (int a_tty)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_value   ("a_tty"     , a_tty);
   DEBUG_PROG   yLOG_value   ("MAX_TTY"   , MAX_TTYS);
   --rce;  if (a_tty < 0 || a_tty >= MAX_TTYS) {
      DEBUG_LOOP   yLOG_note    ("a_tty must be 0 to MAX_TTYS");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_info    (".device"   , g_ttys [a_tty].device);
   /*---(check if watched)---------------*/
   DEBUG_LOOP   yLOG_char    (".watched"  , g_ttys [a_tty].watched);
   --rce;  if (g_ttys [a_tty].watched != TTY_WATCHED) {
      DEBUG_LOOP   yLOG_note    ("a_tty is not watched (nothing to do)");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check existing status)----------*/
   DEBUG_LOOP   yLOG_value   (".fd"       , g_ttys [a_tty].fd);
   --rce;  if (g_ttys [a_tty].fd < 0) {
      DEBUG_LOOP   yLOG_note    ("a_tty not open (nothing to do)");
      DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if (g_ttys [a_tty].fd < 3) {
      DEBUG_LOOP   yLOG_note    ("reserved for stdin, strout, stderr");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set terminal back)--------------*/
   rc = tcsetattr (g_ttys [a_tty].fd, TCSANOW, &(g_ttys [a_tty].original));
   DEBUG_LOOP   yLOG_value   ("setattr"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = close (g_ttys [a_tty].fd);
   DEBUG_LOOP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save file descriptor)-----------*/
   g_ttys  [a_tty].fd = -1;
   g_polls [a_tty].fd = -1;
   g_ttys  [a_tty].watched = TTY_IGNORED;
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> validate a tty device file -------[ ------ [abc.de.fghijk]-*/
tty_display             (int a_tty)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char        x_base      [LEN_LABEL];
   int         x_len       =    0;
   char        x_prompt    [LEN_HUND];
   int         c           =    0;
   tTERMIOS    x_termios;
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_value   ("a_tty"     , a_tty);
   DEBUG_PROG   yLOG_value   ("MAX_TTY"   , MAX_TTYS);
   --rce;  if (a_tty < 0 || a_tty >= MAX_TTYS) {
      DEBUG_LOOP   yLOG_note    ("a_tty must be 0 to MAX_TTYS");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_info    (".device"   , g_ttys [a_tty].device);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_value   (".fd"       , g_ttys [a_tty].fd);
   --rce;  if (g_ttys [a_tty].fd <= 2) {
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save current settings)----------*/
   rc = tcgetattr (g_ttys [a_tty].fd, &(g_ttys [a_tty].original));
   DEBUG_LOOP   yLOG_value   ("getattr"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*> fcntl (fd, F_SETFL, 0);                                                                    <* 
    *> /+---(get current term settings)------+/                                                   <* 
    *> tcgetattr(fd, &s_oldtio);                                                                  <* 
    *> /+---(prepare new one)----------------+/                                                   <* 
    *> bzero(&s_newtio, sizeof (tTERMIOS));                                                       <* 
    *> /+> s_newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;                               <+/   <* 
    *> s_newtio.c_cflag = B2400 | CS8 | CLOCAL | CREAD;                                           <* 
    *> cfsetispeed(&s_newtio, B115200);                                                           <* 
    *> /+> cfsetispeed(&s_newtio, B2400);                                                   <+/   <* 
    *> cfsetospeed(&s_newtio, B115200);                                                           <* 
    *> /+> cfsetospeed(&s_newtio, B2400);                                                   <+/   <* 
    *> /+> s_newtio.c_cflag |= (CLOCAL | CREAD);                                            <*    <* 
    *>  *> s_newtio.c_cflag &= ~CSIZE;                                                      <*    <* 
    *>  *> s_newtio.c_cflag &= ~PARENB;                                                     <*    <* 
    *>  *> s_newtio.c_cflag &= ~CSTOPB;                                                     <*    <* 
    *>  *> s_newtio.c_cflag |= CS8;                                                         <+/   <* 
    *> s_newtio.c_iflag  = IGNPAR  | ICRNL   ;                                                    <* 
    *> s_newtio.c_oflag  = 0       ;                                                              <* 
    *> s_newtio.c_lflag  = ICANON  ;                                                              <* 
    *> tcflush   (fd, TCIFLUSH);                                                                  <* 
    *> tcsetattr (fd, TCSANOW, &s_newtio);                                                        <*/
   /*---(clear existing in/out)----------*/
   rc = tcflush (g_ttys [a_tty].fd, TCIOFLUSH);  /* flush both input/output */
   DEBUG_LOOP   yLOG_value   ("flush"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear the screen)---------------*/
   DEBUG_LOOP   yLOG_value   ("langugage" , g_ttys [a_tty].language);
   DEBUG_LOOP   yLOG_value   ("cluster"   , g_ttys [a_tty].cluster);
   DEBUG_LOOP   yLOG_value   ("host"      , g_ttys [a_tty].host);
   rc = ySTR_prompt (YSTR_BREADCRUMB, g_ttys [a_tty].language, g_ttys [a_tty].cluster, g_ttys [a_tty].host, x_prompt, NULL);
   DEBUG_LOOP   yLOG_value   ("prompt"    , rc);
   DEBUG_LOOP   yLOG_info    ("x_prompt"  , x_prompt);
	write (g_ttys [a_tty].fd, "\033[r\033[H\033[J", 9);
   write (g_ttys [a_tty].fd, x_prompt, strlen (x_prompt));
   write (g_ttys [a_tty].fd," " , 1);
   /*---(log the openning)---------------*/
   rc = ySEC_getty_on  (g_ttys [a_tty].name);
   DEBUG_LOOP   yLOG_value   ("getty_on"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] attempt to open all ttys ------------------------*/
tty_openall        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   int         i           =    0;
   int         c           =    0;
   /*---(begin)--------------------------*/
   DEBUG_LOOP   yLOG_enter  (__FUNCTION__);
   /*---(loop through ttys)--------------*/
   for (i = 0; i < MAX_TTYS; ++i) {
      /*---(display)---------------------*/
      DEBUG_LOOP   yLOG_value   ("index"     , i);
      DEBUG_LOOP   yLOG_info    ("->name"    , g_ttys [i].device);
      /*---(filter)----------------------*/
      DEBUG_LOOP   yLOG_char    ("->allowed" , g_ttys [i].allowed);
      if (g_ttys [i].allowed != TTY_ALLOWED) {
         DEBUG_LOOP   yLOG_note    ("not allowed by hestia, skipping");
         continue;
      }
      DEBUG_LOOP   yLOG_char    ("->active"  , g_ttys [i].active);
      if (g_ttys [i].active  == TTY_ACTIVE) {
         DEBUG_LOOP   yLOG_note    ("already logged in, skipping");
         continue;
      }
      /*---(open)------------------------*/
      rc = tty_open    (i);
      DEBUG_LOOP   yLOG_value   ("open"      , rc);
      if (rc < 0) {
         DEBUG_LOOP   yLOG_note    ("could not prepare tty for use");
         continue;
      }
      /*---(display)---------------------*/
      rc = tty_display (i);
      DEBUG_LOOP   yLOG_value   ("open"      , rc);
      if (rc < 0) {
         DEBUG_LOOP   yLOG_note    ("could not prepare tty for use");
         continue;
      }
      ++c;
      /*---(done)------------------------*/
   }
   DEBUG_LOOP   yLOG_value   ("c"         , c);
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit   (__FUNCTION__);
   return c;
}

char             /* [------] find a running job by name ----------------------*/
tty_review              (void)
{
   /*---(locals)-----------+-----------+-*/
   DIR        *x_procdir   = NULL;
   tDIRENT    *x_procden   = NULL;
   int         x_procnum   =    0;
   char        x_fdname    [LEN_RECD];
   DIR        *x_fddir     = NULL;
   tDIRENT    *x_fdden     = NULL;
   int         x_fdnum     =    0;
   char        x_ttyname   [LEN_RECD];
   char        x_tty       [LEN_RECD];
   int         x_ttynum    =    0;
   int         x_len       =    0;
   char        x_statname  [LEN_RECD];
   FILE       *f           = NULL;
   char        x_line      [LEN_RECD];
   char       *p           = NULL;
   char       *q           = NULL;
   char        x_prog      [LEN_RECD];
   char        c           =    0;
   char        i           =    0;
   /*---(begin)--------------------------*/
   DEBUG_LOOP   yLOG_enter  (__FUNCTION__);
   /*---(preprare)-----------------------*/
   for (i = 0; i < MAX_TTYS; ++i) {
      s_active [i] = '-';
      if (g_ttys [i].active == 'o')  g_ttys [i].active = TTY_UNUSED;
   }
   /*---(open the proc system)-----------*/
   x_procdir = opendir ("/proc");
   /*> DEBUG_LOOP   yLOG_point  ("x_procdir" , x_procdir);                            <*/
   if (x_procdir == NULL) {
      DEBUG_LOOP   yLOG_exitr  (__FUNCTION__, -1);
      return -1;
   }
   /*---(cycle through the entries)------*/
   while (1)  {
      /*---(next process)----------------*/
      x_procden = readdir (x_procdir);
      /*> DEBUG_LOOP   yLOG_point  ("x_procden" , x_procden);                         <*/
      if (x_procden == NULL)  break;
      /*> DEBUG_LOOP   yLOG_info   ("->d_name"  , x_procden->d_name);                 <*/
      x_procnum = atoi (x_procden->d_name);
      /*> DEBUG_LOOP   yLOG_value  ("x_procnum" , x_procnum);                         <*/
      if (x_procnum == 0)     continue;
      /*---(open fds)--------------------*/
      sprintf (x_fdname, "/proc/%s/fd", x_procden->d_name);
      /*> DEBUG_LOOP   yLOG_info   ("x_fdname"  , x_fdname);                          <*/
      x_fddir = opendir (x_fdname);
      /*> DEBUG_LOOP   yLOG_point  ("x_fddir"   , x_fddir);                           <*/
      if (x_fddir == NULL)    continue;
      /*> DEBUG_LOOP   yLOG_enter  (__FUNCTION__);                                    <*/
      while (1)  {
         /*---(next fd)------------------*/
         x_fdden = readdir (x_fddir);
         /*> DEBUG_LOOP   yLOG_point  ("x_fdden"   , x_fdden);                        <*/
         if (x_fdden == NULL)    break;
         /*> DEBUG_LOOP   yLOG_info   ("->d_name"  , x_fdden->d_name);                <*/
         x_fdnum = atoi (x_fdden->d_name);
         /*> DEBUG_LOOP   yLOG_value  ("x_fdnum"   , x_fdnum);                        <*/
         if (x_fdnum < 3)        continue;
         /*---(get link destination)-----*/
         sprintf (x_ttyname, "/proc/%s/fd/%s", x_procden->d_name, x_fdden->d_name);
         /*> DEBUG_LOOP   yLOG_info   ("x_ttyname" , x_ttyname);                      <*/
         realpath (x_ttyname, x_tty);
         x_len = strlen (x_tty);
         /*> DEBUG_LOOP   yLOG_info   ("x_tty"     , x_tty);                          <*/
         if (x_len <= 8)      continue;
         if (strncmp (x_tty, "/dev/tty", 8) != 0)  continue;
         x_ttynum = atoi (x_tty + 8);
         if (x_ttynum >= MAX_TTYS)   continue;
         /*---(get the name)-------------*/
         sprintf (x_statname, "/proc/%s/stat", x_procden->d_name);
         f = fopen (x_statname, "r");
         if (f == NULL) continue;
         fgets (x_line, 1000, f);
         p = strtok (x_line, "(");
         p = strtok (NULL, ")");
         strcpy (x_prog, p);
         fclose (f);
         /*---(verify)----------------------*/
         q = strchr (x_prog, ' ');
         if (q != NULL)  q [0] = '\0';
         /*---(display key data)---------*/
         DEBUG_PROG   yLOG_complex ("found"     , "%-12.12s  %-12.12s  %2d  %c %c %c %c", x_prog, g_ttys [x_ttynum].device, g_ttys [x_ttynum].fd, g_ttys [x_ttynum].valid, g_ttys [x_ttynum].allowed, g_ttys [x_ttynum].watched, g_ttys [x_ttynum].active);
         /*---(flag active)--------------*/
         ++c;
         s_active [x_ttynum] = TTY_ACTIVE;
         /*---(flag others)--------------*/
         if (g_ttys [x_ttynum].allowed == TTY_BLOCKED) {
            g_ttys [x_ttynum].active = TTY_OTHERS;
            /*> } else {                                                                 <* 
             *>    g_ttys [x_ttynum].active = TTY_ACTIVE;                                <*/
      }
      /*---(done)---------------------*/
      }
      /*> DEBUG_LOOP   yLOG_exit   (__FUNCTION__);                                    <*/
      closedir (x_fddir);
   }
   closedir (x_procdir);
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit   (__FUNCTION__);
   return c;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char          unit_answer [ LEN_RECD ];

char*            /*--> unit test accessor ------------------------------*/
tty__unit               (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        t           [LEN_RECD]  = "[]";
   int         c           =    0;
   tFILE      *x_file      = NULL;
   tSTAT       s;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "TTY              : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if (a_num < 0 || a_num >= MAX_TTYS) {
      snprintf (unit_answer, LEN_RECD, "TTY number  (%2d) : out of range", a_num);
      return unit_answer;
   }
   /*---(questions)----------------------*/
   if      (strcmp (a_question, "stats"   )        == 0) {
      /*---(check device file)--------------*/
      sprintf (t, g_ttys [a_num].device);
      rc = stat (t, &s);
      if (rc < 0) 
         snprintf (unit_answer, LEN_RECD, "TTY stats   (%2d) : %-15.15s  -     -     -  -------", a_num, t);
      else {
         snprintf (unit_answer, LEN_RECD, "TTY stats   (%2d) : %-15.15s  y  %4d  %4d  %7.7o", a_num, t, s.st_uid, s.st_gid, s.st_mode);
      }
   }
   else if (strcmp (a_question, "flags"   )        == 0) {
      /*---(check device file)--------------*/
      sprintf (t, g_ttys [a_num].device);
      rc = stat (t, &s);
      if (rc < 0) 
         snprintf (unit_answer, LEN_RECD, "TTY flags   (%2d) : %-15.15s  -  -  -  -  --  ------", a_num, t);
      else {
         snprintf (unit_answer, LEN_RECD, "TTY flags   (%2d) : %-15.15s  %c  %c  %c  %c  %2d  %6d", a_num, t, g_ttys [a_num].valid, g_ttys [a_num].allowed, g_ttys [a_num].watched, g_ttys [a_num].active, g_ttys [a_num].fd, g_ttys [a_num].rpid);
      }
   }
   else if (strcmp (a_question, "active"  )        == 0) {
      snprintf (unit_answer, LEN_RECD, "TTY active       : %-40.40s", s_active);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char tty__unit_allowed (int a_tty)  { g_ttys [a_tty].allowed = TTY_ALLOWED; return 0; }
char tty__unit_blocked (int a_tty)  { g_ttys [a_tty].allowed = TTY_BLOCKED; return 0; }

char tty__unit_watched (int a_tty)  { g_ttys [a_tty].watched = TTY_WATCHED; return 0; }
char tty__unit_ignored (int a_tty)  { g_ttys [a_tty].watched = TTY_IGNORED; return 0; }

char tty__unit_active  (int a_tty)  { g_ttys [a_tty].active  = TTY_ACTIVE;  return 0; }
char tty__unit_unused  (int a_tty)  { g_ttys [a_tty].active  = TTY_UNUSED;  return 0; }

