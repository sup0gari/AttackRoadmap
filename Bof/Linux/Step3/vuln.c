#include <stdio.h>
#include <unistd.h>
#include <string.h>

void vuln() {
    char buffer[64];
    puts("Enter your text:");
    gets(buffer);
    puts("Done.");
}

int main() {
    vuln();
    return 0;
}