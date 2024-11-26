#include "Client/CL_include"
#include "Client/CL_redacteur.h"
#include "Client/CL_driver.h"
#include "Client/CL_moniteur.h"

void redacteur(int voie){
	static int times[2];
	static int buffer1[5];
	static int buffer2[5];

	int* buffer[2];
	buffer[0] = buffer1;
	buffer[1] = buffer2;

    read(pipe_st[voie][0], &buffer[voie][times[voie]], sizeof(int));
    times[voie]++;
    
    if (times[voie] == 5) {
        send_to_driver(buffer[voie], 5, voie);
        times[voie] = 0;
    }
}
