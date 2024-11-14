#include <stdio.h>   // For file I/O
#include <stdlib.h>  // For atoi()
#include <string.h>  // For string functions

int main() {
    // Variables to store opcode, operand, and other necessary information
    char label[10], opcode[10], operand[10], code[10];
    int locctr = 0, start = 0, program_length = 0;

    // File pointers for input, symbol table, output, and opcode table
    FILE *input, *symtab, *output, *optab;

    // Open files and check for successful opening
    input = fopen("inputpass.txt", "r");
    symtab = fopen("symtab.txt", "w");
    output = fopen("out.txt", "w");
    optab = fopen("opcode.txt", "r");


    // Read the first line to check if it has the START directive
    fscanf(input, "%s %s %s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0) {
        start = atoi(operand);         // Convert starting address to integer
        locctr = start;                // Initialize location counter
        fprintf(output, "%s\t%s\t%s\n", label, opcode, operand);  // Write to output file
        fscanf(input, "%s %s %s", label, opcode, operand);        // Read the next line
    }

    // Process each line until the END directive
    while (strcmp(opcode, "END") != 0) {
        fprintf(output, "%d\t", locctr);  // Write location counter to output file

        // If there is a label, add it to the symbol table
        if (strcmp(label, "") != 0) {
            fprintf(symtab, "%d\t%s\n", locctr, label);
        }

        // Check opcode table for matching opcode
        rewind(optab);
        fscanf(optab, "%s", code);
        int found = 0;
        while (strcmp(code, "END") != 0) {
            if (strcmp(opcode, code) == 0) {  // If match is found, increase locctr
                locctr += 3;
                found = 1;
                break;
            }
            fscanf(optab, "%s", code);
        }

        // If no match found in opcode table, handle pseudo-operations
        if (!found) {
            if (strcmp(opcode, "WORD") == 0)
                locctr += 3;
            else if (strcmp(opcode, "RESW") == 0)
                locctr += 3 * atoi(operand);
            else if (strcmp(opcode, "RESB") == 0)
                locctr += atoi(operand);
            else if (strcmp(opcode, "BYTE") == 0)
                locctr += 1;
        }

        // Write current line's information to output file
        fprintf(output, "%s\t%s\t%s\n", label, opcode, operand);

        // Read the next line
        fscanf(input, "%s %s %s", label, opcode, operand);
    }

    // Write the final line and calculate program length
    fprintf(output, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
    program_length = locctr - start;
    printf("Length of the program: %d\n", program_length);

    // Close all files
    fclose(input);
    fclose(symtab);
    fclose(output);
    fclose(optab);

    return 0;   // End of program
}
