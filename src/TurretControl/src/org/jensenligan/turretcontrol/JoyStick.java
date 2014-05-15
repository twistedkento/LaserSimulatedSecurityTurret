package org.jensenligan.turretcontrol;

import android.view.View;
import android.view.View.OnTouchListener;
import android.content.Context;
import android.util.AttributeSet;
import android.graphics.Paint;
import android.graphics.Canvas;
import android.graphics.Color;
import android.view.MotionEvent;
import android.util.Log;
import java.net.Socket;
import java.net.InetAddress;
import java.io.*;
import android.os.AsyncTask;
import java.lang.Void;

public class JoyStick extends View implements OnTouchListener, Runnable
{
	private static final int S_LEFT = 4; 
	private static final int S_RIGHT = 8; 
	private static final int S_UP = 1;
	private static final int S_DOWN = 2; 
	private static final int L_UP = 5;
	private static final int R_UP = 9;
	private static final int L_DOWN = 6; 
	private static final int R_DOWN = 10; 
	private float cX = 0;
	private float cY = 0;
	private float height = 0;
	private float width = 0;
	private float fingerX = -500;
	private float fingerY = -500;
	private boolean touching = false;
	private String IP = null;
	private int PORT = 9999;
	public Socket socket;
	public static boolean running = false;
	private AsyncTask<Integer, Void, Void> hej = null;
	public boolean laser = true;

	public JoyStick()
	{
		super(null, null);
	}

	public JoyStick(Context c, AttributeSet as)
	{
		super(c, as);
		this.setOnTouchListener(this);
		connectToPi();
	}

	private void connectToPi()
	{
		IP = getResources().getString(R.string.IP);
		try
		{
			if(socket != null && socket.isConnected())
				socket.close();
			InetAddress addr = InetAddress.getByName(IP);
			socket = new Socket(addr, PORT);
		} catch(Exception e) {
			Log.e("SOCKET", "could not connect to " + IP);
		}
	}

	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
	{
		width = MeasureSpec.getSize(widthMeasureSpec);
		height = MeasureSpec.getSize(heightMeasureSpec);
		cY = height / 2;
		cX = width / 2;
		fingerY = cY;
		fingerX = cX;
		this.setMeasuredDimension((int)width, (int)height);
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
	}

	protected void onDraw(Canvas c)
	{
		super.onDraw(c);
		Paint p = new Paint(Paint.ANTI_ALIAS_FLAG);
		initPaint(c, p);
		if(touching)
			c.drawCircle(fingerX, fingerY, 25f, p);
		else
			c.drawCircle(cX, cY, 25f, p);
	}

	private void initPaint(Canvas c, Paint p)
	{
		p.setColor(Color.GREEN);
		// vertical line
		c.drawRect(0, cY-1, width, cY+1, p);
		c.drawRect(cX-1, 0, cX+1, height, p);
		c.drawRect(cX-1, 0, cX+1, height, p);
		p.setColor(Color.RED);
		c.drawLine(cX-20, 0, cX-20, height, p);
		c.drawLine(cX+20, 0, cX+20, height, p);
		c.drawLine(0, cY-20, width, cY-20, p);
		c.drawLine(0, cY+20, width, cY+20, p);
	}

	public boolean onTouch(View v, MotionEvent me)
	{
		//Toast.makeText(context, "walla", Toast.LENGTH_SHORT).show();
		int action = me.getAction();
		if(action == MotionEvent.ACTION_UP)
			touching = false;
		if(action == MotionEvent.ACTION_DOWN)
			touching = true;
		Log.e("TOUCH", me.getAction() + "");
		fingerX = me.getX();
		fingerY = me.getY();
		this.invalidate();
		this.run();
		return true;
	}

	public void run()
	{
		Log.e("DATTA",fingerX + "");
		int var = 0;
		if(touching)
		{
			// up
			if(fingerY < cY-20)
			{
				if(fingerY > cY-20 && fingerY < cY+20)
					var = S_UP;
				else if(fingerX > cX-20 && fingerX < cX+20)
					var = S_UP;
				else if(fingerX > cX)
					var = R_UP;
				else
					var = L_UP;
			}
			// down
			else if(fingerY > cY+20)
			{
				if(fingerY > cY-20 && fingerY < cY+20)
					var = S_DOWN;
				if(fingerX > cX-20 && fingerX < cX+20)
					var = S_DOWN;
				else if(fingerX > cX+20)
					var = R_DOWN;
				else if(fingerX < cX-20)
					var = L_DOWN;
			}
			else if(fingerY > cY-20 && fingerY < cY+20)
			{
				if(fingerX < cX-20)
					var = S_LEFT;
				else if(fingerX > cX+20)
					var = S_RIGHT;
			}
			if(hej == null)
			{
				hej = new doStuff().execute(new Integer(var));
			} else if(hej.getStatus() == AsyncTask.Status.FINISHED)
			{
				hej = new doStuff().execute(new Integer(var));
			}
		}
	}

	public void toggleLaser()
	{
		laser = !laser;
	}

	private int addLaserBit(int inp)
	{
		int[] bits = toBinaryBits(inp);
		if(bits[4] == 0)
			bits[4] = 1;
		return toInteger(bits);
	}

	private int toInteger(int[] bits)
	{
		int ret = 0;
		int[] b_vals = {128,64,32,16,8,4,2,1};
		for(int i = 0; i < b_vals.length;i++)
		{
			if(bits[i] == 1)
				ret+=b_vals[i];
		}
		return ret;
	}

	private int[] toBinaryBits(int input)
	{
		int[] bits = new int[8];
		int remaining = input;
		int index = 0;
		for(int i = 128; i >= 1;i=i/2)
		{
			if(remaining >= i)
			{
				bits[index] = 1;
				remaining -= i;
			} else
				bits[index] = 0;
			index++;
		}
		return bits;
	}

	private class doStuff extends AsyncTask<Integer, Void, Void>
	{

		protected Void doInBackground(Integer... b)
		{
			if(JoyStick.running) {
				try {
					int byt = b[0].intValue() << 4;
					DataOutputStream out = new DataOutputStream(JoyStick.this.socket.getOutputStream());
					Log.e("DATTA", Integer.toBinaryString(b[0] << 4));
					if(JoyStick.this.laser)
						byt = addLaserBit(byt);
					out.writeByte(byt);
					out.flush();
					//InputStream is = JoyStick.this.socket.getInputStream();
					//byte[] bajs = new byte[1];
					//is.read(bajs, 0, 1);
				} catch(Exception e) {
					connectToPi();
					Log.e("SOCKET", "error sending");
				}
			}
			return null;
		}

		protected void onPreExecute()
		{
			super.onPreExecute();
			if(!JoyStick.running) 
				JoyStick.running = true;
		}

		protected void onPostExecute(Void bajs)
		{
			super.onPostExecute(bajs);
			JoyStick.running = false;
		}
	}
}
