package com.lvyouhai.opengl3d;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity implements SensorEventListener {
    GL2View mView;

    SensorManager mSensorManager;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //加载JNI接口
        System.loadLibrary( "openal" );
        System.loadLibrary( "MainActivity" );
        mView = new GL2View( getApplication() );
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setContentView( mView );
    }
    @Override protected void onResume()
    {
        super.onResume();

        mView.onResume();
    }


    public static native void Accelerometer( float x, float y, float z );
//检测游标状态
    public void onSensorChanged( SensorEvent event )
    {
        float x = event.values[ SensorManager.DATA_X ],
                y = event.values[ SensorManager.DATA_Y ],
                z = event.values[ SensorManager.DATA_Z ];

        Accelerometer( x, y, z );
    }

    public void onAccuracyChanged(Sensor sensor, int arg1 ) {}
}
