#include "Client/CL_include"
#include "Client/CL_moniteur.h"
#include "Client/CL_lecteur.h"

void lecteur(int voie) {
    while (1) {
        pause();  // Wait for signal

        if (shared_mem_ptr) {
            int data = shared_mem_ptr[voie - 1].tampon[shared_mem_ptr[voie - 1].n];
            printf("Lecteur (Voie %d) : Donnée lue depuis la mémoire partagée : %d\n", voie, data);

            // Send the data to Redacteur (this part can be implemented)
        }
    }
}
/*/////////////////////////////////////////////////////////////////////////*/
	   
void litbuf1() /* semaphore 0 */
{
	voie=1;

    // Lire et afficher les données du buffer associé à la voie 1
    if (shared_mem_ptr) {
        printf("Donnée voie 1 : %d\n", shared_mem_ptr->tampon[shared_mem_ptr->n]);
    }   

	signal(SIGUSR1,litbuf1);
}


/*/////////////////////////////////////////////////////////////////////////*/

void litbuf2()
{
	voie=2;	

    // Lire et afficher les données du buffer associé à la voie 2
    if (shared_mem_ptr) {
        printf("Donnée voie 2 : %d\n", shared_mem_ptr->tampon[shared_mem_ptr->n]);
    }

	signal(SIGUSR2,litbuf2);
}