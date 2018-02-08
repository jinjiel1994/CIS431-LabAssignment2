// OSX compilation:
// g++ -I/opt/X11/include -o twirlfilter twirlfilter_tbb_for.cpp -L/opt/X11/lib -lX11 -ljpeg -ltbb

#include <cstdio>
#include <iostream>
#include <string>
#define cimg_use_jpeg
#include "CImg.h"
#include <fstream>
#include <ctime>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"

using namespace std;

class Distort {

  public:
  int** &X;
  int** &Y; 
  int centreX, centreY;
  float radius;
  float a;
  void operator() (const tbb::blocked_range2d<int> &range) const {
    for (int c = range.rows().begin(); c != range.rows().end(); ++c) {
      for (int r = range.cols().begin(); r != range.cols().end(); ++r){
          
          int x = c - centreX;
          int y = r - centreY;

          float angle = a * exp(-(x*x+y*y)/(radius*radius));

          X[c][r] = cos(angle)*x + sin(angle)*y + centreX;
          Y[c][r] = -sin(angle)*x + cos(angle)*y + centreY;
      }
    }
  }
  Distort(int** &X,int** &Y,int cenX,int cenY,float r,float ae):
  X(X),Y(Y),centreX(cenX),centreY(cenY),radius(r),a(ae) {}

};

int distortion(string file_name){ 

  const char *name = file_name.c_str();

    // import image from jpg file
  cimg_library::CImg<unsigned char> input_img(name);

  // create new image (width, height, depth, channels (RGB))
  cimg_library::CImg<unsigned char> output_img(
      input_img.width(), input_img.height(), 1, 3);

  // iterate over the input image
  int width = input_img.width() ;
  int height = input_img.height();
  int centreX = input_img.width() / 2;
  int centreY = input_img.height() / 2;

  int u, v;

  int** X = new int*[width];
  for(int i = 0; i < width; ++i)
    X[i] = new int[height];  

  int** Y = new int*[width];
  for(int i = 0; i < width; ++i)
    Y[i] = new int[height]; 


  float a = 3.14;
  float radius = 300;

  // Parallel the algorithm
  tbb::parallel_for(tbb::blocked_range2d<int>(0, width, 0, height), Distort(X,Y,centreX,centreY,radius,a) );  


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

  int beginIdx = file_name.rfind('/');
  string filename = "output/dis_" + file_name.substr(beginIdx + 1);
  const char *final_name = filename.c_str();
  output_img.save_jpeg(final_name);

  return 0;
}

int main() {

  string line;
  ifstream inFile;
  inFile.open("./image_list.txt");
  if (!inFile) {
      cout << "Unable to open file datafile.txt";
      exit(1);   // call system to stop
  } 

  // Start the timer
  clock_t start;
  double duration;
  start = clock();

  while ( getline (inFile,line) ){
    distortion(line);
  }
  // Stop the timer
  duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
  cout<< "It takes "<< duration << "s " << endl;
  inFile.close();

  return 0;
}