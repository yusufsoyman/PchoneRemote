package com.example.z003damh.laptopremotecontrol;

import android.app.Activity;
//import android.support.v7.app.ActionBarActivity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;

import java.io.BufferedWriter;
import java.io.ByteArrayOutputStream;
import java.io.IOError;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class MainActivity extends Activity {

    public static final int SERVERPORT = 63783;
    private static final String SERVER_IP = "127.0.0.1";
    private Socket socket;

    private Button mPauseButton, mConnectButton;

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

    public void onConnectButtonClicked(View view) {

        String clientRequest = "2";
        //for connect button
        mConnectButton = (Button)findViewById(R.id.connect);
        mConnectButton.setOnClickListener( new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                final String DEBUG_TAG = "NetworkStatusExample";

                try
                {
                    //InetAddress serverAddr = InetAddress.getByName(SERVER_IP);
                    socket = new Socket(SERVER_IP, SERVERPORT);
                }
                catch (UnknownHostException e1) {
                    e1.printStackTrace();
                }
                catch (IOException e1) {
                    e1.printStackTrace();
                }

                try
                {
                    PrintWriter out = new PrintWriter(new BufferedWriter(
                            new OutputStreamWriter(socket.getOutputStream())),
                            true);
                }
                catch (UnknownHostException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (Exception e) {
                    e.printStackTrace();
                }
                //             ConnectivityManager connMgr = (ConnectivityManager)
                //                   getSystemService(Context.CONNECTIVITY_SERVICE);
                //NetworkInfo networkInfo = connMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
//                boolean isWifiConn = networkInfo.isConnected();
                //              networkInfo = connMgr.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
                //            boolean isMobileConn = networkInfo.isConnected();
          /*      NetworkInfo networkInfo = connMgr.getActiveNetworkInfo();
                if(networkInfo != null) {
                    boolean isDeviceConn = networkInfo.isConnected();
                    Log.d(DEBUG_TAG, "Wifi connected: " + isDeviceConn);
                }*/
//                Log.d(DEBUG_TAG, "Wifi connected: " + isWifiConn);
                //              Log.d(DEBUG_TAG, "Mobile connected: " + isMobileConn);

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