/*===============================[[ beg-code ]]===============================*/
#include    "hestia.h"





int       logger   = -1;

FILE       *conf_file           = NULL;          /* configuration file decriptor   */

/* list of champion chess playing programs   */
char g_hosts [MAX_HOST][LEN_LABEL] = {
   "deep_blue",
   "blue_gene",
   "hydra",
   "deep_crack",
   "kaissa",
   "belle",
   "blitz",
   "deep_thought",
   "gideon",
   "deep_junior",
   "zappa",
   "junior",
   "sjeng",
   "rondo",
   "rybka",
   "fruit",
   "crafty",
   "mephisto",
   "jonny",
   "fute",
   "diep",
   "falcon",
   "rajlich",
   "spike",
   "ikarus",
   "chiron",
   "lion",
   "chaturanga",
   "uragano",
   "parsos",
   "etabeta",
   "loop",
   "the_baron",
   "the_king",
   "hiarcs",
   "cluster_toga",
   "equinox",
   "pandix",
   "joker",
   "darminios",
   "fridonlin",
   "hector",
   "criter",
   "houdini",
   "naum",
   "stockfish",
   "thinker",
   "komodo",
   "alaric",
   "booot",
   "bobcat",
   "cyrano",
   "colossus",
   "dirty",
   "frenzee",
   "glaurung",
   "gull",
   "hannibal",
   "naraku",
   "pharaon",
   "protector",
   "scorpio",
   "strelka",
   "the_mad_prune",
   "tornado",
   "twisted_logic",
   "umko",
   "wildcat",
   "crazy_bishop",
   "---end-of-list-----",
};
int        g_nhost  = 0;



/*====================------------------------------------====================*/
/*===----                            utility                           ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char*      /*l---: PURPOSE : clean whitespace from both sides of a string ----*/
str_trim           (char *a_cstring)
{
   /*---(defenses)-------------------------*/
   if (a_cstring == NULL) return NULL;
   /*---(locals)---------------------------*/
   int i, j;
   int len = 0;
   /*---(leading whitespace)---------------*/
   len = strlen(a_cstring);
   for (i = 0; i <= len; ++i) {
      if (a_cstring[0] != ' ') break;
      for (j = 0; j <= len; ++j)
         a_cstring[j] = a_cstring[j + 1];
      --len;
   }
   /*---(trailing whitespace)--------------*/
   len = strlen(a_cstring);
   for (i = len - 1; i >= 0; --i) {
      if (a_cstring[i] != ' ') break;
      a_cstring[i] = '\0';
   }
   /*---(complete)-------------------------*/
   return a_cstring;
}



/*====================------------------------------------====================*/
/*===----                         configuration                        ----===*/
/*====================------------------------------------====================*/
static void      o___CONF____________________o (void) {;}

