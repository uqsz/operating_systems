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
#define MAX_DATE 128


char buffer[MAX_TEXT];
char input[MAX_TEXT]=""; 
char command[MAX_TEXT]="";
char value1[MAX_TEXT]="";
char value2[MAX_TEXT]=""; 

int id_local;
int msgid_server;
int msgid_local;
bool server_active;

typedef enum{
    INIT=1,
    LIST=2,
    ALL=3,
    ONE=4,
    CHECK=5,
    STOP=6,
    STOP_ALL=7,
    INVALID=8,
    INFORM=9
} ACTION;

struct msg{
    long int to_do;
    char text[MAX_TEXT];
    int id;
    int id_one;
};

void get_time(struct msg send_msg) {

}

void list_server(struct msg send_msg){
    send_msg.to_do=LIST;
    msgsnd(msgid_server, &send_msg, sizeof(send_msg), 0);
    msgrcv(msgid_local, &send_msg, sizeof(send_msg), LIST,0);
    printf("%s",send_msg.text);

}

void all_server(struct msg send_msg, char* mess){
    send_msg.to_do=ALL;
    sprintf(buffer, "%d->ALL: %s", id_local, mess);
    strcpy(send_msg.text,buffer);
    msgsnd(msgid_server, &send_msg, sizeof(send_msg), 0);

    msgrcv(msgid_local, &send_msg, sizeof(send_msg), ALL,0);
    printf("%s\n",send_msg.text);
}

void one_server(struct msg send_msg, int client_id, char* mess){
    send_msg.to_do=ONE;
    send_msg.id_one=client_id;

    sprintf(buffer, "%d->%d: %s", id_local, client_id, mess);
    strcpy(send_msg.text, buffer);
    msgsnd(msgid_server, &send_msg, sizeof(send_msg), 0);

    msgrcv(msgid_local, &send_msg, sizeof(send_msg), ONE,0);
    printf("%s\n",send_msg.text);

}

int check(){
    struct msqid_ds buf;
    msgctl(msgid_local, IPC_STAT, &buf);
    int cnt=buf.msg_qnum;
    printf("You've got %d messages!\n",cnt);
    struct msg re_msg;
    for (int i=0; i<cnt; i++){
        msgrcv(msgid_local, &re_msg, sizeof(re_msg), 0,0);
        if(re_msg.to_do==STOP_ALL) server_active=false;
        printf("%s\n",re_msg.text);
    }
    return 0;
}

void stop_server(){
    struct msg send_msg;
    send_msg.id=id_local;
    send_msg.to_do=STOP;
    msgsnd(msgid_server, &send_msg, sizeof(send_msg), 0);
    check();
    if (server_active){
        msgrcv(msgid_local, &send_msg,sizeof(send_msg), STOP,0);
        printf("%s\n",send_msg.text);
    }
    msgctl(msgid_local,IPC_RMID,0);
    exit(EXIT_SUCCESS);
}

ACTION convert(const char* command){
    if(strcmp(command,"list")==0) return LIST;
    if(strcmp(command,"2all")==0) return ALL;
    if(strcmp(command,"2one")==0) return ONE;
    if(strcmp(command,"stop")==0) return STOP;
    if(strcmp(command,"check")==0) return CHECK;
    return INVALID;
}

int main(){
    msgid_server=msgget((key_t) 12345, 0666|IPC_CREAT);
    msgid_local=msgget(ftok(".", rand() % 256 + getpid()), 0666|IPC_CREAT);

    struct msg send_msg;
    send_msg.to_do=INIT;
    sprintf(buffer, "%d", msgid_local);
    strcpy(send_msg.text,buffer);
    msgsnd(msgid_server, &send_msg, sizeof(send_msg), 0);

    msgrcv(msgid_local, &send_msg, sizeof(send_msg), INIT,0);
    id_local=send_msg.id;
    printf("You have just connected to server! Your ID: %d\n",id_local);

    server_active=true;

    signal(SIGINT,stop_server);

    while(1){
        printf(">");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%s %s %s", command, value1, value2);
        ACTION to_do=convert(command);
        switch (to_do){
            case LIST:
                list_server(send_msg);
                break;
            case ALL:
                all_server(send_msg,value1);
                break;
            case ONE:
                one_server(send_msg,atoi(value1),value2);
                break;
            case STOP:
                stop_server(send_msg);
                break;
            case CHECK:
                check();
                break;
            case INVALID:
                printf("Invalid command!");
                break;
            default:
                break;
        }
    }

}