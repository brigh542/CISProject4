Project Description:A Shell environment for terminal commands.
By: Brighid Rancour and Dakotah Pettry
Possible Commands:
  cd [directory] change directory
  path [+/-] [path] alone prints the current paths, a + adds a path, and a - removes a path
  quit exits the shell 
  any commands linked within the path environment created using path command
  cmd [args...] > [filename] output redirection operation is supported
  cmd [args...] | cmd2 [args2...] pipe operation is supported
 
Group Member Contribution-
Dakotah Pettry  - 50%
Brighid Rancour - 50%

Compailing instruction- there is a makefile provided that can be used to run the program
  type "make" within a terminal set to the project directory
 
practice testrun: 
ZTC:Project4CIS340 ZeusTheCat$ make
gcc -o project4make main.c help.c -I.
ZTC:Project4CIS340 ZeusTheCat$ ./project4make 
$: path + /bin
path: /bin
$: path + /sbin
path: /bin:/sbin
$: ls
Photo on 12-2-17 at 1.16 PM.jpg	helpcopy.c
README.md			main.c
help.c				makefile
help.h				project4make
$: ls -l > foo.txt
$: mkdir tempDir
$:cd tempDir
Working directory has been changed to tempDir.
$:echo "make TempFile.txt" > tempFile.txt
$:ls 
tempFile.txt
$:rm tempFile.txt
$:cd ..
Working directory has been changed to ...
$: quit
Quitting...

Bugs:
-pipe only works on some systems. It worked on the home environment of both team members but would not work on CSU's computers
-When path - [arg] is called the ':' is not always removed correctly resulting in a messy look when path is called e.b ::/bin this does not affect performance however and is merely visual
-program does not support output redirect or pipe operations on the path internal command, as it is not an executable, but a method
