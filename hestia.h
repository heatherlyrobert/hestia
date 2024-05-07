/*===============================[[ beg-code ]]===============================*/
#ifndef HESTIA
#define HESTIA loaded


/*===[[ HEADER ]]=============================================================*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*········· ··········· ´·····························´········································*/
#define     P_FOCUS     "SA (system administration)"
#define     P_NICHE     "au (authentication)"
#define     P_SUBJECT   "getty terminal daemon"
#define     P_PURPOSE   "simple, experimental, and transparent getty terminal daemon"
/*········· ··········· ´·····························´········································*/
#define     P_NAMESAKE  "hestia-polyolbos (full of blessings)"
#define     P_PRONOUNCE "hehs·tee·ah paa·lee·ohl·bohs"
#define     P_HERITAGE  "virgin goddess of hearth, home, architecture, and eternal flame"
#define     P_BRIEFLY   "protector of hearth and home"
#define     P_IMAGERY   "regal, but modestly cloaked and vieled woman with a wooden staff"
#define     P_REASON    "obviously, as hestia is the guardian of the hearth and home"
/*········· ··········· ´·····························´········································*/
#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT
/*········· ··········· ´·····························´········································*/
#define     P_HOMEDIR   "/home/system/khronos.heatherly_cron_daemon"
#define     P_BASENAME  "hestia"
#define     P_FULLPATH  "/sbin/hestia"
#define     P_SUFFIX    "···"
#define     P_CONTENT   "···"
/*········· ··········· ´·····························´········································*/
#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_COMPILER  "gcc 5.3.0"
#define     P_CODESIZE  "small       (appoximately 1,000 slocl)"
#define     P_DEPENDS   "yEXEC, ySEC, ySTR, yPARSE"
/*········· ··········· ´·····························´········································*/
#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2012-01"
/*········· ··········· ´·····························´········································*/
#define     P_VERMAJOR  "1.--, settling into new framework of eos"
#define     P_VERMINOR  "1.1-, preparing for next production roll-out"
#define     P_VERNUM    "1.1c"
#define     P_VERTXT    "update to yEXEC_whoami"
/*········· ··········· ´·····························´········································*/



#define     P_USAGE     "hestia [OPTIONS]"
#define     P_DEBUG     "hestia_debug [URGENTS] [OPTIONS]"

#define     P_SUMMARY   \
 "hestia is a simple, experimental, and transparent getty terminal daemon¦" \
 "that monitors a select set of virtual tty devices for activity, if found,¦" \
 "it spawns a login cycle (hearth) for the newly active ones.¦"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"

#define     P_ALTERNS   "getty, agetty, mingetty, ngetty, mgetty, fbgetty, ..."
#define     P_REMINDER  "there are many better options, but i *own* every line of this one"

#define     P_ASSUME    \
 "-- no remote hosts (disallow -H <host_name>)¦" \
 "-- only virtual ttys (no modem, serial line, fax, voice, framebuffer)¦" \
 "-- no alternate login programs (disallow -l <login_program>)¦" \
 "-- only vt102 terminals (console, xterm, eterm, etc)¦" \
 "-- no issue or alternate issuse (disallow /etc/issue or -f <issue_file>)¦" \
 "-- no relationship to, or use of, /etc/inittab, /etc/ttys, /etc/gettdefs¦"

/*345678901-12345678901-123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/

/*===[[ SUMMARY ]]=============================================================*

 *
 *
 *
 *   getty is the generic name for a program which manages a terminal line,
 *   handles the login process, and protects the system from unauthorized
 *   access.
 *
 *   alternatives...
 *      -- getty        : original configured through conf file
 *      -- agetty       : alternative configured using arguments
 *      -- mingetty     : minimal version for virtual terminals only
 *      -- ngetty       : handles multiple tty in one instance
 *      -- mgetty       : more flexible to handle fax, voice, and others
 *      -- fbgetty      : adds framebuffer and image support
 *
 */

