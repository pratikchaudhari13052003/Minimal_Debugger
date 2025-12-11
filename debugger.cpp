#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <iostream>


using namespace std;

long original_data;
void* addr = (void*)0x401126;


void set_breakpoint(pid_t pid,void *addr){

    original_data = ptrace(PTRACE_PEEKTEXT,pid,addr,NULL);

    long data_with_trap = ( original_data & ~0xFF) | 0xCC;

    ptrace(PTRACE_POKETEXT,pid,addr,data_with_trap);

    cout<<"Breakpoint set at "<<addr<<" .Replaced byte with 0xCC.\n"<<endl;

}


int main(int argc, char *argv[]) {
  
    if(argc<2){
        cout<<"Please enter more arguments"<<endl;
        exit(0);
    }

    pid_t p1=fork();

    if(p1==0){
        //child
        //PTRACE_TRACEME --> Child tells kernel: "My parent will debug me."
        ptrace(PTRACE_TRACEME,0,NULL,NULL);

        execl(argv[1],argv[1],NULL);

    }else{
        //parent

        int status;

// WIFSTOPPED(status)
// Returns true (non-zero) if the child has stopped due to a signal.

    waitpid(p1,&status,0);

    if(WIFSTOPPED(status)){

        cout<<"Debugger started \n";

        long addr_input;
            cout << "Enter memory address to break at (e.g., 401126): 0x";
            cin >> hex >> addr_input;  // Read hex input

            // --- NEW: Set the Breakpoint ---
            set_breakpoint(p1, (void*)addr_input);

            // --- NEW: Continue Execution ---
            // "Let the child run until it hits the 0xCC trap"
            ptrace(PTRACE_CONT, p1, 0, 0);


    }

// WIFEXITED(status)
// Returns true (non-zero) if the child terminated normally .

        if(WIFEXITED(status)){
            cout<<"Child exited normally"<<endl;
        }


    }

    

}