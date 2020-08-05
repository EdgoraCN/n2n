#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdarg.h>
#include "sn_redis.h"

redisContext *get_redis_context(n2n_sn_t *sss){
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    redisContext *c = redisConnectWithTimeout(sss->redis.host, sss->redis.port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            traceEvent(TRACE_ERROR, "Connection error: %s",  c->errstr);
            redisFree(c);
        } else {
            traceEvent(TRACE_ERROR, "Connection error: can't allocate redis context");
        }
        return NULL;
    }
    redisReply *reply;
    reply = redisCommand(c, "AUTH %s" , sss->redis.password);
    traceEvent(TRACE_DEBUG, "AUTH %s [%s]", sss->redis.password,reply->str);
    if(strcmp(reply->str,"OK")!=0){
        traceEvent(TRACE_ERROR, "login error: can't login with [%s], msg = %s", sss->redis.password,reply->str);
        freeReplyObject(reply);
        return NULL;
    }
    freeReplyObject(reply);

    reply = redisCommand(c,"SELECT %d",sss->redis.db);
    traceEvent(TRACE_DEBUG, "SELECT %d [%s]", sss->redis.db , reply->str);
    freeReplyObject(reply);

    /* PING server */
    //reply = redisCommand(c,"PING");
    //printf("PING: %s\n", reply->str);
    //freeReplyObject(reply);
    return c;
}
void hget_str(redisContext *c, char *namespace,char *map,char *key,char *reply_str, int size){
    redisReply *reply;
    reply = redisCommand(c,"HGET %s:%s %s",namespace,map,key);
    traceEvent(TRACE_DEBUG, "HGET %s:%s %s = %s", namespace,map,key,reply->str);
    if(reply->str!=NULL){
        strncpy( reply_str, reply->str,size);
    }
    freeReplyObject(reply);
}
int hget_int(redisContext *c, char *namespace,char *map,char *key){
    redisReply *reply;
    reply = redisCommand(c,"HGET %s:%s %s",namespace,map,key);
    traceEvent(TRACE_DEBUG, "HGET %s:%s %s = %d", namespace,map,key,reply->integer);
    int value = reply->integer;
    freeReplyObject(reply);
    return value;
}
void hset_str(redisContext *c, char *namespace,char *map,char *key,char *value){
    redisReply *reply;
    reply = redisCommand(c,"HSET %s:%s %s %s",namespace,map,key,value);
    traceEvent(TRACE_DEBUG, "HSET %s:%s %s = %s [%s]", namespace,map,key,value,reply->str);
    freeReplyObject(reply);
}
void hset_int(redisContext *c, char *namespace,char *map,char *key,int value){
    redisReply *reply;
    reply = redisCommand(c,"HSET %s:%s %s %d",namespace,map,key,value);
    traceEvent(TRACE_DEBUG, "HSET %s:%s %s = %d [%s]", namespace,map,key,value,reply->str);
    freeReplyObject(reply);
}
void get_comm_conf(char *community_name,redisContext *c, redis_comm_conf *comm_conf){
    hget_str(c,"comm",community_name,"secret",comm_conf->secret,N2N_COMMUNITY_SIZE);
    hget_str(c,"comm",community_name,"owner",comm_conf->owner,N2N_COMMUNITY_SIZE);
    hget_str(c,"comm",community_name,"id",comm_conf->community,N2N_COMMUNITY_SIZE);
    hget_str(c,"comm",community_name,"subnet",comm_conf->subnet,N2N_COMMUNITY_SIZE);
}
void get_edge_conf(n2n_REGISTER_SUPER_t *reg,redisContext *c, redis_edge_conf *edge_conf){
    traceEvent(TRACE_DEBUG, "reg->auth.token=%s", (char*)reg->auth.token);
    hget_str(c,"edge", (char*)reg->auth.token,"secret",edge_conf->secret,N2N_COMMUNITY_SIZE);
    hget_str(c,"edge", (char*)reg->auth.token,"owner",edge_conf->owner,N2N_COMMUNITY_SIZE);
    hget_str(c,"edge", (char*)reg->auth.token,"community",edge_conf->community,N2N_COMMUNITY_SIZE);
    hget_str(c,"edge", (char*)reg->auth.token,"ip",edge_conf->ip,N2N_COMMUNITY_SIZE);
    hget_str(c,"edge", (char*)reg->auth.token,"id",edge_conf->id,N2N_COMMUNITY_SIZE);
    edge_conf->online = hget_int(c,"edge",(char*)reg->auth.token,"online");
}
void get_redis_data(char *community_name,n2n_sn_t *sss,n2n_REGISTER_SUPER_t *reg, redis_comm_conf *comm_conf, redis_edge_conf *edge_conf ){
    redisContext *c = get_redis_context(sss);
    if(c == NULL ){
        return;
    }
    get_comm_conf(community_name,c,comm_conf);
    get_edge_conf(reg,c,edge_conf);
    redisFree(c);
}
void update_edge_conf(n2n_sn_t *sss,redis_edge_conf *edge_conf,int online,char *macaddr){
    redisContext *c = get_redis_context(sss);
    if(c == NULL ){
        return;
    }
    hset_int(c,"edge",edge_conf->secret,"online",online);
    hset_str(c,"edge",edge_conf->secret,"mac",macaddr);
    redisFree(c);
}
void trace_log(int level,char *format, ...){
    va_list ap;
    va_start(ap,format);
    printf(format,ap);
    va_end(ap);
    
}