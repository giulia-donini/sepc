#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Fonctions 
void *etoile();
void *diese();


// Programme principal
int main(void) 
{

  pthread_t thread1, thread2;

  setbuf(stdout, NULL);
  printf("Je vais lancer 2 fonctions.\n");

  pthread_create(&thread1, NULL, etoile, NULL);
  pthread_create(&thread2, NULL, diese, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  // problem: they are executed sequentially
  etoile(); 
  diese();

  return EXIT_SUCCESS;
}



// Fonctions
void *etoile() 
{
  int i;
  char c1 = '*';
   
  for(i=1;i<=200;i++)
  {
      write(1, &c1, 1);
  }
}

void *diese() 
{
  int i;
  char c1 = '#';
   
  for(i=1;i<=200;i++)
  {
      write(1, &c1, 1);
  }
}
