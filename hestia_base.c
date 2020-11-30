/*===============================[[ beg-code ]]===============================*/
#include    "hestia.h"





int       logger   = -1;



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
   while (1) {
      /*---(read)------------------------*/
      rc = yPARSE_read (NULL, NULL, NULL, NULL);
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




/*===============================[[ end-code ]]===============================*/
