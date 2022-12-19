# Pipes

College project. 
The purpose of the project was to gain experience with UNIX (Linus) system calls. 
The project is two programs, Interface & Server. After initialization, the Interface will use the fork system call and an exec system call to start the execution of the Server. The Interface will accept commands from the console and send them to the Server for execution. After executing the command, the Server will send the output to the Interface for display on console. 

This project uses Pipes for communication between Interface and Server. Two pipes, one for sending commands to the Server and one for sending the response to the Interface. 
