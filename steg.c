#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXLENGTH 100 // defining max length for encoding and decoding
#define BUFFEMAX 100  // defining max for buffer

/* The RGB values of a pixel. */
typedef struct pixel
{
    int red;
    int green;
    int blue;
} pixel;

/* Struct created to hold comments */
typedef struct text
{
    char *text;
    struct text *next;
} text;

/* Struct created to hold PPM Image data */
/* Question 1 */
struct PPM
{
    char *format;
    struct text *text;
    int cNo;           // number of comments
    int width, height; // width, height
    int max;           // max size
    struct pixel **p;
};

/* Read from a PPM file */
char *readFile(FILE *f)
{

    char buffer[BUFFEMAX];
    int i, j;
    char ch;
    char *line;

    i = 0;
    ch = getc(f);  // gets the next character from the specified stream
    if (ch == EOF) // EOF - end of the file
        return NULL;
    /*
    If the character is not a new line and the buffer(BUFFEMAX) is not exceeded,
    Insert the character to the array buffer.
    */
    while (ch != '\n' && i < (BUFFEMAX - 1))
    {
        buffer[i] = ch;
        i++;
        ch = getc(f);
    }

    if (ch != '\n')
        while (ch != '\n')
            ch = getc(f);
    buffer[i] = '\0';                      // Mark the end of the string by '\0'
    line = malloc((i + 1) * sizeof(char)); // Allocate memory for the line

    for (j = 0; j <= i; j++) // Append the charaters in the buffer to the line array
        line[i] = buffer[i];
    line[i] = '\0'; // Mark the end of the string by '\0'

    return line;
    free(buffer); // deallocate the memory allocated to buffer
}

/* find the comments(text) in the file*/
void findComment(FILE *f, struct PPM *i)
{
    char ch = getc(f);
    while (ch == '#') // find lines beginning with the "#" character
    {
        ungetc(ch, f);

        // get comment
        char *line = readFile(f);

        // add the found comments to text
        if (i->cNo == 0) // If there're no previous comments add the only comment to text
        {
            i->text = (struct text *)malloc(sizeof(struct text));
            i->text->text = line;
            i->cNo = 1;
        }
        else // If there're previous comments add all comments to text
        {
            int j;
            struct text *current = i->text;

            for (j = 1; j < i->cNo; j++)
                current = current->next;
            current->next = (struct text *)malloc(sizeof(struct text));

            current->next->text = line;
            i->cNo = i->cNo + 1;
        }

        // move to the next line
        ch = getc(f);
    }

    // go back to starting position
    ungetc(ch, f);
}

/* Reads an image from an open PPM file.
 * Returns a new struct PPM, or NULL if the image cannot be read. */
struct PPM *getPPM(FILE *f)
{
    /* Question 2a */
    struct PPM *i = malloc(sizeof(struct PPM)); // allocate memory for image i

    // format
    i->format = malloc(2 * sizeof(char)); // allocate memory for the ppm image format
    i->format = readFile(f);              // read the first line of the file and assign it to the format

    i->cNo = 0;        // Initialize number of comments to 0
    findComment(f, i); // get comments in the file

    // Size
    fscanf(f, "%d", &i->width);  // width
    fscanf(f, "%d", &i->height); // height
    fscanf(f, "%d", &i->max);    // max

    // get Pixel data from the file
    
    i->p = (struct pixel **)malloc(sizeof(struct pixel *) * i->height); // allocate memory for rows

    int j, k;
    for (j = 0; j < i->height; j++)
    {
        i->p[j] = (struct pixel *)malloc(sizeof(struct pixel) * i->width); // allocate memory for columns
        for (k = 0; k < i->width; k++)
        {
            // get pixels for each
            fscanf(f, "%d", &(i->p[j][k].red)); // get memory address
            fscanf(f, "%d", &(i->p[j][k].green));
            fscanf(f, "%d", &(i->p[j][k].blue));
        }
    }

