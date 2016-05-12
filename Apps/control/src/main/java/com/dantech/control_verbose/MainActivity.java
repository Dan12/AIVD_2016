package com.dantech.control_verbose;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.widget.Toast;

public class MainActivity extends Activity {
	
	int width;
    int height;
    
    DrawingView dv;
    
    BluetoothAdapter mBluetoothAdapter;
    BluetoothSocket mmSocket;
    BluetoothDevice mmDevice;
    OutputStream mmOutputStream;
    InputStream mmInputStream;
    Thread workerThread;
    byte[] readBuffer;
    int readBufferPosition;
    int counter;
    volatile boolean stopWorker;
    int restartCount = 0;
    String fullMessage = "";
    boolean findingBluetooth = false;
    volatile boolean stopDeviceFinder = false;

    public RecordData dataRecorder = new RecordData();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		dv = new DrawingView(this);
		
		dv.setBackgroundColor(Color.WHITE);
        
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        width=dm.widthPixels;
        height=dm.heightPixels;
        System.out.println(width + " , "+height);
        dv.setDim(width, height);
        
    	setContentView(dv);
    	
//		try{
//		  findBT();
//		  openBT();
//		  sendData("90,0,0");
//		}
//		catch (IOException ex) { }
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// get masked (not specific to a pointer) action
        
        switch (event.getAction()) {
        	case MotionEvent.ACTION_DOWN:
        		float tX = event.getX();
        		float tY = event.getY();
        		//System.out.println("touch at "+tX+","+tY);
        		dv.touchX = (int)tX;
        		dv.touchY = (int)tY;
        		if(dv.settingsButton.touchButton(tX, tY) && !dv.settingsScreen){
        			dv.settingsScreen = true;
        		}
        		if(dv.settingsScreen){
        			if(dv.backButton.touchButton(tX, tY))
        				dv.settingsScreen = false;
        			if(!dv.isRecording && dv.recordButton.touchButton(tX, tY)) {
                        dv.isRecording = true;
                        dataRecorder.startRecord();
                    }
        			else if(dv.isRecording && dv.stopRecordButton.touchButton(tX,tY)) {
                        dv.isRecording = false;
                        dataRecorder.finishRecording();
                    }
        			if(dv.connectButton.touchButton(tX, tY)){
        				//try{
                            Toast.makeText(getApplicationContext(), "Nullifying Current Stream", Toast.LENGTH_SHORT).show();
                            if(!findingBluetooth)
                                findBT();
//    					}
//    					catch (IOException ex) { }
        			}
        			if(dv.unstickButton.touchButton(tX, tY)){
        				try{
    					  sendData();
    					}
    					catch (IOException ex) { }
        			}
        			if(!dv.isPlaying && dv.playButton.touchButton(tX, tY)){
                        //System.out.println("Pressed");
                        if(!dv.isRecording && !dv.isPlaying){
                            dv.isPlaying = true;
                            dataRecorder.startPlayback();
                        }

        			}
                    else if(dv.stopPlayButton.touchButton(tX, tY)){
                        //System.out.println("Stop playback");
                        if(!dv.isRecording && dv.isPlaying){
                            dv.isPlaying = false;
                            dv.knobXDisp = 0;
                            dv.knobYDisp = 0;
                        }

                    }
        		}
        		break;
        	case MotionEvent.ACTION_UP:
        		//System.out.println("no touch");
        		dv.touchX = -10;
        		dv.touchY = -10;
        		break;
        	case MotionEvent.ACTION_MOVE:
        		//touch
        		float sX = event.getX();
        		float sY = event.getY();
        		dv.touchX = (int)sX;
        		dv.touchY = (int)sY;
        		break;
        }
        
