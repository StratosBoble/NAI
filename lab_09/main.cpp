#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <cmath>
#include <sstream>

using namespace std;
using namespace cv;

const float MARKER_MM = 70;

vector<Point2f> fromPixelToMiliMeters(vector<Point2f> original){
    vector<Point2f> result;
    double distance = 0;
    for (int i = 0; i < 4;i++) {
        Point2f v = (original[i] - original[(i+1)%4]);
        cout << "Corner nr." << i <<endl;
        cout << ""<<original[i] << ", Corner nr."<< (i+1)%4 <<" : " << original[(i+1)%4] << endl;

        cout << "v is equal "<< v << endl;
        distance += sqrt(v.x*v.x + v.y*v.y);
        cout << distance << endl;
    }
    cout << distance << endl;
    distance = (distance / 4);
    for (int i = 0; i < 4;i++) {
        result.emplace_back(((MARKER_MM*(original[i].x / distance))),(MARKER_MM*(original[i].y / distance)));
    }
    cout << "Result is =" << result << endl;

    return result;
}

int main()
{
    // Ładujemy bibliotekę OpenCV do wykrywania i odczytywania kodów ArUco
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
    Ptr<aruco::DetectorParameters> parameters = aruco::DetectorParameters::create();

    // Otwieramy kamerę
    VideoCapture capture(0);
    if (!capture.isOpened())
    {
        cerr << "Nie udało się otworzyć kamery" << endl;
        return -1;
    }

    while (true)
    {
        // Pobieramy klatkę z kamery
        Mat frame;
        capture >> frame;
        if (frame.empty())
        {
            cerr << "Nie udało się pobrać klatki z kamery" << endl;
            break;
        }

        // Wykrywamy marker ArUco na obrazie i odczytujemy ich kody
        vector<vector<Point2f>> corners;
        vector<int> ids;
        aruco::detectMarkers(frame, dictionary, corners, ids, parameters);
        int fontFace=FONT_HERSHEY_SIMPLEX;
        double fontScale=1;
        int  thickness = 2;
        Point  textOrg(10,70);

        // Jeśli znaleziono co najmniej dwa markery ArUco, obliczamy odległość pomiędzy nimi
        if (corners.size() == 2)
        {
            Point2f point1 = corners[0][0];
            Point2f point2 = corners[1][0];

            // Współrzędne wszystkich rogów 1  markera
            auto firstMarker = corners.at(0);
            // Współrzędne wszystkich rogów 2 markera
            auto secondMarker = corners.at(1);

            auto secondMarkerFirstCorn = fromPixelToMiliMeters(secondMarker)[0];
            auto firstMarkerFirstCorn = fromPixelToMiliMeters(firstMarker)[0];

//
//            // Obliczamy odległość euklidesową pomiędzy markerami
            double distance = sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2));
            auto dist = std::to_string(distance);

            double realDistance = sqrt(pow(secondMarkerFirstCorn.x - firstMarkerFirstCorn.x, 2) + pow(secondMarkerFirstCorn.y - firstMarkerFirstCorn.y, 2));

            cout << realDistance <<endl;

            line(frame,point1,point2,Scalar(0,0,255),1,1,0);
            putText(frame,dist,textOrg,fontFace,fontScale,Scalar::all(0),thickness,8);


        }
        else if (corners.size() == 1)
        {
            Point2f point1 = corners[0][0];
            Scalar color(0,255,255);
            //zmieniamy współrzędne na int by pozbyć się przecinka
            int x =std::floor(point1.x);
            int y = std::floor(point1.y);
            //Jeśli jest tylko jeden marker to wypisujemy jego współrzędne
            string text = "Znaleziono 1 marker (x,y): " + std::to_string(x) +", "+std::to_string(y);
            putText(frame,text,textOrg,fontFace,fontScale,color,thickness,8);
            //i rysujemy ten punkt

        }
        else{
            Scalar color(0,0,255);
            string text = "Brak Markerow";
            putText(frame,text,textOrg,fontFace,fontScale,color,thickness,8);
        }

        aruco::drawDetectedMarkers(frame,corners,ids);
        imshow("Detektor aruco",frame);
        char key =(char) waitKey(1);
        if(key == 'q'){
            break;
        }
    }

    return 0;
}
