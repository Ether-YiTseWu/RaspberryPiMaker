#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;

// detectAndDisplay is a function, frame is a object of Mat
void detectAndDisplay( Mat frame );

// face_cascade and eyes_cascade are objects of CascadeClassifier
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

int main( int argc, const char** argv )
{
    // parser is a object of CommandLineParser
    CommandLineParser parser(argc, argv,
                             "{help h||}"
                             "{face_cascade|../../data/haarcascades/haarcascade_frontalface_alt.xml|Path to face cascade.}"
                             "{eyes_cascade|../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|Path to eyes cascade.}"
                             "{camera|0|Camera device number.}");
    parser.about( "\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face + eyes) in a video stream.\n"
                  "You can use Haar or LBP features.\n\n" );
    parser.printMessage();
    // face_cascade_name and eyes_cascade_name are string gotten by parser
    String face_cascade_name = parser.get<String>("face_cascade");
    String eyes_cascade_name = parser.get<String>("eyes_cascade");

    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) )
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };
    if( !eyes_cascade.load( eyes_cascade_name ) )
    {
        cout << "--(!)Error loading eyes cascade\n";
        return -1;
    };

    // get camera code and declare a object of VideoCapture
    int camera_device = parser.get<int>("camera");
    VideoCapture capture;
    //-- 2. Read the video stream
    capture.open( camera_device );
    if ( ! capture.isOpened() )
    {
        cout << "--(!)Error opening video capture\n";
        return -1;
    }
    // declare a Mat object named frame
    Mat frame;
    while ( capture.read(frame) )
    {
        if( frame.empty() )
        {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }
        //-- 3. Apply the classifier to the frame
        detectAndDisplay( frame );
        if( waitKey(10) == 27 )
        {
            break; // escape
        }
    }
    return 0;
}

void detectAndDisplay( Mat frame )
{
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    std::vector<Rect> faces;
    face_cascade.detectMultiScale( frame_gray, faces );
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        // center is a object of Point
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        // ellipse is a function(畫橢圓)
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
        Mat faceROI = frame_gray( faces[i] );

        //-- In each face, detect eyes
        std::vector<Rect> eyes;
        eyes_cascade.detectMultiScale( faceROI, eyes );
        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            // circle is a function(畫圓)
            circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4 );
        }
    }
    //-- Show what you got
    imshow( "Capture - Face detection", frame );
}
