#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../lib/getconfig.h"


configs *getconfig(configs *res, char * path){
    char *line = malloc(256*sizeof(char));
    char *varstr = malloc(256*sizeof(char));
    char *vartwo;
    char *buffl = line;
    char *buffv = varstr;
    int foundu = 0;
    //memset(&res,0,sizeof(configs));
    FILE *fp;
    printf("%s",path);
    

    // has to be changed to path, throws error that path cant be found
    fp = fopen("config.conf", "r+");
    if(fp == NULL){
        free(varstr);
        free(line);
        perror("Cant open file");
        return NULL;
    }

    while(fgets(line,256,fp)!= 0){

        while(*line != '\n'){
            if(!isspace(*line)){
                if(*line == '='){
                    foundu = 1;
                    *varstr = '\0';
                    vartwo = varstr +1;
                }
                else{
                    *varstr = *line;
                }
                varstr++;

            }
            line++;
        }
        *varstr = '\0';
        line = buffl;
        varstr = buffv;
        if(foundu == 1){
            if(0 == strcmp(varstr,"game_kind")) strcpy(res->game_kind, vartwo);
            if(0 == strcmp(varstr,"port_nr")) res->port_nr = (uint16_t) atoi(vartwo);
            if(0 == strcmp(varstr,"host_name")) strcpy(res->host_name, vartwo);
        }
        foundu = 0;
    }
    /*
    if((res->game_kind == NULL)||(res->port_nr == NULL)||(res->host_name == NULL)){
        printf("config.conf missing content");
      return NULL;
    }
    //htons(res.port_nr);
    */
    fclose(fp);
    free(line);
    free(varstr);
    return res;
}