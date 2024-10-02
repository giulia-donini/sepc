//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Fonctions 
void *etoile();
void *diese();


// Programme principal
int main(void) 
{

  setbuf(stdout, NULL);
  printf("Je vais lancer 2 fonctions.\n");

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
