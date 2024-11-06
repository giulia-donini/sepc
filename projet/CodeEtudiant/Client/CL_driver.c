#include "Client/CL_include"
#include "Client/CL_driver.h"

void send_to_driver(int *data, int size, int voie) {
    printf("Driver (Voie %d): ", voie);
    for (int i = 0; i < size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
}