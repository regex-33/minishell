#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_USERNAME_LENGTH 256

char *get_username() {
    int pipefd[2];
    pid_t pid;
    char buffer[MAX_USERNAME_LENGTH];
    int status;

    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process
        close(pipefd[0]);  // Close read end of the pipe

        // Redirect stdout to the write end of the pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("Duplication failed");
            exit(EXIT_FAILURE);
        }

        // Execute the whoami command
        execlp("whoami", "whoami", NULL);
        perror("Execution failed");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        close(pipefd[1]);  // Close write end of the pipe

        // Read the output of the whoami command from the pipe
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer));
        if (bytes_read == -1) {
            perror("Reading from pipe failed");
            exit(EXIT_FAILURE);
        }

        // Null-terminate the string
        buffer[bytes_read] = '\0';

        // Wait for the child process to terminate
        if (waitpid(pid, &status, 0) == -1) {
            perror("Waitpid failed");
            exit(EXIT_FAILURE);
        }

        // Check if the child process terminated successfully
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            fprintf(stderr, "Command execution failed\n");
            exit(EXIT_FAILURE);
        }

        // Close the read end of the pipe
        close(pipefd[0]);

        // Remove trailing newline if present
        if (buffer[bytes_read - 1] == '\n') {
            buffer[bytes_read - 1] = '\0';
        }

        // Allocate memory for the username and copy the buffer contents
        char *username = malloc(bytes_read);
        if (!username) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        strcpy(username, buffer);

        return username;
    }
}

int main() {
    char *username = get_username();
    printf("Username: %s\n", username);
    free(username);
    return 0;
}

