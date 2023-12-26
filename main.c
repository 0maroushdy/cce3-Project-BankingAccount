#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>



//  --------------------------------------Omar---------------------------------------------------------------
typedef struct {
    int month, year;
} Date;

struct users
{
    char username[24];
    char password[24];
};

typedef struct {
    char accNum[15];
    char name[50];
    char address[50];
    float balance;
    char mobile[12];
    Date dateOpened;
} Account;

typedef struct {
    char date[20];
    double amount;
    char description[50];
}Transaction;


// Global Variables ...
const char *months[] = {"January", "February", "March","April", "May", "June","July", "August", "September","October", "November", "December"};

int numAccounts;

/// DONE...
int numberOfAccounts() {
    char line[250];
    int numAccounts= 0;
    FILE *file = fopen("accounts.txt", "r");

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strlen(line) > 1) {
            numAccounts++;
        }
    }

    fclose(file);
    return numAccounts;
}

/// Done...
// All Accounts Load in an array of struct Account, it takes a parameter: Pointer on Accounts Number, then it call by reference
Account *loadAccounts() {
    int i;
    char line[150];

    FILE *file = fopen("accounts.txt", "r");
    if (!file) { printf("Error opening the file..."); }

    int numAccounts = numberOfAccounts();   // Calcuating the number of Accounts & store it in the Global Variable.
    Account *accounts = malloc(numAccounts * sizeof(Account));

    for (i = 0; i < numAccounts; i++) {
        if (fgets(line, sizeof(line), file) && strlen(line)>1 ) {
            char *token = strtok(line, ",");
            strcpy(accounts[i].accNum, token);

            token = strtok(NULL, ",");
            strncpy(accounts[i].name, token, 50);

            token = strtok(NULL, ",");
            strncpy(accounts[i].address, token, 50);

            token = strtok(NULL, ",");
            accounts[i].balance = atof(token);

            token = strtok(NULL, ",");
            strncpy(accounts[i].mobile, token, 12);

            token = strtok(NULL, "-");
            accounts[i].dateOpened.month = atoi(token);

            token = strtok(NULL, "\n");
            accounts[i].dateOpened.year = atoi(token);
        }
    }

    fclose(file);
    return accounts;
}

/// Done...
// Printing An Account REQ. the Struct Account Pointer, Parameter --> allAccount[index]
void printAccount(Account *account, int i){
    if (account && i<numAccounts) {
        printf("------ *Account Details* ------\n\n");
        printf("Name: \t\t%s\n", account[i].name);
        printf("Name: \t\t%s\n", account[i].name);
        printf("E-mail: \t%s\n", account[i].address);
        printf("Balance: \t%.2f $\n", account[i].balance);
        printf("Mobile: \t%s\n", account[i].mobile);
        printf("Date Opened: \t%s %d\n\n", months[account[i].dateOpened.month-1], account[i].dateOpened.year);
    }
    else if(i >= numAccounts){
    printf("Invalid account index, out of Accounts Number!\n");
    } else {
    printf("Failed to load account!\n");
    }
}

/// Done...
/// *QUERY* Search...
char searchAccount(Account *allAcc){
    int i, found=0;
    char id[11], temp;
    printf("Search for The Account Number : ");
    gets(id);
    printf("\n");

    for(i=0; i<numAccounts; i++)
    {
        if(atof(id) == atof(allAcc[i].accNum) )
        {
            found=1; break;
        }
    }

    if(found)
    {
        printAccount(allAcc, i);
    }
    else
    {
       printf("Error ... Account Was Not Found On The System !!\n");
    }

    return id;     //error ------> (local array) <------
}

/// ??? 1 Problem ???
/// *Adding Account*
void addAccount(Account *allAcc )
{
    FILE* file= fopen("accounts.txt","a");
    char lenth[50];
    printf("> Enter The Account Number :");
    gets(lenth);  fprintf(file,"%s,",lenth);

    printf("\n> Enter your Name (First & Last Name) :");
    gets(lenth);  fprintf(file,"%s,",lenth);
    printf("\n> Enter your E-mail :");

    gets(lenth);  fprintf(file,"%s,",lenth);
    fprintf(file,"0,"); // balance

    printf("\n> Enter your Phone Number :");
    gets(lenth);  fprintf(file,"%s,",lenth);

    time_t t = time(NULL);
    struct tm *current_time = localtime(&t);
    fprintf(file,"%02d-%d\n", current_time->tm_mon + 1, current_time->tm_year + 1900);  // Print the formatted date

    fclose(file);

    /// ??? How to reLOAD the Accounts Array ???
    //*allAcc = loadAccounts(); // New Load Updating the Loaded Accounts Data.
    //*allAcc = *newAllAcc;
    //free(newAllAcc);
}


