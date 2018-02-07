# CIS431-LabAssignment1

Using the twirl filter to distort the images of bombus, compiled by both g++ and ispc.

I tested them with 200 pixels radius and 3.14 radians angle. 
the c++ one costs 0.062439s, 
whereas the ispc one costs 0.017738s.

Before compile the code, make sure you have installed ispc and the Cool Image (CImg) library of c++.

* For compiling g++:

OSX compilation:

	g++ -I/opt/X11/include -o twirlfilter twirlfilter_serial.cpp -L/opt/X11/lib -lX11 -ljpeg
	
Run the program in terminal:

	./twirlfilter
	
Then follow the prompt.
	
* For compiling ispc:

~~~~
make
~~~~
	
Run the program in terminal:

	
	./twirlfilter
	
Then follow the prompt.




