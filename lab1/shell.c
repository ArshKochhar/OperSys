//+
// File:	shell.c
//
// Purpose:	This program implements a simple shell program. It does not start
//		processes at this point in time. However, it will change directory
//		and list the contents of the current directory.
//
//		The commands are:
//		   cd name -> change to directory name, print an error if the directory doesn't exist.
//		              If there is no parameter, then change to the home directory.
//		   ls -> list the entries in the current directory.
//			      If no arguments, then ignores entries starting with .
//			      If -a then all entries
//		   pwd -> print the current directory.
//		   exit -> exit the shell (default exit value 0)
//
//		if the command is not recognized an error is printed.
//-

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <ctype.h>

#define CMD_BUFFSIZE 1024
#define MAXARGS 10

int splitCommandLine(char * commandBuffer, char* args[], int maxargs);
void doCommand(char * args[], int nargs);

int main() {

    char commandBuffer[CMD_BUFFSIZE];
    char *args[MAXARGS];

    // print prompt.. fflush is needed because
    // stdout is line buffered, and won't
    // write to terminal until newline
    printf("%%> ");
    fflush(stdout);

    while(fgets(commandBuffer,CMD_BUFFSIZE,stdin) != NULL){

	// Remove newline at end of buffer
	// TODO Step 2: remove newline from end of buffer

	// Split command line into words.
	// TODO Step 2: call splitCommandLine with the right parameters
	
	//calling splitCommandLine with the right parameters
	int nargs = splitCommandLine(commandBuffer, args, MAXARGS);
	

	// Debugging for step 2
	printf("%d\n", nargs);
	int i;
	for (i = 0; i < nargs; i++){
	 printf("%d: %s\n",i,args[i]);
	}

	// Execute the command
	// Remember to check if there is a command (i.e. value of nargs)
	// TODO: Step 3 call doCommand with the right arguments

	// print prompt
	printf("%%> ");
	fflush(stdout);
    }
}

////////////////////////////// String Handling ///////////////////////////////////

//+
// Function:	skipChar
//
// Purpose:	This function skips over a given char in a string
//		For security, the function will not skip null chars.
//
// Parameters:
//    charPtr	Pointer to string
//    skip	character to skip
//
// Returns:	Pointer to first character after skipped chars.
//		Identity function if the string doesn't start with skip,
//		or skip is the null character
//-

char * skipChar(char * charPtr, char skip){
 // TODO Step 2: skip over instances of the char skip
 //    return input value of charPtr if *char is null char
 
 // if the character to be skipped is a NULL character 
 //or the first character is not a skip character , return charPtr
 if (skip == '\0' || charPtr[0] != skip){
     return charPtr;
 }
 // start a while loop, it continues as long as the character is a skip character
 int i = 0;
 while(charPtr[i] == skip) i++;
 
 return &charPtr[i];
}

//+
// Function:	splitCommandLine
//
// Purpose:	This splits a string into an array of strings.
//		The array is passed as an argument. The string
//		is modified by replacing some of the space characters
//		with null characters to terminate each of the strings.
//
// Parameters:
//	commandBuffer	The string to split
//	args		An array of char pointers
//	maxargs		Size of array args (max number of arguments)
//
// Returns:	Number of arguments (< maxargs).
//
//-

int splitCommandLine(char * commandBuffer, char* args[], int maxargs){
   // TODO Step 2 split the command into words using only
   // the functions skipChar and strchr. You may use fprintf to report
   // errors that are encountered to stderr.
   
   char * word = commandBuffer;
   
   int i = 0;
   while(1){
       word = skipChar(word, ' ');
              
       //edge case in the scenario of trailling spaces - decrement index
       if(word[0] == '\0'){
           i--;
           break;
       }
       //This allows us go set the first character of the word 
       //to an element/index in the args array
       args[i] == word;
       
       //pointing to the next spot in commandBuffer as that array needs to be populated with args
       word = strchr(word, ' ');
       
       // if strchr returns a value  NULL, it means that we are the end of the array
       // and the loop can break
       if(word == NULL){
           break;
       }
       
       word[0] = '\0';
       word++;
       i++;
       
   }
   //error statement 
   if (i+1 > maxargs){
       fprintf(stderr, "the amount of args (%d) exceeds the maxargs (%d)\n", i+1, maxargs);
       exit(0);
       
   }
   return i+1;
}


////////////////////////////// Command Handling ///////////////////////////////////

// typedef for pointer to command handling functions

// TODO STEP 3a write the typedef for the function handler pointer to
// use in the structure immediately below.
// See the description of the function prototypes at the bottom of
// the file in the comments.

typedef void (*command)(char * args[], int nargs);


