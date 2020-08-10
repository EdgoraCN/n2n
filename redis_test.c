#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdarg.h>
#include <hiredis.h>

int main(int argc, char **argv) {
    unsigned int j, isunix = 0;
    redisContext *c;
    redisReply *reply;
    const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";

    if (argc > 2) {
        if (*argv[2] == 'u' || *argv[2] == 'U') {
            isunix = 1;
            /* in this case, host is the path to the unix socket */
            printf("Will connect to unix socket @%s\n", hostname);
        }
    }

    int port = (argc > 2) ? atoi(argv[2]) : 6379;

    const char *password = (argc > 3) ? argv[3] : "password";


    int dbnum = (argc > 4) ? atoi(argv[4]) : 0;


    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    if (isunix) {
        c = redisConnectUnixWithTimeout(hostname, timeout);
    } else {
        c = redisConnectWithTimeout(hostname, port, timeout);
    }
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

    reply = redisCommand(c, "AUTH %s" , password);
    printf("AUTH: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"SELECT %d",dbnum);
    printf("SELECT %d: %s\n", dbnum , reply->str);
    freeReplyObject(reply);

    /* PING server */
    reply = redisCommand(c,"PING");
    printf("PING: %s\n", reply->str);
    freeReplyObject(reply);

    /* Set a key */
    reply = redisCommand(c,"SET %s %s", "foo", "hello world");
    printf("SET: %s\n", reply->str);
    freeReplyObject(reply);

    /* Set a key using binary safe API */
    reply = redisCommand(c,"SET %b %b", "bar", (size_t) 3, "hello", (size_t) 5);
    printf("SET (binary API): %s\n", reply->str);
    freeReplyObject(reply);

    /* Try a GET and two INCR */
    reply = redisCommand(c,"GET foo");
    printf("GET foo: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"INCR counter");
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);
    /* again ... */
    reply = redisCommand(c,"INCR counter");
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);


    /* again ... */
    reply = redisCommand(c,"HSET n2n:network:%s %s %s","public-b57ab26b","owner","novboy");
    
    printf("HSET owner: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"HSET n2n:network:%s %s %s","public-b57ab26b","subnet","10.128.1.0");
    printf("HSET subnet: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"HSET n2n:network:%s %s %s","public-b57ab26b","id","public-b57ab26b");
    printf("HSET id: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"HSET n2n:network:%s %s %s","public-b57ab26b","secret","inglOckEsOnAltYrforn");
    printf("HSET secret: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"HGET n2n:network:%s %s","public-b57ab26b","secret");
    printf("HGET secret: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"HGET n2n:network:%s %s","public-b57ab26b","aaa");
    printf("HGET public-b57ab26b:aaa: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"HGET n2n:network:%s %s","maperr","aaa");
    printf("HGET maperr->aaa: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"SET online %d EX %d", 1,60);
    printf("SET online %d EX %d: %s\n", 1,60, reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"GET %s","online");
    printf("GET online: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"TTL online");
    printf("TTL online: %lld\n", reply->integer);
    freeReplyObject(reply);



    reply = redisCommand(c,"HGETALL  n2n:network:%s","public-b57ab26b");
    if ( reply->type == REDIS_REPLY_ERROR ) {
        printf( "Error: %s\n", reply->str );
    } else if ( reply->type != REDIS_REPLY_ARRAY ) {
        printf( "Unexpected type: %d\n", reply->type );
    } else {
        int i;
        for (i = 0; i < reply->elements; i = i + 2 ) {
            printf( "Result: %s = %s \n", reply->element[i]->str, reply->element[i + 1]->str );
        }
    }
    freeReplyObject(reply);

    /* Create a list of numbers, from 0 to 9 */
    reply = redisCommand(c,"DEL mylist");
    freeReplyObject(reply);
    for (j = 0; j < 10; j++) {
        char buf[64];

        snprintf(buf,64,"%u",j);
        reply = redisCommand(c,"LPUSH mylist element-%s", buf);
        freeReplyObject(reply);
    }

    /* Let's check what we have inside the list */
    reply = redisCommand(c,"LRANGE mylist 0 -1");
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (j = 0; j < reply->elements; j++) {
            printf("%u) %s\n", j, reply->element[j]->str);
        }
    }
    freeReplyObject(reply);

    /* Disconnects and frees the context */
    redisFree(c);

    return 0;
}