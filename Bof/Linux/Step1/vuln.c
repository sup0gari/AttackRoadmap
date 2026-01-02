#include <stdio.h>
#include <string.h>

void win() {
    printf("Buffer Overflow done!\n");
}

void vuln() {
    char buffer[64];
    printf("Enter your text: \n");
    gets(buffer);
    printf("You entered: %s\n", buffer);
}

int main() {
    vuln();
    return 0;
}