// cmdStruct type:
// Associates a command name with a command handling function

// TODO STEP 3b use the typedef above (Step 3a) to make a two element
// struct. The first is a char * for the name of the command
// the other is a function pointer for the command handling function
typedef struct {
    char * name;
    command cmd;

} cmdStruct;


// prototypes for command handling functions
// TODO STEP 4b,6: add a function prototype
// for each command handling function

void exitFunction(char * args[], int nargs);
void pwdFunction(char * args[], int nargs);
void lsFunction(char * args[], int nargs);
void cdFunction(char * args[], int nargs);
int filter(const struct dirent * d);

// Array that provides the list commands and functions
// must be terminated by {NULL, NULL} 
// in a real shell, this would be a hashtable.

// TODO Step 4a: add a global array of 
// the type above that contains initializers
// of strings and command handling funciton names

//declaration of the command dispatch table 
// Null Null is the initializer
// all the functions skeletons are at the bottom 
cmdStruct cmdArray[] = {
    {"pwd", pwdFunction},
    {"ls", lsFunction},
    {"cd", cdFunction},
    {"exit", exitFunction},
    {NULL, NULL}
};


//+
// Function:	doCommand
//
// Purpose:	This command calls a command handling funciton from
//		the commands array based on the first argument passed
//		in the args array.
//
// Parameters:
//	args	command and parameters, an array of pointers to strings
//	nargs	number of entries in the args array
//
// Returns	nothing (void)
//-

void doCommand(char * args[], int nargs){
  // TODO Step 5 this function is small
  //  this is the command search loop
  int i = 0; 
  while (1) {
      //edge case check to see if theif teh first field of the entry is NULL  
      if (cmdArray[i].name == NULL) {
          fprintf(stderr, "Error: the command is not recognized\n");
          return;
      } 
      // comparing the first field of each element of the array to args[0]
      else if (strcmp(cmdArray[i].name, args[0]) == 0) {
          cmdArray[i].cmd(args, nargs); // making the call to the cmd function
          return;
      } 
      //proceed
      else {
          i++;
      }
  }
}
//////////////////////////////////////////////////
//            command Handling Functions        //
//////////////////////////////////////////////////
// all command handling functions have the same
// parameter types and return values.
//////////////////////////////////////////////////

//+
// Function:	all handling functions
//
// Purpose:	this command performs the funcdtion
//		associated with the commands.
//		The name of the comomand is in first argument.
//	 	Since it is called by doCommand, it is known
//		that arg is at least one element long
//
// Parameters:
//	args	command and parameters, an array of pointers to strings
//	nargs	number of entries in the args array
//
// Returns	nothing (void)
//-

// TODO step 4b put command handling function for exit here
void exitFunction(char * args[], int nargs){
    exit(0);
}


// TODO step 6 put rest of command handling functions here

void pwdFunction(char * args[], int nargs){
    //the getcwd function will allocate the string 
    char * cwd = getcwd(NULL,0);
    //printing the current working directory
    printf(cwd);
    //releasing the string using free
    free;
}
//implementation of the cd function to locate directory 
void cdFunction(char * args[], int nargs){
    
     int chdirVal; //setting up a variable to store chdir(pw->pw_diir)
     
    if (nargs == 1){
        struct passwd *pw = getpwuid(getuid()); // retrieving a popinter to the password file entry of a user
        return;
        
        //edge case check if password is NULL
        if (pw == NULL){
            fprintf(stderr, "Error: Incorrect Password NULL");
            return;
        }
        
        chdirVal = chdir(pw->pw_dir);
    }
    
    else if (nargs == 2){
         chdirVal = chdir(args[1]);
    }
    
    else {
        fprintf(stderr,"Error: Cannot change directory");
        return;
    }
    
    if (chdirVal != 0){
        fprintf(stderr, "Error: No file or directory exists with that name");
    }
}
void lsFunction(char * args[], int nargs){
    if (nargs > 2){
        fprintf(stderr,"Error: too many arguments received");
        return;
    }
    
    
    
    if (args[1] == "-a"){
        struct dirent ** namelist;
        int numEnts = scandir(".",&namelist,NULL,NULL);
        //a for loop to print out all the file names by looping through directories 
        for (int i = 0; i < numEnts; i++){
            printf("%s \n", namelist[i]->d_name);
        } 
        
     }else{
            fprintf(stderr, "Error: Input is invalid");
            return;
        }
        
    }
  
  // this is the function to check which entity should or should not be included in the list. 
int filterValidation(int i ){
    struct dirent ** namelist;
    int numEnts = scandir(".",&namelist,NULL,NULL);
    if(filter != 0){
        return 1;
    }
    else {
        return 0;
    }
    
}