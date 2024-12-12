#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

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
    do
    {
        printf("Query: ");
        scanf("%[^\n]%*c", buff);
        char **suggestions = malloc(sizeof(char *) * 10);
        int num_suggestions = trie_prefix_match(buff, &trie, suggestions, 10);
        if (num_suggestions > 0)
        {
            printf("Suggestions with prefix \"%s\":\n", buff);
            for (int i = 0; i < num_suggestions; i++)
            {
                printf("%s\n", suggestions[i]);
                free(suggestions[i]);
            }
            printf("\n");
        }
        free(suggestions);

        printf("Search again? (y/n): ");
        search = getchar();
        while (getchar() != '\n');
    } while (search == 'y');


    trie_unload(&trie);
    return 0;
}