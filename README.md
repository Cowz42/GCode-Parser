GCode Parser

IMPORTANT
You need to write your own Machine::run_low() function
this function is for interface with the baremetal components, and will not be the same between hardware implementations, I will write my own but it will only work for my own project, and it will be in a different file
you can see the source code to figure out how to connect it to your own implementation


for parsing gcode into easy for machines to read packets of numbers

compiled with 
~$ g++ -o a.out main.cpp gcodeparse.cpp gcode.cpp commandlineread.cpp
then just run the a.out as ./a.out -f GCODE_FILE_NAME.gcode in the same directory
if you have a different compilier I don't know how to help you

Dependencies
all the code is already in here, should just be able to run the g++ command

Help
idk if you can message on github so do that if you can

By Colin Iverson

Version History

Now:
Alpha 0.0.1

License
Idk how this works, but the MIT license of whatever, I forgot and I'm lazy

Inspiration
My friend was talking about gcode and I'm a nerd