//  --------------------------------------Mostafa----------------------------------------------------------
int check_Validation_OfAddress(const char* address){
    // Check if address is NULL
    if (address == NULL) {
        return 0; // Invalid address
    }

    // Make a copy of the address
    char* addressCopy = strdup(address);
    if (addressCopy == NULL) {
        return 0; // Memory allocation failed
    }

    char* token;

    // Check if address contains '@'
    token = strtok(addressCopy, "@");
    if (token == NULL) {
        free(addressCopy);
        return 0; // Invalid address
    }

    // Check if address contains '.' after '@'
    token = strtok(NULL, ".");
    if (token == NULL || strlen(token) == 0) {
        free(addressCopy);
        return 0; // Invalid address
    }

    // Check if there are characters after '.'
    token = strtok(NULL, "");
    if (token == NULL || strlen(token) == 0) {
        free(addressCopy);
        return 0; // Invalid address
    }

    free(addressCopy);
    return 1; // Valid address
}
//Done
int isValidName(const char *namePart) {
    int length = strlen(namePart);
    for (int i = 0; i < length; i++) {
        if (!isalpha(namePart[i])) {
            return 0; // If a non-alphabetic character is found, the name part is not valid
        }
    }
    return 1; // If all characters are alphabetic, the name part is valid
}
//Done
bool accountExists(const char* accountNumber, Account allAccounts[], int numAccounts, int *i) {
    for ( *i = 0; *i < numAccounts; (*i)++) {
        if (strcmp(allAccounts[*i].accNum, accountNumber) == 0) {
            return true;
        }
    }
    return false;
}
//Done
void appendTransaction(const char* filename, double amount, const char* description) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file for appending.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(file, "%d-%02d-%02d %.2f %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, amount, description);

    fclose(file);
}
//Done
void save_accountdata( Account allAccounts[], int numAccounts) {
    FILE* file = fopen("accounts.txt", "w");
    if (file == NULL) {
        printf("Error opening the file .\n");
        return;
    }

    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%s,%s,%s,%.2f,%s,%d,%d\n", allAccounts[i].accNum, allAccounts[i].name, allAccounts[i].address, allAccounts[i].balance, allAccounts[i].mobile, allAccounts[i].dateOpened.month, allAccounts[i].dateOpened.year);
    }

    fclose(file);
}
//Done
void withdraw(Account allAccounts[], int numAccounts)
{
    char inputAccountNumber[15];
    double amount;
    bool accountFound = false;
    int i;
    system("cls");

    do
    {
        printf("Enter account number to make a withdrawal: ");
        scanf("%s", inputAccountNumber);
        if (accountExists(inputAccountNumber, allAccounts, numAccounts, &i) != 0)
        {
            accountFound = true;
            do
            {
                printf("Enter the amount to withdraw: ");
                if (scanf("%lf", &amount) == 1 && amount > 0 && amount <= 10000 && amount <= allAccounts[i].balance)
                {
                    allAccounts[i].balance -= amount;
                    printf("Withdrawal successful. Updated balance: $%.2lf\n", allAccounts[i].balance);
                    char saveChanges;
                    printf("Do you want to save the changes? (Y/N): ");
                    scanf(" %c", &saveChanges);
                    if (saveChanges == 'Y' || saveChanges == 'y')
                    {
                        save_accountdata(allAccounts, numAccounts);
                        printf("Changes saved.\n");
                        appendTransaction(inputAccountNumber, amount, "withdraw");
                    }
                    else
                    {
                        printf("Changes not saved. \n");
                        allAccounts[i].balance += amount;
                        printf("Withdrawal reverted. Updated balance: $%.2lf\n", allAccounts[i].balance);
                        sleep(3);
                    }
                    break;  // Exit the inner do-while loop
                }
                else
                {
                    if (amount > 10000)
                    {
                        printf("Withdrawal amount exceeds the maximum limit of $10000. Please enter a smaller amount.\n");
                    }
                    else
                    {
                        printf("Invalid withdrawal amount or insufficient balance.\n");
                    }
                }
                while (getchar() != '\n');  // Clear the input buffer
            }
            while (1);    // Infinite loop until a valid amount is entered
        }
        else
        {
            printf("Account number not found. Please enter a valid account number.\n");
        }
    }
    while (!accountFound);
}
//Done
void modifyAccount(Account allAccounts[], int numAccounts) {
    char inputAccountNumber[15];
    bool accountFound = false;
    int i;
    system("cls");

    do {
        printf("Enter account number to modify: ");
        scanf("%s", inputAccountNumber);

        if (accountExists(inputAccountNumber, allAccounts, numAccounts, &i)) {
            accountFound = true;
            char newFirstName[50];
            char newLastName[50];

            printf("Enter new first name: ");
            scanf("%s", newFirstName);
            while (!isValidName(newFirstName)) {
                printf("Invalid first name. \nPlease enter a valid first name: ");
                scanf("%s", newFirstName);
            }

            printf("Enter new last name: ");
            scanf("%s", newLastName);
            while (!isValidName(newLastName)) {
                printf("Invalid last name. \nPlease enter a valid last name: ");
                scanf("%s", newLastName);
            }

// Capitalize the first letter of the first and last names
            newFirstName[0] = toupper(newFirstName[0]);
            newLastName[0] = toupper(newLastName[0]);

// Combine the first and last names with a space

            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            char newEmail[100];
            printf("Enter new Email address: ");
            fgets(newEmail,100,stdin);
            newEmail[strcspn(newEmail, "\n")] = '\0';

            while (!check_Validation_OfAddress(newEmail)){// Validate the new email
                printf("Invalid Email. \nPlease enter a valid Email: ");
                fgets(newEmail,50,stdin);
                newEmail[strcspn(newEmail, "\n")] = '\0';
            }
            char newMobile[20];
            printf("Enter new phone number: ");
            scanf("%s", newMobile);
            while (strlen(newMobile) != 11 || strncmp(newMobile, "01", 2) != 0) { // Validate the new mobile number
                printf("Invalid phone number. \nPlease Enter a valid phone number: ");
                scanf("%s", newMobile);
            }


            printf("Account details updated successfully.\n");
            char saveChanges;
            printf("Do you want to save the changes? (Y/N): ");
            scanf(" %c", &saveChanges);
            if (saveChanges == 'Y' || saveChanges == 'y') {
                sprintf(allAccounts[i].name, "%s %s", newFirstName, newLastName);
                strcpy(allAccounts[i].address, newEmail);
                strcpy(allAccounts[i].mobile, newMobile);
                save_accountdata(allAccounts, numAccounts);
                printf("Changes saved.\n");
                sleep(1);
            } else {
                printf("Changes not saved.\n");
            }
            break;
        } else {
            printf("Account number not found. Please enter a valid account number.\n");
        }
    } while (!accountFound);
}
//Done
void transfer(Account allAccounts[], int numAccounts) {
    char senderAccountNumber[15];
    char receiverAccountNumber[15];
    double amount;
    bool senderFound = false;
    bool receiverFound = false;
    int i, j;
    system("cls");

    do {
        printf("Enter the sender's account number: ");
        scanf("%s", senderAccountNumber);
        if (accountExists(senderAccountNumber, allAccounts, numAccounts, &i) != 0) {
            senderFound = true;
        } else {
            printf("Sender's account number not found. Please enter a valid account number.\n");
        }
    } while (!senderFound);

    do {
        printf("Enter receiver's account number: ");
        scanf("%s", receiverAccountNumber);
        if (accountExists(receiverAccountNumber, allAccounts, numAccounts, &j) != 0 && strcmp(receiverAccountNumber, senderAccountNumber) != 0) {
            receiverFound = true;
        } else if (strcmp(receiverAccountNumber, senderAccountNumber) == 0) {
            printf("Receiver's and sender's account numbers are the same. Please enter a different account number.\n");
        } else {
            printf("Receiver's account number not found. Please enter a valid account number.\n");
        }
    } while (!receiverFound);

    do {
        printf("Enter the amount to transfer: ");
        if (scanf("%lf", &amount) != 1) {
            printf("Invalid input. Please enter a valid amount.\n");
            while (getchar() != '\n');  // Clear the input buffer
        } else {
            if (amount > 0 && amount <= allAccounts[i].balance) {

                printf("Transfer successful. Updated balance for sender (%s): $%.2lf, receiver (%s): $%.2lf\n",
                       allAccounts[i].accNum, allAccounts[i].balance-amount, allAccounts[j].accNum, allAccounts[j].balance+amount);


                char saveChanges;
                printf("Do you want to save the changes? (Y/N): ");
                scanf(" %c", &saveChanges);
                if (saveChanges == 'Y' || saveChanges == 'y') {
                    allAccounts[i].balance -= amount;
                    allAccounts[j].balance += amount;
                    save_accountdata(allAccounts, numAccounts);
                    printf("Changes saved.\n");
                    appendTransaction(senderAccountNumber, amount, "send");
                    appendTransaction(receiverAccountNumber, amount, "receive");
                } else {
                    printf("Changes not saved.\n");
                }
                break;
            } else {
                printf("Invalid transfer amount or insufficient balance in sender's account.\n");
                senderFound = false;
            }
        }
    } while (!senderFound);
    sleep(1);
}
//Done
void  last5Transactions(Account allAccounts[], int numAccounts)
{
    Transaction transactions[10];
    char inputAccountNumber[15];
    bool accountFound = false;
    int j,i;
    system("cls");
    do
    {
        printf("Enter account number: ");
        scanf("%s", inputAccountNumber);
        int accountIndex = accountExists(inputAccountNumber, allAccounts, numAccounts, &j);
        if (accountIndex != 0)
        {
            accountFound = true;
        }
    }
    while(!accountFound);
    FILE* file = fopen(inputAccountNumber, "r");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
        return;
    }

    int numTransactions = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (numTransactions >= 5)
        {
            for ( i = 0; i < 4; i++)
            {
                transactions[i] = transactions[i + 1];
            }
            numTransactions = 4;
        }

        sscanf(line, "%s %lf %s", transactions[numTransactions].date, &transactions[numTransactions].amount, transactions[numTransactions].description);
        numTransactions++;
    }

    fclose(file);
    printf("Last 5 transactions:\n");
    int start = (numTransactions > 5) ? numTransactions - 5 : 0;
    for ( i = start; i < numTransactions; i++)
    {
        printf("Date: %s, Amount: %.2f, Description: %s\n", transactions[i].date, transactions[i].amount, transactions[i].description);
    }
    int x;
    do
    {
        printf("Enter (1) to go back: ");
        scanf("%d",&x);
    }
    while(x != 1);
}
//Done
int menu()
{
    int option;
    system("cls");
    printf("\033[1;31m");// Set text color to red
    printf(" \n\t\t\t CUSTOMER ACCOUNT BANKING MANAGEMENT");
    printf("\033[1;32m");  // Set text color to green
    printf("\n\n \t\t\t     WELCOME TO THE MAIN MENU");
    printf("\033[0m");// Reset text color
    printf("\033[3;36m");// Set text color to cyan and underline
    printf("\n\n1.Add a new account\n2.Delete an existing account\n3.Modify account details\n4.Search account information\n5.Advanced search\n6.Withdraw\n7.Deposit\n8.Transfer\n9.Report\n10.Print the data of all accounts\n11.Exit\n");
    printf("\033[0m");// Reset text color

    do {
        printf("\n\n\t\t Enter your choice: ");
        if (scanf(" %d", &option) == 1 && option >= 1 && option <= 11)
        {getchar();
            break;}
        else {
            printf("Invalid choice. \nPlease enter a valid option (1-11): ");
        }
        while (getchar() != '\n');  // Clear the input buffer
    }
    while (1);
    return option;
}


    /*
    //  --------------------------------------Mariam------------------------------------------------------------
//معلش نسختها عندي عشان ترتيب الحاجه اعتبروها Done
void save_accountdata(int nOfAcc, Account accounts[]){
FILE *file;
file=fopen("accounts.txt","w");
if(file==NULL){
    printf("Error opening the file");
    return;
}

for(int i=0;i<nOfAcc;i++){
  fprintf(file,"%s,%s,%s,%f,%s,%d-%d",accounts[i].accNum,accounts[i].name,accounts[i].address,accounts[i].balance,accounts[i].mobile,accounts[i].dateOpened.month-accounts[i].dateOpened.year);

}
printf("Account data saved succefully");
}


void searchByKeyword(char key[100],int nOfAcc, Account accounts[] ) {

int i=0,flag=0;

 char *token;
token=strtok(accounts[i].name," ");
 while(token!=NULL&&i<=nOfAcc){
       {token=strtok(accounts[i].name," ");
                if(!strcmp( key,token)){
    printAccount(accounts[i].accNum);
      flag=1;
     }if (!flag){
token=strtok(NULL," ,");
if(!strcmp( key,token)){
      printAccount(accounts[i].accNum);
      flag=1;}}

i++;}}
 if(flag==0){
    printf("No matches are found");
 }


/*  int check_Validation_OfAcc()
{
    int flag=1;
    if (strlen(accNum) != 10)
    {
        flag = 0;
    }
    else
    {
        for (int i = 0; i < 10; i++)
        {
            if (!isdigit(accNum[i]))
            {
                flag = 0;// Invalid account number

                break;
            }
        }
    }
    if(flag==0)
    {
        return 0;// Invalid account number

    }
    else if(flag==1)
    {
        return 1;// valid account number

    }

//عدلت عليها  و حطيتها فوق عندي Done بردو
}int check_Validation_Ofaddress(){
    // Check if address contains '@'
    token = strtok(address,"@");
    if (token == NULL) {
        return 0; // Invalid address
    }

    // Check if address contains '.'
    token = strtok(NULL,".");
    if (token == NULL) {
        return 0; // Invalid address
    }
  return 1; // Valid address
}
*/


