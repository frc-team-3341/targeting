#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include <ctime>

#include "Rectangle.hpp"

using namespace cv;
using namespace std;

int thresh = 50, N = 11;
const char* wndname = "Square Detection Demo";

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();
    
    Mat pyr, timg, gray0(image.size(), CV_8U), gray;
    
    // down-scale and upscale the image to filter out the noise
    pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    pyrUp(pyr, timg, image.size());
    vector<vector<Point> > contours;
    
    // find squares in every color plane of the image
    // original: for( int c = 0; c < 3; c++ )
    for( int c = 0; c < 1; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);
        
        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                Canny(gray0, gray, 0, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/N;
            }

            // find contours and store them all as a list
            findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

            vector<Point> approx;
            
            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
                
                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 1000 &&
                    isContourConvex(Mat(approx)) )
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
            }
        }
    }
    // my addition
    for (unsigned i=0; i<squares.size(); ++i) {
      for (unsigned j=0; j<squares.at(i).size(); ++j)
	cout <<squares.at(i).at(j) <<endl;
      cout <<endl;
    }
}


// the function draws all the squares in the image
void drawSquares( Mat& image, const vector<vector<Point> >& squares )
{
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(image, &p, &n, 1, true, Scalar(0,255,0), 3, CV_AA);
    }

    imshow(wndname, image);
}


int main(int argc, char* argv[])
{
    namedWindow( wndname, 0 );
    vector<vector<Point> > squares;
    VideoCapture cap;
    bool isFile=false;

    // Check Argument Type
    cout <<"argv[1]=" <<argv[1] <<endl;
    if (string(argv[1])=="-f")
	isFile=true;
      
    if (!isFile) {
      // Get Video Capture Device
      cap.open(atoi(argv[2]));
      if (!cap.isOpened()) {
	cerr <<"Unable to open capture device " <<argv[2] <<"." <<endl;
	return -1;
      }
    }
    
    while (true)
    {
      Mat original;
      Mat output;
      Mat image;
      if (isFile)
	original=imread(argv[2]); // Load Image from File
      else
	cap >>original; // Load Image from Video Capture Device

      original.copyTo(image);
      original.copyTo(output);
      cvtColor(image, image, CV_RGB2GRAY);
      cvtColor(image, image, CV_GRAY2RGB);
      threshold(image, image, 200, 255, CV_THRESH_BINARY);
      findSquares(image, squares);
      drawSquares(original, squares);
      int keycode=waitKey(10);
      if (keycode==120)
      {
	stringstream filename;
	filename <<time(NULL) <<".jpg";
	imwrite(filename.str().c_str(), output);
      }
      else if (keycode == 27)
	break;
      if (isFile)
      {
	waitKey();
	break;
      }
    }

    return 0;
}