        return super.onTouchEvent(event);
	}
	
	void findBT(){
        findingBluetooth = true;
        mmInputStream = null;
        mmOutputStream = null;
        mmDevice = null;
        mmSocket = null;
        stopWorker = true;
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(mBluetoothAdapter == null) {
            //System.out.println("No bluetooth adapter available");
            toastMessage("No bluetooth adapter available");
        }

        if(!mBluetoothAdapter.isEnabled()) {
            Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBluetooth, 0);
        }

        final Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            CharSequence devices[] = new CharSequence[pairedDevices.size()];
            int i = 0;
            for (BluetoothDevice device : pairedDevices) {
                devices[i] = device.getName();
                i++;
            }

            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle("Pick a device");
            builder.setItems(devices, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialogInterface, int i) {
                    int k = 0;
                    for (BluetoothDevice device : pairedDevices) {
                        if(i == k) {
                            tryConnection(device);
                            break;
                        }
                        k++;
                    }

                }
            });

            builder.show();
        }

        stopDeviceFinder = false;

    }

    void tryConnection(BluetoothDevice tempDiv){
        final BluetoothDevice device = tempDiv;
        Thread openDeviceThread = new Thread(new Runnable() {
            public void run() {
                boolean success = false;
                for (int i = 0; i < 1; i++) {
                    toastMessage("Bluetooth try "+device.getName());
                    if(stopDeviceFinder) {
                        System.out.println("Stopping device finder");
                        break;
                    }
                    System.out.println(device.getName());
                    if (mmOutputStream != null) {
                        success = true;
                        break;
                    }
                    mmDevice = device;
                    try {
                        openBT();
                        //System.out.println("success here");
                        success = true;
                        break;
                    } catch (IOException e) {
                        //System.out.println("Failed");
                    }
                }
                if (!success)
                    toastMessage("Failed To Open The Device");
                else{}
                //System.out.println("Bluetooth Device Found");
                findingBluetooth = false;
            }
        });
        openDeviceThread.start();
    }
    
    void openBT() throws IOException{
        UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb"); //Standard SerialPortService ID
        mmSocket = mmDevice.createRfcommSocketToServiceRecord(uuid);
        mmSocket.connect();
        mmOutputStream = mmSocket.getOutputStream();
        mmInputStream = mmSocket.getInputStream();
        
        beginListenForData();
        
        System.out.println("Bluetooth Opened");
        toastMessage("Bluetooth Opened");

        if(mmOutputStream != null) {
            sendData();
        }
    }
    
    void beginListenForData(){
        stopWorker = false;
        workerThread = new Thread(new Runnable(){
            public void run(){                
               while(!Thread.currentThread().isInterrupted() && !stopWorker){
                    try {
                        int bytesAvailable = mmInputStream.available(); 
                        if(bytesAvailable > 0){
                            byte[] packetBytes = new byte[bytesAvailable];
                            mmInputStream.read(packetBytes);
                            for(int i=0;i<bytesAvailable;i++){
                                byte b = packetBytes[i];
                                //if(b != 10 && b != 13);
                                    byte[] encodedBytes = new byte[1];
                                    encodedBytes[0] = b;
                                    String data = new String(encodedBytes, "US-ASCII");
                                    if(!data.equals("*"))
                                    	fullMessage+=data;
                                    else{
	                                    dv.verbose = fullMessage;
                                        if(dv.isPlaying){
                                            int[] point = dataRecorder.getPoint();
                                            if(point[0] != Integer.MIN_VALUE) {
                                                dv.knobXDisp = point[0];
                                                dv.knobYDisp = point[1];
                                            }
                                            else{
                                                dv.isPlaying = false;
                                                dv.knobXDisp = 0;
                                                dv.knobYDisp = 0;
                                                toastMessage("Finished recording");
                                            }
                                        }
                                        sendData(String.format("%d,%d" , (int)DrawingView.map(dv.knobXDisp, -dv.squareHalfSide, dv.squareHalfSide, -200, 200),(int)DrawingView.map(dv.knobYDisp, -dv.squareHalfSide, dv.squareHalfSide, -200, 200)));
                                        if(dv.isRecording)
                                            dataRecorder.recordPoint(dv.knobXDisp, dv.knobYDisp);
	                                    fullMessage = "";
                                    }
                                //}
                            }
                        }
                    } 
                    catch (IOException ex) {stopWorker = true;}
               }
            }
        });
        workerThread.start();
    }

    private void toastMessage(final String m){
        runOnUiThread(new Runnable() {
            public void run() {
                Toast.makeText(getApplicationContext(), m, Toast.LENGTH_LONG).show();
            }
        });
    }
    
	void sendData(String m) throws IOException{
        String msg = m;
        mmOutputStream.write(msg.getBytes());
        //System.out.println("Data Sent");
    }

    void sendData() throws IOException{
        sendData("0,0");
    }
	
    void closeBT() throws IOException{
        stopWorker = true;
        if(mmOutputStream != null)
            mmOutputStream.close();
        if(mmInputStream != null)
            mmInputStream.close();
        if(mmSocket != null)
            mmSocket.close();
        System.out.println("BlueTooth Closed");
    }
    
    protected void onDestroy(){
    	System.out.println("Destroyed");
    	super.onDestroy();
    	System.out.println(isFinishing());
    	try {
			closeBT();
		} catch (IOException e) {e.printStackTrace();}
    	finish();
    }
}