//  --------------------------------------Mohamed----------------------------------------------

/// Wanna Some Clarification.
void remove_newline(char in[])
{
    int len = strlen(in);
    if(in[len-1]=='\n')
    {
        in[len-1] = 0;
    }
    return;
}

/// DONE...
int login()
{
    void remove_newline(char in[]);
    struct users userInput;
    char line[100],*tok;
    FILE *fptr;

    if((fptr = fopen("users.txt","r")) == NULL)
        printf("ERR FILE NOT FOUND");

    else
    {
        //take input from user
        printf("------ *Login* ------\n\n");
        printf("username:\n> ");
        fgets(userInput.username,23,stdin);
        fflush(stdin);
        printf("password:\n> ");
        fgets(userInput.password,23,stdin);
        //removes unwanted \n from input
        remove_newline(userInput.username);
        remove_newline(userInput.password);

        //take input from file
        while(!feof(fptr))
        {
            //gets line
            fgets(line,99,fptr);
            //break line, take username
            tok = strtok(line," ");
            //compare between user inputed username
            //and username stored in  .txt file
            if(strcmp(tok,userInput.username) == 0)
            {
                //break line, take password
                tok = strtok(NULL,"\n");
                //compare between user inputed password
                //and password stored in  .txt file
                if(strcmp(tok,userInput.password) == 0)
                    return 1;
                else
                {
                    system("cls");
                    printf("ERR INCORECT PASSWORD!! \n");
                    break;
                }
            }
        }
    }
    fclose(fptr);
    return 0;
}

