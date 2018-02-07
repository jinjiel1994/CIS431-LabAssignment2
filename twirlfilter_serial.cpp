// OSX compilation:
// g++ -I/opt/X11/include -o twirlfilter twirlfilter_serial.cpp -L/opt/X11/lib -lX11 -ljpeg

#include <cstdio>
#include <iostream>

#define cimg_use_jpeg
#include "CImg.h"
#include <ctime>
using namespace std;


int main() {

  // import image from jpg file
  cimg_library::CImg<unsigned char> input_img("bombus.jpg");

  // create new image (width, height, depth, channels (RGB))
  cimg_library::CImg<unsigned char> output_img(
      input_img.width(), input_img.height(), 1, 3);

  // iterate over the input image
  int width = input_img.width() ;
  int height = input_img.height();
  int centreX = input_img.width() / 2;
  int centreY = input_img.height() / 2;
  float radius, angle, a;
  int x, y, u, v;
  int X[width][height], Y[width][height];

  cout << "The image is " << width << " * " << height << endl;
  cout << "Please enter the radius of the effect(from the center) : ";
  cin >> radius;
  cout << "Please enter the angle of twist, in radians(recommand 3.14) : ";
  cin >> a;

  // Start the timer
  clock_t start;
  double duration;
  start = clock();

  // Parallel the algorithm
  for (int c = 0; c < input_img.width(); ++c) {
    for (int r = 0; r < input_img.height(); ++r) {

    	// Credited by https://stackoverflow.com/questions/225548/resources-for-image-distortion-algorithms
    	x = c - centreX;
    	y = r - centreY;

        angle = a * exp(-(x*x+y*y)/(radius*radius));

    	X[c][r] = cos(angle)*x + sin(angle)*y + centreX;
        Y[c][r] = -sin(angle)*x + cos(angle)*y + centreY;

    }
  }

  // Stop the timer
  duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
  cout<< "It takes "<< duration << "s " << endl;

  for (int c = 0; c < input_img.width(); ++c) {
      for (int r = 0; r < input_img.height(); ++r) {
    	  // write to the output image
          if (X[c][r] > 0 && Y[c][r] > 0 ){
          	output_img(c, r, 0) = input_img(X[c][r], Y[c][r], 0);
          	output_img(c, r, 1) = input_img(X[c][r], Y[c][r], 1);
            output_img(c, r, 2) = input_img(X[c][r], Y[c][r], 2);
          }
      }
  }

  output_img.save_jpeg("output.jpg");
}
