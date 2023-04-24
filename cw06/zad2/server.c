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
#define MAX_DATE 256

int size=0;
char buffer[MAX_TEXT]="";
int msgid_server;

typedef enum{
    STOP=1,
    LIST=2,
    ALL=3,
    ONE=4,
    CHECK=5,
    INIT=6,
    STOP_ALL=7,
    INVALID=8,
    INFORM=9
} ACTION;

struct msg{
    long int to_do;
    char text[MAX_TEXT];
    char date[MAX_DATE];
    int id;
    int id_one;
};

struct usr{
    int id;
    int msgid_usr;
    bool active;
};

struct usr clients[20];

void add_client(struct msg curr_msg){
    struct usr new_user;
    new_user.id=size;
    new_user.msgid_usr=atoi(curr_msg.text);
    new_user.active=true;
    clients[size]=new_user;
    size++;

    curr_msg.id=size-1;
    msgsnd(clients[size-1].msgid_usr, &curr_msg, sizeof(curr_msg), 0);
}

void list_client(struct msg curr_msg){
    char res[248]="";
    char buffer1[10];
    for (int i=0;i<size;i++){
        if(clients[i].active){
            sprintf(buffer1, "%d", i);
            strcat(res, buffer1);
            strcat(res, " ");
        }
    }
    strcpy(curr_msg.text,res);
    msgsnd(clients[curr_msg.id].msgid_usr, &curr_msg, sizeof(curr_msg), 0);
    
}

void all_client(struct msg curr_msg){
    struct msg new_msg;
    new_msg.to_do=INFORM;
    strcpy(new_msg.text,curr_msg.text);
    for (int i=0;i<size;i++){
        if(clients[i].active && i!=curr_msg.id){
            msgsnd(clients[i].msgid_usr, &new_msg, sizeof(new_msg), 0);
        }
    }
    strcpy(curr_msg.text,"Message has been sent successfully!");
    msgsnd(clients[curr_msg.id].msgid_usr, &curr_msg, sizeof(curr_msg), 0);
}

void one_client(struct msg curr_msg){
    strcpy(buffer,curr_msg.text);
    if (curr_msg.id_one<size && clients[curr_msg.id_one].active){
        struct msg new_msg;
        new_msg.to_do=INFORM;
        strcpy(new_msg.text,curr_msg.text);
        msgsnd(clients[curr_msg.id_one].msgid_usr, &new_msg, sizeof(new_msg), 0);
        strcpy(curr_msg.text,"Direct message has been sent successfully!");
    }
    else{
        strcpy(curr_msg.text,"User is no longer active!");
    }
    msgsnd(clients[curr_msg.id].msgid_usr, &curr_msg, sizeof(curr_msg), 0);
}

void stop_client(struct msg curr_msg){
    clients[curr_msg.id].active=false;
    strcpy(curr_msg.text,"You have been disconnected!");
    msgsnd(clients[curr_msg.id].msgid_usr, &curr_msg, sizeof(curr_msg), 0);
}

void stop_all(){
    struct msg new_msg;
    new_msg.to_do=STOP_ALL;
    strcpy(new_msg.text,"Server stopped! All queues has been deleted.");
    for (int i=0;i<size;i++){
        if(clients[i].active){
            msgsnd(clients[i].msgid_usr, &new_msg, sizeof(new_msg), 0);
        }
    }
    msgctl(msgid_server,IPC_RMID,0);
    printf("\nServer is down!\n");
    exit(EXIT_SUCCESS);
}


int main(){
    msgid_server=msgget((key_t) 12345, 0666|IPC_CREAT);
    struct msg curr_msg;
    signal(SIGINT,stop_all);

    while(1){
        msgrcv(msgid_server, &curr_msg, sizeof(curr_msg), 0,0);
        switch (curr_msg.to_do){
        case INIT:
            add_client(curr_msg);
            printf("ID: %d -> INIT | KEY: %d\n",size-1,clients[size-1].msgid_usr);
            break;
        case LIST:
            list_client(curr_msg);
            printf("ID: %d -> LIST\n",curr_msg.id);
            break;
        case ALL:
            all_client(curr_msg);
            printf("ID: %d -> 2ALL\n",curr_msg.id);
            break;
        case ONE:
            one_client(curr_msg);
            printf("ID: %d -> 2ONE\n",curr_msg.id);
            break;
        case STOP:
            stop_client(curr_msg);
            printf("ID: %d -> STOP\n",curr_msg.id);
            break;
        default:
            break;
        }
    }
}