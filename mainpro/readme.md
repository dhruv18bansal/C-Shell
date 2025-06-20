this is an implementation of shell in C 

to run the following code:
"make main"
"./a.out"
>commands.c,commands.h inludes the fucntion for checking ehich command to execute and io redirection 

The shell supports  ‘;’ seperated commands. I have assumed that the maximum number of ";" seperated command which would be given would be 1000.
hop’ command changes the directory that the shell is currently in.
"hop" command helps in changing the directory that the shell is currently in.It also prints the full path of working directory after changing.Here home directory is the c-linux-shell.“.”, “..”, “~”, and “-” flags are implemented in hop. ~ represents the home directory of shell.If no argument is present, then hop into the home directory.
>files:hop.c,hop.h


"reveal" command lists all the files and directories in the specified directories in lexicographic order (default peek does not show hidden files).It also supports -a and -l flags.
    -l : displays extra information
    -a : displays all files, including hidden files
also has color coding blue for folders and green for files.
>files: reveal.h,reveal.c

 "log" command stores (and output) the 15 most recent command statements given as input to the shell based on some constraints. It overwrites the oldest commands if more than the set number (15) of commands are entered.Pastevents does not store a command if it is the exactly same as the previously entered command.It stores all statements except commands that include log or log purge or log execute .
>files:log.c,log.h
  "log purge" clears all the log currently stored.It is not stored in log.
  
  "log execute <n>" executes the nth recent command from the log .

"proclore" is used to obtain information regarding a process. If an argument is missing, print the information of your shell.
Information printed : pid,Process Status (R/R+/S/S+/Z),Process group,Virtual Memory,Executable path of process
Process states :
>files: proclore.h,proclore.c
   1.  R/R+ : Running
   2. S/S+ : Sleeping in an interruptible wait
  "+" indicates foreground process.

"seek" command looks for a file/directory in the specified target directory (or current if no directory is specified). It returns a list of relative paths (from target directory) of all matching files/directories (files in green and directories in blue) separated with a newline character.
>files: seek.h,seek.c
  Flags :

   1. -d : Only look for directories (ignore files even if name matches)
   2. -f : Only look for files (ignore directories even if name matches)
   3. -e : This flag is effective only when a single file or a single directory with the name is found. If only one file (and no directories) is found, then print it’s output. If only one directory (and no files) is found, then change current working directory to it. Otherwise, the flag has no effect. This flag should work with -d and -f flags. If -e flag is enabled but the directory does not have access permission (execute) or file does not have read permission, then output “Missing permissions for task!”

 Print “No match found!” in case no matching files/directories is found. Note that -d and -f flag can’t be used at the same time return error message “Invalid flags!”.

 A call to this command will always be in the format :

                                   seek <flags> <search> <target_director>


🌟 
## I/O Redirection
">": Redirects output to a file. Creates the file if it doesn’t exist or overwrites it.
**Example: echo Hello > file.txt**
">>": Appends output to a file. Creates the file if it doesn’t exist.
**Example: echo World >> file.txt**
<: Reads input from a file. Shows an error if the file doesn’t exist.
**Example: cat < file.txt**


🔗
##  Pipes
|: Passes output from one command to another. Supports multiple pipes.
**Example: echo "Hello" | tr '[:lower:]' '[:upper:]'**
🔄 Redirection with Pipes
Combine I/O redirection and pipes in a single command.
**Example: cat < input.txt | grep pattern > output.txt**
My shell does not support commands  like **cat a.txt & | wc** It treats the command cat a.txt &  as just cat a.txt.


🛠️ 
## Process Management
>files: activities.h,activities.c,ping.c.ping.h,fgbg.h,fgbg.c
activities: Lists processes with PID, command name, and state (running or stopped).
**Example: activities**
ping <pid> <signal_number>: Sends a signal to a process.
**Example: ping 1234 9 (Sends SIGKILL to process 1234)**
fg <pid>: Brings a background process to the foreground.
**Example: fg 5678**
bg <pid>: Resumes a stopped background process.
**Example: bg 5678**

⌨️ 
## Keyboard Commands
>files : signal.c , signal.h
Ctrl-C: Interrupts the current foreground process (SIGINT).
semicolon seperated commands are considered seperate processes and thus if 3 semicolon seperated commands are given, you need to do ctrl c 3 times.
Ctrl-D: Logs out of the shell and terminates all processes.


✨ 
## Neonate 
> files: neonate.c,neonate.h
neonate -n [time_arg]: Displays the PID of the most recent process every [time_arg] seconds until 'x' is pressed.
**Example: neonate -n 5**

🌐 
## iMan (Networking)
> files : man.c,man.h 
iMan <command_name>: Fetches man pages from http://man.he.net/ for the specified command.
**Example: iMan ls**
