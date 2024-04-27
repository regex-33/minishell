#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* strjoin(char** strings, int numRows) {
    // Calculate total length of the joined string including spaces
    int totalLength = 0;
    for (int i = 0; i < numRows; i++) {
        totalLength += strlen(strings[i]);
        // Add space for all but the last string
        if (i < numRows - 1)
            totalLength++;
    }

    // Allocate memory for the joined string
    char* joinedString = (char*)malloc((totalLength + 1) * sizeof(char));
    if (joinedString == NULL) {
        // Memory allocation failed
        return NULL;
    }

    // Copy each string into the joined string with a space in between
    int currentPosition = 0;
    for (int i = 0; i < numRows; i++) {
        strcpy(joinedString + currentPosition, strings[i]);
        currentPosition += strlen(strings[i]);
        // Add space if it's not the last string
        if (i < numRows - 1) {
            joinedString[currentPosition] = ' ';
            currentPosition++;
        }
    }

    // Null-terminate the joined string
    joinedString[totalLength] = '\0';

    return joinedString;
}

int main() {
    char* strings[] = {"Hello", "world", "!"};
    int numRows = sizeof(strings) / sizeof(strings[0]);

    char* joinedString = strjoin(strings, numRows);
    if (joinedString != NULL) {
        printf("Joined String: %s\n", joinedString);

        // Free the memory allocated for the joined string
        free(joinedString);
    } else {
        printf("Memory allocation failed.\n");
    }

    return 0;
}

