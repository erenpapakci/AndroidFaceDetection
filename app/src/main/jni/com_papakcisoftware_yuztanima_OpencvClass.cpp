#include "com_papakcisoftware_yuztanima_OpencvClass.h"


JNIEXPORT void JNICALL Java_com_papakcisoftware_yuztanima_OpencvClass_faceDetection
  (JNIEnv *, jclass, jlong addrRgba){

    Mat& frame = *(Mat*)addrRgba;

    detect(frame);

  }
  void detect(Mat& frame){


   //-----------------------------------------------------------------------------------------
      // Bu xml dosyaları projenin yükleneceği cihazlara atılmalıdır.
     String face_cascade_name = "/storage/emulated/0/data/haarcascade_frontalface_alt.xml";
     String eyes_cascade_name = "/storage/emulated/0/data/haarcascade_eye_tree_eyeglasses.xml";
     /*Nesneyi tespit etmek için öncelikle nesneyi sisteme tanıtmamız ve daha sonra bu tanımlanmış veri setini
       kullanarak görüntü üzerinde arama yapmamız gerekir. Haar cascade sınıflandırıcı bizden xml dosyası
       alır bu xml dosyaları bir nesnenin binlerce negatif ve pozitifi ile hazırlanmış veri setidir. Pozitif olarak
       tanımlanan görüntüler istenilen nesnenin bulunduğu negatif olarak tanımlananlar ise bulunması istenilen
       nesnenin bulunmadığı görüntülerdir.
     */
     //CascadeClassifier sınıfından değişkenler oluşturuldu
     CascadeClassifier face_cascade;
     CascadeClassifier eyes_cascade;
     // xml dosyalarının kontrolü
      if( !face_cascade.load( face_cascade_name ) )
      {

      printf("--(!)Error loading\n");

      return;

      };
      if(!eyes_cascade.load( eyes_cascade_name ) )
      {

      printf("--(!)Error loading\n");

      return;

      };
   //--------------------------------------------YUZ TANIMA
       std::vector<Rect> faces;
       //Mat, OpenCV kütüphanesinde görüntü barındırma sınıfıdır.
       Mat frame_gray;
       //cvtColor hazır bir OpenCV fonksiyonu üç değer alır ve görüntüyü griye dönüştürür.
       //Üçüncü parametre'de ne tür bir dönüştürme işlemi gerçekleştireceğimizi yazarız.
       cvtColor( frame, frame_gray, CV_BGR2GRAY ); //cvtColor( image, gray_image, CV_BGR2GRAY );
       //Neden RGB değil de BGR. Çünkü kamera üreticileri bu sıralamaya göre kullandığı için.
       //Histogram eşitleme işlemi gerçekleştirilir.
       //equalizeHist( frame_gray, frame_gray );
       // haar cascade sınıflandırıcısı gri renk uzayında tarama yapabilmektedir.
       // İstenirse histogram eşitlenebilir ancak bu reel time çalışmalarında test aşamasında performans olarak fark ortaya konamadı..


      //C++: void CascadeClassifier::detectMultiScale(const Mat& image, vector<Rect>& objects, double (ölçek fak)scaleFactor=1.1,
      //     int minNeighbors=2, int flags=0, Size minSize=Size(), Size maxSize=Size())
      //
      face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30) );

        for( size_t i = 0; i < faces.size(); i++ )
        {
        //yakalanan yüz üzerine elips çizilir.
          Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
          ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

          Mat faceROI = frame_gray( faces[i] );


      //-----------------------------------------GOZ TANIMA
          std::vector<Rect> eyes;


          eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30) );

          for( size_t j = 0; j < eyes.size(); j++ )
           {
           //yakalanan yüz üzerine şember çizilir.
             Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
             int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
             circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
           }
        }

  }