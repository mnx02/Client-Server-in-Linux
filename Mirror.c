/*
AUTHORS : 

SAHIL MODI 110100446
MANAV CHETWANI 110100957 

*/




#define _XOPEN_SOURCE 500 // Vrsion 5oo is neded to rn nftw
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#define PORT 11958 // p0rt for rnning srver

static char *S_MAfilenam;// flenames asked by usr
struct stat MA_Sfilests;
int SAfileno = 0; // T0 store num. of fle 
char S_MAfleNam[100][256];
char MApathfle[100][256];
char SAnamExt[4][128]; // usd by targfz
int CHnumExt = sizeof(SAnamExt) / sizeof(SAnamExt[0]);
int MAunqFlenum = 0;
char **CHunqflenam = NULL;
char **CHunqflepath = NULL;
char MAdate0[11]; //Impklementation of datefunc -- getdirf
char MAdate1[11];
int MAno = 0;
int SAsize0 = 0; //Impklementation of sizfunc -- tarfgetz
int SAsize1 = 0;
int MAfleNum = 0;
char **CHunqfleNAM = NULL;
char **CHunqflePAth = NULL;

int SA_cmp_fle(const void *mc, const void *sm)
{
    return strcmp((const char *)mc, (const char *)sm);
}

int MC_fle_call(const char *MAflepth, const struct stat *SMfleSts, int typeflag, struct FTW *ftwbuf)
{
if (typeflag == FTW_F || typeflag == FTW_D)
{
char *SM_fleNAm = strrchr(MAflepth, '/');
if (SM_fleNAm != NULL)
    {
SM_fleNAm++; //Iterat to '/'
struct tm MCfleTmestmp;
strptime(MAdate0, "%Y-%m-%d", &MCfleTmestmp);
time_t date1Timestamp = mktime(&MCfleTmestmp);
strptime(MAdate1, "%Y-%m-%d", &MCfleTmestmp);
time_t date2Timestamp = mktime(&MCfleTmestmp);//Timestmp
    if (SMfleSts->st_mtime >= date1Timestamp && SMfleSts->st_mtime <= date2Timestamp)
    {
        if (SAfileno < 100)
        {
            int MAchckdupl = 0;
            for (int Mnum = 0; Mnum < SAfileno; Mnum++)
            {
                if (strcmp(S_MAfleNam[Mnum], SM_fleNAm) == 0)
                {
                    MAchckdupl = 1;
                    break;
                }
            }
            if (MAchckdupl == 0)//stor flname and filepath
            {
                strcpy(S_MAfleNam[SAfileno], SM_fleNAm);
                strcpy(MApathfle[SAfileno], MAflepth);
                SAfileno++;
            }
        }
    }
}
}
return 0;
}
void MAzipTarC(int SMsockFileDist)
{
FILE *MAfileDistp;
long int MAsizFle;
MAfileDistp = fopen("MASA_SENT.tar.gz", "rb");
if (MAfileDistp == NULL)
{
    fprintf(stderr, "Unable to reach file while opening file\n");
    return -1;
}
fseek(MAfileDistp, 0, SEEK_END);
MAsizFle = ftell(MAfileDistp);
fseek(MAfileDistp, 0, SEEK_SET);
    // cnvertin the flesize to char
    char SMchrfilesze[1024];
    sprintf(SMchrfilesze, "%ld", MAsizFle);
    // Snd the sze ofthe file as char
    send(SMsockFileDist, SMchrfilesze, sizeof(SMchrfilesze), 0);
    fclose(MAfileDistp);
    // Opn file to be transfered
    int MAfileDis = open("MASA_SENT.tar.gz", O_RDONLY);
    if (MAfileDis < 0)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
char MAbuf[1024];
ssize_t MArdvle;
// Red fle cntents and snd to cllien
while ((MArdvle = read(MAfileDis, MAbuf, 1024)) > 0)
{
    send(SMsockFileDist, MAbuf, MArdvle, 0);
}
// Close fle and socket
memset(MAbuf, 0, sizeof(MAbuf));
close(MAfileDis);
}

static int SM_Findflecall(const char *MAflepTh, const struct stat *SMfileStS, int flag, struct FTW *ftwbuf)
{
    if (flag == FTW_F)// Chckes if its rgulr fle
    { 
        if (strcmp(MAflepTh + ftwbuf->base, S_MAfilenam) == 0)
        {
            MA_Sfilests = *SMfileStS; // Cpy th file_stat strct tob glbl variable
            return 1;                    
        }
    }

    return 0; // Cntinue trvrsl
}

