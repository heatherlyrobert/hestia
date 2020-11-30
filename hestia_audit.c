/*============================[[    beg-code    ]]============================*/
#include    "hestia.h"

int              /* [------] search in audit log for a particular login ------*/
audit_find         (char *a_dev, int  a_pid,  int *a_pos)
{
   /*---(locals)-----------+-----------+-*/
   FILE       *f           = NULL;          /* audit file                     */
   int         line        = 0;             /* audit file current line        */
   char        recd        [500];           /* audit file current record      */
   int         len         = 0;             /* audit file current line length */
   char       *p           = NULL;          /* strtok token                   */
   char       *q           = "";          /* strtok delimiter               */
   char       *s           = NULL;          /* strtok context                 */
   char        t           [100];           /* sprintf temp string            */
   int         pid         = 0;
   int         pos         = 0;
   /*---(gather info)--------------------*/
   f = fopen (HWTMP_FILE, "r");
   if (f == NULL) {
      printf ("cant open file\n");
      return -1;
   }
   while (1) {
      /*---(read)------------------------*/
      pos = ftell (f);
      /*> printf ("run %d is %d\n", line, pos);                                       <*/
      fgets (recd, 500, f);
      if (feof (f))      break;
      /*---(prepare)---------------------*/
      len = strlen (recd) - 1;
      recd [len] = '\0';
      ++line;
      /*---(defense)---------------------*/
      if (len     ==  0 )            continue;
      if (recd[0] == '#')            continue;
      /*---(type)------------------------*/
      p = strtok_r (recd, q, &s);
      if (p == NULL)                 continue;
      str_trim (p);
      if (strcmp (p, "ACTIVE") != 0) continue;
      /*---(host)------------------------*/
      p = strtok_r (NULL, q, &s);
      if (p == NULL)                 continue;
      /*---(device)----------------------*/
      p = strtok_r (NULL, q, &s);
      if (p == NULL)                 continue;
      str_trim (p);
      if (strcmp (p, a_dev ) != 0)   continue;
      /*---(pid)-------------------------*/
      p = strtok_r (NULL, q, &s);
      if (p == NULL)                 continue;
      pid = atoi (p);
      if (pid != a_pid)   continue;
      /*---(success)---------------------*/
      fclose (f);
      if (a_pos != NULL)  *a_pos = pos;
      /*> printf ("find pos is %d, %p\n", pos, a_pos);                                <*/
      return line;
   }
   if (a_pos != NULL)  *a_pos = 0;
   /*---(complete)-----------------------*/
   fclose (f);
   return -(line + 1);
}

char             /* [------] append a login record ---------------------------*/
audit_login        (char *a_dev, char *a_user, int a_rpid)
{
   /*---(locals)-----------+-----------+-*/
   FILE       *f           = NULL;          /* audit file                     */
   int         line        = 0;             /* audit file current line        */
   char        recd        [500];           /* audit file current record      */
   int         len         = 0;             /* audit file current line length */
   char       *p           = NULL;          /* strtok token                   */
   char       *q           = "";          /* strtok delimiter               */
   char       *s           = NULL;          /* strtok context                 */
   char        t           [300];           /* sprintf temp string            */
   char        host        [50];
   char        startf      [50];
   char        starts      [50];
   time_t      time_date;
   tTIME      *curr_time   = NULL;
   int         ri          = 0;
   /*---(check for existing)-------------*/
   ri = audit_find (a_dev, a_rpid, NULL);
   if  (ri >= 0)    return -1;
   /*---(prepare)------------------------*/
   gethostname (host, &len);
   if (len >= 50)   return -2;
   /*---(get the date)-------------------*/
   time_date = time(NULL);
   curr_time = localtime(&time_date);
   strftime(startf, 50, "%y.%m.%d.%H.%M.%S.%U", curr_time);
   strftime(starts, 50, "%s"                  , curr_time);
   /*---(gather info)--------------------*/
   f = fopen (HWTMP_FILE, "a");
   snprintf (t, 300, "%-10.10s  %-20.20s  %-20.20s  %10d  %-20.20s  %-20.20s  %-20.20s  %-20.20s  %-20.20s  %-10.10s  %-10.10s  %-10.10s ",
         "ACTIVE", host, a_dev, a_rpid, a_user, startf, starts, " ", " ", " ", " ", " ");
   fprintf (f, "%s\n", t);
   /*---(complete)-----------------------*/
   fclose (f);
   return -ri;
}