/// DONE...
int edit_balance()
{//edits file by duping it
    void remove_newline(char in[]);
    FILE *tptr;
    FILE *fptr;
    char accNum[100];
    char newbalance[100];
    char *tok;
    char line[100];
    char linecpy[100];
    char *acc;
    char *errcheck;
    double balance;
    int flag = 0;
    int index;

    fprintf(stdout,"account number:\n? ");
    fgets(accNum,99,stdin);
    remove_newline(accNum);
    fflush(stdin);
    fprintf(stdout,"deposit ammount:\n? ");
    fgets(newbalance,99,stdin);
    remove_newline(newbalance);
    fflush(stdin);

    strtod(newbalance,&errcheck);
    if(strcmp(errcheck,"\0"))
    {
        printf("ERR BAD INPUT\n");
        return(0);
    }


    balance=atof(newbalance);
    if(balance > (float)10000)
    {
        printf("\nERR MAX DEPOSIT LIMT EXCEEDED\n"
               "TRANSACTION UNSUCCESSFUL\n");
        return 0;
    }

    if((fptr = fopen("accounts.txt","r")) == NULL)
        printf("ERR COULDNT OPEN FILE");
    if((tptr = fopen("temp.txt","w"))==NULL)
        printf("ERR COULDNT EDIT FILE");
    else
    {
        while(fgetc(fptr)!=EOF)
        {
            fseek(fptr,-1,SEEK_CUR);
            fgets(line,99,fptr);


            strcpy(linecpy,line);
            acc = strtok(line,",");
            if(strcmp(acc,accNum) == 0)
            {
                flag = 1;
                fprintf(tptr,"%s,",acc);
                for(index = 0;index<2;index++)
                {tok = strtok(NULL,",");
                fprintf(tptr,"%s,",tok);}

                tok = strtok(NULL,",");
                balance += atof(tok);
                fprintf(tptr,"%0.2lf,",balance);


                tok = strtok(NULL,",");
                fprintf(tptr,"%s,",tok);
                tok = strtok(NULL,",");
                fprintf(tptr,"%s",tok);
            }
            else
            {
                fprintf(tptr,"%s",linecpy);
            }

        }
    }

    fclose(fptr);
    fclose(tptr);

    remove("accounts.txt");
    rename("temp.txt","accounts.txt");

    if(flag)
        printf("\ntransaction successful!\n");
    else
        printf("\nTRANSACTION FAILED\n");

    return(1);
}