    return i; // return image
}

/*
 *output the PPM image img to stdout
 *showPPM prints information stored in PPM struct in the given structural format
 */
void showPPM(struct PPM *i)
{
    /* Question 2b */

    // print format of PPM
    printf("%s\n", i->format);

    int j, k, l;
    // print text - comments
    struct text *t = i->text;
    printf("%s\n", i->text->text);
    for (j = 1; j < i->cNo; j++)
    {
        t = t->next;
        printf("%s\n", t->text);
    }

    // print width, height, and max value
    printf("%d\n", i->width);
    printf("%d\n", i->height);
    printf("%d\n", i->max);

    // print pixels
    for (k = 0; k < i->height; k++)
    {
        for (l = 0; l < i->width; l++)
        {
            struct pixel p = i->p[k][l];
            printf("%i %i %i\n", p.red, p.green, p.blue);
        }
    }
}

/*
 * Encode the string text into the red channel of image i.
 * Returns a new struct PPM, or NULL on error.
 * Return PPM image i with message (hidden within red field)
 */
struct PPM *encode(char *txt, struct PPM *i)
{
    /* Question 2c */
    srand(time(NULL)); // Randomize seed. Seed set to current time.
    int j, length, sum, random;

    sum = 0;
    length = strlen(txt);

    /*
     * Check for compatability
     */
    if ((length * 75) < (i-> width * i->height))
    {
        printf("steg: File is appropriate for encoding. \n");
    }
    else
    {
        printf("steg: Error - Image size is not acceptable for encoding. \n");
        exit(0);
    }

    // for each char
    for (j = 0; j < length;)
    {
        random = (rand() % 100);
        sum = sum + random;

        // calculate the pixel in terms of row and col
        int row, column;
        row = sum / (i->width);
        column = sum - (row * i->width);

        // get pixel
        struct pixel *p = &(i->p[row][column]); // Use memory address

        // check red pixel val not equals ASCII of the letter
        if (txt[j] != p->red)
        {
            /*
             * Assign the characters of the text one by one to the red pixels of the pixels calculated above
             */
            p->red = txt[j];
            j++;
        }
        else // If the red pixel value equals ASCII of the letter
        {
            fprintf(stderr, "[%d] - [%c] same. Retrying. \n", j, txt[j]);
        }
    }

    return i;
}

/* Extract the string encoded in the red channel of newimg, by comparing it
 * with oldimg. The two images must have the same size.
 * Returns a new C string, or NULL on error.
 */
char *decode(struct PPM *i1, struct PPM *i2)
{
    /* Question 2d */
    char *buffer = malloc(sizeof(char) * MAXLENGTH);

    // confirm that the files are the 'same'
    // If the width and height of two images don't match, print the error message
    if (!(i1->height == i2->height && i1->width == i2->width))
    {
        printf("steg: Error - PPM files do not match. \n");
        exit(0);
    }

    int j, k, l, m, n;
    l = 0;

    // Go through each pixel of the two images and compare red pixel values
    // for each row
    for (j = 0; j < i2->height; j++)
    {
        // for each column
        for (k = 0; k < i2->width; k++)
        {
            // check if red pixels are not equal
            if (i2->p[j][k].red != i1->p[j][k].red)
            {
                // get encoded character and append to buffer
                buffer[l] = i2->p[j][k].red;
                l = l + 1;
            }
        }
    }

    m = l - 1;

    // If the  end of the buffer is a new line, replace it with '\0' to mark the end
    if (buffer[m] == '\n')
    {
        buffer[m] = '\0';
    }

    char *str = malloc(sizeof(char) * m + 1);

    for (n = 0; n < m + 1; n++)
    {
        str[n] = buffer[n]; // Append each character in buffer to str
    }
    str[m + 1] = '\0'; // Mark the end of the string with '\0'

    free(buffer); // deallocate the memory allocated to buffer
    return str;
}

