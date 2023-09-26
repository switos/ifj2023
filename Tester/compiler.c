#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    char buffer[MAX_BUFFER_SIZE];
    const char *hello_world = "Hello world!";
    const char *goodbye_world = "Goodbye world!";

    // Reading the input text
    while ( fgets(buffer, sizeof(buffer), stdin) != NULL ) {
        // We check whether the input text contains the phrase "Hello world!"
        if (strstr(buffer, hello_world) != NULL) {
            // If yes, we print "Goodbye world!"
            printf("%s\n", goodbye_world);
        } else {
            // Otherwise, we output the input text
            printf("%s", buffer);
        }
    }

    return 0;
}
