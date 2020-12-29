/*+
 * File:  lab2.c
 *
 * Purpose: This program exposes some kernel internals. 
 *          It reports on some memory information about the processors and delivers the results through a /proc file. 
 *          The program has two linux modules that generate a /proc file that contains one line for each process process, 
 *          with each line containing the PID, the user id (called uid) the total amount of virtual memory used 
 *          by the process and the amount of virtual memory that is in RAM.
 *
-*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

static struct task_struct * firstTask, *theTask;

int cnt;

int my_read_proc(char * page, char **start, off_t fpos, int blen, int * eof, void * data){

    int numChars= 0; // Variable to know where the second write starts
    if (fpos == 0){
	     // Write the headers to the buffer using a sprintf call
         numChars+= sprintf(page + numChars, "PID\tUID\tVSZ\tRSS\n"); // Headers are PID, UID, VSZ and RSS
	    
        // Finds the first valid task
        firstTask=&init_task;
        // While loop to find the first task who's PID (theTask->pid) is not equal to zero
        while(firstTask->pid == 0){   
            firstTask=firstTask->next_task;
        }
       theTask=firstTask;
        // Add the process id and user id to the buffer page
        numChars+= sprintf (page+numChars,"%d\t%d\t",theTask->pid,theTask->uid);
        if (theTask->mm == NULL){ //  Check if mm is NULL, if so, then add two 0’s to the buffer
            numChars+= sprintf (page+numChars,"0\t0\n");
        }
        else { // Otherwise add the total_vm and the rss fields of the mm field * tmp with the page size to the buffer
            int tmp= PAGE_SIZE >> 10;
            theTask->mm->rss*=tmp;
            theTask->mm->total_vm*=tmp;
            numChars+= sprintf (page+numChars,"%d\t%d\n",theTask->mm->total_vm,theTask->mm->rss);
        }
        // Advance the variable theTask to point to the next valid task
        do {
            theTask=theTask->next_task;
        } while(theTask->pid == 0);
    
    } else {
        if(theTask==firstTask){  // At back at begining of list
            *eof = 0;
            *start = page;
            return 0;
        }
	    // Add the process id and user id to the buffer page (writting task info for one task)
        numChars+= sprintf (page+numChars,"%d\t%d\t",theTask->pid,theTask->uid);
        if(theTask->mm == NULL){
            numChars+= sprintf (page+numChars,"0\t0\n");
        }
        else {
            int tmp= PAGE_SIZE >> 10;
            theTask->mm->rss*=tmp;
            theTask->mm->total_vm*=tmp;
            numChars+= sprintf (page+numChars,"%d\t%d\n",theTask->mm->total_vm,theTask->mm->rss);
        }
	    // Same as above, advance to next valid task
        do {
            theTask=theTask->next_task;
        } while (theTask->pid == 0);
    }
    *eof = 1;
    *start = page;
    return numChars;
}
/* 
The init_module function declares a pointer to a proc_dir_entry struct, 
then it creates a new proc entry (“lab2” file) using the create_proc_entry function.
*/
int init_module(){
   struct proc_dir_entry * proc_entry;
   //creates "lab2" file, 0444 makes readable to everyone, NULL gives directory
   proc_entry = create_proc_entry("lab2", 0444, NULL);
   if(proc_entry==NULL){
       return -1;
   }
   else {
       proc_entry->read_proc=&my_read_proc;
       return 0;
   }
}

/* 
The cleanup_module function is called by the kernel when the module is removed
This removes the /proc file by taking in the “lab2” parameter, 
the file created and the NULL parameter.
*/
void cleanup_module(){
    remove_proc_entry("lab2", NULL);
}