/// DONE...
//0 = notfound
//1 = found and removed
//-1 = found and not removed
int delete_account(void)
{
    void remove_newline(char in[]);
    FILE *tptr;
    FILE *fptr;
    char accNum[100];
    char *tok;
    char line[100];
    char linecpy[100];
    char *acc;
    double balance;
    int flag = 0;
    int index;

    fprintf(stdout,"account number:\n? ");
    fgets(accNum,99,stdin);
    remove_newline(accNum);
    fflush(stdin);

    if((fptr = fopen("accounts.txt","r")) == NULL)
        printf("ERR COULDNT OPEN FILE");
    if((tptr = fopen("temp.txt","w"))==NULL)
        printf("ERR COULDNT EDIT FILE");
    else
    {
        while(fgetc(fptr)!=EOF)
        {
            fseek(fptr,-1,SEEK_CUR);
            fgets(line,99,fptr);


            strcpy(linecpy,line);
            acc = strtok(line,",");
            if(strcmp(acc,accNum) == 0)
            {
                flag = 1;
                for(index = 0;index<2;index++)
                {
                    tok = strtok(NULL,",");
                }

                //balance reached
                tok = strtok(NULL,",");
                balance = atof(tok);
                if(balance == 0)
                    continue;
                else
                {
                    flag = -1;
                    fprintf(tptr,"%s",linecpy);
                    continue;
                }
            }
            else
            {
                fprintf(tptr,"%s",linecpy);
            }
        }

    }
    //closing files
    fclose(fptr);
    fclose(tptr);
    //magic
    remove("accounts.txt");
    rename("temp.txt","accounts.txt");

    if(flag == 0)
        printf("\nACCOUNT NOT DELETED\nREASON: "
               "ACCOUNT NON-EXISTENT\n");
    else if(flag == 1)
        printf("\nACCOUNT DELETED\n");
    else if(flag == -1)
        printf("\nACCOUNT NOT DELETED\nREASON: "
               "ACCOUNT HAS BALANCE\n");

    return flag;
}

