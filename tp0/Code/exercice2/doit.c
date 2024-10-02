#include <stdio.h>
#include <unistd.h>  // Pour execvp
#include <stdlib.h>  // Pour exit

int main(int argc, char *argv[]) {
    // Vérifie si la commande est passée en paramètre
    if (argc < 2) {
        fprintf(stdout, "Utilisation: %s <commande> [arguments]\n", argv[0]);
        exit(EXIT_FAILURE);  // Quitte avec une erreur
    }

    // Exécute la commande avec ses arguments
    // execvp doesnt need to pass each argument, just the address of the first one
    execvp(argv[1], &argv[1]);

    exit(EXIT_FAILURE);  // Quitte avec une erreur si execvp échoue
}
