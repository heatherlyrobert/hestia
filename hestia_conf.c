/*===============================[[ beg-code ]]===============================*/
#include    "hestia.h"



/*====================------------------------------------====================*/
/*===----                         configuration                        ----===*/
/*====================------------------------------------====================*/
static void      o___CONF____________________o (void) {;}

char
conf__handler           (int n, uchar *a_verb, char a_exist, void *a_handler)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         c           =    0;
   char        x_dev       [LEN_LABEL] = "";
   char        x_listen    [LEN_TERSE] = "";
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_verb"    , a_verb);
   --rce;  if (strcmp (a_verb, "line") != 0) {
      DEBUG_INPT  yLOG_note    ("incorrect verb handler called");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(check field count)--------------*/
   rc = yPARSE_ready (&c);
   DEBUG_INPT  yLOG_value   ("fields"    , c);
   --rce;  if (c < 3) {
      DEBUG_INPT  yLOG_note    ("minimum three fields");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(parse fields)-------------------*/
   rc = yPARSE_scanf (a_verb, "LT", x_dev, x_listen);
   DEBUG_INPT  yLOG_value   ("scanf"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(defense)------------------------*/
   rc = tty_authorize (x_dev, x_listen);
   DEBUG_INPT  yLOG_value   ("authorize" , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
conf_driver             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_info    ("n_conf"     , my.n_conf);
   rc = yPARSE_autoread (my.n_conf, NULL, conf__handler);
   DEBUG_PROG  yLOG_value   ("config"    , rc);
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}





/*===============================[[ end-code ]]===============================*/
