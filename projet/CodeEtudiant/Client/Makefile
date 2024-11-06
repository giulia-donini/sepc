CC = gcc
CFLAGS = -Wall -D _DEFAULT_SOURCE -g
LECTEUR = ./CL_lecteur.c
MONITEUR = ./CL_moniteur.c
MONITEUR = ./CL_moniteur.c
REDACTEUR = ./CL_redacteur.c
DRIVER = ./CL_driver.c

OBJETS = CL_lecteur.o CL_moniteur.o CL_redacteur.o CL_driver.o

all: client CL

client: $(OBJETS) CL.c
	$(CC) $(CFLAGS) CL.c $(OBJETS) -o CL

CL_moniteur.o: $(MONITEUR)
	$(CC) -c $(CFLAGS) $(MONITEUR) -o CL_moniteur.o

CL_lecteur.o: $(LECTEUR)
	$(CC) -c $(CFLAGS) $(LECTEUR) -o CL_lecteur.o

CL_redacteur.o: $(REDACTEUR)
	$(CC) -c $(CFLAGS) $(REDACTEUR) -o CL_redacteur.o

CL_driver.o: $(DRIVER)
	$(CC) -c $(CFLAGS) $(DRIVER) -o CL_driver.o

clean:
	@rm -f *~ *.o driver CL