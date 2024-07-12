/* cliTCPIt.c - Exemplu de client TCP
   Trimite un nume la server; primeste de la server "Hello nume".
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <sqlite3.h>
#include <pthread.h>
#include "dateGlobale.h"

/* codul de eroare returnat de anumite apeluri */
pthread_mutex_t mlock=PTHREAD_MUTEX_INITIALIZER;  
extern int errno;
extern int indiceUseri;
 sqlite3 *db;
 sqlite3_stmt *stmt;
   static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

extern int checkExistaUser(char s[])
{
    sqlite3 *db;
    sqlite3_stmt* res;
    int rc = sqlite3_open("tabele.db", &db);
        if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
   
    }
      char *sql = "SELECT name from USERII";  
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0); 
    const char *myname;
    // in s avem userul 
    int ok=0;
    while(sqlite3_step(res)!=SQLITE_DONE)
    {
      myname= sqlite3_column_text(res, 0);
      if(strcmp(myname,s)==0)
    {ok=1;
     //  printf("%s: ",myname);
    }
       
    }

return ok;


}
void NewUserCreate(char s[])
{
  indiceUseri++;

}
/* portul de conectare la server*/

 void *citire(void * sd)
 {
  char text[255];
while(1==1)
{
 int k=read(sd,text,255);
        if (k < 0)
            error("ERROR reading from socket");
            if(text[0]!='\0'&&text[0]!=' ')
 printf("\n-> %s\n",text);
 text[0]='\0';
}

 }

int port;

int num;
char msg[100];
int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  		// mesajul trimis
  int nr=0;
  char mesaj[100];
  char buf[1000];

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1) //-ACCEPT
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
  /* citirea mesajului */
  fflush (stdout);
  int suntConectat=0;
pthread_t cap;
pthread_create (&cap, NULL, &citire, sd);
while(1==1)
{
     if(fgets(buf, 255, stdin) != NULL)
     {
      if(strncmp(buf,"quit",4)==0)
      {
        printf("\nClientul se deconecteaza de la server...\n");
        sleep(1);
        int n=pthread_cancel(cap);
        if(n!=0)
        {
          printf("EROARE THREAD-UL NU S A TERMINAT");
        }
        write (sd,&buf,255);

       kill(getpid(), SIGINT);
      }
     if(buf[0]!='\n'&&buf[0]!=' ')
     write (sd,&buf,255);

     }
     //bzero(buf,256);
    
}

/* buf[strlen(buf)] = '\0';
 num=strlen(buf);
 if (strncmp(buf, "login : ", 8) == 0&&suntConectat==0) 
 {
  strncpy(msg, buf + 8, num - 7); //in msg am userul si il caut in DB
  int k=checkExistaUser(msg);
  if(k==0)
  {
    NewUserCreate(msg); 
  }
 // else UserBackOn(msg);
   suntConectat=1;
    if (write (sd,&buf,sizeof(buf)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }

  /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
 /* if (read (sd, &buf,sizeof(buf)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit 
  printf ("[client]Mesajul primit este: %s\n", buf);
 }
 else if(suntConectat==1)
 {
printf("\n userul cu sd: %i este conectat sau nu: %i \n",sd,suntConectat);
  //scanf("%d",&nr);
  
  printf("[client] Am citit de la tastatura: %s\n",buf);

  /* trimiterea mesajului la server 
  if (write (sd,&buf,sizeof(buf)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }

  /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) 
  if (read (sd, &buf,sizeof(buf)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit 
  printf ("[client]Mesajul primit este: %s\n", buf);
*/
  /* inchidem conexiunea, am terminat 
 }
 else if(suntConectat==0)
 {
  printf("Foloseste comanda login pentru a te loga. \n");
 }
 
*/

 
  sqlite3_close(db);
}
