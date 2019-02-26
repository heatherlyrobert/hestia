/*===============================[[ beg-code ]]===============================*/

#include    "hestia.h"




/*====================------------------------------------====================*/
/*===----                        cronpulse file                        ----===*/
/*====================------------------------------------====================*/
static void      o___PULSE___________________o (void) {;};

char         /*--> write the heartbeat ---------------------------------------*/
rptg_heartbeat          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   DEBUG_RPTG   yLOG_info    ("file"      , my.name_heartbeat);
   f = fopen (my.name_heartbeat, "wt");
   DEBUG_RPTG   yLOG_point   ("f"         , f);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   DEBUG_RPTG   yLOG_info    ("heartbeat" , my.heartbeat);
   fputs (my.heartbeat, f);
   /*---(close)--------------------------*/
   rc = fclose (f);
   DEBUG_RPTG   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
rptg_ttys               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        i           =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(walk trhu ttys)-----------------*/
   DEBUG_RPTG   yLOG_value   ("MAX_TTY"   , MAX_TTYS);
   for (i = 0; i < MAX_TTYS; ++i) {
      DEBUG_RPTG   yLOG_value   ("tty#"      , i);
      DEBUG_RPTG   yLOG_complex ("g_ttys"    , "%2d %-12.12s %c %c %c %c", i, g_ttys [i].device, g_ttys [i].valid, g_ttys [i].allowed, g_ttys [i].watched, g_ttys [i].active);
      printf ("%2d %-6.6s %-11.11s %2d %2d %2d %c",
            i, g_ttys [i].name  , g_ttys [i].device, g_ttys [i].language,
            g_ttys [i].cluster  , g_ttys [i].host  , g_ttys [i].style);
      printf ("   %c %c %c %c   %2d %6d",
            g_ttys [i].valid, g_ttys [i].allowed, g_ttys [i].watched, g_ttys [i].active,
            g_ttys [i].fd, g_ttys [i].rpid);
      printf ("   %2d %2d %2d %2d %2d",
            g_ttys [i].attempts, g_ttys [i].complete, g_ttys [i].kills,
            g_ttys [i].errors, g_ttys [i].failures);
      printf ("\n");
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> read the last heartbeat -----------------------------------*/
rptg__unit_heartbeat   (char *a_heartbeat)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   DEBUG_RPTG   yLOG_info    ("file"      , my.name_heartbeat);
   f = fopen (my.name_heartbeat, "rt");
   DEBUG_RPTG   yLOG_point   ("f"         , f);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   fgets (a_heartbeat, LEN_HUND, f);
   DEBUG_RPTG   yLOG_info    ("heartbeat" , a_heartbeat);
   /*---(close)--------------------------*/
   rc = fclose (f);
   DEBUG_RPTG   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}


char*            /*--> unit test accessor ------------------------------*/
rptg__unit              (char *a_question, int a_num)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_heartbeat [LEN_HUND];
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "RPTG             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "heartbeat"     )  == 0) {
      snprintf (unit_answer, LEN_RECD, "RPTG heartbeat   : %s", my.heartbeat);
   }
   else if (strcmp (a_question, "lastbeat"      )  == 0) {
      rptg__unit_heartbeat (x_heartbeat);
      snprintf (unit_answer, LEN_RECD, "RPTG lastbeat    : %s", x_heartbeat);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