char             /* [------] append a login record ---------------------------*/
audit_fail         (char *a_dev, char a_type)
{
   /*---(locals)-----------+-----------+-*/
   FILE       *f           = NULL;          /* audit file                     */
   int         line        = 0;             /* audit file current line        */
   char        recd        [500];           /* audit file current record      */
   int         len         = 0;             /* audit file current line length */
   char       *p           = NULL;          /* strtok token                   */
   char       *q           = "";          /* strtok delimiter               */
   char       *s           = NULL;          /* strtok context                 */
   char        t           [300];           /* sprintf temp string            */
   char        msg         [300];
   char        host        [50];
   char        startf      [50];
   char        starts      [50];
   time_t      time_date;
   tTIME      *curr_time   = NULL;
   int         ri          = 0;
   /*---(prepare)------------------------*/
   gethostname (host, &len);
   if (len >= 50)   return -2;
   /*---(get the date)-------------------*/
   time_date = time(NULL);
   curr_time = localtime(&time_date);
   strftime(startf, 50, "%y.%m.%d.%H.%M.%S.%U", curr_time);
   strftime(starts, 50, "%s"                  , curr_time);
   /*---(gather info)--------------------*/
   f = fopen (HWTMP_FILE, "a");
   if      (a_type == 'Z')   strcpy (msg, "timed_out");
   else if (a_type == 'B')   strcpy (msg, "cancelled");
   else if (a_type == 'k')   strcpy (msg, "knock");
   else if (a_type == 'p')   strcpy (msg, "prefix");
   else if (a_type == 'u')   strcpy (msg, "user_name");
   else if (a_type == 'I')   strcpy (msg, "user_name");
   else if (a_type == 'i')   strcpy (msg, "infix");
   else if (a_type == 'c')   strcpy (msg, "password");
   else if (a_type == 'S')   strcpy (msg, "password");
   else if (a_type == 's')   strcpy (msg, "suffix");
   else                      strcpy (msg, "failure");
   snprintf (t, 300, "%-10.10s  %-20.20s  %-20.20s  %10d  %-20.20s  %-20.20s  %-20.20s  %-20.20s  %-20.20s  %10d  %-10.10s  %10d ",
         "fail"  , host, a_dev, -666, "n/a", startf, starts, startf, starts, 0, msg, -666);
   fprintf (f, "%s\n", t);
   /*---(complete)-----------------------*/
   fclose (f);
   return 0;
}