/// DONE ...

void low(char *m)
{
    int index = 0;
    while(m[index]!='\0')
    {
        m[index] = tolower(m[index]);

        ++index;
    }
    return;
}

void SortByDate(void)
{
        struct lol
    {
        char line_[100];
        double opened_;
    };

    void remove_newline(char in[]);
    void low(char *m);
    FILE *fptr;
    FILE *tptr;
    char *month_;
    char *year_;
    char temp[100];
    char line[100];
    char linecpy[100];
    double temp_op;
    int size_of_array = -1;
    int index = 0;
    int flag = 0;

        //reading from duplicate
    fptr = fopen("accounts.txt","r");

            //reading size of file
            while(!feof(fptr))
            {

                fgets(line,98,fptr);

                ++size_of_array;
                ++index;

            }

        //create struct array
    struct lol *sptr = calloc(sizeof(struct lol),size_of_array);

            //putting in array
            rewind(fptr);
            index = 0;
            fgets(line,98,fptr);
            while(!feof(fptr))
            {

                strcpy(linecpy,line);

                //account num
                strtok(line,",");
                //name
                strtok(NULL,",");
                //email
                strtok(NULL,",");
                //balance
                strtok(NULL,",");
                //mobile
                strtok(NULL,",");
                //date part
                month_ = strtok(NULL,"-");
                year_ = strtok(NULL,"\n");

                strcpy(sptr[index].line_,linecpy);
                sptr[index].opened_ = atoi(year_)+atof(month_)/12;

                fgets(line,98,fptr);
                ++index;
            }
            //i dont need it no more
            fclose(fptr);

        // sorting time!!!
            while(flag == 0)
            {
                flag = 1;
                index = 1;
                while(index<size_of_array)
                {
                    //;) for a good time look down
                 /* printf("%s %i\t%s %i\n%i\n",sptr[index-1].balance_,index -1,sptr[index].balance_,index,
                           strcmp(sptr[index - 1].balance_,sptr[index].balance_));*/

                    if(sptr[index - 1].opened_<sptr[index].opened_)
                       {
                           flag = 0;
                           //change date
                           temp_op = sptr[index - 1].opened_;
                           sptr[index - 1].opened_ = sptr[index].opened_;
                           sptr[index].opened_ = temp_op;
                           //change line
                           strcpy(temp,sptr[index - 1].line_);
                           strcpy(sptr[index - 1].line_,sptr[index].line_);
                           strcpy(sptr[index].line_,temp);
                       }
                    ++index;
                }
            }

            index = 0;
            tptr = fopen("temp.txt","w");
            while(index < size_of_array)
            {
                fprintf(tptr,"%s",sptr[index].line_);
                ++index;
            }
            //close all stuff i dont need
            free(sptr);
            fclose(tptr);
            //magic time!!
            remove("accounts.txt");
            rename("temp.txt","accounts.txt");




         /*       //email
                tok = strtok(NULL,",");
                //balance
                tok = strtok(NULL,",");
                //mobile
                tok = strtok(NULL,",");
                //date part
                tok = strtok(NULL,"-");
                tok = strtok(NULL,"\n");
         */


    return;
}

