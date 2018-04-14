/*
 * Socket.cpp
 *
 *  Created on: 2013-9-17
 *      Author: LiuHao
 */
#include "stdafx.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/un.h>
#include <unistd.h>

#include "process.h"

/* Read text from the socket and print it out. Continue until the
       socket closes. Return nonzero if the client sent a "quit"
       message, zero otherwise.
 */
static int server (int client_socket)
{
    while (true)
    {
        int length;
        char* text;
        /* First, read the length of the text message from the socket. If
               read returns zero, the client closed the connection. */
        if (read (client_socket, &length, sizeof (length)) == 0)
            return 0;
        /* Allocate a buffer to hold the text. */
        text = (char*) malloc (length);
        /* Read the text itself, and print it. */
        read (client_socket, text, length);
        printf ("%s\n", text);
        /* Free the buffer. */
        free (text);
        /* If the client sent the message "quit," we¡¯re all done. */
        if (!strcmp (text, "quit"))
            return 1;
    }

    return 0;
}

void ServerMain1()
{
    const char* const socket_name = "/tmp/socket";
    int socket_fd;
    struct sockaddr_un name;
    int client_sent_quit_message;
    /* Create the socket. */
    socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);
    /* Indicate that this is a server. */
    name.sun_family = AF_LOCAL;
    strcpy (name.sun_path, socket_name);
    bind (socket_fd, (struct sockaddr*)&name, SUN_LEN (&name));
    /* Listen for connections. */
    listen (socket_fd, 5);
    /* Repeatedly accept connections, spinning off one server() to deal
        with each client. Continue until a client sends a ¡°quit¡± message. */
    do
    {
        struct sockaddr_un client_name;
        socklen_t client_name_len = (socklen_t)sizeof(struct sockaddr_un);
        int client_socket_fd;
        /* Accept a connection. */
        client_socket_fd = accept (socket_fd, (struct sockaddr *)&client_name, &client_name_len);
        /* Handle the connection. */
        client_sent_quit_message = server (client_socket_fd);
        /* Close our end of the connection. */
        close (client_socket_fd);
    }while (!client_sent_quit_message);
    /* Remove the socket file. */
    close (socket_fd);
    unlink (socket_name);
}

static void write_text (int socket_fd, const char* text)
{
    /* Write the number of bytes in the string, including NUL-termination. */
    int length = strlen (text) + 1;
    write (socket_fd, &length, sizeof (length));
    /* Write the string. */
    write (socket_fd, text, length);
}
void ClientMain1()
{
    const char* const socket_name = "/tmp/socket";
    const char* const message = "This is the test message.";
    int socket_fd;
    struct sockaddr_un name;
    /* Create the socket. */
    socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);
    /* Store the server¡¯s name in the socket address. */
    name.sun_family = AF_LOCAL;
    strcpy (name.sun_path, socket_name);
    /* Connect the socket. */
    connect (socket_fd, (struct sockaddr*)&name, SUN_LEN (&name));
    /* Write the text on the command line to the socket. */
    write_text (socket_fd, message);
    close (socket_fd);
}

/* Print the contents of the home page for the server¡¯s socket.
   Return an indication of success.
 */
static void get_home_page (int socket_fd)
{
    char buffer[10000];
    ssize_t number_characters_read;
    /* Send the HTTP GET command for the home page. */
    sprintf (buffer, "GET /\n");
    write (socket_fd, buffer, strlen (buffer));
    /* Read from the socket. The call to read may not
       return all the data at one time, so keep
       trying until we run out.
     */
    while (true)
    {
        number_characters_read = read (socket_fd, buffer, 10000);
        if (number_characters_read == 0)
            return;
        /* Write the data to standard output. */
        fwrite (buffer, sizeof (char), number_characters_read, stdout);
    }
}

void TestInternetSocket()
{
    int socket_fd;
    struct sockaddr_in name;
    struct hostent* hostinfo;
    /* Create the socket. */
    socket_fd = socket (PF_INET, SOCK_STREAM, 0);
    /* Store the server¡¯s name in the socket address. */
    name.sin_family = AF_INET;
    /* Convert from strings to numbers. */
    hostinfo = gethostbyname ("www.baidu.com");
    if (hostinfo == NULL)
    {
        printf("Failed to gethostbyname. \r\n");
        return ;
    }
    else
        name.sin_addr = *((struct in_addr *) hostinfo->h_addr);
    /* Web servers use port 80. */
    name.sin_port = htons (80);
    /* Connect to the Web server */
    if (connect (socket_fd, (struct sockaddr*)&name, sizeof (struct sockaddr_in)) == -1)
    {
        printf ("Failed to connect. \r\n");
        return ;
    }
    /* Retrieve the server¡¯s home page. */
    get_home_page (socket_fd);
}

