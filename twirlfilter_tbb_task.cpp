// OSX compilation:
// g++ -I/opt/X11/include -o twirlfilter twirlfilter_tbb_task.cpp -L/opt/X11/lib -lX11 -ljpeg -ltbb

#include <cstdio>
#include <iostream>
#include <string>
#define cimg_use_jpeg
#include "CImg.h"
#include <fstream>
#include <ctime>
#include "tbb/task_scheduler_init.h"
#include "tbb/task_group.h"

using namespace std;

class DistortTask: public tbb::task {
public:
  const string file_name;
  DistortTask( string line_) :
  file_name(line_) {}
  task* execute() { // Overrides virtual function task::execute
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

  float angle;
  int x, y, u, v;

  int** X = new int*[width];
  for(int i = 0; i < width; ++i)
    X[i] = new int[height];  

  int** Y = new int*[width];
  for(int i = 0; i < width; ++i)
    Y[i] = new int[height]; 


  float a = 3.14;
  float radius = 300;

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


  return NULL;
  }
};

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

    DistortTask& a = *new(tbb::task::allocate_root()) DistortTask(line);
    tbb::task::spawn_root_and_wait(a);
  }

  // Stop the timer
  duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
  cout<< "It takes "<< duration << "s " << endl;
  inFile.close();

  return 0;
};