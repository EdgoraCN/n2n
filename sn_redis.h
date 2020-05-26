#include "n2n.h"
#include <hiredis.h>

typedef struct redis_community_conf
{
    char community[N2N_COMMUNITY_SIZE]; 
    char subnet[N2N_COMMUNITY_SIZE]; /** network**/
    char secret [N2N_COMMUNITY_SIZE]; /* community secret */
    char owner[N2N_COMMUNITY_SIZE]; 
} redis_comm_conf;
typedef struct redis_edge_conf
{
    char community[N2N_COMMUNITY_SIZE]; 
    char secret[N2N_AUTH_TOKEN_SIZE]; /** auth secret**/
    char id[N2N_COMMUNITY_SIZE]; /* edge id */
    char owner[N2N_COMMUNITY_SIZE];
    char ip[N2N_COMMUNITY_SIZE]; /* edge ip*/
    int online;
} redis_edge_conf;


void get_redis_data(char *community_name,n2n_sn_t *sss,n2n_REGISTER_SUPER_t *reg, redis_comm_conf *comm_conf, redis_edge_conf *edge_conf );
void get_comm_conf(char *community_name,redisContext *c, redis_comm_conf *comm_conf);
void get_edge_conf(n2n_REGISTER_SUPER_t *reg,redisContext *c, redis_edge_conf *edge_conf);
void hget_str(redisContext *c, char *prefix,char *map,char *key,char *reply_str, int size);
int hget_int(redisContext *c, char *prefix,char *map,char *key);
redisContext *get_redis_context(n2n_sn_t *sss);
void hset_str(redisContext *c, char *prefix,char *map,char *key,char *value);
void hset_int(redisContext *c, char *prefix,char *map,char *key,int value);
void update_edge_conf(n2n_sn_t *sss,redis_edge_conf *edge_conf,int online,char *macaddr);