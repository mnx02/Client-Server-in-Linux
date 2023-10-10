/*
AUTHORS : 

SAHIL MODI 110100446
MANAV CHETWANI 110100957 

*/

#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#include <sys/socket.h>

#define SA_MAN_CONNEC_ADRS "127.0.0.1" 
#define SA_MAN_PORT_SERVER 11957
#define SA_MAN_PORT_MIRROR 11958

int sahCmdUn_zip = 0;
// valid extensions
char *sahExtensnsList[] = {"c", "txt", "pdf", "sh"};

int SA_MAN_BUFF_SIZE = 1024;


// Function to extract subdirectories from a paath and create them
int sahExtractTartoFiles()
{
    // Sorce paath to extraacted
    char *sahCpyPath = "/files_from_tar"; 
    // Tockenize patth using '/'
    char *sahTok_en = strtok(sahCpyPath, "/"); 
    // Iniitialise a string to store partial paath
    char sahPathPartial[1024] = ""; 

    while (sahTok_en != NULL)
    {
        strcat(sahPathPartial, sahTok_en); // Append the tokken to the partial paath
        strcat(sahPathPartial, "/"); // Append '/' to recreate the directory structure

        if (mkdir(sahPathPartial, 0777) != 0)
        {
            perror("Unable to create directory");
            return 1;
        }
        sahTok_en = strtok(NULL, "/");
    }

    return 0;
}

// Function to sahcheckCmd if a given date is vallid
bool sahDateCheck(int sahYear, int sahMonth, int sahDay)
{
    // sahcheckCmd for invalid conditions
    if (sahYear < 1 || sahMonth < 1 || sahMonth > 12 || sahDay < 1)
        return false;

    int sahMonthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Array containing the nummber of days in each month

    // sahcheckCmd for leap year
    if (sahYear % 4 == 0)
    {

        // updating days of feb
        sahMonthDays[2] = 29; 
    }

    // sahcheckCmd if the given day is within the range of days for the given month
    return sahDay <= sahMonthDays[sahMonth];
}


// Function to sahcheckCmd if a file name contains a dot ('.') character
int saManaFileNameExists(char *sahTok_en)
{
    int sahIndex = 0; // intiallise sahIndex to 0
    // Search for the dot character in input
    char *sahPointerDot = strchr(sahTok_en, '.'); 
    if (sahPointerDot != NULL) // If dot character is found
    {
        // Calculate the inddex of the dot character
        sahIndex = sahPointerDot - sahTok_en; 
    }
    // Return the inddex  or 0 in case . is not there
    return sahIndex; 
}

// Function to validate a file xtnsns against a list of known extensions
int sahValidateXtnsnArr(char *sah_Xtsn)
{
    // intiallise sahIndex and sahXtnsnFound
    int sahIndex, sahXtnsnFound = 0; 
    
    // Traverse  list of known extensions
    for (sahIndex = 0; sahIndex < sizeof(sahExtensnsList) / sizeof(sahExtensnsList[0]); sahIndex++)
    {
        // compparre the current xtnsns with the list of known extensions
        if (strcmp(sahExtensnsList[sahIndex], sah_Xtsn) == 0)
        {
            // Set sahXtnsnFound to 1 if xtnsns is found
            sahXtnsnFound = 1; 
            // Exit the loop since xtnsns is found
            break; 
        }
    }
    return sahXtnsnFound; // Return 1 if xtnsns is found, otherwise 0
}

