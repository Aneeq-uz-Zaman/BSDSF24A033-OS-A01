#include <stdio.h>
#include <stdlib.h>
#include "../include/mystrfunctions.h"
#include "../include/myfilefunctions.h"

int main() {
    printf("--- Testing String Functions ---\n");

    char buffer[100];

    int len = mystrlen("Hello, World!");
    printf("mystrlen(\"Hello, World!\") = %d\n", len);

    mystrcpy(buffer, "Copied string");
    printf("mystrcpy result: %s\n", buffer);

    mystrncpy(buffer, "Truncate this text", 9);
    printf("mystrncpy (n=9) result: %s\n", buffer);

    mystrcpy(buffer, "Hello");
    mystrcat(buffer, ", World!");
    printf("mystrcat result: %s\n", buffer);

    printf("\n--- Testing File Functions ---\n");

    const char* testFileName = "test_input.txt";
    FILE* out = fopen(testFileName, "w");
    if (out == NULL) {
        printf("Failed to create test file.\n");
        return 1;
    }
    fprintf(out, "This is line one.\n");
    fprintf(out, "This line has the word target in it.\n");
    fprintf(out, "Another target appears here too.\n");
    fclose(out);

    FILE* in = fopen(testFileName, "r");
    if (in == NULL) {
        printf("Failed to open test file.\n");
        return 1;
    }

    int lines, words, chars;
    if (wordCount(in, &lines, &words, &chars) == 0) {
        printf("wordCount -> lines: %d, words: %d, chars: %d\n", lines, words, chars);
    } else {
        printf("wordCount failed.\n");
    }
    fclose(in);

    in = fopen(testFileName, "r");
    if (in == NULL) {
        printf("Failed to reopen test file.\n");
        return 1;
    }

    char** matches = NULL;
    int matchCount = mygrep(in, "target", &matches);
    if (matchCount >= 0) {
        printf("mygrep found %d matching line(s):\n", matchCount);
        for (int i = 0; i < matchCount; i++) {
            printf("  %s", matches[i]);
            free(matches[i]);
        }
        free(matches);
    } else {
        printf("mygrep failed.\n");
    }
    fclose(in);

    remove(testFileName);

    return 0;
}