char
base_config             (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_tty       =    0;
   char        x_status    [LEN_LABEL];
   char        x_style     [LEN_LABEL];
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(open file)----------------------*/
   DEBUG_INPT  yLOG_info    ("name_conf"  , my.name_conf);
   rc = yPARSE_open_in (my.name_conf);
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(lines)--------------------------*/
   while (rc >= 0) {
      /*---(read)------------------------*/
      rc = yPARSE_read (NULL, NULL);
      DEBUG_INPT   yLOG_value   ("yparse"    , rc);
      if (rc == 0) {
         DEBUG_INPT  yLOG_note    ("end-of-file");
         break;
      }
      if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(number)----------------------*/
      rc = yPARSE_popint  (&x_tty);
      DEBUG_INPT   yLOG_value   ("popint"    , rc);
      if (rc < 0) {
         DEBUG_INPT   yLOG_note    ("could not read tty number (must skip)");
         continue;
      }
      DEBUG_INPT   yLOG_value   ("x_tty"     , x_tty);
      /*---(status)----------------------*/
      rc = yPARSE_popstr  (&x_status);
      DEBUG_INPT   yLOG_value   ("popstr"    , rc);
      if (rc < 0) {
         DEBUG_INPT   yLOG_note    ("could not read status (default off)");
         strcpy (x_status, "off");

      }
      DEBUG_INPT   yLOG_info    ("x_status"  , x_status);
      /*---(style)-----------------------*/
      rc = yPARSE_popstr  (&x_style);
      DEBUG_INPT   yLOG_value   ("popstr"    , rc);
      if (rc < 0) {
         DEBUG_INPT   yLOG_note    ("could not read style (default complex)");
         strcpy (x_style, "complex");
      }
      DEBUG_INPT   yLOG_info    ("x_style"   , x_style);
      /*---(defense)---------------------*/
      DEBUG_INPT   yLOG_value   ("MAX_TTYS"  , MAX_TTYS);
      if (x_tty < 0 || x_tty >= MAX_TTYS) {
         DEBUG_INPT   yLOG_note    ("tty number not 0 to MAX_TTY");
         continue;
      }
      /*---(validity)--------------------*/
      if (strcmp (x_status, "on") == 0 && g_ttys [x_tty].valid == TTY_VALID) {
         g_ttys [x_tty].allowed = TTY_ALLOWED;
      } else {
         g_ttys [x_tty].allowed = TTY_BLOCKED;
      }
      /*---(style)-----------------------*/
      if (strcmp (x_style , "complex") == 0) {
         g_ttys [x_tty].style   = TTY_COMPLEX;
      } else {
         g_ttys [x_tty].style   = TTY_STANDARD;
      }
      /*---(next)------------------------*/
      ++c;
      /*---(done)------------------------*/
   }
   /*---(close file)---------------------*/
   rc = yPARSE_close_in ();
   DEBUG_INPT   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

/*> char             /+ [------] open the configuration file ---------------------+/   <* 
 *> conf_open          (void)                                                          <* 
 *> {                                                                                  <* 
 *>    /+---(begin)--------------------------+/                                        <* 
 *>    yLOG_enter  (__FUNCTION__);                                                     <* 
 *>    /+---(locals)-----------+-----------+-+/                                        <* 
 *>    tSTAT       s;                                                                  <* 
 *>    int         rci         = 0;                                                    <* 
 *>    int         count       = 0;                                                    <* 
 *>    char        recd        [500];                                                  <* 
 *>    /+---(check existance)----------------+/                                        <* 
 *>    yLOG_info   ("conf_file" , my.name_conf);                                       <* 
 *>    rci = lstat (my.name_conf, &s);                                                 <* 
 *>    if  (rci < 0) {                                                                 <* 
 *>       yLOG_info   ("existance" , "FATAL, file does not exist");                    <* 
 *>       yLOG_exit   (__FUNCTION__);                                                  <* 
 *>       return -1;                                                                   <* 
 *>    }                                                                               <* 
 *>    yLOG_info   ("existance" , "configuration existance verified");                 <* 
 *>    /+---(check linking)------------------+/                                        <* 
 *>    if  (S_ISLNK(s.st_mode))  {                                                     <* 
 *>       yLOG_info   ("linking"   , "FATAL, symbolic link (insecure)");               <* 
 *>       yLOG_exit   (__FUNCTION__);                                                  <* 
 *>       return -2;                                                                   <* 
 *>    }                                                                               <* 
 *>    yLOG_info   ("linking"   , "configuration is not a symbolic link");             <* 
 *>    /+---(check file type)----------------+/                                        <* 
 *>    if  (!S_ISREG(s.st_mode))  {                                                    <* 
 *>       yLOG_info   ("normal"    , "FATAL, not a regular file");                     <* 
 *>       yLOG_exit   (__FUNCTION__);                                                  <* 
 *>       return -3;                                                                   <* 
 *>    }                                                                               <* 
 *>    yLOG_info   ("normal"    , "configuration is a normal file");                   <* 
 *>    /+---(check owner)--------------------+/                                        <* 
 *>    if  (s.st_uid  != 0)  {                                                         <* 
 *>       yLOG_info   ("owner"     , "FATAL, not owned by root (insecure)");           <* 
 *>       yLOG_exit   (__FUNCTION__);                                                  <* 
 *>       return -4;                                                                   <* 
 *>    }                                                                               <* 
 *>    yLOG_info   ("owner"     , "configuration is owned by root");                   <* 
 *>    /+---(check group)--------------------+/                                        <* 
 *>    if  (s.st_gid  != 0)  {                                                         <* 
 *>       yLOG_info   ("group"     , "FATAL, group is not root (insecure)");           <* 
 *>       yLOG_exit   (__FUNCTION__);                                                  <* 
 *>       return -5;                                                                   <* 
 *>    }                                                                               <* 
 *>    yLOG_info   ("group"     , "configuration group is root also");                 <* 
 *>    /+---(check permissions)--------------+/                                        <* 
 *>    if  ((s.st_mode & 00777) != 00600)  {                                           <* 
 *>       yLOG_info   ("permission", "FATAL, permissions not 0600 (insecure)");        <* 
 *>       yLOG_exit   (__FUNCTION__);                                                  <* 
 *>       return -6;                                                                   <* 
 *>    }                                                                               <* 
 *>    yLOG_info   ("permission", "configuration permissions are 0600");               <* 
 *>    yLOG_info   ("security", "perfecto, trusted source");                           <* 
 *>    /+---(open the file)------------------+/                                        <* 
 *>    conf_file = fopen (my.name_conf, "r");                                          <* 
 *>    if (conf_file == NULL) {                                                        <* 
 *>       yLOG_info   ("opening"   , "can not open the file");                         <* 
 *>       yLOG_exit   (__FUNCTION__);                                                  <* 
 *>       return -7;                                                                   <* 
 *>    }                                                                               <* 
 *>    yLOG_info   ("opening"   , "configuration file successfully openned");          <* 
 *>    while (1) {                                                                     <* 
 *>       fgets (recd, 500, conf_file);                                                <* 
 *>       if (feof(conf_file))  break;                                                 <* 
 *>       if (recd [0] == '#' )  continue;                                             <* 
 *>       if (recd [0] == '\0')  continue;                                             <* 
 *>       if (recd [0] == '\n')  continue;                                             <* 
 *>       if (recd [0] == ' ' )  continue;                                             <* 
 *>       if (recd [0] == 't' )  ++count;                                              <* 
 *>    }                                                                               <* 
 *>    yLOG_value  ("ttys"      , count);                                              <* 
 *>    /+---(reopen the file)----------------+/                                        <* 
 *>    fclose (conf_file);                                                             <* 
 *>    conf_file = fopen (my.name_conf, "r");                                          <* 
 *>    if (conf_file == NULL) {                                                        <* 
 *>       yLOG_info   ("opening"   , "can not open the file");                         <* 
 *>       yLOG_exit   (__FUNCTION__);                                                  <* 
 *>       return -7;                                                                   <* 
 *>    }                                                                               <* 
 *>    yLOG_info   ("opening"   , "configuration file successfully openned");          <* 
 *>    /+---(allocation)---------------------+/                                        <* 
 *>    ntty      = count;                                                              <* 
 *>    ttys      = (tTTY  *) malloc (sizeof (tTTY ) * ntty);                           <* 
 *>    yLOG_note   ("allocated ttys");                                                 <* 
 *>    g_polls   = (tPOLL *) malloc (sizeof (tPOLL) * ntty);                           <* 
 *>    yLOG_note   ("allocated polling");                                              <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    yLOG_exit   (__FUNCTION__);                                                     <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char             /+ [------] open the configuration file ---------------------+/                             <* 
 *> conf_read          (void)                                                                                    <* 
 *> {                                                                                                            <* 
 *>    /+---(begin)--------------------------+/                                                                  <* 
 *>    yLOG_enter  (__FUNCTION__);                                                                               <* 
 *>    /+---(locals)-----------+-----------+-+/                                                                  <* 
 *>    int         i           = 0;                                                                              <* 
 *>    char        recd        [500];                                                                            <* 
 *>    int         count       = 0;                                                                              <* 
 *>    char       *p           = NULL;                                                                           <* 
 *>    char       *s           = NULL;                                                                           <* 
 *>    char        tty_group   [20];                                                                             <* 
 *>    char        tty_dev     [20];                                                                             <* 
 *>    char        tty_style   [20];                                                                             <* 
 *>    char        tty_status  [20];                                                                             <* 
 *>    /+---(read lines)---------------------+/                                                                  <* 
 *>    while (1) {                                                                                               <* 
 *>       fgets (recd, 500, conf_file);                                                                          <* 
 *>       if (feof(conf_file))  break;                                                                           <* 
 *>       if (recd [0] == '#' )  continue;                                                                       <* 
 *>       if (recd [0] == '\0')  continue;                                                                       <* 
 *>       if (recd [0] == '\n')  continue;                                                                       <* 
 *>       if (recd [0] == ' ' )  continue;                                                                       <* 
 *>       if (recd [0] != 't' )  continue;                                                                       <* 
 *>       recd [strlen (recd)] = '\0';                                                                           <* 
 *>       yLOG_note   ("New TTY");                                                                               <* 
 *>       p = strtok (recd, "");                                                                                <* 
 *>       if (p == NULL) continue;                                                                               <* 
 *>       str_trim (p);                                                                                          <* 
 *>       p = strtok (NULL, "");                                                                                <* 
 *>       if (p == NULL) continue;                                                                               <* 
 *>       str_trim (p);                                                                                          <* 
 *>       strncpy (tty_group , p, 15);                                                                           <* 
 *>       yLOG_info   ("group"     , tty_group);                                                                 <* 
 *>       p = strtok (NULL,  "");                                                                               <* 
 *>       if (p == NULL) {                                                                                       <* 
 *>          yLOG_note   ("tty_dev is null");                                                                    <* 
 *>          continue;                                                                                           <* 
 *>       }                                                                                                      <* 
 *>       str_trim (p);                                                                                          <* 
 *>       strncpy (tty_dev   , p, 15);                                                                           <* 
 *>       yLOG_info   ("device"    , tty_dev);                                                                   <* 
 *>       p = strtok (NULL,  "");                                                                               <* 
 *>       if (p == NULL) {                                                                                       <* 
 *>          yLOG_note   ("tty_style is null");                                                                  <* 
 *>          continue;                                                                                           <* 
 *>       }                                                                                                      <* 
 *>       str_trim (p);                                                                                          <* 
 *>       strncpy (tty_style , p, 15);                                                                           <* 
 *>       yLOG_info   ("style"     , tty_style);                                                                 <* 
 *>       p = strtok (NULL,  "");                                                                               <* 
 *>       if (p == NULL) {                                                                                       <* 
 *>          yLOG_note   ("tty_status is null");                                                                 <* 
 *>          continue;                                                                                           <* 
 *>       }                                                                                                      <* 
 *>       str_trim (p);                                                                                          <* 
 *>       strncpy (tty_status, p, 15);                                                                           <* 
 *>       yLOG_info   ("status"    , tty_status);                                                                <* 
 *>       p = strtok (NULL,  "");                                                                               <* 
 *>       if (p == NULL) {                                                                                       <* 
 *>          yLOG_note   ("tty_type is null");                                                                   <* 
 *>          continue;                                                                                           <* 
 *>       }                                                                                                      <* 
 *>       str_trim (p);                                                                                          <* 
 *>       yLOG_info   ("type"      , p);                                                                         <* 
 *>       strcpy  (ttys[count].name, tty_dev);                                                                   <* 
 *>       sprintf (ttys[count].full, "/dev/%s",  ttys[count].name);                                              <* 
 *>       ttys [count].host_num = rand() % 69;                                                                   <* 
 *>       sprintf (ttys [count].host_name, "#%02d.%s", ttys[count].host_num, g_hosts [ttys [count].host_num]);   <* 
 *>       ttys    [count].allowed  =  'y';                                                                       <* 
 *>       ttys    [count].watched  =  'y';                                                                       <* 
 *>       ttys    [count].active   =  '-';                                                                       <* 
 *>       ttys    [count].rpid     =   0 ;                                                                       <* 
 *>       ttys    [count].tries    =   0 ;                                                                       <* 
 *>       ttys    [count].complete =   0 ;                                                                       <* 
 *>       ttys    [count].failure  =   0 ;                                                                       <* 
 *>       g_polls [count].fd       =  -1;                                                                        <* 
 *>       g_polls [count].events   = NULL;                                                                       <* 
 *>       if (strcmp(tty_status, "on") == 0)  {                                                                  <* 
 *>          yLOG_info   ("allowed"     , "yes");                                                                <* 
 *>          g_polls [count].events   = POLLIN;                                                                  <* 
 *>       } else {                                                                                               <* 
 *>          ttys    [count].allowed  =  'n';                                                                    <* 
 *>          ttys    [count].watched  =  '-';                                                                    <* 
 *>          yLOG_info   ("allowed"     , "no");                                                                 <* 
 *>       }                                                                                                      <* 
 *>       ++count;                                                                                               <* 
 *>    }                                                                                                         <* 
 *>    yLOG_value  ("records"   , count);                                                                        <* 
 *>    /+---(complete)-----------------------+/                                                                  <* 
 *>    yLOG_exit   (__FUNCTION__);                                                                               <* 
 *>    return 0;                                                                                                 <* 
 *> }                                                                                                            <*/

/*> char                                                                              <* 
 *> conf_close         (void)                                                         <* 
 *> {                                                                                 <* 
 *>    /+---(begin)--------------------------+/                                       <* 
 *>    yLOG_enter  (__FUNCTION__);                                                    <* 
 *>    /+---(locals)-----------+-----------+-+/                                       <* 
 *>    int         rci         = 0;                                                   <* 
 *>    /+---(close)--------------------------+/                                       <* 
 *>    rci = fclose(conf_file);                                                       <* 
 *>    if (rci != 0) {                                                                <* 
 *>       yLOG_info   ("closing"   , "can not close the file");                       <* 
 *>       yLOG_exit   (__FUNCTION__);                                                 <* 
 *>       return -7;                                                                  <* 
 *>    }                                                                              <* 
 *>    yLOG_info   ("closing"   , "configuration file successfully closed");          <* 
 *>    /+---(null pointer)-------------------+/                                       <* 
 *>    conf_file = NULL;                                                              <* 
 *>    yLOG_info   ("pointer"   , "file pointer set to null");                        <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    yLOG_exit   (__FUNCTION__);                                                    <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/




/*===============================[[ end-code ]]===============================*/