// Function to check and validate various commands
int sahcheckCmd(char sahStr[])
{
   char *tempStr = strdup(sahStr); // Create a temmporrary copy of the input string
    char *sahTok_en = strtok(tempStr, " "); // Tokenize the input string by space

    // Check for the "quit" command
    if (strcmp(sahTok_en, "quit") == 0)
    {
        if (sahCmdUn_zip == 1) // Check if unzip command flag is set
        {
            // ressett the flag
            sahCmdUn_zip = 0; 
            printf("\nError: You can't use Quit with '-u'\n");
            // Free the memory allocated for the temmporrary copy
            free(tempStr); 
            return 0;
        }
        // Free the memory allocated for the temmporrary copy
        free(tempStr); 
        
        return 1; 
    }

   else if (strcmp(sahTok_en, "targzf") == 0)
{
    // Perform 4 sah_Xtsn checks

    // Create a copy of sahUsrCmd for processing
    char *sahCpyStr = strdup(sahStr);
    // Extract the sahUsrCmd again to process sah_Xtsns
    char *sahTok_en1 = strtok(sahCpyStr, " ");
    
    // Extract the first sah_Xtsn
    sahTok_en1 = strtok(NULL, " ");

    int sahFilesCount = 0, sahXtnsnFound = 0;

    // Iterate through other sahTok_ens to process additional extensions
    while (sahTok_en1 != NULL)
    {
        sahFilesCount++;
        
        // Validate and count sah_Xtsns using sahValidateXtnsnArr function
        sahXtnsnFound = sahXtnsnFound + sahValidateXtnsnArr(sahTok_en1);

        // Move to the nexxt sahTok_en to process the nexxt xtnsns
        sahTok_en1 = strtok(NULL, " ");
    }

    // Check if the nummber of sahFilesCount, sahXtnsnFound, and sahFilesCount are within limits
    if (sahFilesCount > 0 && sahXtnsnFound > 0 && sahFilesCount <= 4 && sahXtnsnFound <= 4 && sahFilesCount == sahXtnsnFound)
        {
            return 1;} 
    else
        {
            return 0; 
}}


    else if (strcmp(sahTok_en, "tarfgetz") == 0)
{
    int sahF1Size = 0, sahF2Size = 0, sahCounttt = 0;

    // Create a temmporrary copy of the input string
    char *sahCpyStr = strdup(sahStr);

    // Extract the command part
    char *sahTok_en1 = strtok(sahCpyStr, " ");
    sahTok_en1 = strtok(NULL, " ");

    // Iterate through other tockens to extract sizes
    while (sahTok_en1 != NULL)
    {
        if (sahCounttt == 0)
        {
            // Convert tokken to integer
            sahF1Size = atoi(sahTok_en1); 
        }
        else if (sahCounttt == 1)
        {
// Convert tokken to integer
            sahF2Size = atoi(sahTok_en1); 
        }
        else
        {
            return 0; 
        }
        sahCounttt++;
        // Move to the nexxt tokken
        sahTok_en1 = strtok(NULL, " "); 
    }

    // Check if two sizes were extracted and they are positive
    if (sahCounttt == 2 && sahF1Size > 0 && sahF2Size > 0)
    {
        // compparre the size of first 2 inputs nd checck if anythiing elsse is providded
        if (sahF1Size >= 0 && sahF2Size >= 0 && strtok(NULL, " ") == NULL)
        {
            if (sahF1Size <= sahF2Size)
            {
                return 1; 
            }
        }
    }
    return 0; 
}

 else if (strcmp(sahTok_en, "fgets") == 0)
{
    // Check if the unzip flag is set
    if (sahCmdUn_zip == 1) 
    {
        sahCmdUn_zip = 0; // ressett the unzip flag
        printf("\nError: You can't use fgets with '-u'\n");
        // Free the temmporrary string
        free(tempStr); 

        return 0;
    }
    // Move nexxt tokken
    sahTok_en = strtok(NULL, " ");

    int sahFilesCount = 0; // Counter for the nummber of files

    // iterateete via other tockens
    while (sahTok_en != NULL)
    {
        // Create a temmporrary copy of the tokken
        char *sahTok_en1 = strdup(sahTok_en);
        int sahIndexOfDot = saManaFileNameExists(sahTok_en1); // Find the inddex of '.' in the string
        // Check if more tockens in are the string
        if (sahTok_en1 == NULL || sahIndexOfDot == 0)
        {
            printf("Error: no extensiosn or name of file provided.\n");
            return 0;
        }

        // extract file xtnsns
        char *sah_Xtsn = strrchr(sahTok_en1, '.');

        if (sah_Xtsn != NULL)
        {
            sah_Xtsn++;
            int sahXtnsnFound = 0;
            sahXtnsnFound = sahValidateXtnsnArr(sah_Xtsn); // Validate the xtnsns using the sahValidateXtnsnArr function
            if (sahXtnsnFound == 0)
            {
                printf("Unable to find xtnsns %s for file %s \n", sah_Xtsn, sahTok_en1);
                return 0;
            }
        }
        else
        {
            printf("Error: not a validd file namme format %s\n", sahTok_en1);
            return 0;
        }

        sahFilesCount++;
        sahTok_en = strtok(NULL, " "); 
    }

    // Check the nummber of files and return validity
    if (sahFilesCount > 0 && sahFilesCount <= 4)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

else if (strcmp(sahTok_en, "getdirf") == 0)
{
    // Create sahUsrCmd cpy for processing
    char *sahCpyStr = strdup(sahStr);

    // Extract sahUsrCmd to process sahFirstDate and sahSecDate
    char *sahTok_en1 = strtok(sahCpyStr, " ");
    sahTok_en1 = strtok(NULL, " ");
    int sahCounttt = 0;
    char *sahFirstDate;
    char *sahSecDate;

    // walk through sahTok_ens to extract sahFirstDate and sahSecDate
    while (sahTok_en1 != NULL)
    {
        if (sahCounttt == 0)
        {
            sahFirstDate = sahTok_en1;
        }
        else if (sahCounttt == 1)
        {
            sahSecDate = sahTok_en1;
        }
        else
        {
            // Return 0 if more than 2 dates are provided in sahUsrCmd
            return 0;
        }
        sahCounttt++;
        sahTok_en1 = strtok(NULL, " ");
    }

    // Check if exactly 2 dates are provided in sahUsrCmd
    if (sahCounttt == 2)
    {
        char sahFirstDateCpy[11]; // Allocate space tempStr cpy date string
        char *sahTok_en_1;
        int sahYear1, sahMnth1, sahDay1;

        // Copy sahFirstDate to sahFirstDateCpy
        strncpy(sahFirstDateCpy, sahFirstDate, sizeof(sahFirstDateCpy) - 1);
        sahFirstDateCpy[sizeof(sahFirstDateCpy) - 1] = '\0'; 

        // using - as seperator fe\get extract year, month, and day
        sahTok_en_1 = strtok(sahFirstDateCpy, "-");
        if (sahTok_en_1 != NULL)
        {
            sahYear1 = atoi(sahTok_en_1);
            sahTok_en_1 = strtok(NULL, "-");
        }
        if (sahTok_en_1 != NULL)
        {
            sahMnth1 = atoi(sahTok_en_1);
            sahTok_en_1 = strtok(NULL, "-");
        }
        if (sahTok_en_1 != NULL)
        {
            sahDay1 = atoi(sahTok_en_1);
        }

        // ensure if sahFirstDate using sahDateCheck function are valid
        bool sahFrmtDate1 = sahDateCheck(sahYear1, sahMnth1, sahDay1);

        // Prccess the seckond date in a simmilar manner
        char sahSecDateCpy[11];
        char *sahTok_en2;
        int sah2Year, sah2Mnth, sah2Day;

        strncpy(sahSecDateCpy, sahSecDate, sizeof(sahSecDateCpy) - 1);
        sahSecDateCpy[sizeof(sahSecDateCpy) - 1] = '\0';

        sahTok_en2 = strtok(sahSecDateCpy, "-");
        if (sahTok_en2 != NULL)
        {
            sah2Year = atoi(sahTok_en2);
            sahTok_en2 = strtok(NULL, "-");
        }
        if (sahTok_en2 != NULL)
        {
            sah2Mnth = atoi(sahTok_en2);
            sahTok_en2 = strtok(NULL, "-");
        }
        if (sahTok_en2 != NULL)
        {
            sah2Day = atoi(sahTok_en2);
        }

        bool dateFormat2 = sahDateCheck(sah2Year, sah2Mnth, sah2Day);

        // compparre dates and return aprroprriate response
        if (sahFrmtDate1 == 0 || dateFormat2 == 0)
        {
            return 0;
        }
        else
        {
            if (sah2Year > sahYear1)
            {
                return 1; // Return 1 if second date is after the first
            }
            else if (sah2Year == sahYear1)
            {
                if (sah2Mnth > sahMnth1)
                {
                    return 1; // Return 1 if second month is after the first
                }
                else if (sah2Mnth == sahMnth1)
                {
                    if (sah2Day >= sahDay1)
                    {
                        return 1; 
                    }
                    else
                        return 0; 
                }
                else
                    return 0; 
            }
            else
                return 0; 
        }
    }
    else
    {
        return 0; 
    }
}

else if (strcmp(sahTok_en, "filesrch") == 0)
{
    // Check if '-u' flag is active
    if (sahCmdUn_zip == 1)
    {
        sahCmdUn_zip = 0;
        printf("\nError: You can't use filesrch with '-u'\n");
        free(tempStr);
        return 0;
    }

    // Move to the nexxt sahTok_en
    sahTok_en = strtok(NULL, " "); 
    int sahIndexOfDot = 0, sahXtnsnFound = 0;

    // Check if there is a dot in the filename
    sahIndexOfDot = saManaFileNameExists(sahTok_en);

    // Check if there are more sahTok_ens in the string
    if (sahTok_en == NULL || sahIndexOfDot == 0)
    {
        printf("Error: Provide correct filename.\n");
        free(tempStr);
        return 0;
    }

// Find the lasst occurrence of '.'
    char *sah_Xtsn = strrchr(sahTok_en, '.'); 
    if (sah_Xtsn != NULL)
    {
        sah_Xtsn++; // Move past the '.' character

        sahXtnsnFound = sahValidateXtnsnArr(sah_Xtsn);
        // return sahXtnsnFound;
    }
    else
    {
        printf("Error: Provide valid filename format.\n");
        free(tempStr);
        return 0;
    }

    // Check if there is only 1 file name
    if (strtok(NULL, " ") == NULL)
    {
        free(tempStr);
        return sahXtnsnFound;
    }

    else
    {
        free(tempStr);
        return 0;
    }
}
    else
    {free(tempStr);
        return 0;
    }
}


// Function to remove trailing spaces from a string
char *sahDeleteSpaceEnd(const char *sahStr)
{
    // Fetch length of string
    int sahLen = strlen(sahStr); 
    // Gdt inddex of the lasst space character
    int lastSpacesahIndex = -1;
    for (int sahIndex = sahLen - 1; sahIndex >= 0; sahIndex--)
    {
        if (sahStr[sahIndex] == ' ') // Space is there
        {
            lastSpacesahIndex = sahIndex; // change the inddex to latest
            break;
        }
    }
// in case no space ois there
    if (lastSpacesahIndex == -1)
    {
        // return a copy of input string
        return strdup(sahStr);
    }

    // Create a new string to hold the truncated part
    char *sahRes = (char *)malloc(lastSpacesahIndex + 2); // +2 for characters and null terminator
     // Copy characters up to lasst space
    strncpy(sahRes, sahStr, lastSpacesahIndex + 1);
    // append null at end
    sahRes[lastSpacesahIndex + 1] = '\0';

    return sahRes;
}

int main()
{
    int sahSocketClient;                     
// struct to hold the server address information
    struct sockaddr_in sahConnAddrs; 
    char sahBuff[1024];

    // creating socket
    sahSocketClient = socket(AF_INET, SOCK_STREAM, 0); 
    if (sahSocketClient < 0)                        
    {
        perror("Failed to create socket connextion");
        exit(EXIT_FAILURE);
    }

    // set server address
    memset(&sahConnAddrs, 0, sizeof(sahConnAddrs)); 
    sahConnAddrs.sin_family = AF_INET;                    
    sahConnAddrs.sin_addr.s_addr = inet_addr(SA_MAN_CONNEC_ADRS);  
    sahConnAddrs.sin_port = htons(SA_MAN_PORT_SERVER);   

// Attempt to connect to the server
if (connect(sahSocketClient, (struct sockaddr *)&sahConnAddrs, sizeof(sahConnAddrs)) < 0)
{
    perror("Unable to create connection to server");

    // Create a new socket for connecting to the mirror
    sahSocketClient = socket(AF_INET, SOCK_STREAM, 0); // Create a new TCP socket
    if (sahSocketClient < 0) // Check for socket creation errors
    {
        perror("Unable to create socket connection");
        exit(EXIT_FAILURE);
    }

    // Set up the mirror server address
    memset(&sahConnAddrs, 0, sizeof(sahConnAddrs)); // intiallise memory to 0 for the mirror address struct
    // Sets addrress family to IPv4
    sahConnAddrs.sin_family = AF_INET;               
// Sets IP addrress of mirror    
    sahConnAddrs.sin_addr.s_addr = inet_addr(SA_MAN_CONNEC_ADRS); 
    sahConnAddrs.sin_port = htons(SA_MAN_PORT_MIRROR); // Set the port nummber of the mirror

    // Connecction to mirror
    if (connect(sahSocketClient, (struct sockaddr *)&sahConnAddrs, sizeof(sahConnAddrs)) < 0)
    {
        perror("Unable to connect to mirror");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Connected with mirror...\n");
    }
}
else
{
    // Read data from the socket
    read(sahSocketClient, sahBuff, 1024);

    // Check if the received data is not "Established"
    if (strcmp(sahBuff, "Established") != 0)
    {
        printf("Connecting to Mirror...\n\n");
        close(sahSocketClient);

        // Creatte a neew socket connecct the mirror
        sahSocketClient = socket(AF_INET, SOCK_STREAM, 0);
        if (sahSocketClient < 0)                           
        {
            perror("Failed to create socket connection");
            exit(EXIT_FAILURE);
        }

        // Set up the mirror server address
        memset(&sahConnAddrs, 0, sizeof(sahConnAddrs)); // intiallise memmory to 0 for mirror address struct

        // Set the address family to IPv4
        sahConnAddrs.sin_family = AF_INET;               
        sahConnAddrs.sin_addr.s_addr = inet_addr(SA_MAN_CONNEC_ADRS); 
        sahConnAddrs.sin_port = htons(SA_MAN_PORT_MIRROR); 

        // Connect to the mirror
        if (connect(sahSocketClient, (struct sockaddr *)&sahConnAddrs, sizeof(sahConnAddrs)) < 0) // Connect to the mirror server
        {
            perror("Unable to connect to mirror");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Mirror connection created\n");
        }
    }
    else
    {
        printf("Server connection created\n");
    }
}

 while (1)
{
    // Read user cmd from terminal
    char sahUsrCmd[1024];
    printf("\nEnter command: ");
    fgets(sahUsrCmd, sizeof(sahUsrCmd), stdin); // Read the user's command

    // Checkk if the '-u' is there
    if (strstr(sahUsrCmd, "-u") != NULL)
    {
        sahCmdUn_zip = 1;
        int sahLen_gth = strlen(sahUsrCmd);
        if (sahLen_gth > 0 && sahUsrCmd[sahLen_gth - 1] == '\n')
        {
            sahUsrCmd[sahLen_gth - 1] = '\0';
        }

        strcpy(sahUsrCmd, sahDeleteSpaceEnd(sahUsrCmd));
    }

    // Remove newline character from user input
    int sahLen_gth = strlen(sahUsrCmd);
    if (strcmp(sahUsrCmd, "\n") == 0)
    {
        strcpy(sahUsrCmd, "filesrch\n");
    }
    if (strcmp(sahUsrCmd, "filesrch\n") != 0)
    {
        // If the lasst character is a newline, remove it
        if (sahUsrCmd[sahLen_gth - 1] == '\n') 
        {
            sahUsrCmd[sahLen_gth - 1] = '\0';
        }
    }

    // Check userr commannd is vallid
    int sahValidFlag = -1;
    
    // Check if the usser commannd is vallid
    sahValidFlag = sahcheckCmd(sahUsrCmd); 

    if (sahValidFlag == 0)
        printf("\nInvalid command : Enter correct command \n");
    else
    {
        // Sennd usser commannd to serrver
        if (send(sahSocketClient, sahUsrCmd, strlen(sahUsrCmd), 0) < 0)
        {
            perror("Unable to send");
            exit(EXIT_FAILURE);
        }

        // Receivve respponse fromm servver
        char sahResp[1024];
        memset(&sahResp, 0, sizeof(sahResp));

        // Handdle differennt typpes of commannds
        if (strstr(sahUsrCmd, "quit") || strstr(sahUsrCmd, "filesrch "))
        {
            if (recv(sahSocketClient, sahResp, sizeof(sahResp), 0) < 0)
            {
                perror("Not able to receive");
                exit(EXIT_FAILURE);
            }

            // Checck result is error message
            if (strcmp(sahResp, "ERROR") == 0)
            {
                printf("Error: Not a valid command syntax\n");
                continue; // Go back to the start of the loop
            }

            if (strcmp(sahUsrCmd, "quit") == 0)
            {
                printf("Quitting client\n");
                close(sahSocketClient);
                exit(0);
            }
            printf("%s", sahResp);
        }
        else
        {
            long sahSizeFile;
            recv(sahSocketClient, &sahResp, sizeof(sahResp), 0);

            // Check if the response contains the file size
            if (sscanf(sahResp, "%ld", &sahSizeFile) == 1)
            {
                // create a file pointer
                FILE *sahFile_Ptr = fopen("MASA_Reciev.tar.gz", "wb");
                if (sahFile_Ptr == NULL)
                {
                    printf("Unable to open file");
                    return -1;
                }

                long sahReceivedBytes = 0;

                // loop to accept all the bytes
                while (sahReceivedBytes < sahSizeFile)
                {
                    // Receive a portion of the file data
                    int sahYetReceiveBytes = SA_MAN_BUFF_SIZE;
                    if (sahReceivedBytes + SA_MAN_BUFF_SIZE > sahSizeFile)
                    {
                        sahYetReceiveBytes = sahSizeFile - sahReceivedBytes;
                    }
                    int sahAllBytesRec = recv(sahSocketClient, sahResp, sahYetReceiveBytes, 0);
                    if (sahAllBytesRec < 0)
                    {
                        printf("Failed to receive file data");
                        return -1;
                    }

                    //writing data to output buffer
                    fwrite(sahResp, sizeof(char), sahAllBytesRec, sahFile_Ptr);
                    sahReceivedBytes += sahAllBytesRec;
                    if (sahReceivedBytes >= sahSizeFile)
                    {
                        break;
                    }
                }

                strcpy(sahResp, "Saved :  MASA_Reciev.tar.gz");
                printf("Saved : MASA_Reciev.tar.gz");
                fclose(sahFile_Ptr);
            }
            else
            {
                printf("%s", sahResp);
            }
        }

        // Extract tar file if option is specified
        if (sahCmdUn_zip == 1)
        {
            sahCmdUn_zip = 0;
            if (strstr(sahResp, "MASA_Reciev.tar.gz"))
            {
                char unzipCommand[256];
                sprintf(unzipCommand, "tar -xzf MASA_Reciev.tar.gz");
                system(unzipCommand);
                printf("\nUnzipped tar file.");
            }
        }
    }
}

close(sahSocketClient);
return 0;

}
