package org.jensenligan.turretcontrol;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Button;
import android.view.View;
import android.view.View.OnClickListener;

public class Main extends Activity
{

	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.joystickview);
		Button laser = (Button) findViewById(R.id.laserBtn);
		//Button stream = (Button) findViewById(R.id.streamingBtn);
		laser.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				JoyStick js = (JoyStick) findViewById(R.id.joystickview);
				js.toggleLaser();
			}
		});
		// only joystick, no buttons
		//setContentView(new JoyStick(this, null));
    }
}