char             /* [------] append a login record ---------------------------*/
audit_logout       (char *a_dev, int a_rpid, int a_rc)
{
   yLOG_senter (__FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   FILE       *f           = NULL;          /* audit file                     */
   int         line        = 0;             /* audit file current line        */
   char        recd        [500];           /* audit file current record      */
   int         len         = 0;             /* audit file current line length */
   char       *p           = NULL;          /* strtok token                   */
   char       *q           = "";          /* strtok delimiter               */
   char       *s           = NULL;          /* strtok context                 */
   char        t           [300];           /* sprintf temp string            */
   char        host        [50];
   int         start       = 0;
   char        endf        [50];
   char        ends        [50];
   int         dur         = 0;
   time_t      time_date;
   tTIME      *curr_time   = NULL;
   int         ri          = 0;
   int         pos         = 0;
   /*---(check for existing)-------------*/
   ri = audit_find (a_dev, a_rpid, &pos);
   yLOG_svalue ("index"     , ri);
   if  (ri <  0) {
      yLOG_sexit  (__FUNCTION__);
      return -1;
   }
   /*---(gather info)--------------------*/
   f = fopen (HWTMP_FILE, "r+");
   if (f == NULL) {
      yLOG_snote  ("error on logging file");
      yLOG_sexit  (__FUNCTION__);
   }
   yLOG_snote  ("file open");
   fseek (f, pos      , SEEK_SET);
   fprintf (f, "complete");
   yLOG_snote  ("marked");
   /*---(get the stop)-------------------*/
   fseek (f, pos + 118, SEEK_SET);
   fread (recd, 10,  1, f);
   recd [10] = '\0';
   start = atoi (recd);
   /*> printf ("testing = <<%s>>\n", recd);                                           <*/
   /*---(get the date)-------------------*/
   time_date = time(NULL);
   curr_time = localtime(&time_date);
   strftime(endf, 50, "%y.%m.%d.%H.%M.%S.%U", curr_time);
   strftime(ends, 50, "%s"                  , curr_time);
   fseek (f, pos + 141, SEEK_SET);
   fprintf (f, endf);
   fseek (f, pos + 164, SEEK_SET);
   fprintf (f, ends);
   yLOG_snote  ("dates");
   /*---(duration)-----------------------*/
   fseek (f, pos + 187, SEEK_SET);
   /*> fprintf (f, a_rc);                                                             <*/
   dur = atoi(ends) - start;
   snprintf (t, 300, "%10d", dur);
   fprintf (f, "%s", t);
   fseek (f, pos + 200, SEEK_SET);
   if      (dur <    5)      fprintf (f, "exec_only");
   else if (dur <   60)      fprintf (f, "quickie");
   else if (dur <  600)      fprintf (f, "utility");
   else                      fprintf (f, "worker");
   yLOG_snote  ("dur");
   /*---(return code)--------------------*/
   fseek (f, pos + 213, SEEK_SET);
   fprintf (f, a_rc);
   snprintf (t, 300, "%10d", a_rc);
   fprintf (f, "%s", t);
   yLOG_snote  ("status");
   /*---(complete)-----------------------*/
   fclose (f);
   yLOG_sexit  (__FUNCTION__);
   return 0;
}

char             /* [------] register a boot record --------------------------*/
audit_system       (char a_type)
{
   /*---(locals)-----------+-----------+-*/
   FILE       *f           = NULL;          /* audit file                     */
   int         line        = 0;             /* audit file current line        */
   char        recd        [500];           /* audit file current record      */
   int         len         = 0;             /* audit file current line length */
   char       *p           = NULL;          /* strtok token                   */
   char       *q           = "";          /* strtok delimiter               */
   char       *s           = NULL;          /* strtok context                 */
   char        t           [300];           /* sprintf temp string            */
   char        msg         [300];           /* sprintf temp string            */
   char        host        [50];
   char        startf      [50];
   char        starts      [50];
   time_t      time_date;
   tTIME      *curr_time   = NULL;
   int         ri          = 0;
   /*---(prepare)------------------------*/
   gethostname (host, &len);
   if (len >= 50)   return -2;
   /*---(get the date)-------------------*/
   time_date = time(NULL);
   curr_time = localtime(&time_date);
   strftime(startf, 50, "%y.%m.%d.%H.%M.%S.%U", curr_time);
   strftime(starts, 50, "%s"                  , curr_time);
   /*---(gather info)--------------------*/
   f = fopen (HWTMP_FILE, "a");
   if      (a_type == 'b')  strcpy (msg, "BOOT------");
   else if (a_type == 's')  strcpy (msg, "DOWN------");
   else                     strcpy (msg, "UNKNOWN---");
   snprintf (t, 300, "%-10.10s  %-20.20s  %-20.20s  %10d  %-20.20s  %-20.20s  %-20.20s  %-20.20s  %-20.20s  %10d  %-10.10s  %10d ",
         msg, host, "--------------------", 1, "--------------------", startf, starts, startf, starts, 0, "system", 0);
   fprintf (f, "%s\n", t);
   /*---(complete)-----------------------*/
   fclose (f);
   return -ri;
}


/*============================[[    end-code    ]]============================*/
