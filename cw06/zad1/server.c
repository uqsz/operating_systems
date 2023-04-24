#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <sys/wait.h>
#include <stdbool.h>


#define MAX_TEXT 1024

int size=0;
char buffer[MAX_TEXT];
int msgid_server;

typedef enum{
    INIT=1,
    LIST=2,
    ALL=3,
    ONE=4,
    CHECK=5,
    STOP=6,
    STOP_ALL=7,
    INVALID=8
} ACTION;

struct usr{
    int id;
    int msgid_usr;
    bool active;
};

struct msg{
    ACTION to_do;
    char text[MAX_TEXT];
    int id;
    int id_one;
};


void add_client(struct msg curr_msg,struct usr *clients){
    struct usr new_user;
    new_user.id=size;
    new_user.msgid_usr=atoi(curr_msg.text);
    new_user.active=true;
    clients[size]=new_user;
    size++;

    struct msg re_msg;
    sprintf(buffer, "%d", clients[size-1].id);
    re_msg.to_do=INIT;
    strcpy(re_msg.text,buffer);
    msgsnd(clients[size-1].msgid_usr, &re_msg, MAX_TEXT, 0);
}

void list_client(struct msg curr_msg,struct usr *clients){
    char res[248]="";
    char buffer1[10];
    for (int i=0;i<size;i++){
        if(clients[i].active){
            sprintf(buffer1, "%d", i);
            strcat(res, buffer1);
            strcat(res, " ");
        }
    }
    struct msg re_msg;
    re_msg.to_do=LIST;
    strcpy(re_msg.text,res);
    msgsnd(clients[curr_msg.id].msgid_usr, &re_msg, MAX_TEXT, 0);
}

void all_client(struct msg curr_msg,struct usr *clients){
    struct msg new_msg;
    new_msg.to_do=ALL;
    strcpy(new_msg.text,curr_msg.text);
    for (int i=0;i<size;i++){
        if(clients[i].active && i!=curr_msg.id){
            msgsnd(clients[i].msgid_usr, &new_msg, MAX_TEXT, 0);
        }
    }
    struct msg re_msg;
    re_msg.to_do=ALL;
    strcpy(re_msg.text,"Message has been sent successfully!");
    msgsnd(clients[curr_msg.id].msgid_usr, &re_msg, MAX_TEXT, 0);
}

void one_client(struct msg curr_msg,struct usr *clients){
    char buffer[MAX_TEXT];
    strcpy(buffer,curr_msg.text);

    int send_id=curr_msg.id_one;
    printf("%d\n",curr_msg.id_one);

    struct msg re_msg;
    re_msg.to_do=ONE;

    if (send_id<size && clients[send_id].active){
        struct msg new_msg;
        new_msg.to_do=ONE;
        strcpy(new_msg.text,curr_msg.text);
        msgsnd(clients[send_id].msgid_usr, &new_msg, MAX_TEXT, 0);
        strcpy(re_msg.text,"Direct message has been sent successfully!");
    }
    else{
        strcpy(re_msg.text,"User is no longer active!");
    }
    msgsnd(clients[curr_msg.id].msgid_usr, &re_msg, MAX_TEXT, 0);
}

void stop_client(struct msg curr_msg,struct usr *clients){
    clients[curr_msg.id].active=false;
    struct msg re_msg;
    re_msg.to_do=STOP;
    strcpy(re_msg.text,"You have been disconnected!");
    msgsnd(clients[curr_msg.id].msgid_usr, &re_msg, MAX_TEXT, 0);
}

void stop_handler(){
    struct msg new_msg;
    new_msg.to_do=STOP_ALL;
    msgsnd(msgid_server, &new_msg, MAX_TEXT, 0);
}


void stop_all(struct usr *clients){
    struct msg new_msg;
    new_msg.to_do=STOP_ALL;
    strcpy(new_msg.text,"Server stopped! All queues has been deleted.");
    for (int i=0;i<size;i++){
        if(clients[i].active){
            msgsnd(clients[i].msgid_usr, &new_msg, MAX_TEXT, 0);
        }
    }
    msgctl(msgid_server,IPC_RMID,0);
    exit(EXIT_SUCCESS);
}

int main(){
    // stworzenie tablicy zawierającą dane klientów
    struct usr clients[20];

    // stworzenie kolejki komunikatow serwera
    msgid_server=msgget((key_t) 12345, 0666|IPC_CREAT);
    

    struct msg curr_msg;

    signal(SIGINT,stop_handler);

    // obsluga przychodzących komunikatów
    while(1){
        msgrcv(msgid_server, &curr_msg, MAX_TEXT, 0,0);
        switch (curr_msg.to_do){
        case INIT:
            add_client(curr_msg,clients);
            printf("ID: %d -> INIT | KEY: %d\n",size-1,clients[size-1].msgid_usr);
            break;
        case LIST:
            list_client(curr_msg,clients);
            printf("ID: %d -> LIST\n",curr_msg.id);
            break;
        case ALL:
            all_client(curr_msg,clients);
            printf("ID: %d -> 2ALL\n",curr_msg.id);
            break;
        case ONE:
            one_client(curr_msg,clients);
            printf("ID: %d -> 2ONE\n",curr_msg.id);
            break;
        case STOP:
            stop_client(curr_msg,clients);
            printf("ID: %d -> STOP\n",curr_msg.id);
            break;
        case STOP_ALL:
            stop_all(clients);
        default:
            break;
        }
    }
}

