package org.jensenligan.turretcontrol;


import android.preference.PreferenceActivity;
import android.os.Bundle;

public class Preferences extends PreferenceActivity
{
	@Override
	public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
		addPreferencesFromResource(R.xml.preferences);
	}


	/*public static class PrefFragment extends PreferenceFragment {
        @Override
        public void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);

            // Can retrieve arguments from preference XML.
            Log.i("args", "Arguments: " + getArguments());

            // Load the preferences from an XML resource
            addPreferencesFromResource(R.xml.preferences);
        }
    }*/
}
