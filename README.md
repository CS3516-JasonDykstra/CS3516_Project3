<h2>Compiling The Code</h2>
The makefile in this folder will compile the code and create the necessary executable for you. In order to run the makefile, simply type <b>"make"</b> in the linux command line while inside of the project directory. This will compile all .c files associated with the server and create an executable. 
<br><br>
The makefile simply uses gcc to compile the .c files into an executable.

<h2>Running The Code</h2>
Once you have run <b>"make"</b> in the linux command line, there should be an executable called "project3". To run it, type <b>"./project3"</b> into the command line.

<h3>Usage</h3>
./project3 \<Trace Level\>
<br>
The trace level can be input directly into the call to the executable, or if no trace level is input then the user will be prompted for it.


<h3>Cleanup</h3>
While you *can* just retype <b>"make"</b>, you can tidy up the project directory by removing the executable file by typing <b>"make clean"</b>.