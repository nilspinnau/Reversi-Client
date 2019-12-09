typedef struct {
    char game_kind[10];
    char host_name[35];
    unsigned short port_nr;
}configs;

configs *getconfig(configs* res, char * path); 