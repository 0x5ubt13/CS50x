#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t matchesMagicBytes(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, char *filename);
void generateFiles(FILE *f);
void printError(char *filename);
void printUsage(void);
 
int main(int argc, char *argv[])
{
    printf("Starting program\n");
    // Check that only one argument has been passed
    if (argc != 2)
    {
        printUsage();
        return 1;
    }

    // Open file
    FILE *raw_file = fopen(argv[1], "r");

    printf("Main\n");
    
    // RETURN VALUE
    //   Upon successful completion fopen(), fdopen(), and freopen() return a FILE pointer.   Otherwise,  NULL  is  re‐
    //   turned and errno is set to indicate the error.
    if (raw_file == NULL)
    {
        printError(argv[1]);
        return 1;
    }

    // Generated files should be named ###.jpg, starting with 000
    generateFiles(raw_file);

    // Clean up
    fclose(raw_file);
    
    return 0;
}

// Recover every one of the JPEGs from input file, storing each as a separate file in the current working directory. 
void generateFiles(FILE *f)
{
    printf("start generateFiles\n");

    // Allocate buffer to work with the raw file, and also allocate space for a dynamic filename
    uint8_t *buffer = malloc(512 * sizeof(uint8_t));
    char *filename = malloc(10 * sizeof(char));

    // Declaring needed vars and setting them accordingly
    FILE *nf;
    int filename_count = 0;
    short open_new = 1;
    short writing = 0;
    short counter = 0;

    // Start and Keep reading until EOF
    while (fread(buffer, 1, 512, f) == 512)
    {
        // printf("Reading buffer... -> %x, %x, %x, %x\n", buffer[0], buffer[1], buffer[2], buffer[3]); // Debug

        // Check whether a new jpg has been found
        uint8_t isMatch = matchesMagicBytes(buffer[0], buffer[1], buffer[2], buffer[3], filename);
        if (isMatch)
        {
            writing = 1;          
            counter++;

            // Close current file, enforced from 001.jpg 
            if (counter > 1)
            {
                fclose(nf);
                filename_count++;
                open_new = 1;
            }
        }

        // Create new filename and open new file
        if (open_new)
        {
            sprintf(filename, "%03i.jpg", filename_count); // Print a digit with 3 numbers to represent it, 
            printf("[+] Opening new file: %s\n", filename);
            nf = fopen(filename, "w");
        }

        // Write bytes from file <f> to new file <nf> with fwrite(data, size, number, outptr)
        if (writing)
        {
            fwrite(buffer, 1, 512, nf);
            // printf("[+] Written 512 bytes to %s\n", filename);
        }
        
        open_new = 0;
    }

    // Tidying up
    printf("[+] EOF reached. Closing %s\n", filename);
    fclose(nf);
    free(filename);
    free(buffer);
}

// Helper to find JPEG's magic bytes
uint8_t matchesMagicBytes(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, char *filename)
{

    // Using bitwise operation & 0xf0 to read only the first 4 bits
    if (byte1 == 0xff && byte2 == 0xd8 && byte3 == 0xff && (byte4 & 0xf0) == 0xe0)
    {   
        printf("[+] New image found. Closing %s\n", filename);
        return 1;
    }

    return 0;
}

// Prints error opening the file
void printError(char *fname)
{
    printf("There was an error trying to open file '%s'\n", fname);
}

// Prints usage to the user
void printUsage(void)
{
    printf("Error detected. Please, pass only one command-line argument.\nUsage:");
    printf("./recover <filename>\n");
}