/*
 *  original
 *     - asks for username and password with simple prompts
 *
 *  minimal
 *     - input username and password without any ques at all
 *
 *  tokens
 *     - given a number/text on a private device to use
 *
 *  grabled
 *     - passes a small strip of digits and you must scramble also
 *
 *  complex
 *     - banner page with clues in ascii art
 *
 *
 *
 *
 *  authentication is about confirming the truth of an identity or fact
 *
 *  1st, accepting proof given by a credible person
 *  2nd, comparing attributes to what is known of objects of that origin
 *  3rd, documentation or external affirmations
 *
 *  ownership   : something the user has (id card, token, phone, bracelet ...)
 *  knowledge   : something the user knows (password, challenge-response, ...)
 *  inherence   : something the user is (biometrics, signature, voice, ...)
 *
 *  two-factor  : bankcard and pin, token and password, etc
 *
 *  authentication (Au) is proving you are who you say you are
 *  authorization  (Az) is proving you permitted to do what you are trying to do
 *
 *
 *
 */
/*
 *   the goddess hekate is the guardian of gates and doorways and is sometimes
 *   shown as triple bodied
 *
 *   the job of this piece of software is to...
 *      - open and initialize a tty line (/dev file)
 *      - read a login name
 *      - invoke /bin/login
 *
 *   hekate seeks to be a clean, simplified getty that takes out the wierd
 *   features and focuses on our normal systems.
 *      - runs as a single daemon (ngetty)
 *      - always asks for user name
 *      - has a tiny securyty extra in user name
 *      - does not use the /etc/issue file or "-f <issue_file>"
 *      - does not use /etc/ttys, /etc/gettydefs, /etc/gettytab
 *      - can not change the login program using "-l <login_program>"
 *      - only used for the local host so no "-H <host_name>"
 *      - time out is automatic so no "-t <timeout>"
 *      - always assume 8-bit clean
 *      - all terminals are assumed to be vt102 (like eterm, xterm, ...)
 *      - let login take care of the fact root can not login except using su
 *      - no modems
 *
 *
 *
 *  each time hestia prompts for a login it uses a different host name
 *
 */


/* rapidly evolving version number to aid with visual change confirmation ----*/


/* configuration files -------------------------------------------------------*/
#define     FILE_CONF        "hestia.conf"
#define     FILE_EXEC        "hestia.execution_feedback"
#define     FILE_STATUS      "hestia.status"
#define     FILE_HEARTBEAT   "hestia.heartbeat"

#define     DIR_ETC          "/etc/"
#define     DIR_TMP          "/tmp/"
#define     DIR_RUN          "/run/"
#define     DIR_YLOG         "/var/log/yLOG/"
#define     DIR_YHIST        "/var/log/yLOG.historical/"
#define     DIR_UNIT         "/tmp/hestia_test/"


#define     EXEC_FILE        "/var/log/yLOG/hestia_exec"
#define     HUTMP_FILE       "/var/log/hestia.active"
#define     HWTMP_FILE       "/var/log/hestia.logging"
#define     HSTAT_FILE       "/var/log/khaos.hestia"



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
#include    <yEXEC.h>               /* HEATHERLY execution services           */
#include    <yLOG.h>                /* HEATHERLY program logger               */
#include    <yPARSE.h>              /* HEATHERLY file parsing                 */
#include    <ySEC.h>                /* HEATHERLY security logging             */
#include    <ySTR.h>                /* HEATHERLY string handling              */
#include    <yURG.h>                /* HEATHERLY debugging support            */

#include    <stdio.h>               /* ANSI-C    i/o library                  */
#include    <stdlib.h>              /* ANSI-C    general library              */
#include    <string.h>              /* ANSI-C    c-string library             */
#include    <signal.h>              /* ANSI-C    signal handling              */
#include    <time.h>
#include    <fcntl.h>
#include    <poll.h>
#include    <unistd.h>              /* stat(),crpyt()                      */
#include    <sys/types.h>           /* stat()                              */
#include    <sys/stat.h>            /* stat()                              */
#include    <termios.h>
#include    <sys/ioctl.h>
#include    <math.h>

