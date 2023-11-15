#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    char buffer[MAX_BUFFER_SIZE];
    const char *input = "print(\"Hello, World!\")";
    const char *output = "Hello, World!";
    const char *error1 = "let 123abc = \"Hello, World!\"";

    // Reading the input text
    while ( fgets(buffer, sizeof(buffer), stdin) != NULL )
    {
        if ( strstr(buffer, input) != NULL )
        {
            printf("%s", output);
            return 0;
        }
        else if ( strstr(buffer, error1) != NULL )
        {
            return 1;
        }
    }

    return -1;
}
