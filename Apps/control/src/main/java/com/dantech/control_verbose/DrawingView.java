package com.dantech.control_verbose;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.drawable.Drawable;
import android.view.View;

public class DrawingView extends View{

	int touchX;
	int touchY;
	
	int knobXDisp = -10;
	int knobYDisp = -10;
	
	int winWidth;
	int winHeight;
	
	int squareHalfSide;
	int centerCircleX;
	int centerCircleY;
	
	boolean isRecording = false;
	boolean isPlaying = false;
	boolean settingsScreen = false;

	Resources res = getResources();
	pressableButton settingsButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.settings));
	pressableButton backButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.back));
	pressableButton recordButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.record));
	pressableButton stopRecordButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.stoprecording));
	pressableButton connectButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.connect));
	pressableButton unstickButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.unstick));
	pressableButton playButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.playback));
	pressableButton stopPlayButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.stopplayback));
	
	
	String verbose = "Output\nNewLine";
	int swivelViewYCent = 0;

	Path knobPath = new Path();
	
	Paint paint = new Paint();
	
	public DrawingView(Context context) {
		super(context);
	}
	
	//class for buttons
	public class pressableButton{
		int x1;
		int x2;
		int y1;
		int y2;
		Drawable img;
		
		public pressableButton(int left, int top, int right, int bottom, Drawable i){
			x1 = left;
			x2 = right;
			y1 = top;
			y2 = bottom;
			img = i;
		}
		
		public boolean touchButton(float tX, float tY){
			if(tX >= x1 && tX <= x2 && tY >= y1 && tY <= y2)
				return true;
			return false;
		}
		
		public void setBounds(int left, int top, int right, int bottom){
			img.setBounds(left, top, right, bottom);
			x1 = left;
			x2 = right;
			y1 = top;
			y2 = bottom; 
		}
	}
	
	@Override
    public void onDraw(Canvas canvas) {
		long startTime = System.nanoTime();
		
		updateTouch();
		
		//big square
		paint.setColor(Color.BLACK);
		canvas.drawRect(centerCircleX - squareHalfSide, centerCircleY - squareHalfSide, centerCircleX + squareHalfSide, centerCircleY + squareHalfSide, paint);
		paint.setColor(Color.WHITE);
		canvas.drawRect(centerCircleX + 4 - squareHalfSide, centerCircleY + 4 - squareHalfSide, centerCircleX + squareHalfSide - 4, centerCircleY + squareHalfSide - 4, paint);
		//small circle
		paint.setColor(Color.BLACK);
		canvas.drawCircle(centerCircleX, centerCircleY, (float) (squareHalfSide *0.2), paint);
		//knob circle
		paint.setColor(Color.BLACK);
		canvas.drawCircle(centerCircleX +knobXDisp, centerCircleY +knobYDisp, (float) (squareHalfSide *0.15), paint);
		//knob connector line
		double knobAngle = Math.atan2(knobXDisp,-knobYDisp) * (180/Math.PI) + 180;
		knobAngle = Math.toRadians(knobAngle);
		knobPath.reset();
		knobPath.moveTo((float) (centerCircleX +(squareHalfSide *0.2*Math.cos(knobAngle))), (float) (centerCircleY +(squareHalfSide *0.2*Math.sin(knobAngle))));
		knobPath.lineTo((float) (centerCircleX +knobXDisp), centerCircleY +knobYDisp);
		//knobPath.lineTo((float) (centerCircleX+knobXDisp+squareHalfSide*0.15), (centerCircleY)+knobYDisp);
		knobPath.lineTo((float) (centerCircleX +(squareHalfSide *0.2*Math.cos(knobAngle+Math.PI))), (float) (centerCircleY +(squareHalfSide *0.2*Math.sin(knobAngle+Math.PI))));
		canvas.drawPath(knobPath, paint);
		//knob
		paint.setColor(Color.BLUE);
		canvas.drawCircle(centerCircleX +knobXDisp, centerCircleY +knobYDisp, (float) (squareHalfSide *0.2)-4, paint);
		
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(20);
		paint.setColor(Color.WHITE);
		canvas.drawRect(10, 10, winWidth-10, swivelViewYCent+10, paint);
		paint.setColor(Color.GRAY);
		paint.setStrokeWidth(4);
		canvas.drawRect(20, 20, winWidth-20, swivelViewYCent, paint);
		paint.setStyle(Paint.Style.FILL);

		paint.setColor(Color.BLACK);
		paint.setTextSize(32);
		String[] lines = verbose.split("\n");
		for(int i = 0; i < lines.length; i++)
			canvas.drawText(lines[i], 30, swivelViewYCent-10-35*i, paint);

		paint.setColor(Color.RED);
		paint.setTextSize(38);
		if(isRecording)
			canvas.drawText("Recording", 30, swivelViewYCent-10-35*lines.length, paint);
		else if(isPlaying)
			canvas.drawText("Playing Recording", 30, swivelViewYCent-10-35*lines.length, paint);
		else
			canvas.drawText("Ready to play/record", 30, swivelViewYCent-10-35*lines.length, paint);

		//button
		settingsButton.img.draw(canvas);
		if(settingsScreen){
			paint.setARGB(100, 100, 100, 100);
			canvas.drawRect(0, 0, winWidth, winHeight, paint);
			if(isRecording)
				stopRecordButton.img.draw(canvas);
			else
				recordButton.img.draw(canvas);
			if(isPlaying)
				stopPlayButton.img.draw(canvas);
			else
				playButton.img.draw(canvas);
			backButton.img.draw(canvas);
			connectButton.img.draw(canvas);
			unstickButton.img.draw(canvas);

		}
		
		//account for system lag and keep at updating every 20 mill
    	long endTime = System.nanoTime();
    	long duration = (endTime-startTime)/1000000;
    	
	    //System.out.println(duration);
    	
    	if (duration<20){
    		//try {Thread.sleep(20-duration);} catch (InterruptedException e) { }
    	}
    	
    	//call repaint
        invalidate();
	}
	
	public void updateTouch(){
		if(!settingsScreen && !isPlaying){
			if(touchY > winHeight-winWidth){
				if(touchX < squareHalfSide*0.15)
					touchX = (int) (squareHalfSide*0.15);
				if(touchX > winWidth-squareHalfSide*0.15)
					touchX = (int) (winWidth-squareHalfSide*0.15);
				if(touchY < (winHeight-winWidth)+squareHalfSide*0.15)
					touchY = (int) ((winHeight-winWidth)+squareHalfSide*0.15);
				if(touchY > winHeight-squareHalfSide*0.15)
					touchY = (int) (winHeight-squareHalfSide*0.15);
				knobXDisp = touchX - centerCircleX;
				knobYDisp = touchY - centerCircleY;
			}
			else{
				knobXDisp = 0;
				knobYDisp = 0;
			}
//			if(touchX <= winWidth-30 && touchX >= 30 && touchY <= sliderLineY && touchY >= sliderLineY-sliderLineHeight && !isRecording){
//				sliderX = touchX;
//			}
		}
	}

	public void setDim(int width, int height) {
		winWidth = width;
		winHeight = height;
		squareHalfSide = (winWidth - 40)/2;
		centerCircleX = 20+squareHalfSide;
		centerCircleY = winHeight-20-squareHalfSide;
		swivelViewYCent = winHeight-40- squareHalfSide *2;
		int buttonWidth = (int) (winWidth*0.4);
		int buttonHeight = (buttonWidth*2)/5;
		settingsButton.setBounds(10, winHeight-((int) (squareHalfSide *0.2)+10), 10+(int) (squareHalfSide *0.2), winHeight-10);
		backButton.setBounds(winWidth/2-buttonWidth/2, (int) (winHeight-buttonHeight*1.5), winWidth/2+buttonWidth/2, (int) (winHeight-buttonHeight*0.5));
		recordButton.setBounds(winWidth/2-buttonWidth/2, (int) (buttonHeight*0.5), winWidth/2+buttonWidth/2, (int) (buttonHeight*1.5));
		stopRecordButton.setBounds(winWidth/2-buttonWidth/2, (int) (buttonHeight*0.5), winWidth/2+buttonWidth/2, (int) (buttonHeight*1.5));
		connectButton.setBounds(winWidth/2-buttonWidth/2, (int) (buttonHeight*2), winWidth/2+buttonWidth/2, (int) (buttonHeight*3));
		unstickButton.setBounds(winWidth/2-buttonWidth/2, (int) (buttonHeight*3.5), winWidth/2+buttonWidth/2, (int) (buttonHeight*4.5));
		playButton.setBounds(winWidth/2-buttonWidth/2, (int) (buttonHeight*5), winWidth/2+buttonWidth/2, (int) (buttonHeight*6));
		stopPlayButton.setBounds(winWidth/2-buttonWidth/2, (int) (buttonHeight*5), winWidth/2+buttonWidth/2, (int) (buttonHeight*6));
	}
	
	public static double map(double x, double in_min, double in_max, double out_min, double out_max){
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
}
