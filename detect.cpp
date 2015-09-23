#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
  double factor = 1.0625;

  if (argc < 2)
  {
    printf("usage: <image> [<factor>]\n");
    return 1;
  }
  else if (argc >= 3)
  {
    factor = stod(argv[2]);
  }

  const char FaceCascade[] = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt_tree.xml";
  const char EyeCascade[]  = "/usr/share/opencv/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

  CascadeClassifier face_cas(FaceCascade);
  CascadeClassifier eye_cas(EyeCascade);

  Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  equalizeHist(image, image);

  vector<Rect> faces;

  face_cas.detectMultiScale(image, faces, factor,
                            2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

  for (auto face : faces)
  {
    Mat f = image(face);

    imshow("Preview", f);
    waitKey(0);
    destroyAllWindows();

    string person("");
    printf("Please enter the person name: ");
    getline(cin, person);

    if (person.empty())
      person = "unknown";

    vector<Rect> eyes;
    eye_cas.detectMultiScale(f, eyes, factor,
                             2, 0 | CASCADE_SCALE_IMAGE, Size(10, 10));

    if (eyes.size() == 2)
    {
      Rect left_eye  = (eyes[0].x < eyes[1].x) ? eyes[0] : eyes[1];
      Rect right_eye = (eyes[0].x > eyes[1].x) ? eyes[0] : eyes[1];

      Point le_center(face.x +  left_eye.x +  left_eye.width  / 2,
                      face.y +  left_eye.y +  left_eye.height / 2);
      Point re_center(face.x + right_eye.x + right_eye.width  / 2,
                      face.y + right_eye.y + right_eye.height / 2);

      printf("(\"%s\" \"%s\" (%d %d %d %d) (%d %d) (%d %d))\n",
             argv[1], person.c_str(), face.x, face.y, face.width, face.height,
             le_center.x, le_center.y, re_center.x, re_center.y);
    }
    else
    {
      printf("(\"%s\" \"%s\" (%d %d %d %d))\n",
             argv[1], person.c_str(), face.x, face.y, face.width, face.height);
    }
  }

  return 0;
}

