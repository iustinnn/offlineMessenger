/* servTCPPreTh.c - Exemplu de server TCP concurent care deserveste clientii
   printr-un mecanism de prethread-ing; cu blocarea mutex de protectie a lui accept(); 
   Asteapta un numar de la clienti si intoarce clientilor numarul incrementat.
  
   
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sqlite3.h>
#include "dateGlobale.h"

/* portul folosit */
#define PORT 2909

  char *zErrMsg = 0;
   static int callback(void *NotUsed, int argc, char **argv, char **azColName) { //functia callback
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
sqlite3_stmt* res;
int y;

int indiceMesajeUpdatat()
{

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    int k=0;
    
    int rc = sqlite3_open("tabel1.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
      //  return 1;
    }
    
    char *sql = "SELECT ID_MESAJ FROM ISTORIC ORDER BY ID_MESAJ DESC";
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {  
        
       
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    int step = sqlite3_step(res);
  //  printf(" \n %s \n ", "x");
    if (step == SQLITE_ROW) {
     
k=sqlite3_column_int(res,0);
    } 
  
    sqlite3_finalize(res);
    sqlite3_close(db);
    k++;
    return k;

}
void UltimulExpeditor(char dest[],char out[])
{

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("tabel1.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_finalize(res);
        
     //   return 1;
    }
    
    char *sql = "SELECT EXPEDITOR FROM ISTORIC WHERE DESTINATAR = ? ORDER BY ID_MESAJ DESC";
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {  
        
        sqlite3_bind_text(res, 1, dest, -1, NULL);
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    int step = sqlite3_step(res);

    if (step == SQLITE_ROW) {

        strcpy(out,(const char*)sqlite3_column_text(res, 0));

    } 
  
    sqlite3_finalize(res);
    sqlite3_close(db);
   
}
int indiceUseriUpdatat()
{

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    int k=0;
    
    int rc = sqlite3_open("tabel1.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
      //  return 1;
    }
    
    char *sql = "SELECT ID_USER FROM USERII ORDER BY ID_USER DESC";
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {  
        
       
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    int step = sqlite3_step(res);
  //  printf(" \n %s \n ", "x");
    if (step == SQLITE_ROW) {
     
k=sqlite3_column_int(res,0);
    } 
  
    sqlite3_finalize(res);
    sqlite3_close(db);
    k++;
    return k;

}
int Istoric(char user[],char user2[],int cl)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    int result;
    int rc = sqlite3_open("tabel1.db", &db);
    char *sql = "SELECT EXPEDITOR, MESAJ FROM ISTORIC WHERE ( ( EXPEDITOR = ? and DESTINATAR = ? ) or ( EXPEDITOR = ? and DESTINATAR = ? ) ) ORDER BY ID_MESAJ ASC";
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    else
    {
 sqlite3_bind_text(res,1,user,-1,NULL);
 sqlite3_bind_text(res,2,user2,-1,NULL);
  sqlite3_bind_text(res,3,user2,-1,NULL);
 sqlite3_bind_text(res,4,user,-1,NULL);
 //sqlite3_bind_text(res,3,user2,-1,NULL);
//  sqlite3_bind_text(res,4,user,-1,NULL);
  while((rc = sqlite3_step(res) == SQLITE_ROW)) {
            
              char mesaj[255];
      mesaj[0]='\0';
       const char *mes1=(const char *)sqlite3_column_text(res, 0);
       const char *mes2=(const char *)sqlite3_column_text(res, 1);
  //printf("\n name sa zice: %s: \n", numee);
  char auxiliar[100]="[ ";
   // auxiliar[0]='\0';
    strcat(auxiliar,mes1);
   strcat(auxiliar," ] - ");
   strcat(auxiliar, mes2);
   // printf("\n aux %s: \n", auxiliar);
   // strcpy(mesaj,auxiliar);
     // mesaj[strlen(mesaj)]='\0';
      if(auxiliar[0]!='\0'&&auxiliar[0]!=' ')
      {
        write(cl,&auxiliar,100); 
        usleep(300);
      }
        
  }
    }
   
    sqlite3_finalize(res);
    
    sqlite3_close(db);
  
return 1;
}
void UpdateNotificari(char nume[],int cl)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    int result;
    int rc = sqlite3_open("tabel1.db", &db);
     char *sql = "UPDATE ISTORIC set IS_READ = 1 WHERE DESTINATAR = ? and IS_READ = 0";
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    else
    {
 sqlite3_bind_text(res,1,nume,-1,NULL);
  while((rc = sqlite3_step(res) == SQLITE_ROW)) {
            // printf("\n [ %s ]: %s \n", (const char*)sqlite3_column_text(res, 0),(const char*)sqlite3_column_text(res, 1));
              char mesaj[255];
      mesaj[0]='\0';
       
        
  }
    }
    
    sqlite3_finalize(res);
    
    sqlite3_close(db);
}

int Notificari(char nume[],int cl)
{int NuExista=-1;
//printf("\n salut \n");
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    int result;
    int rc = sqlite3_open("tabel1.db", &db);
    char *sql = "SELECT EXPEDITOR, MESAJ FROM ISTORIC WHERE IS_READ = ? and DESTINATAR = ?";
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    else
    {
 sqlite3_bind_int(res,1,0);
 sqlite3_bind_text(res,2,nume,-1,NULL);
  while((rc = sqlite3_step(res) == SQLITE_ROW)) {
            // printf("\n[ %s ]: %s \n", (const char*)sqlite3_column_text(res, 0),(const char*)sqlite3_column_text(res, 1));
              char mesaj[255];
      mesaj[0]='\0';
       const char *mes1=(const char *)sqlite3_column_text(res, 0);
       const char *mes2=(const char *)sqlite3_column_text(res, 1);
  //printf("\n name sa zice: %s: \n", numee);
  char auxiliar[100]="[ ";
   // auxiliar[0]='\0';
    strcat(auxiliar,mes1);
   strcat(auxiliar," ] - ");
   strcat(auxiliar, mes2);
   // printf("\n aux %s: \n", auxiliar);
   // strcpy(mesaj,auxiliar);
     // mesaj[strlen(mesaj)]='\0';
      if(auxiliar[0]!='\0'&&NuExista==-1)
       { 
        char trimite[255];
        trimite[0]='\0';
        strcpy(trimite,"Cat ati fost offline ati primit urmatoarele mesaje:\n-> ");
        strcat(trimite,auxiliar);
        write(cl,&trimite,255); 
         usleep(300);
       NuExista=1;
       }
       else if(auxiliar[0]!='\0')
       {
        write(cl,&auxiliar,100); 
         usleep(300);
       }
        
  }
    }
 
   
    sqlite3_finalize(res);
    
    sqlite3_close(db);
    return NuExista;
  
}
int UserOnClient(char nume[])
{int valabil=0;
int zz=0;
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("tabel1.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    // printf("\n  nm:%s \n", nume);
    char *sql = "SELECT CLIENT FROM USERII WHERE NAME = ?";
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {
        
        sqlite3_bind_text(res, 1, nume, -1, NULL);
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    int step = sqlite3_step(res);
   // printf(" \n %s \n ", "x");
    if (step == SQLITE_ROW) {
       
         valabil=sqlite3_column_int(res, 0);
       //  printf("\n  cli:%d \n", sqlite3_column_int(res, 0));


    } 
   
  zz=valabil;
 //  printf("\n  cli:%d \n", zz);
    sqlite3_finalize(res);
    sqlite3_close(db);
   return zz;

}
int puneIstoric(char conv[],char exp[],char dest[],int marcat)
{


    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    char *sql;
    
    int rc = sqlite3_open("tabel1.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
   sql = "INSERT INTO ISTORIC (ID_MESAJ,EXPEDITOR,DESTINATAR,MESAJ,IS_READ) "  \
         "VALUES (?, ?, ?, ?, ?); ";
        int www=indiceMesajeUpdatat();
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {
        
        sqlite3_bind_int(res,1,www);
        sqlite3_bind_text(res, 2, exp, -1, NULL);
        sqlite3_bind_text(res, 3, dest, -1, NULL);
        sqlite3_bind_text(res, 4, conv, -1, NULL);
         sqlite3_bind_int(res,5,marcat);
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    int step = sqlite3_step(res);
    //printf(" \n %s \n ", "x");
    if (step == SQLITE_ROW) {
       // printf("\n e conectat: %d \n", sqlite3_column_int(res, 0));
      //   printf("\n e conectat: %d \n", sqlite3_column_int(res, 1));
      
      

    } 
  
    sqlite3_finalize(res);
    sqlite3_close(db);
 

}
int verificaOnline(char nume[])  //returneaza 1 daca connected, 0 daca nu
{
  int valabil=0;
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("tabel1.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    char *sql = "SELECT ID_USER, IS_CONNECTEDD FROM USERII WHERE NAME = ?";
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {
        
        sqlite3_bind_text(res, 1, nume, -1, NULL);
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    int step = sqlite3_step(res);

    if (step == SQLITE_ROW) {
    
      
         valabil=sqlite3_column_int(res, 1);

    } 
  
    sqlite3_finalize(res);
    sqlite3_close(db);
    if(valabil==1)
   return 1;
   else return 0;
}
int clientActualCon(int cl)
{
  
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    int ceva=-1;
    int rc = sqlite3_open("tabel1.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    char *sql = "SELECT IS_CONNECTEDD FROM USERII WHERE CLIENT = ?";
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {
        
        sqlite3_bind_int(res, 1, cl);
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);
   // printf(" \n %s \n ", "gggg");
    if (step == SQLITE_ROW) {
     //   printf("\n %s: \n", sqlite3_column_text(res, 1));
    ceva=sqlite3_column_int(res, 0);
    // printf("\n %s: \n", ceva);
    } 
 
    sqlite3_finalize(res);
    sqlite3_close(db);
    return ceva;
  //  printf("\n aux %s: \n", nume);
}
int ReconectStatus()
{
//printf("\n salut \n");
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    int result;
    int rc = sqlite3_open("tabel1.db", &db);
    char *sql = "UPDATE USERII set IS_CONNECTEDD = 0, CLIENT = 0, ID_THREAD = -1";  
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        //return 1;
    }
    else
    {
  while((rc = sqlite3_step(res) == SQLITE_ROW)) {
         
  printf("Client delogat.\n");
     
        
  }
    }
 
   
    sqlite3_finalize(res);
    sqlite3_close(db);
   
}
int isSomeoneConnected(int cl,char nume[])
{
  
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    const unsigned char *ceva=NULL;
    int rc = sqlite3_open("tabel1.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    char *sql = "SELECT NAME FROM USERII WHERE CLIENT = ?";
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {
        
        sqlite3_bind_int(res, 1, cl);
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);
   // printf(" \n %s \n ", "gggg");
    if (step == SQLITE_ROW) {
     //   printf("\n %s: \n", sqlite3_column_text(res, 1));
    ceva=sqlite3_column_text(res, 0);
    // printf("\n %s: \n", ceva);
    } 
    if(ceva!=NULL)
    {
  const char *numee=(const char *)ceva;
  //printf("\n name sa zice: %s: \n", numee);
  char auxiliar[50];
    auxiliar[0]='\0';
    
    strncpy(auxiliar, numee, 50);
    auxiliar[strlen(auxiliar)]='\0';
    //  printf("\n aux %s: \n", auxiliar);
    strcpy(nume,auxiliar);
    }
    // printf("\n aux %s: \n", nume);
    sqlite3_finalize(res);
    sqlite3_close(db);
  //  printf("\n aux %s: \n", nume);
  return 1;
}
int checkExistaUser(char s[])
{
    sqlite3 *db;
    sqlite3_stmt* res;
    int rc = sqlite3_open("tabel1.db", &db);
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
sqlite3_finalize(res);
sqlite3_close(db);
}
extern int errno;
 sqlite3 *db;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
//void raspunde(void *);

void CreateDB() { //DESCHIDE DB SI FACE CELE 2 TABELE
 

   char *zErrMsg = 0;
   int rc,rc1;

    char *err_msg = 0;
   /* Open database */
   rc = sqlite3_open("tabel1.db", &db);
    // char *sql = "DROP TABLE ISTORIC;";
  
    //char *sql;

   /* Create SQL statement */
 char* userTab = "CREATE TABLE IF NOT EXISTS USERII("  \
      "ID_USER INT      ," \
      "NAME           TEXT    ," \
      "ID_THREAD            INT     ," \
      "IS_CONNECTEDD       INT," \
      "CLIENT INT );";
  rc = sqlite3_exec(db, userTab, callback, 0, &zErrMsg);
         /* Create SQL statement */
 char* istoricTab = "CREATE TABLE IF NOT EXISTS ISTORIC("  \
      "ID_MESAJ INT      ," \
       "EXPEDITOR           NAME    ," \
      "DESTINATAR           NAME    ," \
      "MESAJ           TEXT     ," \
      "IS_READ       INT);";
      
      // rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   /* Execute SQL statement */

    rc1 = sqlite3_exec(db, istoricTab, callback, 0, &zErrMsg);
  sqlite3_close(db);
   int zz=ReconectStatus();
printf("\n Created DB\n");
}

typedef struct {
	pthread_t idThread; //id-ul thread-ului
	int thCount; //nr de conexiuni servite
}Thread;

Thread *threadsPool; //un array de structuri Thread

int sd; //descriptorul de socket de ascultare
int nthreads;//numarul de threaduri

pthread_mutex_t mlock=PTHREAD_MUTEX_INITIALIZER;              // variabila mutex ce va fi partajata de threaduri 
void raspunde(int cl,int idThread);
int DeleteMe(char nume[])
{ 
 sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("tabel1.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
      //  return 1;
    }
     printf("\n Deleting: %s \n", nume);
     char *sql;

    sql = "DELETE from USERII where NAME = ?";

        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {
          sqlite3_bind_text(res, 1, nume, -1,NULL);
      
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    int step = sqlite3_step(res);
    sqlite3_finalize(res);
    sqlite3_close(db);
}
void CreeazaUser(char userNume[],int cl,int idThread)
{ int rc;
   char *userTab,*istoricTab;

   /* Open database */
   rc = sqlite3_open("tabel1.db", &db);
     char *sql;
    int kkk=indiceUseriUpdatat();
    ///if(kkk==0)
   sql = "INSERT INTO USERII (ID_USER,NAME,ID_THREAD,IS_CONNECTEDD,CLIENT) "  \
         "VALUES (?, ?, ?, 1, ?); ";
       rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
   sqlite3_bind_int(res, 1, kkk);
   sqlite3_bind_text(res, 2, userNume, -1,NULL);
    sqlite3_bind_int(res, 3, idThread);
    sqlite3_bind_int(res, 4, cl);
   sqlite3_step(res);
            sqlite3_finalize(res);
               //  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
               //      sqlite3_finalize(res);
              //   printf("%s",sql);
           
    sqlite3_close(db);
}
int updateConectat(int valoare, char nume[],int cl, int zz, int thr)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("tabel1.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
     printf("\n updated :%s \n", nume);
     char *sql;
     if(zz==0)
     thr=-1;
    sql = "UPDATE USERII set IS_CONNECTEDD = ?, CLIENT = ?, ID_THREAD=? WHERE NAME = ?";

        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {
        sqlite3_bind_int(res, 1, valoare);
         sqlite3_bind_int(res, 2, cl);
         sqlite3_bind_int(res, 3, thr);
        sqlite3_bind_text(res, 4, nume, -1, NULL);
       
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    int step = sqlite3_step(res);
    sqlite3_finalize(res);
    sqlite3_close(db);

  
}
int main (int argc, char *argv[])
{CreateDB();
sleep(1);
  struct sockaddr_in server;	// structura folosita de server  	
  void threadCreate(int);

   if(argc<2)
   {
        fprintf(stderr,"Eroare: Primul argument este numarul de fire de executie...");
	exit(1);
   }
   nthreads=atoi(argv[1]);
   if(nthreads <=0)
	{
        fprintf(stderr,"Eroare: Numar de fire invalid...");
        fflush(stdout);
	exit(1);
	}
    threadsPool = calloc(sizeof(Thread),nthreads);

   /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
  /* utilizarea optiunii SO_REUSEADDR */
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));

  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY); //LOCAL
  /* utilizam un port utilizator */ 
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 15) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }

   printf("Nr threaduri %d \n", nthreads); fflush(stdout);
   int i;
   for(i=0; i<nthreads;i++) threadCreate(i);

	
  /* servim in mod concurent clientii...folosind thread-uri */
  for ( ; ; ) 
  {
//	printf ("[server]Asteptam la portul %d...\n",PORT);
  fflush(stdout);
        pause(); //semnal				
  }
};
				
void threadCreate(int i)
{
	void *treat(void *);
	
	pthread_create(&threadsPool[i].idThread,NULL,&treat,(void*)i);
	return; /* threadul principal returneaza */
}

void *treat(void * arg)
{		
		int client;
		        
		struct sockaddr_in from; 
 	        bzero (&from, sizeof (from));
 		printf ("[thread]- %d - pornit...\n", (int) arg);fflush(stdout);

		for( ; ; )
		{
			int length = sizeof (from);
			pthread_mutex_lock(&mlock);
			
			if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)//ret EP clientului  in from nr bytes
				{
	 			 perror ("[thread]Eroare la accept().\n");	  			
				}
        printf("Thread trezit\n",(int)arg);
			pthread_mutex_unlock(&mlock);
			threadsPool[(int)arg].thCount++;

			raspunde(client,(int)arg); //procesarea cererii
			/* am terminat cu acest client, inchidem conexiunea */
      
      close (client);	
      sleep(1);		
           printf("\nThread eliberat\n");
		}	
}


void raspunde(int cl,int idThread)
{
        char nr[255];		//mesajul primit de trimis la client  
	      char userNume[100];
        nr[0]='\0';
        while(1==1)
        {int zzz=-1;
     nr[0]='\0';
          int n=read (cl, &nr,255);
          if(strncmp(nr,"quit",4)==0)
          {char kk[255];
          kk[0]='\0';
            isSomeoneConnected(cl,kk);
            if(kk[0]!='\0')
            {int w;
           w= updateConectat(0,kk,0,0,idThread);
            }
    
            return;
          }
          //pthread_mutex_lock(&mlock);
     //    printf("cifre: %s %d  = ",nr,strlen(nr));
          zzz=n;
         // pthread_mutex_unlock(&mlock);
          if(nr[0]!='\0'&&nr[0]!=' ')
          {		int cinevaCon=clientActualCon(cl);
        if (strncmp(nr, "login : ", 8) == 0) 
 {
  strncpy(userNume, nr + 8, strlen(nr) - 7); //in msg am userul si il caut in DB
  userNume[strlen(userNume)-1]='\0';
  int k=checkExistaUser(userNume);
  int w=clientActualCon(cl);
  if(w==-1)
  {

int y=verificaOnline(userNume);
  if(k==0)
  { CreeazaUser(userNume,cl,idThread);
  strcpy(nr,"User nou conectat.");
  char nume[100];
  nume[0]='\0';
  isSomeoneConnected(cl,nume);
     
  write (cl, &nr, 255);
  }
  else if(k==1&&y==0){ //userul exista dar nu e activ
  strcpy(nr,"User reconectat. ");
  updateConectat(1,userNume,cl,1,idThread);
 int r=Notificari(userNume,cl);
 if(r==-1)
 {
   strcpy(nr,"User reconectat. Nicun mesaj nou primit.");
   printf("\nUser reconnected.\n");
write (cl, &nr, 255);
 }

 //printf("avem notif: %i \n",r);
 UpdateNotificari(userNume,cl);
  }
  else if(k==1&&y==1) //userul exista dar e deja on
  {
    printf("\nUser deja conectat pe terminal.\n");
strcpy(nr,"User-ul este deja conectat. Incearca unul nou.");
write (cl, &nr, 255);
  }
 
  }
  else if(w==1||w==0)
  {
    printf("\nUser deja conectat pe terminal.\n");
char uy[100]="Avem un user deja conectat. Va rugam delogati-va de pe acest terminal.";

write(cl, &uy, 100);
  }
  
 }
 else if (strncmp(nr, "send : ", 7) == 0 && cinevaCon!=(-1)) 
 {
char capat[255];
capat[0]='\0';
char mesaj[255];
mesaj[0]='\0';
char conversatie[255];
conversatie[0]='\0';
strncpy(capat, nr + 7, strlen(nr) - 6);
capat[strlen(capat)]='\0';
//printf("\n %s %d \n",capat,strlen(capat));
int gasit=0;
int indice=nr+7;
int k=0;
while(gasit==0)
{
if(capat[k]==':')
{
  gasit=1;
  mesaj[k-1]='\0';
  break;
}
else 
{
  mesaj[k]=capat[k];
  k++;

}
}
indice=indice+k;
//printf("\n %s %d\n",mesaj,strlen(mesaj));//in mesaj e destinatarul

int a=checkExistaUser(mesaj);
if(a==1)
{
//fflush(stdout);
int z=verificaOnline(mesaj);
int marcatVazut=0;
if(z==1)
marcatVazut=1;
strncpy(conversatie, nr+9+k, strlen(nr)-10-k);
conversatie[strlen(nr)-10-k]='\0';
//conversatie[strlen(conversatie)]='\0';
//sprintf("\n %s %d\n",conversatie,strlen(conversatie)); //in conversatie e tot mesajul
//fflush(stdout);
if(marcatVazut==1)
{printf("\n Mesaj trimis. Destinarul este activ. \n");
  strcpy(nr, "Mesaj trimis. Destinarul este activ."); 
nr[37]='\0';
write (cl, &nr, 255);
}
else 
{
   printf("\n Mesaj trimis. Destinarul este off. \n");
strcpy(nr, "Mesaj trimis. Destinarul este off."); 
nr[35]='\0';
write (cl, &nr, 255);
}

char nume[50];
  nume[0]='\0';
  isSomeoneConnected(cl,nume); // in nume avem luam numele clientul cu CL - adica user-ul de la care trimitem
  char mesajLung[200];
mesajLung[0]='\0';
strcat(mesajLung,"[ ");
strcat(mesajLung,nume);
strcat(mesajLung," ] : ");
strcat(mesajLung,conversatie);
int n=puneIstoric(conversatie,nume,mesaj,marcatVazut); //pune in storic
int w=UserOnClient(mesaj);
if(z==1)
write(w,&mesajLung,200);
mesajLung[0]='\0';
conversatie[0]='\0';
//bzero(mesajLung,1001);
//printf("\n Clientul are clientul : %d \n",w);
//verific dupa nume daca e connected
}
else if(a==0)
{
   printf("\n Nu exista destinatarul. \n");
  char cuv[255];
  cuv[0]='\0';
  strcpy(cuv, "Nu exista user cu acest nume pe server.");
  write(cl,&cuv,255);
}

 }
  else if (strncmp(nr, "send : ", 7) == 0 && cinevaCon==(-1)) 
  {
char mesag[255];
mesag[0]='\0';
strcpy(mesag, "Va rugam logati-va inainte de a folosi aceasta comanda.");
 printf("\n User nelogat. \n");
write(cl,&mesag,255);
  }
 else if (strncmp(nr, "logout", 6) == 0&& cinevaCon!=(-1))
 {
    char nume[100];
  nume[0]='\0';
  isSomeoneConnected(cl,nume);
   printf("\n User deconectat. \n");
 // printf("\n  in logout: %s \n",nume);
updateConectat(0,nume,0,0,idThread);
  char mes[100];
  mes[0]='\0';
  strcpy(mes, "User disconnected.");
  write(cl,&mes,100);
 } 
  else if (strncmp(nr, "logout", 6) == 0&& cinevaCon==(-1))
  {
    char mesag[255];
mesag[0]='\0';
strcpy(mesag, "Va rugam logati-va inainte de a folosi aceasta comanda.");
 printf("\n User nelogat. \n");
write(cl,&mesag,255);
  }
else if (strncmp(nr, "history : ", 10) == 0&& cinevaCon!=(-1))
{  char user[100];
user[0]='\0';
    strncpy(user, nr + 10, strlen(nr) - 9); //in msg am userul si il caut in DB
  user[strlen(user)-1]='\0';
  int k=checkExistaUser(user);
  printf(" \n %i \n",k);
  if(k==1)
  {
    char auxy[255];
    auxy[0]='\0';
   int blabla=isSomeoneConnected(cl,auxy);
    printf(" \n %s \n",auxy);
    int gg=Istoric(user,auxy,cl);
  }
  else 
  {
    char cuvv[255];
    cuvv[0]='\0';
    strcpy(cuvv,"User inexistent pe server.");
    write(cl,&cuvv,255);
  }

}
else if (strncmp(nr, "history : ", 10) == 0&& cinevaCon==(-1))
{
    char mesag[255];
mesag[0]='\0';
strcpy(mesag, "Va rugam logati-va inainte de a folosi aceasta comanda.");
 printf("\n User nelogat. \n");
write(cl,&mesag,255);
  }
  else if (strncmp(nr, "reply : ", 8) == 0&&cinevaCon!=(-1))
{char mesajTrimis[255]="Replied by ";

  char convo[255];
  convo[0]='\0';
  strncpy(convo, nr + 8, strlen(nr) - 7);
  char out[255];
  out[0]='\0';
  char nm[255];
  nm[0]='\0';

 int rr=isSomeoneConnected(cl,nm);//in nm pune numele clientului in care ma aflu
  //   printf("\n pe ecran:%s \n",nm);
     strcat(mesajTrimis,nm);
     strcat(mesajTrimis," -> ");
     strcat(mesajTrimis,convo);
  UltimulExpeditor(nm,out); //in out am ultimul expeditor pt clientul curent
  int status=verificaOnline(out);
  int w=UserOnClient(out);
   int ww=UserOnClient(nm);
  if(status==1)
  {
   int v = puneIstoric(convo,nm,out,1);
write(w,&mesajTrimis,255);
  }
  else if(out[0]!='\0')
  {
       int v = puneIstoric(convo,nm,out,0);
  strcpy(mesajTrimis,"User-ul pentru care e destinat mesajul s-a deconectat intre timp.");
   printf("\n User nelogat. \n");
  write(ww,&mesajTrimis,255);
  }
  else
  {
    strcpy(mesajTrimis,"User-ul nu a primit niciun mesaj.");
     printf("\n User-ul nu a primit niciun mesaj. \n");
  write(ww,&mesajTrimis,255);
  }


}
else if (strncmp(nr, "reply : ", 8) == 0&& cinevaCon==(-1))
{ printf("\n User nelogat. \n");
    char mesag[255];
mesag[0]='\0';
strcpy(mesag, "Va rugam logati-va inainte de a folosi aceasta comanda.");
write(cl,&mesag,255);
  }
  else if(strncmp(nr, "delete me", 9) == 0&& cinevaCon!=(-1))
  {
     char messaj[255]="User-ul a fost sters din baza de date.";
    char nmm[255];
  nmm[0]='\0';
  int cc=isSomeoneConnected(cl,nmm);//in nm pune numele clientului in care ma aflu
  DeleteMe(nmm);
  printf("\n User-ul a fost sters din baza de date.\n");
  write(cl,&messaj,255);
  }
  else if(strncmp(nr, "delete me", 9) == 0&& cinevaCon==(-1))
  {
    char mesag[255];
mesag[0]='\0';
strcpy(mesag, "Va rugam logati-va inainte de a folosi aceasta comanda.");
 printf("\n User nelogat. \n");
write(cl,&mesag,255); 
  }
else {
  char msj[255];
  msj[0]='\0';
  
  strcpy(msj,"Comanda inexistenta.");
  write(cl,&msj,255);
}

 /*
 else
 {
 printf("\n %d \n",cl);
	 if (write (cl, &nr, sizeof(nr)) <= 0)
		{
		 printf("[Thread %d] ",idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		}
	else
		printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",idThread);	
 }
			 // printf ("[Thread %d]Mesajul a fost receptionat.....%s\n",idThread, nr);
		      
		      /*pregatim mesajul de raspuns */
		     // nr[0]='a';      
	//printf("[Thread %d]Trimitem mesajul inapoi...%s\n",idThread, nr);
		      
		      
		      /* returnam mesajul clientului */

			
			}

          
        }
	
			
			}
    
	


