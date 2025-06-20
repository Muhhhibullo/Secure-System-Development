#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* getString() {
    char* message = malloc(100 * sizeof(char));
    if (message == NULL) {
        return NULL;
    }
    strcpy(message, "Hello World!");
    return message;
}

void program4() {
    char* str = getString();
    if (str != NULL) {
        printf("String: %s\n", str);
        free(str);
    }
}

int main() {
    program4();
}