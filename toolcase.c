#include <unistd.h>
#include "constantes.h"
#include <string.h>

#include <stdio.h>

//Affiche le message d'accueil 
void InitShell(){
    write(STDOUT_FILENO, Message_Bienvenue, strlen(Message_Bienvenue));
}
//Affiche Prompt
void Prompt(){
    //UserID = getuid();
    //write(STDOUT_FILENO, UserID, strlen(UserID));
    write(STDOUT_FILENO, Message_Prompt, strlen(Message_Prompt));
}

void NewShell(){
    int fd = open("enseash", O_WRONLY | O_CREAT, 0644);
    if (fd==-1){
        perror("error_open_enseash");
        exit(EXIT_FAILURE);
    }
}

void ReadCommandSimple(char Buffer[]){
    struct timespec start, end;
    ssisze_t retour=read(STDIN_FILENO, Buffer, BUFFER_SIZE);
    Buffer[retour-1]=0;

    if (strcmp(Buffer, "exit")==0){
        exit(EXIT_SUCCESS);

    clock_gettime(CLOCK_MONOTONIC, &start);
    if (fork()==0){
        execlp(Buffer, Buffer, NULL);
        exit(EXIT_FAILURE);
    }
    wait(NULL);
    clock_gettime(CLOCK_MONOTONIC, &end);

    char prompt[128]={0};
    int duree = ((end.tv_sec-start.tv_sec)*1000)+((end.tv_nsec-start.tv_nsec)/1000000);
    sprintf(prompt,"t: %dms\n", duree);
    write(STDOUT_FILENO, prompt, strlen(prompt));
}
// Lis la commande utilisateur 
// Lit la commande utilisateur
void ReadCommand(char *buffer, size_t buffer_size) {
    ssize_t bytes_read = read(STDIN_FILENO, buffer, buffer_size);
    if (bytes_read > 0) {
        buffer[bytes_read - 1] = '\0'; // Remplace le caractère '\n' par '\0'
    }
}

void readCommandComplexe(char Buffer[]){
    const char *separators = ",.-|";
    // recherche à récup un par un tous le smots token du buffer 
    char * strToken = strtok ( Buffer, separators);
    readCommandSimple(strToken);
    while(strToken != NULL){
        printf("%s\n", strToken);
        // token d'après 
        strToken = strtok (NULL, separators);
        readCommandSimple(strToken);
    }
}

// execute une commande simple (sans arguments)
void ExecuteCommand(const char *command){
    pid_t pid = fork(); // crée un processus enfant 
    if (pid ==0) {
        // Processus enfant : tente d'exécuter la commande
        execlp(command, command, NULL);
        // Si execlp échoue, affiche un message d'erreur 
        perror("Command execution failed");
        exit(EXIT_FAILURE);
    }   else if (pid>0){
        // Processus parent : attend la fin du processus enfant
        int status; 
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)){
            char exit_message[64];
            snprintf(exit_message, sizeof(exit_message), "Bye bye: %d\n", WEXITSTATUS(status));
            write(STDOUT_FILENO, exit_message, strlen(exit_message));

        }
    } else {
        // Erreur lors du fork 
        perror("Fork failed");
    }
}



