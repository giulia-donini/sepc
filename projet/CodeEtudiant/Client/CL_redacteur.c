#include "Client/CL_include"
#include "Client/CL_redacteur.h"
#include "Client/CL_driver.h"

void redacteur(int voie) {
    int batch[5];
    int count = 0;

    while (1) {
        if (count == 5) {
            send_to_driver(batch, 5, voie);
            count = 0;
        }
    }
}