int MAfleFnd(char *SMfilNamsrc, char *MASAroot, struct stat *SMfleSts)
{
S_MAfilenam = SMfilNamsrc; // St th glbl flename varbl
if (nftw(MASAroot, SM_Findflecall, 10, FTW_PHYS) == -1)
{
    perror("nftw");
    return 0; // Fle nt fnd
}

// S_ISREG is a mcr tht takesfle mde vlue nd chck whthe it is sme to a rglar fle r nt
if (S_ISREG(MA_Sfilests.st_mode))
{
    *SMfleSts = MA_Sfilests; 
    return 1;
}

return 0;
}

int MA_extsimil(const char *S_MAfilenam, char SMEXt[4][128], int CHnumExt)
{
const char *fileExt = strrchr(S_MAfilenam, '.');
if (fileExt != NULL)
{
for (int Mnum = 0; Mnum < CHnumExt; Mnum++)
{
    if (strcmp(fileExt, SMEXt[Mnum]) == 0)
    {
        return 1;
    }
}
}
return 0;
}

int MA_chckFilNam(const char *MAfleNaM)
{
for (int Scount = 0; Scount < MAunqFlenum; Scount++)
{
    if (strcmp(MAfleNaM, CHunqflenam[Scount]) == 0)
{
    return 1; // Already added to CHunqflenam
}
}
return 0;
}

int SM_chckDtefileNam(const char *MAfleNaM)
{
for (int Scount = 0; Scount < MAfleNum; Scount++)
{
if (strcmp(MAfleNaM, CHunqfleNAM[Scount]) == 0)
{
    return 1; // Already added to CHunqflenam
}
}
return 0;
}

int MAprocfle(const char *MAflepTh, const struct stat *SMfleSysIn, int typeflag, struct FTW *ftwbuf)
{
if (typeflag == FTW_F && MA_extsimil(MAflepTh, SAnamExt, CHnumExt))
{
const char *MAfleNaM = strrchr(MAflepTh, '/');
if (MAfleNaM == NULL)
{
    MAfleNaM = MAflepTh; 
}
else
{
    MAfleNaM++; 
}

if (MA_chckFilNam(MAfleNaM))
{
    return 0; // Nt a un1qe fle nme
}

CHunqflenam = realloc(CHunqflenam, (MAunqFlenum + 1) * sizeof(char *));
CHunqflenam[MAunqFlenum] = strdup(MAfleNaM);

CHunqflepath = realloc(CHunqflepath, (MAunqFlenum + 1) * sizeof(char *));
CHunqflepath[MAunqFlenum] = strdup(MAflepTh);

MAunqFlenum++;
}
return 0;
}

int tarfGprocMAfile(const char *MAflepTh, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
if (typeflag == FTW_F && sb->st_size > SAsize0 && sb->st_size < SAsize1)
{
    const char *MAfleNaM = strrchr(MAflepTh, '/');
    if (MAfleNaM == NULL)
    {
        MAfleNaM = MAflepTh; // No '/' fnd, APpl th whle pth as the fle nme
    }
    else
    {
        MAfleNaM++;
    }

    if (SM_chckDtefileNam(MAfleNaM))
    {
        return 0; 
    }

    CHunqfleNAM = realloc(CHunqfleNAM, (MAfleNum + 1) * sizeof(char *));
    CHunqfleNAM[MAfleNum] = strdup(MAfleNaM);

    CHunqflePAth = realloc(CHunqflePAth, (MAfleNum + 1) * sizeof(char *));
    CHunqflePAth[MAfleNum] = strdup(MAflepTh);

    MAfleNum++;
}
return 0;
}

