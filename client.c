#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <regex.h>

#include "conio.h"

#define PORT 8080

// Keys mappings
#define MOVE_FORWARD_KEY 'w'
#define MOVE_BACKWARD_KEY 's'
#define TURN_LEFT_KEY 'a'
#define TURN_RIGHT_KEY 'd'
#define STOP_MOTORS_KEY 'h'
#define ESC_KEY 27
//#define IP_REGEX "^( ( [0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5] )\\. ){3}( [0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5] )$"

#define IP_REGEX "( ( 25[0-5] )|( 2[0-4][0-9] )|( 1[0-9][0-9] )|( [1-9][0-9] )|( [0-9] ) )[.]( ( 25[0-5] )|( 2[0-4][0-9] )|( 1[0-9][0-9] )|( [1-9][0-9] )|( [0-9] ) )\\.( ( 25[0-5] )|( 2[0-4][0-9] )|( 1[0-9][0-9] )|( [1-9][0-9] )|( [0-9] ) )[.]( ( 25[0-5] )|( 2[0-4][0-9] )|( 1[0-9][0-9] )|( [1-9][0-9] )|( [0-9] ) )"

int GetIPStringFromString( const char * str, char * dest )
{
    regex_t regex;
    int reti = regcomp( &regex,IP_REGEX, REG_EXTENDED );
    if ( reti ) {

        printf( "Could not compile regex.\n" );
        return 1;
    }

    regmatch_t pmatch[1];

    reti = regexec( &regex, str, 1, pmatch, 0 );
   
    if ( reti == REG_NOMATCH ) {
        printf( "No IP found in '%s'.\n", str );
        return 1;
    }

    const char * match_start = str + pmatch[0].rm_so;
    const char * match_end = str + pmatch[0].rm_eo;
    int match_len = match_end - match_start;

    char match_copy[32];
    if ( match_len >= sizeof( match_copy ) )
    {
        match_len = sizeof( match_copy );
    }

    memcpy( match_copy, match_start, match_len );
    match_copy[match_len] = '\0';

    strncpy( dest, match_copy, sizeof( match_copy ) );
    regfree( &regex );
    printf( "about to return 0 success\n" );
    return 0;
}

int GetIPAddressForHost( char* host, char* server_ip )
{
    char buffer[64];
    
    char pingCommand[128];
    sprintf( pingCommand, "ping -c 1 %s", host );

    FILE *fd = popen( pingCommand, "r" );
    fgets( buffer, 64, fd );
    pclose( fd );
    char *pos;
    if ( ( pos = strchr( buffer, '\n' ) ) != NULL ) {
        *pos = '\0';
    }
    
    if ( !GetIPStringFromString( buffer, server_ip ) ) {
        printf( "The IP address is %s\n.", server_ip );
        return 0;
    }

    printf( "No IP address found for hostname.\n" );
    return 1;
}

int ConnectToServer( const char * server_ip )
{
    printf( "Connecting to IP: %s\n", server_ip );
    int sockfd = 0, n = 0;
    struct sockaddr_in serv_addr;
    
    if ( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
        printf( "\nError : Could not create socket \n" );
        return 1;
    }
    
    memset( &serv_addr, '0', sizeof( serv_addr ) );
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( PORT );
    if ( inet_pton( AF_INET, server_ip, &serv_addr.sin_addr ) <= 0 ) {

        printf( "inet_pton error occured\n" );
        return 1;
    }
    
    if ( connect( sockfd, ( struct sockaddr * )&serv_addr, sizeof( serv_addr ) ) < 0 ) {

        printf( "\n Error : Connect Failed \n" );
        return 1;
    }
    
    if ( n < 0 ) {

        printf( "\n Read error \n" );
        return 1;
    }
    
    return sockfd;
}

int EstablishConnection( const char * server_ip )
{
    int sockfd = ConnectToServer( server_ip );
    if ( sockfd != 1 ) {

        printf( "Connection established.\n" );
        int ch;

        while ( ( ch = getch(  ) ) != ESC_KEY ) {

            write( sockfd, &ch, sizeof( ch ) );
            if ( ch == 'q' ) {
                return 0;
            }

            sleep( 1 );
        }
    }

    printf( "Could not establish connection.\n" );
    return 1;
}

int main( int argc, char* argv[] )
{
    char cl_input[64] = "raspberrypi.local";
    if ( argc < 2 ) {
        printf( "Usage: %s <Host name>|<IP> \nUsing default parameter ( 'raspberrypi.local' ).\n", argv[0] );
    } else {
        cl_input[0] = '\0';
        strcpy( cl_input, argv[1] );
    }
    
    char server_ip[32];
    
    if ( GetIPStringFromString( cl_input, server_ip ) ) {

        printf( "Not an IP. Attempting to resolve host name.\n" );
        if ( GetIPAddressForHost( cl_input, server_ip ) ) {

            printf( "IP for host not found. Exiting.\n" );
            exit( EXIT_FAILURE );
        }
    }

    printf( "We got server_ip: %s\n", server_ip );
    EstablishConnection( server_ip );

    return 0;
}
