package com.papakcisoftware.yuztanima;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceView;
import android.view.Window;
import android.widget.Toast;

import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.JavaCameraView;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.android.BaseLoaderCallback;
import org.opencv.imgproc.Imgproc;

import dalvik.system.BaseDexClassLoader;

public class MainActivity extends AppCompatActivity  implements CameraBridgeViewBase.CvCameraViewListener2{

    private static final String TAG ="MainActivity";
    JavaCameraView javaCameraView;
    Mat mRgba, imgGray,imgCanny;
    BaseLoaderCallback mLoaderCallBack = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status){

                case BaseLoaderCallback.SUCCESS:{
                    //Log.i(TAG, "OpenCV Yüklendi");
                    javaCameraView.enableView();
                    break;

                }
                default:{

                    super.onManagerConnected(status);
                    break;

                }
            }
        }
    };
    static {
            System.loadLibrary( "MyOpencvLibs");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        javaCameraView = (JavaCameraView) findViewById(R.id.java_camera_view);
        javaCameraView.setVisibility(SurfaceView.VISIBLE);
        javaCameraView.setCvCameraViewListener(this);

    }

    @Override
    protected void onPause() {
        super.onPause();
        if(javaCameraView!=null){
            javaCameraView.disableView();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(javaCameraView!=null){
            javaCameraView.disableView();
        }

    }
    //duraklatıldığında uygulamaya devam etmek.
    @Override
    protected void onResume() {
        super.onResume();
        if(OpenCVLoader.initDebug()){
            Log.i(TAG,"OpenCV Başlatıldı!..");
            mLoaderCallBack.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
        else{
            Log.i(TAG,"Open CV Başlatılamadı!..");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_4_0,this,mLoaderCallBack);
        }


    }

    //Kamera önizleme başladığında bu yöntem çağrılır. Bu yöntem çağrıldıktan sonra, çerçeveler onCameraFrame () geri çağrısı üzerinden istemciye teslim edilmeye başlayacaktır.
    @Override
    public void onCameraViewStarted(int width, int height) {

        mRgba = new Mat(height,width,CvType.CV_8UC4);
       // imgGray = new Mat(height,width,CvType.CV_8UC1);
       // imgCanny = new Mat(height,width,CvType.CV_8UC1);
    }

    @Override
    public void onCameraViewStopped() {
        mRgba.release();
    }
    //Çerçevenin iletilmesi gerektiğinde bu fonksiyon kullanılır.
    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {

        mRgba = inputFrame.rgba();
        OpencvClass.faceDetection(mRgba.getNativeObjAddr());
        return mRgba;
    }
}
