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

char input[MAX_TEXT]=""; 
char command[MAX_TEXT]="";
char value1[MAX_TEXT]="";
char value2[MAX_TEXT]=""; 
char buffer[MAX_TEXT]="";
int id_local;
int msgid_server;
int msgid_local;
char time_buff[20];

typedef enum{
    INIT,
    RESPOND,
    INFORM,
    CHECK,
    LIST,
    ALL,
    ONE,
    STOP,
    INVALID,
} ACTION;

struct msg{
    ACTION to_do;
    char text[MAX_TEXT];
};

void get_time(){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(time_buff, sizeof(time_buff), "%d.%m.%Y", tm);
}

void list_server(){
    struct msg send_msg;
    send_msg.to_do=LIST;
    sprintf(buffer, "%d", id_local);
    strcpy(send_msg.text,buffer);
    msgsnd(msgid_server, &send_msg, MAX_TEXT, 0);

    struct msg re_msg;
    msgrcv(msgid_local, &re_msg, MAX_TEXT, 0,0);
    printf("Active clients IDs: %s\n",re_msg.text);
}

void all_server(char* mess){
    struct msg send_msg;
    send_msg.to_do=ALL;
    strcpy(buffer,"");

    get_time();

    sprintf(buffer, "%d@%s@%s", id_local, time_buff, mess);
    strcpy(send_msg.text,buffer);
    msgsnd(msgid_server, &send_msg, MAX_TEXT, 0);

    struct msg re_msg;
    msgrcv(msgid_local, &re_msg, MAX_TEXT, 0,0);
    printf("%s\n",re_msg.text);
}

void one_server(char* mess, int client_id){
    struct msg send_msg;
    send_msg.to_do=ONE;
    strcpy(buffer,"");
    get_time();
    sprintf(buffer, "%d@%d@%s@%s", id_local, client_id, time_buff, mess);
    strcpy(send_msg.text, buffer);
    msgsnd(msgid_server, &send_msg, MAX_TEXT, 0);

    struct msg re_msg;
    msgrcv(msgid_local, &re_msg, MAX_TEXT, 0,0);
    printf("%s\n",re_msg.text);
}

void stop_server(){
    struct msg send_msg;
    send_msg.to_do=STOP;
    strcpy(buffer,"");

    sprintf(buffer, "%d", id_local);
    strcpy(send_msg.text,buffer);
    msgsnd(msgid_server, &send_msg, MAX_TEXT, 0);

    struct msg re_msg;
    msgrcv(msgid_local, &re_msg, MAX_TEXT, 0,0);
    printf("%s\n",re_msg.text);
    msgctl(msgid_local,IPC_RMID,0);

    exit(EXIT_SUCCESS);
}

void check(){
    struct msqid_ds buf;
    msgctl(msgid_local, IPC_STAT, &buf);
    int cnt=buf.msg_qnum;
    printf("You've got %d messages!\n",cnt);
    for (int i=0; i<cnt; i++){
        struct msg re_msg;
        msgrcv(msgid_local, &re_msg, MAX_TEXT, 0,0);
        printf("%s\n",re_msg.text);
    }
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
    // otworz kolejke serwera
    msgid_server=msgget((key_t) 12345, 0666|IPC_CREAT);

    // otworz kolejke lokalna
    msgid_local=msgget(ftok(".", rand() % 256), 0666|IPC_CREAT);

    // wyslanie komunikatu INIT do serwera
    struct msg send_msg;
    send_msg.to_do=INIT;
    sprintf(buffer, "%d", msgid_local);
    strcpy(send_msg.text,buffer);
    msgsnd(msgid_server, &send_msg, MAX_TEXT, 0);

    // odebranie swojego id z serwera
    struct msg re_msg;
    msgrcv(msgid_local, &re_msg, MAX_TEXT, 0,0);
    id_local=atoi(re_msg.text);
    printf("You have just connected to server! Your ID: %d\n",id_local);

    signal(SIGINT,stop_server);

    while(1){
        printf(">");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%s %s %s", command, value1, value2);
        ACTION to_do=convert(command);
        switch (to_do){
            case LIST:
                list_server();
                break;
            case ALL:
                all_server(value1);
                break;
            case ONE:
                one_server(value2,atoi(value1));
                break;
            case STOP:
                stop_server();
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

