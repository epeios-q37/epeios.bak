package org.zeusw.esketch;

public class MainActivity extends org.zeusw.dalvik.EpeiosActivity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate( android.os.Bundle savedInstanceState )
    {
		super.loadAndLaunch( "esketch", savedInstanceState );
    }
}