package org.jensenligan.turretcontrol;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.preference.PreferenceActivity;
import android.widget.Button;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
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
				js.fireLaser();
			}
		});
		// only joystick, no buttons
		//setContentView(new JoyStick(this, null));
    }

	public boolean onCreateOptionsMenu(Menu menu) {
    	MenuInflater inflater = getMenuInflater();
    	inflater.inflate(R.menu.main_activity_actions, menu);
    	return super.onCreateOptionsMenu(menu);
  	}


	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle presses on the action bar items
		switch (item.getItemId()) {
			case R.id.action_settings:
				Intent i = new Intent(Main.this, org.jensenligan.turretcontrol.Preferences.class);
				i.putExtra(PreferenceActivity.EXTRA_NO_HEADERS, true);
				startActivity(i);
				return true;
			default:
				return super.onOptionsItemSelected(item);
		}
	}
}
