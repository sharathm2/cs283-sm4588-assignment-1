#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

int  count_words(char *, int, int);


int setup_buff(char *buff, char *user_str, int len) {
    int i = 0; //String index         
    int j = 0; //Buffer index        
    int last_was_space = 1; 
    char *p = user_str;
    char *b = buff;
    
    // Check length
    while (*p != '\0') {
        i++;
        if (i > len) return -1;
        p++;
    }
    
    p = user_str; // Reset pointer
    
    // Process string
    while (*p != '\0') {
        if (*p == ' ' || *p == '\t') {
            if (!last_was_space) {
                *b = ' ';
                b++;
                j++;
                last_was_space = 1;
            }
        } else {
            *b = *p;
            b++;
            j++;
            last_was_space = 0;
        }
        p++;
        i++;
    }
    
    // Fill with dots
    while (j < len) {
        *b = '.';
        b++;
        j++;
    }
    
    return i;
}

//print buffer
void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int word_count = 0;
    int in_word = 0;

    for (int i = 0; i < str_len; i++) {
        if (*(buff + i) != ' ') {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }

    return word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    
    /* This check is safe because the condition argc < 2 is evaluated first
    Once this check is done, if argc < 2, then the second condition will not be evaluated
    This prevents any illegal memory access of argv[1]
    */

    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    
    /* This if statement is to make sure that there are enough commands
    in order to run the program. If there are not enough commands, then the program will exit
    with a return code of 1.
    */
    
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;


        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            {
                //Reverse the string
                char temp;
                char *start = buff;
                char *end = buff + user_str_len - 1;
                
                while (start < end) {
                    temp = *start;
                    *start = *end;
                    *end = temp;
                    start++;
                    end--;
                }
                
                printf("Reversed String: ");
                for (int i = 0; i < user_str_len; i++) {
                    putchar(*(buff + i));
                }
                printf("\n");
            }
            break;

        case 'w':
            {
                //Print word count of string
                int word_count = 0;
                int char_count = 0;
                int in_word = 0;
                char *p = buff;
                
                printf("Word Print\n----------\n");
                
                for (int i = 0; i < user_str_len; i++, p++) {
                    if (*p != ' ') {
                        if (!in_word) {
                            word_count++;
                            if (word_count > 1) {
                                printf(" (%d)\n", char_count);
                            }
                            printf("%d. ", word_count);
                            char_count = 0;
                            in_word = 1;
                        }
                        putchar(*p);
                        char_count++;
                    } else {
                        in_word = 0;
                    }
                }
                if (char_count > 0) {
                    printf(" (%d)\n", char_count);
                }
                printf("\nNumber of words: %d\n", word_count);
            }
            break;
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
/*      I think that providing both the pointer and the length
        is a good practice because it allows the function to be more flexible.
    It also allows the function to be more efficient because it does not need to
    calculate the length of the buffer. Since we know the length of the buffer,
    the function can work more efficiently knowing the length of the buffer
*/