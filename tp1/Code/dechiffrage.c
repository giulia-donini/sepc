/* Maintenant que vous possédez la valeur X de la clef, vous pouvez construire
un filtre en C pour déchiffrer le message en une simple commande :
$ cat messageChiffre | ./dechiffrage X
Votre filtre lira un à un les caractères sur l’entrée standard, soustraira la
valeur de la clef et écrira les caractères ainsi obtenus sur la sortie standard.
Enfin, plutôt que d’entrer la valeur de la clef à la main, écrivez en une ligne la
commande permettant de récupérer la clef et de déchiffrer le message. Il ne
vous aura pas échappé que ce filtre vous permettra également de chiffrer un
message avec une clef que vous aurez vous-même choisie. */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Convert the argument (key) to an integer
    int key = atoi(argv[1]);

    // Read characters one by one from standard input
    int ch;
    while ((ch = getchar()) != EOF) {
        // Decrypt the character by subtracting the key
        putchar(ch - key);
    }

    return 0;
}
