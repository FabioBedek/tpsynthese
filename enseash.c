#include <stdio.h>
//#include <POSIX.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "toolcase.h"
#include <string.h>
#include <stdlib.h>

int main(void)
{ //Affichage du message d'accueil 
    InitShell();

  //Boucle principale du shell
  char command[1024];
  while(1){
    Prompt(); // Affichage du prompt "enseash"
    // Lecture de la commande utilisateur
    ReadCommand(command, 1024);
    // Si tape exit on quitte
    if (strcmp(command, "exit") == 0){
        write(STDOUT_FILENO, "Goodbye!\n",9);
        exit(EXIT_SUCCESS);
    } 
    write(STDOUT_FILENO, "Unknown command.\n", 17);
  }
return 0;
}