/* Write image in PPM format. */
void writePPM(FILE *f, struct PPM *i, const char *fileName)
{
    FILE *pfile = NULL;
    int x, y;

    pfile = fopen(fileName, "w");                        // opens the file in write mode and the file name is set to specified file name.
    fprintf(pfile, "P3\n%d %d\n255\n", i->width, i->height); // Sends the format, width, height, and the max value to the stream

    for (x = 0; x < i->height; x++) // Sends pixel data to the stream
    {
        for (y = 0; y < i->width; y++)
        {
            struct pixel p = i->p[x][y];
            fprintf(pfile, "%i %i %i \n", p.red, p.green, p.blue);
        }
    }
    fclose(pfile); // close the file
}

/* Question 3 */
int main(int argc, char const *argv[])
{

    /* Initialise the random number generator, using the time as the seed */
    srand(time(NULL));

    /* Parse command-line arguments */
    if (argc == 3 && strcmp(argv[1], "t") == 0) // Argument count is 3 and 2nd argument is t
    {
        /* Mode "t" - test PPM reading and writing */
        FILE *f = fopen(argv[2], "r"); // Opens the file specified in 3rd argument in read mode
        struct PPM *img = getPPM(f);     // Pass the file f to getPPM to Read and assign the returned value to img
        showPPM(img);                    // output the PPM image img to stdout
    }
    else if (argc == 3 && strcmp(argv[1], "e") == 0) // Argument count is 3 and the 2nd argument is e
    {
        /* Mode "e" - encode PPM */
        char txt[MAXLENGTH];
        int j;

        FILE *f = fopen(argv[2], "r"); // Opens the file specified in 3rd argument in read mode

        // If the file pointer is null
        if (f == NULL)
        {
            fprintf(stderr, " steg: Error - Unable to open the file '%s' .\n", argv[2]);
            exit(0);
        }

        // Pass the file f to getPPM to Read and assign the returned value to i
        struct PPM *i = getPPM(f);

        /* prompt for a message from the user, and read it into a string */
        fprintf(stderr, "Enter the message to encode: ");
        fgets(txt, MAXLENGTH, stdin);

        j = strlen(txt) - 1;
        if (txt[j] == '\n') // If the last character of the text is a new line repalce it with '\0' to mark the end
            txt[j] = '\0';

        fprintf(stderr, "Encoding in progress...\n");

        /* encode the text into the image with encode, and assign to newimg */
        struct PPM *newimg;
        newimg = encode(txt, i);

        /* write the image to stdout with writePPM */
        writePPM(f, i, "newimg.ppm"); // newimg.ppm is the file name of the file to be created with encoded text
        printf("Encoding completed.");
    }
    else if (argc == 4 && strcmp(argv[1], "d") == 0) // If the argument count is 4 and the 2nd argument is d
    {
        /* Mode "d" - decode PPM */

        struct PPM *oldimg;
        /* get original file filename from argv, load it with
           getPPM, then assign to oldimg */

        FILE *i1 = fopen(argv[2], "r"); // Opens the file specified in 3rd argument in read mode

        if (i1 == NULL)
        {
            printf("steg: Error - File '%s' could not be opened. \n", argv[2]);
            exit(0);
        }

        oldimg = getPPM(i1);

        struct PPM *newimg;
        /* get encoded file filename from argv, load it with
           readPPM, then assign to newimg */

        FILE *i2 = fopen(argv[3], "r"); // Opens the file specified in 4th argument in read mode

        if (i2 == NULL)
        {
            printf("steg: Error - File '%s' could not be opened. \n", argv[3]);
            exit(0);
        }

        newimg = getPPM(i2);

        /* decode the encoded message with the decode and assign to message */

        char *message = decode(oldimg, newimg);

        /* print the decoded message to stdout */
        printf("Decoded message: %s\n", message);
    }
    else
    {
        fprintf(stderr, "Unrecognised or incomplete command line.\n");
        return 1;
    }

    return 0;
}