void SortByBalance(void)
{
    struct lol
    {
        char line_[100];
        char balance_[24];
    };

    void remove_newline(char in[]);
    void low(char *m);
    FILE *fptr;
    FILE *tptr;
    char *tok;
    char temp[100];
    char line[100];
    char linecpy[100];
    int size_of_array = -1;
    int index = 0;
    int flag = 0;

        //reading from duplicate
    fptr = fopen("accounts.txt","r");

            //reading size of file
            while(!feof(fptr))
            {

                fgets(line,98,fptr);

                ++size_of_array;
                ++index;

            }

        //create struct array
    struct lol *sptr = calloc(sizeof(struct lol),size_of_array);

            //putting in array
            rewind(fptr);
            index = 0;
            fgets(line,98,fptr);
            while(!feof(fptr))
            {

                strcpy(linecpy,line);

                //account num
                tok = strtok(line,",");
                //name
                tok = strtok(NULL,",");
                //email
                tok = strtok(NULL,",");
                //balance
                tok = strtok(NULL,",");

                strcpy(sptr[index].line_,linecpy);
                strcpy(sptr[index].balance_,tok);

                fgets(line,98,fptr);
                ++index;
            }
            //i dont need it no more
            fclose(fptr);

        // sorting time!!!
            while(flag == 0)
            {
                flag = 1;
                index = 1;
                while(index<size_of_array)
                {
                    //;) for a good time look down
                 /* printf("%s %i\t%s %i\n%i\n",sptr[index-1].balance_,index -1,sptr[index].balance_,index,
                           strcmp(sptr[index - 1].balance_,sptr[index].balance_));*/

                    if(atof(sptr[index - 1].balance_)<atof(sptr[index].balance_))
                       {
                           flag = 0;
                           //change name
                           strcpy(temp,sptr[index - 1].balance_);
                           strcpy(sptr[index - 1].balance_,sptr[index].balance_);
                           strcpy(sptr[index].balance_,temp);
                           //change line
                           strcpy(temp,sptr[index - 1].line_);
                           strcpy(sptr[index - 1].line_,sptr[index].line_);
                           strcpy(sptr[index].line_,temp);
                       }
                    ++index;
                }
            }

            index = 0;
            tptr = fopen("temp.txt","w");
            while(index < size_of_array)
            {
                fprintf(tptr,"%s",sptr[index].line_);
                ++index;
            }
            //close all stuff i dont need
            free(sptr);
            fclose(tptr);
            //magic time!!
            remove("accounts.txt");
            rename("temp.txt","accounts.txt");




         /*       //email
                tok = strtok(NULL,",");
                //balance
                tok = strtok(NULL,",");
                //mobile
                tok = strtok(NULL,",");
                //date part
                tok = strtok(NULL,"-");
                tok = strtok(NULL,"\n");
         */


    return;
}

void SortByName(void)
{
    struct lol
    {
        char line_[100];
        char name_[24];
    };

    void remove_newline(char in[]);
    void low(char *m);
    FILE *fptr;
    FILE *tptr;
    char *tok;
    char temp[100];
    char line[100];
    char linecpy[100];
    int size_of_array = -1;
    int index = 0;
    int flag = 0;

        //reading from duplicate
    fptr = fopen("accounts.txt","r");

            //reading size of file
            while(!feof(fptr))
            {

                fgets(line,98,fptr);

                ++size_of_array;
                ++index;

            }

        //create struct array
    struct lol *sptr = calloc(sizeof(struct lol),size_of_array);

            //putting in array
            rewind(fptr);
            index = 0;
            fgets(line,98,fptr);
            while(!feof(fptr))
            {

                strcpy(linecpy,line);

                //account num
                tok = strtok(line,",");
                //name
                tok = strtok(NULL,",");

                strcpy(sptr[index].line_,linecpy);
                strcpy(sptr[index].name_,tok);

                fgets(line,98,fptr);
                ++index;
            }
            //i dont need it no more
            fclose(fptr);

        // sorting time!!!
            while(flag == 0)
            {
                flag = 1;
                index = 1;
                while(index<size_of_array)
                {
                    //;) for a good time look down
                 /*          printf("%s %i\t%s %i\n%i\n",sptr[index-1].name_,index -1,sptr[index].name_,index,
                           strcmp(sptr[index - 1].name_,sptr[index].name_));*/

                    low(sptr[index - 1].name_);
                    low(sptr[index].name_);
                    if(strcmp(sptr[index - 1].name_,sptr[index].name_)>0)
                       {
                           flag = 0;
                           //change name
                           strcpy(temp,sptr[index - 1].name_);
                           strcpy(sptr[index - 1].name_,sptr[index].name_);
                           strcpy(sptr[index].name_,temp);
                           //change line
                           strcpy(temp,sptr[index - 1].line_);
                           strcpy(sptr[index - 1].line_,sptr[index].line_);
                           strcpy(sptr[index].line_,temp);
                       }
                    ++index;
                }
            }

            index = 0;
            tptr = fopen("temp.txt","w");
            while(index < size_of_array)
            {
                fprintf(tptr,"%s",sptr[index].line_);
                ++index;
            }
            //close all stuff i dont need
            free(sptr);
            fclose(tptr);
            //magic time!!
            remove("accounts.txt");
            rename("temp.txt","accounts.txt");




         /*       //email
                tok = strtok(NULL,",");
                //balance
                tok = strtok(NULL,",");
                //mobile
                tok = strtok(NULL,",");
                //date part
                tok = strtok(NULL,"-");
                tok = strtok(NULL,"\n");
         */


    return;
}

