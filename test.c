#include <stdio.h>
#include <stdlib.h>

extern char **environ; // Declare environ variable

int main() {
    char **env_ptr = environ; // Pointer to iterate through the environment variables
    
    while (*env_ptr != NULL) {
        printf("%s\n", *env_ptr); // Print each environment variable
        env_ptr++;
    }
    
    return 0;
}