#include    <dirent.h>

#include    <pwd.h>                    /* LINUX     password access           */
#include    <shadow.h>                 /* LINIX     password shadow access    */

#include    <yASCII.h>


#define     LOGIN       "/bin/login"
#define     MAX_ROW     64
#define     MAX_COL     180
#define     MAX_HOST    100

typedef struct FILE      tFILE;
typedef struct tm        tTIME;
typedef struct stat      tSTAT;
typedef struct passwd    tPASSWD;
typedef struct spwd      tSHADOW;
typedef struct dirent    tDIRENT;
typedef struct termios   tTERMIOS;

extern  char          unit_answer [LEN_RECD];


#define     MODE_PARK        '#'
#define     MODE_VERIFY      'v'
#define     MODE_DAEMON      'd'
#define     MODE_NORMAL      'n'
#define     MODE_UNIT        'u'
#define     MODE_VALID       "#vdnu"


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
typedef struct cACCESSOR  tACCESSOR;
struct cACCESSOR
{
   /*---(behavior)-------------*/
   char        prog_name   [LEN_PATH];      /* name of running program        */
   char        user_mode;                   /* validate, daemon, or unit      */
   /*---(owner)----------------*/
   int         uid;                         /* uid of person who launched eos */
   char        who         [LEN_LABEL];     /* user name who launched eos     */
   int         pid;                         /* process id of eos              */
   int         ppid;                        /* parent process id of eos       */
   /*---(time)-----------------*/
   int         wait_dsec;                   /* update period in deci-seconds  */
   long        now;                         /* current epoch                  */
   char        heartbeat   [LEN_HUND];      /* latest heartbeat               */
   /*---(files)----------------*/
   char        n_conf      [LEN_RECD];      /* name of configuration file     */
   char        name_exec   [LEN_RECD];      /* name of execution detail file  */
   char        name_status [LEN_RECD];      /* name of status file            */
   char        name_heartbeat [LEN_PATH];   /* pulser file name               */
   /*---(done)------------------*/
};
extern tACCESSOR  my;



void             /* [------] receive signals ---------------------------------*/
PROG_signal        (int a_signal, siginfo_t *a_info, void *a_nada);


/*---(fonts)------------------------------------------------------------------*/
struct cFONT {
   char        name        [20];
   char        r;
   char        array       [20];
   char       *p;
   char        v, vo;
   char        h, hs, ho;
};


extern char        butterfly   [40] [MAX_ROW] [MAX_COL];
extern char        chunky      [10] [300];
extern char        chunky_full [10] [300];
extern char        alligator   [10] [300];
extern char        goofy       [10] [300];
extern char        binary      [10] [300];
extern char        dots        [10] [300];
extern char        basic       [10] [300];



extern struct cFONT       font [50];
extern int         nfont;
extern int         cfont;



typedef struct cENTRY tENTRY;
struct   cENTRY {
   char     knock       [ 20];
   char     prefix      [ 20];
   char     rot         [ 20];
   char     username    [ 20];
   char     user_fix    [ 20];
   char     infix       [ 20];
   char     password    [ 20];
   char     pointer     [ 20];
   char     suffix      [ 20];
};
extern tENTRY      entry;

typedef struct cTITLES tTITLES;
struct    cTITLES {
   char     language    [20];
   char     cluster     [20];
   char     seq         [20];
   char     date        [20];
   char     host        [20];
   char     user        [20];
   char     token       [20];
   char     password    [20];
   char     attempt     [20];
};
extern tTITLES     titles      [30];




