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

typedef struct
{
    uint8_t num_suggestions;
    uint16_t *suggestions_len;
    char **suggestions;
} Response;

void clear_response(Response *res, int num);

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
        return 1;
    }

    char buff[QUERY_LIMIT];
    printf("Building Trie...\n");
    while (fscanf(file, "%[^\n]%*c", buff) != EOF)
        trie_insert(buff, &trie);
    fclose(file);

    struct sockaddr_un sockaddr = {0};
    sockaddr.sun_len = sizeof(struct sockaddr_un);
    sockaddr.sun_family = AF_UNIX;
    strcpy(sockaddr.sun_path, SOCKPATH);

    int sockfd;
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Error: Something went wrong\n");
        return 1;
    }

    unlink(SOCKPATH);

    bind(sockfd, (struct sockaddr *) &sockaddr, sockaddr.sun_len);

    listen(sockfd, 1);

    printf("Server is listening on Unix socket: %s\n", SOCKPATH);

    Response res;
    res.suggestions = malloc(sizeof(char *) * NUM_SUGGESTIONS);
    int client_sockfd; 
    while ((client_sockfd = accept(sockfd, NULL, NULL)) != -1)
    {
        printf("Connection accepted!\n");

        for (int i = 0, c = 0; c != '\n' && i < QUERY_LIMIT - 1; i++)
        {
            int conn_is_alive = recv(client_sockfd, &c, 1, 0);
            if (conn_is_alive > 0 && c != '\n')
            {
                buff[i] = c;
                buff[i + 1] = '\0';

                res.num_suggestions = trie_prefix_match(buff, &trie, res.suggestions, NUM_SUGGESTIONS);
                send(client_sockfd, &res.num_suggestions, sizeof(res.num_suggestions), 0);
                res.suggestions_len = malloc(sizeof(*res.suggestions_len) * res.num_suggestions);
                if (res.num_suggestions > 0)
                {
                    for (int j = 0; j < res.num_suggestions; j++)
                    {
                        res.suggestions_len[j] = strlen(res.suggestions[j]);
                        send(client_sockfd, &res.suggestions_len[j], sizeof(*res.suggestions_len), 0);
                        send(client_sockfd, res.suggestions[j], res.suggestions_len[j], 0);
                    }
                }
                clear_response(&res, res.num_suggestions);
            }
        }
        close(client_sockfd);
        printf("Connection closed!\n");
    }

    free(res.suggestions);
    trie_unload(&trie);
    close(sockfd);
    unlink(SOCKPATH);

    return 0;
}

void clear_response(Response *res, int num)
{
    free(res->suggestions_len);
    for (int i = 0; i < num; i++)
        free(res->suggestions[i]);
}