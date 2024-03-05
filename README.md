# minishell

#### tputs:

In C programming, tputs() is a function that's commonly used in terminal manipulation, particularly for dealing with terminal capabilities such as moving the cursor, clearing the screen, changing text attributes, and more. It's part of the "terminfo" library which provides a database of terminal capabilities.

Here's the prototype of the tputs() function:

```c
int tputs(const char *str, int affcnt, int (*putc)(int));
```

    str: The string containing the terminal capabilities sequence to be sent to the terminal.
    affcnt: An integer representing the number of lines affected by the operation (not often used, can usually be set to 1).
    putc: A pointer to a function that will be used to output characters to the terminal.

Here's a simple example demonstrating the usage of tputs() to clear the screen:

```c

#include <stdio.h>
#include <term.h>
#include <curses.h> // On some systems, term.h may be included via curses.h

int main() {
    setupterm(NULL, fileno(stdout), (int *)0); // Initialize terminal
    tputs("clear", 1, putchar); // Send the "clear" capability to clear the screen
    return 0;
}
```

In this example:

    setupterm() initializes the terminal. It takes three arguments: term (the terminal type, NULL uses the value of the TERM environment variable), filedes (the file descriptor for the terminal, fileno(stdout) gets the file descriptor for standard output), and errret (a pointer to an integer to receive error codes, set to (int *)0 to ignore errors).

    tputs("clear", 1, putchar) sends the capability string associated with the "clear" operation to the terminal. The second argument, 1, indicates that one line of the terminal is affected. putchar is a pointer to the putchar function from the standard library, which will be used to output characters to the terminal.

This example clears the screen by sending the appropriate terminal capability sequence, as defined in the terminfo database.
###

#### tgoto:

In C programming, tgoto() is a function that's often used in conjunction with tputs() for terminal manipulation. It is used to build control strings for cursor positioning and other cursor-related operations.

Here's the prototype of the tgoto() function:

```c

char *tgoto(const char *cap, int col, int row);
```
    cap: The name of the terminal capability being used.
    col: The column position to move the cursor to.
    row: The row position to move the cursor to.

The tgoto() function constructs a string containing the necessary terminal control sequences to position the cursor at the specified column and row.

Here's a simple example demonstrating the usage of tgoto() to generate a cursor positioning sequence:

```c

#include <stdio.h>
#include <term.h>
#include <curses.h> // On some systems, term.h may be included via curses.h

int main() {
    setupterm(NULL, fileno(stdout), (int *)0); // Initialize terminal
    int col = 10; // Desired column
    int row = 5; // Desired row
    char *cursor_move_seq = tgoto(tgetstr("cm", NULL), col, row); // Get cursor movement sequence
    tputs(cursor_move_seq, 1, putchar); // Move cursor to (col, row)
    printf("Cursor moved to row %d, column %d\n", row, col);
    return 0;
}
```

In this example:

    setupterm() initializes the terminal, similar to the previous example.
    tgetstr("cm", NULL) retrieves the control sequence for cursor movement (cm) from the terminfo database. This returns a string like \033[%i%p1%d;%p2%dH, where %p1%d is replaced with the row and %p2%d is replaced with the column.
    tgoto(tgetstr("cm", NULL), col, row) generates the actual cursor movement sequence for the desired row and column.
    tputs(cursor_move_seq, 1, putchar) sends the cursor movement sequence to the terminal using tputs().
    The printf() statement simply prints a message indicating that the cursor has been moved to the specified row and column.

This example moves the cursor to the specified row (5) and column (10) on the terminal.
###
#### tgetstr:
#### tgetnum:
#### tgetflag:
#### tgetent:
#### tcgetattr:
#### tcsetattr:
#### getenv:
#### ioctl:
#### ttyslot:
#### ttyname:
#### isatty:
#### perror:
#### strerror:
#### closedir:
#### readdir:
#### opendir:
#### pipe:
#### dup2:
#### dup:
#### execve:
#### unlink:
#### fstat:
#### stat:
#### chdir:
#### getcwd:
#### sigaddset:
#### sigemptyset:
#### sigaction:
#### signal:
#### wait4:
#### wait3:
#### waitpid:
#### wait:
#### add_history:
#### rl_redisplay:
#### rl_replace_line:
#### rl_on_new_line:
#### rl_clear_history:
#### readline:
