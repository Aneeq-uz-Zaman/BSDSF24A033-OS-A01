#include "../include/myfilefunctions.h"
#include <stdlib.h>
#include <string.h>

int wordCount(FILE* file, int* lines, int* words, int* chars) {
    if (file == NULL || lines == NULL || words == NULL || chars == NULL) return -1;

    *lines = 0;
    *words = 0;
    *chars = 0;

    int c;
    int inWord = 0;
    while ((c = fgetc(file)) != EOF) {
        (*chars)++;

        if (c == '\n') {
            (*lines)++;
        }

        if (c == ' ' || c == '\t' || c == '\n') {
            inWord = 0;
        } else if (!inWord) {
            inWord = 1;
            (*words)++;
        }
    }

    return 0;
}

int mygrep(FILE* fp, const char* search_str, char*** matches) {
    if (fp == NULL || search_str == NULL || matches == NULL) return -1;

    int capacity = 10;
    int count = 0;
    char** results = malloc(capacity * sizeof(char*));
    if (results == NULL) return -1;

    char line[1024];
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, search_str) != NULL) {
            if (count == capacity) {
                capacity *= 2;
                char** resized = realloc(results, capacity * sizeof(char*));
                if (resized == NULL) {
                    for (int i = 0; i < count; i++) free(results[i]);
                    free(results);
                    return -1;
                }
                results = resized;
            }

            char* lineCopy = malloc(strlen(line) + 1);
            if (lineCopy == NULL) {
                for (int i = 0; i < count; i++) free(results[i]);
                free(results);
                return -1;
            }
            strcpy(lineCopy, line);
            results[count] = lineCopy;
            count++;
        }
    }

    *matches = results;
    return count;
}
