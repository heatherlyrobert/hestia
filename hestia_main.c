/*===============================[[ beg-code ]]===============================*/
#include    "hestia.h"




int              /* [------] main driver ------------------------------------*/
main               (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(standard startup)---------------*/
   if (rc >= 0)  rc = PROG_preinit ();
   if (rc >= 0)  rc = yURG_logger  (a_argc, a_argv);
   if (rc >= 0)  rc = yURG_urgs    (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_init    ();
   if (rc >= 0)  rc = PROG_args    (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_begin   ();
   if (rc >= 0)  rc = PROG_final   ();
   if (rc < 0) {
      return rc;
   }
   /*---(main)---------------------------*/
   switch (my.user_mode) {
   case MODE_DAEMON :
      rc = PROG_daemon  ();
      DEBUG_TOPS   yLOG_value   ("daemon"    , rc);
      rc = tty_openall ();
      DEBUG_TOPS   yLOG_value   ("openall"   , rc);
      rc = exec_loop    ();
      DEBUG_TOPS   yLOG_value   ("loop"      , rc);
      break;
   case MODE_VERIFY :
   default          :
      rc = rptg_ttys   ();
      break;
   }
   /*---(done)---------------------------*/
   rc = PROG_end     ();
   /*---(complete)-----------------------*/
   return 0;
}


