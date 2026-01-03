#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <input_file> <encoding> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* str = malloc(sizeof(char) * 1024);
    printf("Enter a string: ");
    scanf("%s", str);
    printf("%s", str);
    free(str);

    return EXIT_SUCCESS;
}

