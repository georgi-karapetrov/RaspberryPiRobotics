///////////////////////////////////////////////////
//
// server.c
//
// Georgi Karapetrov
///////////////////////////////////////////////////

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#include "controller.h"

#define PORT 8080
#define SLEEP_INTERVAL_MICROSECONDS 40000

int main( int argc, char *argv[] )
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    listenfd = socket( AF_INET, SOCK_STREAM, 0 );
    memset( &serv_addr, '0', sizeof( serv_addr ) );

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );
    serv_addr.sin_port = htons( PORT ); 

    bind( listenfd, ( struct sockaddr* )&serv_addr, sizeof( serv_addr ) );

    listen( listenfd, 10 ); 

    setupPins();

    printf( "Server is up and running.\n" );
    connfd = accept( listenfd, ( struct sockaddr* )NULL, NULL );

    char recvBuff[32];
    while( 1 ) {

        int n;
        while ( n = read( connfd, recvBuff, sizeof( recvBuff ) ) > 0 ) {

            printf( "Client said: %s\n", recvBuff );

            if ( strcmp( recvBuff, "w" ) == 0 ) {

                moveForward();
                usleep( SLEEP_INTERVAL_MICROSECONDS );
                stopMotors();
            } else if ( strcmp( recvBuff, "s" ) == 0 ) {

                moveBackward();
                usleep( SLEEP_INTERVAL_MICROSECONDS );
                stopMotors();
            } else if ( strcmp( recvBuff, "a" ) == 0 ) {

                turnLeft();
                usleep( SLEEP_INTERVAL_MICROSECONDS );
                stopMotors();
            } else if ( strcmp( recvBuff, "d" ) == 0 ) {

                turnRight();
                usleep( SLEEP_INTERVAL_MICROSECONDS );
                stopMotors();
            } 
            else if ( strcmp( recvBuff, "h" ) == 0 ) {

                printf( "Issued a stop command.\n" );
                stopMotors();

            } else if ( strcmp( recvBuff, "q" ) == 0 ) {

                printf( "Exiting.\n" );
                stopMotors();
                close( connfd );
                exit( EXIT_SUCCESS );
            }
        }
     }

    close( connfd );
}
