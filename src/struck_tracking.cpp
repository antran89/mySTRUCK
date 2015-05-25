#include "Tracker.h"
#include "Config.h"

#include <iostream>
#include <fstream>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "vot.hpp"

using namespace std;
using namespace cv;

static String windowName = "STRUCK tracking";
static cv::Mat image;
static int x_tl;
static int y_tl;
static FloatRect initBB;
static bool paused;
static bool selectObject = false;
static bool dragOn = false;

static const char* keys =
{   "{@configPath        | | config path       }"
    "{@video_name        | | video name        }" };

static void help()
{
  cout << "\nThis example shows the functionality of \"Long-term optical tracking API\""
       "-- pause video [p] and draw a bounding box around the target to start the tracker\n"
       "Call:\n"
       "./tracker <config_path> <video_name>\n"
       "tracker_algorithm can be: MIL, BOOSTING, MEDIANFLOW, TLD"
       << endl;

  cout << "\n\nHot keys: \n"
       "\tq - quit the program\n"
       "\tp - pause video\n";
}

static void onMouse( int event, int x, int y, int, void* )
{
  if( !selectObject )
  {
    switch ( event )
    {
      case EVENT_LBUTTONDOWN:
        //set origin of the bounding box
        dragOn = true;
        x_tl = x;
        y_tl = y;
        break;
      case EVENT_LBUTTONUP:
        //set width and height of the bounding box
        initBB = FloatRect( x_tl, y_tl, std::abs(x - x_tl), std::abs(y - y_tl) );
        paused = false;
        selectObject = true;
        break;
      case EVENT_MOUSEMOVE:

        if( dragOn && !selectObject )
        {
          //draw the bounding box
          Mat currentFrame;
          image.copyTo( currentFrame );
          cv::rectangle( currentFrame, Point( x_tl, y_tl ), Point( x, y ), Scalar( 255, 0, 0 ), 2, 1 );
          imshow( windowName, currentFrame );
        }
        break;
    }
  }
}


int main(int argc, char* argv[])
{

    if (argc < 3)
    {
      help();
      return EXIT_FAILURE;
    }

    String configPath = argv[1];
    String video_name = argv[2];

    //open the capture
    VideoCapture cap;
    cap.open( video_name );

    if( !cap.isOpened() )
    {
      help();
      cout << "***Could not initialize capturing...***\n";
      cout << "Current parameter's value: \n";
      return -1;
    }

    Mat frame;
    paused = true;
    namedWindow( windowName );
    setMouseCallback( windowName, onMouse, 0 );

    //instantiates the specific tracker
    Config conf( configPath );
    cout << conf << endl;
    if (conf.features.size() == 0)
    {
        cout << "error: no features specified in config" << endl;
        return EXIT_FAILURE;
    }
    Tracker tracker( conf );

    //get the first frame
    cap >> frame;
    frame.copyTo( image );
    imshow( windowName, image );

    bool initialized = false;
    for ( ;; )
    {
      if( !paused )
      {

        if( !initialized && selectObject )
        {
            //initializes the tracker
            tracker.Initialise( image, initBB );
            initialized = true;

            cv::Rect newBB(initBB.XMin(), initBB.YMin(), initBB.Width(), initBB.Height());
            cv::rectangle( image, newBB, Scalar( 255, 0, 0 ), 2, 1 );
        }
        else if( initialized )
        {
            cap >> frame;
            if ( frame.empty() ) {
                cout << "\n\nend of sequence, press any key to exit" << endl;
                waitKey();
                break;
            }
            frame.copyTo( image );

            tracker.Track( image );
            if ( !conf.quietMode && conf.debugMode )
            {
                tracker.Debug();
            }

            FloatRect bb = tracker.GetBB();
            cv::Rect newBB(bb.XMin(), bb.YMin(), bb.Width(), bb.Height());
            cv::rectangle( image, newBB, Scalar( 255, 0, 0 ), 2, 1 );
        }

        imshow( windowName, image );
      }

      char c = (char) waitKey( 2 );
      if( c == 'q' || c == 27 )
        break;
      if( c == 'p' )
        paused = !paused;

    }

    return EXIT_SUCCESS;
}