/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
#define     MAX_TTYS       40
/*---(statuses)--------------*/
#define     TTY_VALID      'y'
#define     TTY_INVALID    '-'
#define     TTY_ALLOWED    'y'
#define     TTY_BLOCKED    '-'
#define     TTY_WATCHED    'y'
#define     TTY_WAITING    'W'
#define     TTY_OTHERS     'o'
#define     TTY_IGNORED    '-'
#define     TTY_ACTIVE     'y'
#define     TTY_UNUSED     '-'
/*---(login styles)----------*/
#define     TTY_COMPLEX    'C'
#define     TTY_STANDARD   's'
#define     TTY_MINIMAL    'm'


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
typedef     struct      cTTY        tTTY;
struct   cTTY {
   /*---(device)---------------*/
   char        name        [LEN_LABEL];         /* tty name                   */
   char        device      [LEN_LABEL];         /* full device path           */
   char        style;                           /* style for login screen     */
   /*---(host)-----------------*/
   int         language;                        /* language number            */
   int         cluster;                         /* cluster number             */
   int         host;                            /* host number                */
   char        host_name   [LEN_DESC];          /* full host name             */
   /*---(flags)----------------*/
   char        valid;                           /* exist                      */
   char        allowed;                         /* allowed for hestia         */
   char        watched;                         /* something else is using    */
   char        active;                          /* whether it is active       */
   /*---(working)--------------*/
   int         fd;                              /* current file descriptor    */
   tTERMIOS    original;                        /* original termios flags     */
   int         rpid;                            /* current running job        */
   /*---(stats)----------------*/
   int         attempts;                        /* number of times launched   */
   int         complete;                        /* completed sessions         */
   int         kills;                           /* number of terminations     */
   int         errors;                          /* number of launch failures  */
   int         failures;                        /* failures to login          */
   /*---(done)-----------------*/
};
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
extern      tTTY        g_ttys      [MAX_TTYS];



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
typedef     struct      pollfd      tPOLL;
extern      tPOLL       g_polls     [MAX_TTYS];



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char*       PROG_version            (void);
/*---(startup)--------------*/
char        PROG_defaults           (void);
char        PROG_init               (void);
char        PROG_args               (int a_argc, char *a_argv[]);
char        PROG_begin              (void);
char        PROG_final              (void);
/*---(runtime)--------------*/
void        PROG_comm               (int a_signal, siginfo_t *a_info, char *a_name, char *a_desc);
char        PROG_daemon             (void);
/*---(wrapup)---------------*/
char        PROG_end                (void);
/*---(unittest)-------------*/
char        prog__unit_quiet        (void);
char        prog__unit_loud         (void);
char        prog__unit_end          (void);
char*       prog__unit              (char *a_question, int a_num);

char        conf__handler           (int n, uchar *a_verb, char a_exist, void *a_handler);
char        conf_driver             (void);

char        tty_init                (void);
char        tty_wrap                (void);
char        tty_authorize           (char *a_dev, char *a_listen);
char        tty_open                (int a_tty);
char        tty_close               (int a_tty);
char        tty_display             (int a_num);
char        tty_openall             (void);
char        tty_review              (void);
char*       tty__unit               (char *a_question, int a_num);
char        tty__unit_allowed       (int a_tty);
char        tty__unit_blocked       (int a_tty);
char        tty__unit_watched       (int a_tty);
char        tty__unit_ignored       (int a_tty);
char        tty__unit_active        (int a_tty);
char        tty__unit_unused        (int a_tty);

char        exec_park               (void);
long        exec_time               (long a_now);
char        exec_poll               (void);
char        exec_loop               (void);
char*       exec__unit              (char *a_question, int a_num);
char        exec__unit_ping         (int a_tty);

char        poller                  (void);

char        rptg_heartbeat          (void);
char        rptg_ttys               (void);
char*       rptg__unit              (char *a_question, int a_num);

     
char*       unit_accessor      (char*, int);


extern char        dev         [30];
extern char        user        [30];
extern char        shell       [30];
extern int         veil_rpid;
extern int         rpid;



#endif
/*===============================[[ end-code ]]===============================*/
