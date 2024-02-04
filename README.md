# Text Editor
So, I was just diving into the specifics of how a text editor works. This was the result. And no, I have yet to complete it. 
## Requirements
The project is based on termios.h header file and will not work on Windows PowerShell or CMD. You can use it in Windows by installing WSL (Windows Subsystem for Linux).
- MacOS / Linux
- g++, cmake
## How to Run
- First, create a directory 'build' and cd into it
- From there, run `cmake ../`
- Then, rum `make`
- Right now, you need TextEditor.cpp in the same folder to run, not because it is a dependency, just because I hard coded it to only open and edit that file. So, run `cp ../TextEditor.cpp ./`
- Finally, you will be able to run the TextEditor. Just run `./TextEditor`.

**NOTE THAT I HAVE YET TO COMPLETE MANY FEATURES**
