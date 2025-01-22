#include "proxy_parse.h"

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <netdb.h>

#include <arpa/inet.h>

#include <unistd.h>

#include <fcntl.h>

#include <time.h>

#include <sys/wait.h>

#include <errno.h>

#include <pthread.h>

#include <semaphore.h>

#include <time.h>



#define MAX_BYTES 4096    //max allowed size of request/response

#define MAX_CLIENTS 400     //max number of client requests served at a time

#define MAX_SIZE 200*(1<<20)     //size of the cache

#define MAX_ELEMENT_SIZE 10*(1<<20)     //max size of an element in cache



typedef struct cache_element cache_element;



struct cache_element{

    char* data;         //data stores response

    int len;          //length of data i.e.. sizeof(data)...

    char* url;        //url stores the request

	time_t lru_time_track;    //lru_time_track stores the latest time the element is  accesed

    cache_element* next;    //pointer to next element

};

pthread_mutex_t lock;   //lock is used for locking the cache



cache_element* find(char* url);

int add_cache_element(char* data,int size,char* url);

void remove_cache_element();



int port_number = 8080;				// Default Port

int proxy_socketId;					// socket descriptor of proxy server

pthread_t tid[MAX_CLIENTS];         //array to store the thread ids of clients

sem_t seamaphore;	                //if client requests exceeds the max_clients this seamaphore puts the

                                    //waiting threads to sleep and wakes them when traffic on queue decreases

//sem_t cache_lock;			       





cache_element* head;                //pointer to the cache

int cache_size=0;             //cache_size denotes the current size of the cache



/**
 * Sends an HTTP error message to the client socket based on the status code.
 * 
 * @param socket       The socket descriptor for the client.
 * @param status_code  The HTTP status code to send (e.g., 400, 403, etc.).
 * @return 1 on success, -1 on failure.
 */
