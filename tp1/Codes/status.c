#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void affiche_status (pid_t pid)
{
   FILE *fp;
   char chaine[80];
   
   sprintf (chaine, "/proc/%u/status", pid);
   if ((fp = fopen (chaine, "r")) == NULL) 
   {
      fprintf (stdout, "Processus inexistant\n\n");
      return;
   }
   while (fgets (chaine, 80, fp) != NULL)
      if (strncmp (chaine, "State", 5) == 0) 
      {
         fputs (chaine, stdout);
         fprintf (stdout, "\n");
         break;
      }
   fclose (fp);
}

int main (void)
{
   pid_t pid;

   fprintf (stdout, "Je suis le père : PID = %u\n", getpid());
   fprintf (stdout, "État attendu = running\n");
   affiche_status (getpid ());
   
   fprintf (stdout, "Je suis le père et je crée un fils !\n");
   if ((pid = fork ()) == -1) 
   {
      perror ("fork ()");
      exit (1);
   }
   if (pid != 0) 
   {      
      fprintf (stdout, "Je suis le père et je m'endors pendant 5 s !\n\n");
      sleep (5);
      fprintf (stdout, "Je suis le père et je consulte l'état de mon fils ...\n");
      fprintf (stdout, "État attendu = zombie\n");
      affiche_status (pid);
      fprintf (stdout, "Je suis le père et j'attends la fin de mon fils ...\n");
      wait (NULL);
      fprintf (stdout, "Je suis le père et je consulte l'état de mon fils ...\n");
      fprintf (stdout, "État attendu = inexistant\n");
      affiche_status (pid);
   } 
   else 
   {
      fprintf (stdout, "Je suis le fils : PID = %u\n", getpid());
      fprintf (stdout, "Je suis le fils et je consulte l'état de mon père ...\n");
      fprintf (stdout, "État attendu = sleeping\n");
      affiche_status (getppid ());	
      fprintf (stdout, "Je suis le fils et je me termine.\n\n");
      exit (0);
   }

   exit (0);
}
