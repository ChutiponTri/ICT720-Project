#include <stdio.h>
#include <stdint.h>

int main() {
    // Define a byte array (char array in C)
    uint8_t data[] = {0x00, 'L'};

    int my = data[1];

    // Print the individual bytes
    printf("First byte: %d\n", data[0]);  // Output: 0
    printf("Second byte: %d\n", my); // Output: 76

    // Print the character representation of the second byte
    printf("Second byte as char: %c\n", data[1]); // Output: L

    return 0;
}