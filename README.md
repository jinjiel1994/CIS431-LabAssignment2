# CIS431-LabAssignment2

Using the twirl filter to distort the images of bombus, optimized by TBB parallel_for, TBB tasks and both respectively.

All of the three ways cost similar time, 12.353s, 10.6708s and 12.3832s respectively.

Before compile the code, make sure you have installed tbb and the Cool Image (CImg) library of c++.

* For compiling twirlfilter_tbb_for.cpp:

OSX compilation:

	g++ -I/opt/X11/include -o twirlfilter twirlfilter_tbb_for.cpp -L/opt/X11/lib -lX11 -ljpeg -ltbb
	
Run the program in terminal:

	./twirlfilter

* For compiling twirlfilter_tbb_task.cpp:

OSX compilation:

	g++ -I/opt/X11/include -o twirlfilter twirlfilter_tbb_task.cpp -L/opt/X11/lib -lX11 -ljpeg -ltbb
	
Run the program in terminal:

	./twirlfilter
	
* For compiling twirlfilter_tbb_both.cpp:

OSX compilation:

	g++ -I/opt/X11/include -o twirlfilter twirlfilter_tbb_both.cpp -L/opt/X11/lib -lX11 -ljpeg -ltbb
	
Run the program in terminal:

	./twirlfilter
	







