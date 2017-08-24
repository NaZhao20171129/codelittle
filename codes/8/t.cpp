#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096
enum CHECK_STATE { CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER, CHECK_STATE_CONTENT };
enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_OPEN };
enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST, FORBIDDEN_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION };
static const char* szret[] = { "I get a correct result\n", "Something wrong\n" };


HTTP_CODE parse_requestline( char* szTemp )
{
    char* szURL = strpbrk( szTemp, " \t" );
    if ( ! szURL )
    {
        return BAD_REQUEST;
    }
    printf("%s\n",szURL);
    szURL++; 
    printf("%s\n",szURL--);
    char* szMethod = szTemp;
    if ( strncasecmp( szMethod, "GET" ,3) == 0 )
    {
        printf( "The request method is GET\n" );
    }
    else
    {
        return BAD_REQUEST;
    }

    int n=strspn( szURL, " \t" );
    printf("%d\n",n);
    szURL+=n;
    printf("%s\n",szURL);
    char* szVersion = strpbrk( szURL, " \t" );
    if ( ! szVersion )
    {
        return BAD_REQUEST;
    }
    printf ("%c\n",*szVersion++ );
    printf("aaaa");
    *szVersion++ = '\0';
    szVersion += strspn( szVersion, " \t" );

    if ( strncasecmp( szURL, "http://", 7 ) == 0 )
    {
        szURL += 7;
    printf("aaca");
        szURL = strchr( szURL, '/' );
    }

    if ( ! szURL || szURL[ 0 ] != '/' )
    {
        return BAD_REQUEST;
    }

    //URLDecode( szURL );
    printf( "The request URL is: %s\n", szURL );
    return NO_REQUEST;
}

HTTP_CODE parse_headers( char* szTemp )
{
    if ( szTemp[ 0 ] == '\0' )
    {
        return GET_REQUEST;
    }
    else if ( strncasecmp( szTemp, "Host:", 5 ) == 0 )
    {
        szTemp += 5;
        szTemp += strspn( szTemp, " \t" );
        printf( "the request host is: %s\n", szTemp );
    }
    else
    {
        printf( "I can not handle this header\n" );
    }

    return NO_REQUEST;
}


int main( int argc, char* argv[] )
{

        char  temp[128]="GET http://www.baidu.com/index.html HTTP/1.1";
        int a=0;
        parse_requestline(temp);
}
