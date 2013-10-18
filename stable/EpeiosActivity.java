package org.zeusw.dalvik;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.view.View;
import android.widget.AdapterView;

class EpeiosListener {
	protected Activity activity;
	protected long callback;
	public EpeiosListener(
		Activity activity,
		long callback )
	{
		this.activity = activity;
		this.callback = callback;
	}
}


class EpeiosOnClickListener extends EpeiosListener implements View.OnClickListener {
	@Override
	public native void onClick(View view);
	public EpeiosOnClickListener(
		Activity activity,
		long callback )
	{
		super( activity, callback );
	}
}

class EpeiosOnItemSelectedListener extends EpeiosListener implements AdapterView.OnItemSelectedListener {
	@Override
	public native void onItemSelected(AdapterView<?> parent, View view, int position, long id);
	public native void onNothingSelected(AdapterView<?> parent);
	public EpeiosOnItemSelectedListener(
		Activity activity,
		long callback )
	{
		super( activity, callback );
	}
}

public class EpeiosActivity extends Activity
{
	private long steering;
	private native void launch(
		String targetName,
		Bundle bundle );
	
    public void loadAndLaunch(
		String targetName,
		Bundle savedInstanceState )
    {
		System.loadLibrary( targetName + "dvk" );
		launch( targetName + "dvk", savedInstanceState );
    }
}