char *month_num_words(int n,char value[])
{
    char months[13][20] = {"0","January","February","March","April",
                        "May","June","July","August","September",
                        "October","November","December"};

    strcpy(value,months[n]);

    return value;
}

void print_sorted(void)
{
    FILE *fptr;
    char *month_num_words(int n,char value[]);
    void remove_newline(char in[]);
    void SortByName();
    void SortByBalance();
    void SortByDate();
    char input_user[10];
    char *tok;
    char line[100];
    char month_n[20];
    int month;
    int year;

        //taking sorting option
        printf("Sort By Name :    1\nSort By Date :    2\n"
               "Sort By Balance : 3\n> ");
        scanf("%8s",input_user);

        if(strcmp(input_user,"1")==0)
            SortByName();
        else if(strcmp(input_user,"2")==0)
            SortByDate();
        else if(strcmp(input_user,"3")==0)
            SortByBalance();
        else
            {
                printf("\nERR OPTION NON-EXISTENT\n");
                return;
            }
        fptr = fopen("accounts.txt","r");

        //nothing more than printing from 'ere
        while(fgetc(fptr)!=EOF)
        {
            fseek(fptr,-1,SEEK_CUR);
            fgets(line,98,fptr);

            tok = strtok(line,",");
            printf("\nAccount Number : %s\n",tok);

            tok = strtok(NULL,",");
            printf("Name : %s\n",tok);

            tok = strtok(NULL,",");
            printf("E-mail : %s\n",tok);

            tok = strtok(NULL,",");
            printf("Balance : %.2lf\n",atof(tok));


            tok = strtok(NULL,",");
            printf("Mobile : %s\n",tok);

            //date part
            tok = strtok(NULL,"-");
            month = atoi(tok);
            tok = strtok(NULL,"\n");
            year = atoi(tok);
            //convert month to words and print
            month_num_words(month,month_n);
            printf("Opened : %s %i\n",month_n,year);


        }

    fclose(fptr);
    return;
}


int main() {
    int option;
    char x[2];
    Account *allAccounts = loadAccounts();

    // Number Of Accounts, REQ_Parameters --> None
    /*
    numberOfAccounts();
    printf(">> %d\n", numAccounts);
    */

    // Printing Account Info, REQ_Parameters --> All Accounts Pointer & Index of Account.
    /*/
    while(1 && x!='z'){
     gets(x);
     printAccount(allAccounts, atoi(x) );
    }
    */

    // Search *Query*, REQ_Parameters --> All Accounts Pointer & Number Of Accounts.
    /*
    searchAccount(allAccounts);
    */

    // Printing Account Info, REQ_Parameters --> All Accounts Pointer & Index of Account.
    /*
    */

    // Login User, NO REQ_Parameters.

    while(!login());


    // Deposit, NO REQ_Parameters.
    /*
    */

    // Deleting, NO REQ_Parameters.
    /*
    */


    // Print Sorted, NO REQ_Parameters.
    /*
    */

        while(1) {
        option = menu();

        switch(option) {
            case 1 :
                addAccount(allAccounts);
                break;
            case 2 :
                delete_account();
                break;
            case 3 :
                modifyAccount(allAccounts, numAccounts);
                break;
            case 4 :
                break;
            case 5 :
                break;
            case 6 :
                withdraw(allAccounts, numAccounts);
                break;
            case 7 :
                edit_balance();
                break;
            case 8 :
                transfer(allAccounts, numAccounts);
                break;
            case 9 :
                last5Transactions(allAccounts, numAccounts);
                break;
            case 10 :
                print_sorted();
                break;
            case 11 :
                exit(0);
                break;
        }
    }

    printf("\n---------------------------------\n");


    return 0;

}




