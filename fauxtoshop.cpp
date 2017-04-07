// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: rewrite this comment

#include <iostream>
#include "console.h"
#include "gwindow.h"
#include "grid.h"
#include "simpio.h"
#include "strlib.h"
#include "gbufferedimage.h"
#include "gevents.h"
#include "math.h" //for sqrt and exp in the optional Gaussian kernel
using namespace std;

static const int    WHITE = 0xFFFFFF;
static const int    BLACK = 0x000000;
static const int    GREEN = 0x00FF00;
static const double PI    = 3.14159265;

void doFauxtoshop(GWindow &gw, GBufferedImage &img);

bool openImageFromFilename(GBufferedImage& img, string filename);
bool saveImageToFilename(const GBufferedImage &img, string filename);
void getMouseClickLocation(int &row, int &col);
void scatterImage(GBufferedImage &img);
void saveImage(GBufferedImage &img);

/* STARTER CODE FUNCTION - DO NOT EDIT
 *
 * This main simply declares a GWindow and a GBufferedImage for use
 * throughout the program. By asking you not to edit this function,
 * we are enforcing that the GWindow have a lifespan that spans the
 * entire duration of execution (trying to have more than one GWindow,
 * and/or GWindow(s) that go in and out of scope, can cause program
 * crashes).
 */
int main() {
    GWindow gw;
    gw.setTitle("Fauxtoshop");
    gw.setVisible(true);
    GBufferedImage img;
    doFauxtoshop(gw, img);
    return 0;
}

/* This is yours to edit. Depending on how you approach your problem
 * decomposition, you will want to rewrite some of these lines, move
 * them inside loops, or move them inside helper functions, etc.
 *
 * TODO: rewrite this comment.
 */
void doFauxtoshop(GWindow &gw, GBufferedImage &img) {
  cout << "Welcome to Fauxtoshop!" << endl;

  while (true) {
    cout << "Enter name of image file to open (or blank to quit)" << endl;    
    string line = getLine();
    if (line == "") {
      break;			// terminate program
    } else {
      if (openImageFromFilename(img, line)) {
	gw.setCanvasSize(img.getWidth(), img.getHeight());
	gw.add(&img,0,0);
	//Grid<int> original = img.toGrid();

	cout << "Which image filter would you like to apply?" << endl;
        cout << "1) Scatter" << endl;
        cout << "2) Edge detection" << endl;
        cout << "3) Green screen with another image" << endl;
        cout << "4) Compare with another image" << endl;

	while (true) {
	  int choice = getInteger("Your choice: ");
	  cout << endl;
	  if (choice == 1) {
	    scatterImage(img);
	    saveImage(img);
	    break;
	  } else if (choice == 2) {
	    cout << "edge detection" << endl;
	    break;
	  } else if (choice == 3) {
	    cout << "green screen" << endl;
	    break;
	  } else if (choice == 4) {
	    cout << "compare" << endl;
	    break;
	  } else {
	    cout << "Wrong input, select again: " << endl;
	    continue;
	  }
	}
      }else {
	cout << "Invalid file name, try again: " << endl;
      }
    }

    //cout << "clean window" << endl;
    gw.clear();			// clean up for the next run
  }

  // GBufferedImage img2;
  // openImageFromFilename(img2, "beyonce.jpg");
  // img.countDiffPixels(img2);

  // int row, col;
  // getMouseClickLocation(row, col);
  // gw.clear();
}


/* Function: scatterImage
 * Usage:    scatterImage(img);
 * ----------------------------
 * Precondition:
 * Postcondition
 */
void scatterImage(GBufferedImage &img) {
  Grid<int> original = img.toGrid();
  Grid<int> scattered = Grid<int>(original.numRows(), original.numCols());
  int degree = 0;
  
  while (true) {
    degree = getInteger("Enter degree of scatter (1-100): ");
    if (degree >= 1 && degree <= 100) break;
  }

  for (int i = 0; i < scattered.numRows(); i++) {
    for (int j = 0; j < scattered.numCols(); j++) {
      int row = 0;
      int col = 0;
      while (true) {
	row = randomInteger(i - degree, i + degree);
	if (row >=0 && row < scattered.numRows()) break;
      }
      while (true) {
	col = randomInteger(j - degree, j + degree);
	if (col >=0 && col < scattered.numCols()) break;
      }
      scattered.set(i, j, original[row][col]);
    }
  }

  img.fromGrid(scattered);
}


/* Function: saveImage
 * Usage:    saveImage(img)
 * ------------------------
 */
void saveImage(GBufferedImage &img) {
  while (true) {
    cout << "Enter file name to save image (or blank to skip saving)" << endl;
    string line = getLine();
    if (line == "") {
      break;
    } else {
      if (saveImageToFilename(img, line)) {
	break;
      }
    }
  }  
}



/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to open the image file 'filename'.
 *
 * This function returns true when the image file was successfully
 * opened and the 'img' object now contains that image, otherwise it
 * returns false.
 */
bool openImageFromFilename(GBufferedImage& img, string filename) {
    try { img.load(filename); }
    catch (...) { return false; }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to save the image file to 'filename'.
 *
 * This function returns true when the image was successfully saved
 * to the file specified, otherwise it returns false.
 */
bool saveImageToFilename(const GBufferedImage &img, string filename) {
    try { img.save(filename); }
    catch (...) { return false; }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Waits for a mouse click in the GWindow and reports click location.
 *
 * When this function returns, row and col are set to the row and
 * column where a mouse click was detected.
 */
void getMouseClickLocation(int &row, int &col) {
    GMouseEvent me;
    do {
        me = getNextEvent(MOUSE_EVENT);
    } while (me.getEventType() != MOUSE_CLICKED);
    row = me.getY();
    col = me.getX();
}

/* OPTIONAL HELPER FUNCTION
 *
 * This is only here in in case you decide to impelment a Gaussian
 * blur as an OPTIONAL extension (see the suggested extensions part
 * of the spec handout).
 *
 * Takes a radius and computes a 1-dimensional Gaussian blur kernel
 * with that radius. The 1-dimensional kernel can be applied to a
 * 2-dimensional image in two separate passes: first pass goes over
 * each row and does the horizontal convolutions, second pass goes
 * over each column and does the vertical convolutions. This is more
 * efficient than creating a 2-dimensional kernel and applying it in
 * one convolution pass.
 *
 * This code is based on the C# code posted by Stack Overflow user
 * "Cecil has a name" at this link:
 * http://stackoverflow.com/questions/1696113/how-do-i-gaussian-blur-an-image-without-using-any-in-built-gaussian-functions
 *
 */
Vector<double> gaussKernelForRadius(int radius) {
    if (radius < 1) {
        Vector<double> empty;
        return empty;
    }
    Vector<double> kernel(radius * 2 + 1);
    double magic1 = 1.0 / (2.0 * radius * radius);
    double magic2 = 1.0 / (sqrt(2.0 * PI) * radius);
    int r = -radius;
    double div = 0.0;
    for (int i = 0; i < kernel.size(); i++) {
        double x = r * r;
        kernel[i] = magic2 * exp(-x * magic1);
        r++;
        div += kernel[i];
    }
    for (int i = 0; i < kernel.size(); i++) {
        kernel[i] /= div;
    }
    return kernel;
}
