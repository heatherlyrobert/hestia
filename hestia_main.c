/*===============================[[ beg-code ]]===============================*/
#include    "hestia.h"




int              /* [------] main driver ------------------------------------*/
main               (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;


   printf ("starting...\n");
   tty_review  ();
   return 0;


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
   /*---(prepare)------------------------*/
   if (rc == 0)  rc = conf_open    ();
   if (rc == 0)  rc = conf_read    ();
   if (rc == 0)  rc = conf_close   ();
   if (rc == 0)  rc = tty_existing ();
   if (rc == 0)  rc = tty_openall  ();
   /*---(run)----------------------------*/
   if (rc == 0)  rc = poller       ();
   /*---(done)---------------------------*/
   if (rc == 0)  rc = PROG_end     ();
   /*---(complete)-----------------------*/
   return 0;
}