void processclient(int SMsockFileDist)
{
char MAbuf[256];
int mNum;
while (1)
{
bzero(MAbuf, sizeof(MAbuf));
mNum = read(SMsockFileDist, MAbuf, sizeof(MAbuf) - 1);
if (mNum <= 0)
{
    perror("Unable To read from socket establish from client");
    break;
}
printf("Command from Client -->: %s\n", MAbuf);
char MAClientResp[1024] = "Command received.\n";
if (mNum < 0)
{
    perror("Unable To write into socket establish from client");
    break;
}
else if (strcmp(MAbuf, "quit") == 0)
{
    printf("Command from Client is to Quit\n");
    break;
}
else if (strncmp(MAbuf, "filesrch ", 9) == 0)
{
    char *MAcmd = strtok(MAbuf, " ");
    char *S_MAfilenam = strtok(NULL, " ");
    struct stat SMfleSts;
    memset(&MA_Sfilests, 0, sizeof(MA_Sfilests)); // Nw fle stuct is voided
    // srch fr fle in dirctr tre rted at '~'
    if (MAfleFnd(S_MAfilenam, getenv("HOME"), &SMfleSts) != 0)
    {
        // format MAClientResp string
        char SMdteUpdt[20] = "";
        strftime(SMdteUpdt, sizeof(SMdteUpdt), "%Y-%m-%d %H:%M:%S", localtime(&SMfleSts.st_mtime));
        sprintf(MAClientResp, "Name of File Requested--> %s ~~  Size of File Requested--> %lld bytes  ~~  Updated Date of File Requested--> %s", S_MAfilenam, (long long)SMfleSts.st_size, SMdteUpdt);
        // send MAClientResp to client
        mNum = write(SMsockFileDist, MAClientResp, strlen(MAClientResp));
        if (mNum < 0)
        {
            perror("Unable To write into socket establish from client");
            break;
        }
    }
    else
    {           // send "No such given Files were present " MAClientResp to client
        sprintf(MAClientResp, "No such given Files were present ");
        mNum = write(SMsockFileDist, MAClientResp, strlen(MAClientResp));
        if (mNum < 0)
        {
            perror("Unable To write into socket establish from client");
            break;
        }
    }
}
else if (strncmp(MAbuf, "getdirf ", 8) == 0)
{            char *MAcmd = strtok(MAbuf, " ");
    strcpy(MAdate0, strtok(NULL, " "));
    strcpy(MAdate1, strtok(NULL, " "));
    char MAhom[256];
    const char *ENVsmHom = getenv("HOME");
    if (ENVsmHom == NULL)
    {
        perror("Unable to reach Home Dir");
        return 1;
    }
    strncpy(MAhom, ENVsmHom, sizeof(MAhom));
    MAhom[sizeof(MAhom) - 1] = '\0';
    int SMresNFTW = nftw(MAhom, MC_fle_call, 20, FTW_PHYS);
    if (SMresNFTW == -1)
    {
        perror("Unable to Traverse");
        return 1;
    }
    if (SAfileno == 0)
    {
        strcpy(MAClientResp, "Unable to Find files requested for Tar Zip.");
        mNum = write(SMsockFileDist, MAClientResp, strlen(MAClientResp));

    }
    qsort(S_MAfleNam, SAfileno, sizeof(S_MAfleNam[0]), SA_cmp_fle);//sort arr
    // Remv duplcats frm the fle nmes arr
    int MAunqNumcunt = 0;
    for (int Mnum = 0; Mnum < SAfileno; Mnum++)
    {
        if (Mnum == 0 || strcmp(S_MAfleNam[Mnum], S_MAfleNam[Mnum - 1]) != 0)
        {
            strcpy(S_MAfleNam[MAunqNumcunt], S_MAfleNam[Mnum]);
            strcpy(MApathfle[MAunqNumcunt], MApathfle[Mnum]);
            MAunqNumcunt++;
        }
    }
    char MAzipTarcmd[65535];
    snprintf(MAzipTarcmd, sizeof(MAzipTarcmd), "tar -czf MASA_SENT.tar.gz --transform='s,.*/,,'");
    for (int Mnum = 0; Mnum < MAunqNumcunt; Mnum++)
    {
        snprintf(MAzipTarcmd + strlen(MAzipTarcmd), sizeof(MAzipTarcmd) - strlen(MAzipTarcmd), " \"%s\"", MApathfle[Mnum]);
    }
    int MAoutputZip = system(MAzipTarcmd);
    if (MAoutputZip == 0)
    {
        strcpy(MAClientResp, "Temp.tar.gz is created");
        MAzipTarC(SMsockFileDist);
    }
    else
    {
        printf("Unable to Zip Requsted files\n");
        mNum = write(SMsockFileDist, MAClientResp, strlen(MAClientResp));
    }
}
else if (strncmp(MAbuf, "tarfgetz ", 9) == 0)
{
    char *MAcmd = strtok(MAbuf, " ");
    SAsize0 = atoi(strtok(NULL, " "));
    SAsize1 = atoi(strtok(NULL, " "));
    int MAflg = FTW_PHYS; // Neglct fllwing symb lnks
    if (nftw(getenv("HOME"), tarfGprocMAfile, 20, MAflg) == -1)
    {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
    if (MAfleNum > 0)
    {
        char MAzipTarcmd[99999];
        snprintf(MAzipTarcmd, sizeof(MAzipTarcmd), "tar -czf MASA_SENT.tar.gz --transform='s,.*/,,'");
        for (int Mnum = 0; Mnum < MAfleNum; Mnum++)
        {
            snprintf(MAzipTarcmd + strlen(MAzipTarcmd), sizeof(MAzipTarcmd) - strlen(MAzipTarcmd), " \"%s\"", CHunqflePAth[Mnum]);
        }
        if (system(MAzipTarcmd) == 0)
        {
            strcpy(MAClientResp, "Requested Tar Made--> MASA_SENT.tar.gz");
            // Snd zipp tar --> client
            MAzipTarC(SMsockFileDist);
        }
        else
        {
            strcpy(MAClientResp, "Unable to Zip Requsted files");
            mNum = write(SMsockFileDist, MAClientResp, strlen(MAClientResp));
        }
    }
    else
    {
        strcpy(MAClientResp, "Unable to Find files requested for Tar Zip");
        // printf("Unable to Find files requested for Tar Zip.\n");
        mNum = write(SMsockFileDist, MAClientResp, strlen(MAClientResp));
    }
}
else if (strncmp(MAbuf, "targzf ", 7) == 0)
{
    MAno = 0;
    MAunqFlenum = 0;
    char *MAcmd = strtok(MAbuf, " ");
    char *SMEXTnam = strtok(NULL, " ");
    while (SMEXTnam != NULL)
    {
    strcpy(SAnamExt[MAno], ".");
        strcat(SAnamExt[MAno], SMEXTnam);
        MAno++;
        SMEXTnam = strtok(NULL, " ");
    }

    const char *MAdir = getenv("HOME"); // To dirpth
    int MAflg = FTW_PHYS;                 // Use FTW_PHYS to avoi fllwing symbc lnks
    if (nftw(MAdir, MAprocfle, 20, MAflg) == -1)
    {
        perror("nftw");
        return 1;
    }
    if (MAunqFlenum > 0)
    {
        char MAzipTarcmd[99999];
        snprintf(MAzipTarcmd, sizeof(MAzipTarcmd), "tar -czf MASA_SENT.tar.gz --transform='s,.*/,,'");
        for (int Mnum = 0; Mnum < MAunqFlenum; Mnum++)
        {
            snprintf(MAzipTarcmd + strlen(MAzipTarcmd), sizeof(MAzipTarcmd) - strlen(MAzipTarcmd), " \"%s\"", CHunqflepath[Mnum]);
        }

        if (system(MAzipTarcmd) == 0)
        {
            strcpy(MAClientResp, "Requested Tar Made--> MASA_SENT.tar.gz");
            MAzipTarC(SMsockFileDist);
        }
        else
        {
            strcpy(MAClientResp, "Unable to Zip Requsted files");
            mNum = write(SMsockFileDist, MAClientResp, strlen(MAClientResp));
        }
    }
    else
    {
        strcpy(MAClientResp, "Unable to Find files requested for Tar Zip");
        // printf("Unable to Find files requested for Tar Zip.\n");
        mNum = write(SMsockFileDist, MAClientResp, strlen(MAClientResp));
    }
    for (int Mnum = 0; Mnum < 4; Mnum++)
    {
        strcpy(SAnamExt[Mnum], "");
    }
}
else if (strncmp(MAbuf, "fgets ", 6) == 0)
{
    char MA_lstflepresent[4][256]; // Arr to stre fond file pTHH
    int MAno = 0;
    char *MAcmd = strtok(MAbuf, " ");
    char *S_MAfilenam = strtok(NULL, " ");
    while (S_MAfilenam != NULL)
    {
        strcpy(MA_lstflepresent[MAno], S_MAfilenam);
        MAno++;
        S_MAfilenam = strtok(NULL, " ");
    }
    int numFoundFiles = 0; // Conter for fnd files
    for (int Mnum = 0; Mnum < MAno; Mnum++)
    {
        char MAcmd[1024];
        snprintf(MAcmd, sizeof(MAcmd), "find ~/ -type f -name %s -print -quit", MA_lstflepresent[Mnum]);
        // Execute the MAcmd
        FILE *MAfileDistp = popen(MAcmd, "r");
        if (MAfileDistp != NULL)
        {
            char SMflePth[256];
            if (fgets(SMflePth, sizeof(SMflePth), MAfileDistp) != NULL)
            {
                SMflePth[strcspn(SMflePth, "\n")] = '\0'; // Rmve trimmin nwline
                strcpy(MA_lstflepresent[numFoundFiles], SMflePth);
                numFoundFiles++;
            }
            else
            {
                //continue
            }
            pclose(MAfileDistp);
        }
    }
    if (numFoundFiles > 0)
    {
        char MAzipTarcmd[256];
        snprintf(MAzipTarcmd, sizeof(MAzipTarcmd), "tar -czf MASA_SENT.tar.gz --transform='s,.*/,,'");
        for (int Mnum = 0; Mnum < numFoundFiles; Mnum++)
        {
            snprintf(MAzipTarcmd + strlen(MAzipTarcmd), sizeof(MAzipTarcmd) - strlen(MAzipTarcmd), " \"%s\"", MA_lstflepresent[Mnum]);
        }
        int MAoutputZip = system(MAzipTarcmd);
        if (MAoutputZip == 0)
        {
            strcpy(MAClientResp, "Requested Tar Made--> MASA_SENT.tar.gz");

            MAzipTarC(SMsockFileDist);
        }
        else
        {
            strcpy(MAClientResp, "Unable to Zip Requsted files");
            mNum = write(SMsockFileDist, MAClientResp, strlen(MAClientResp));
        }
    }
    else
    {
        strcpy(MAClientResp, "Unable to Find files requested for Tar Zip");
        mNum = write(SMsockFileDist, MAClientResp, strlen(MAClientResp));
    }
}
else
{
    printf("Buffer: %s\n", MAbuf);
}
}

close(SMsockFileDist);
}


//main

int main()
{
int SMsockFileDist, SMsockFileDist2, MAProsID; // Sckt fle dist pid
socklen_t MAlenclint;                       
struct sockaddr_in SMsrvradd, MAcliadd; // Strv srvr nd clent add
// develop a TCP scket
SMsockFileDist = socket(AF_INET, SOCK_STREAM, 0);
if (SMsockFileDist < 0)//err handlin
{
perror("Unable to reach Socket which is created");
exit(1);
}
//set
bzero((char *)&SMsrvradd, sizeof(SMsrvradd));
SMsrvradd.sin_family = AF_INET;         // Add fmly
SMsrvradd.sin_addr.s_addr = INADDR_ANY; // CLindt ip
SMsrvradd.sin_port = htons(PORT);       // PRT Num

// bind()
if (bind(SMsockFileDist, (struct sockaddr *)&SMsrvradd, sizeof(SMsrvradd)) < 0)
{
perror("Unable to bind Socket which is creted");
exit(1);
}

//Listn()
listen(SMsockFileDist, 5);
MAlenclint = sizeof(MAcliadd);
printf("Listening to Port 11958----\n");
while (1)
{
//Accpt()
SMsockFileDist2 = accept(SMsockFileDist, (struct sockaddr *)&MAcliadd, &MAlenclint);
if (SMsockFileDist2 < 0)
{ 
    perror("Unable to accept request");
    exit(1);
}
printf("New Clint has been establish\n");

// ForK()to newly rcvied clint req.
MAProsID = fork();
if (MAProsID < 0)
{ 
    perror("Unable to fork PRos");
    exit(1);
}
if (MAProsID == 0)
{
    // Chld
    close(SMsockFileDist);            // RMv th prnt socket
    processclient(SMsockFileDist2); 
    exit(0);                  // Ext()
}
else
{
    // Parnt process
    close(SMsockFileDist2); 
    waitpid(-1, NULL, WNOHANG);
}
}
close(SMsockFileDist); // Close srver scket
return 0;
}