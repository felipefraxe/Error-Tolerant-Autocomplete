#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "trie.h"

#define SOCKPATH "/tmp/trie_socket"
#define NUM_SUGGESTIONS 10
#define QUERY_LIMIT 2048

void clear_suggestions(char **suggestions, int num);
char *json_serialize(char *key, char **val, int n);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: %s <DICT_PATH>\n", argv[0]);
        return 1;
    }

    Trie trie = trie_constructor();
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file %s!", argv[1]);
        return 2;
    }

    char buff[QUERY_LIMIT];
    printf("Building Trie...\n");
    while (fscanf(file, "%[^\n]%*c", buff) != EOF)
        trie_insert(buff, &trie);
    fclose(file);
    trie_sort_nodes(&trie);

    struct sockaddr_un sockaddr = {0};
    sockaddr.sun_len = sizeof(struct sockaddr_un);
    sockaddr.sun_family = AF_UNIX;
    strcpy(sockaddr.sun_path, SOCKPATH);

    int sockfd;
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Error: Something went wrong\n");
        return 3;
    }

    unlink(SOCKPATH);

    bind(sockfd, (struct sockaddr *) &sockaddr, sockaddr.sun_len);

    listen(sockfd, 1);

    printf("Server is listening on Unix socket: %s\n", SOCKPATH);

    char **suggestions = malloc(sizeof(char *) * NUM_SUGGESTIONS);
    if (suggestions == NULL)
    {
        perror("Error: Could not allocate memory for suggestions\n");
        return 1;
    }
    int client_sockfd; 
    while ((client_sockfd = accept(sockfd, NULL, NULL)) != -1)
    {
        printf("Connection accepted!\n");

        int query_len = recv(client_sockfd, &buff, sizeof(buff), 0);
        if (query_len > 0)
        {
            buff[query_len] = '\0';

            int num_suggestions = trie_prefix_match(buff, &trie, suggestions, NUM_SUGGESTIONS);

            char *res = json_serialize("suggestions", suggestions, num_suggestions);

            send(client_sockfd, res, sizeof(char) * strlen(res), 0);

            free(res);
            clear_suggestions(suggestions, num_suggestions);
        }
        close(client_sockfd);
        printf("Connection closed!\n");
    }

    free(suggestions);
    trie_unload(&trie);
    close(sockfd);
    unlink(SOCKPATH);

    return 0;
}

void clear_suggestions(char **suggestions, int num)
{
    for (int i = 0; i < num; i++)
        free(suggestions[i]);
}

char *json_serialize(char *key, char **val, int n)
{
    size_t total_len = strlen(key) + 8;
    for (int i = 0; i < n; i++)
        total_len += strlen(val[i]) + 3;
    char *res = malloc(total_len);

    if (n == 0)
    {
        sprintf(res, "{\"%s\":[]}", key);
        return res;
    }

    char *ptr = res;
    ptr += sprintf(ptr, "{\"%s\":[\"%s\"", key, val[0]);
    for (int i = 1; i < n; i++)
        ptr += sprintf(ptr, ",\"%s\"", val[i]);
    ptr += sprintf(ptr, "]}");

    return res;
}