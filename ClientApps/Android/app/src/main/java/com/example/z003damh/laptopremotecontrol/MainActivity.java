package com.example.z003damh.laptopremotecontrol;

import android.app.Activity;
//import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

import java.io.ByteArrayOutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class MainActivity extends Activity {

    private Button mPauseButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //for pause button
        mPauseButton = (Button)findViewById(R.id.pause_play);
        mPauseButton.setOnClickListener( new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                if (mPauseButton.getText() == "PAUSE")
                    //call pause method
                    mPauseButton.setText("PLAY");
                else
                    //call play method
                    mPauseButton.setText("PAUSE");
            }
        });
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}

/*
ToDo:
1- button ların uzerine islevlerini yaz. ileri, geri, artır, azalt, hızlı ileri, hızlı geri
2- ekran dondurme icin yeni layout tasarla.
 */