int sendErrorMessage(int socket, int status_code) {
    char str[1024];             // Buffer to store the HTTP error response.
    char currentTime[50];       // Buffer to store the current date and time in HTTP format.
    time_t now = time(0);       // Get the current system time.

    // Convert the current time to the required HTTP date format (RFC 7231 format).
    struct tm data = *gmtime(&now);  // Convert time to GMT (Coordinated Universal Time).
    strftime(currentTime, sizeof(currentTime), "%a, %d %b %Y %H:%M:%S %Z", &data);

    // Generate the appropriate error response based on the HTTP status code.
    switch (status_code) {
        case 400:  // Bad Request
            snprintf(
                str, sizeof(str),
                "HTTP/1.1 400 Bad Request\r\n"
                "Content-Length: 95\r\n"
                "Connection: keep-alive\r\n"
                "Content-Type: text/html\r\n"
                "Date: %s\r\n"
                "Server: VaibhavN/14785\r\n\r\n"
                "<HTML><HEAD><TITLE>400 Bad Request</TITLE></HEAD>\n"
                "<BODY><H1>400 Bad Request</H1>\n</BODY></HTML>",
                currentTime
            );
            printf("400 Bad Request\n");  // Log the error type.
            send(socket, str, strlen(str), 0);  // Send the error response to the client.
            break;

        case 403:  // Forbidden
            snprintf(
                str, sizeof(str),
                "HTTP/1.1 403 Forbidden\r\n"
                "Content-Length: 112\r\n"
                "Connection: keep-alive\r\n"
                "Content-Type: text/html\r\n"
                "Date: %s\r\n"
                "Server: VaibhavN/14785\r\n\r\n"
                "<HTML><HEAD><TITLE>403 Forbidden</TITLE></HEAD>\n"
                "<BODY><H1>403 Forbidden</H1><br>Permission Denied\n</BODY></HTML>",
                currentTime
            );
            printf("403 Forbidden\n");
            send(socket, str, strlen(str), 0);  // Send the error response.
            break;

        case 404:  // Not Found
            snprintf(
                str, sizeof(str),
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Length: 91\r\n"
                "Connection: keep-alive\r\n"
                "Content-Type: text/html\r\n"
                "Date: %s\r\n"
                "Server: VaibhavN/14785\r\n\r\n"
                "<HTML><HEAD><TITLE>404 Not Found</TITLE></HEAD>\n"
                "<BODY><H1>404 Not Found</H1>\n</BODY></HTML>",
                currentTime
            );
            printf("404 Not Found\n");
            send(socket, str, strlen(str), 0);
            break;

        case 500:  // Internal Server Error
            snprintf(
                str, sizeof(str),
                "HTTP/1.1 500 Internal Server Error\r\n"
                "Content-Length: 115\r\n"
                "Connection: keep-alive\r\n"
                "Content-Type: text/html\r\n"
                "Date: %s\r\n"
                "Server: VaibhavN/14785\r\n\r\n"
                "<HTML><HEAD><TITLE>500 Internal Server Error</TITLE></HEAD>\n"
                "<BODY><H1>500 Internal Server Error</H1>\n</BODY></HTML>",
                currentTime
            );
            // printf("500 Internal Server Error\n");
            send(socket, str, strlen(str), 0);
            break;

        case 501:  // Not Implemented
            snprintf(
                str, sizeof(str),
                "HTTP/1.1 501 Not Implemented\r\n"
                "Content-Length: 103\r\n"
                "Connection: keep-alive\r\n"
                "Content-Type: text/html\r\n"
                "Date: %s\r\n"
                "Server: VaibhavN/14785\r\n\r\n"
                "<HTML><HEAD><TITLE>501 Not Implemented</TITLE></HEAD>\n"
                "<BODY><H1>501 Not Implemented</H1>\n</BODY></HTML>",
                currentTime
            );
            printf("501 Not Implemented\n");
            send(socket, str, strlen(str), 0);
            break;

        case 505:  // HTTP Version Not Supported
            snprintf(
                str, sizeof(str),
                "HTTP/1.1 505 HTTP Version Not Supported\r\n"
                "Content-Length: 125\r\n"
                "Connection: keep-alive\r\n"
                "Content-Type: text/html\r\n"
                "Date: %s\r\n"
                "Server: VaibhavN/14785\r\n\r\n"
                "<HTML><HEAD><TITLE>505 HTTP Version Not Supported</TITLE></HEAD>\n"
                "<BODY><H1>505 HTTP Version Not Supported</H1>\n</BODY></HTML>",
                currentTime
            );
            printf("505 HTTP Version Not Supported\n");
            send(socket, str, strlen(str), 0);
            break;

        default:  // Unsupported or unknown status code
            return -1;  // Return -1 to indicate failure.
    }

    return 1;  // Return 1 to indicate success.
}

int connectRemoteServer(char* host_addr, int port_num)

{

	// Creating Socket for remote server ---------------------------



	int remoteSocket = socket(AF_INET, SOCK_STREAM, 0);



	if( remoteSocket < 0)

	{

		printf("Error in Creating Socket.\n");

		return -1;

	}

	

	// Get host by the name or ip address provided



	struct hostent *host = gethostbyname(host_addr);	

	if(host == NULL)

	{

		fprintf(stderr, "No such host exists.\n");	

		return -1;

	}



	// inserts ip address and port number of host in struct `server_addr`

	struct sockaddr_in server_addr;



	bzero((char*)&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;

	server_addr.sin_port = htons(port_num);



	bcopy((char *)host->h_addr,(char *)&server_addr.sin_addr.s_addr,host->h_length);



	// Connect to Remote server ----------------------------------------------------



	if( connect(remoteSocket, (struct sockaddr*)&server_addr, (socklen_t)sizeof(server_addr)) < 0 )

	{

		fprintf(stderr, "Error in connecting !\n"); 

		return -1;

	}



	return remoteSocket;

}




