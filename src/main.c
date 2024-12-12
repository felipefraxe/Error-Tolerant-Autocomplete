#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

#define NUM_SUGGESTIONS 10

void clean_suggestions(char **suggestions, int num);

int main(void)
{
    Trie trie = trie_constructor();
    FILE *file = fopen("data/relatedBySessionRaw.txt", "r");
    char buff[2048];

    printf("Building Trie...\n");
    while (fscanf(file, "%[^\n]%*c", buff) != EOF)
    {
        char *token = strtok(buff, "|");
        for (int i = 0; i < 3; i++)
            token = strtok(NULL, "|");
        trie_insert(token, &trie);
    }
    fclose(file);

    int search;
    char **suggestions = malloc(sizeof(char *) * NUM_SUGGESTIONS);
    do
    {
        printf("Query: ");
        for (int i = 0, c; (c = getchar()) != '\n' && i < 2047; i++)
        {
            buff[i] = c;
            buff[i + 1] = '\0';
            int num_suggestions = trie_prefix_match(buff, &trie, suggestions, NUM_SUGGESTIONS);
            if (num_suggestions > 0)
            {
                printf("Suggestions with prefix \"%s\":\n", buff);
                for (int j = 0; j < num_suggestions; j++)
                    printf("%s\n", suggestions[j]);
                printf("\n");
            }
            clean_suggestions(suggestions, num_suggestions);
        }

        printf("Search again? (y/n): ");
        search = getchar();
        while (getchar() != '\n');
    } while (search == 'y');

    free(suggestions);
    trie_unload(&trie);
    return 0;
}

void clean_suggestions(char **suggestions, int num)
{
    for (int i = 0; i < num; i++)
        free(suggestions[i]);
}