/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'xdhdvkq'.

    'xdhbrwq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhbrwq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhbrwq'.  If not, see <http://www.gnu.org/licenses/>.
*/

package info.q37.xdhdvkq;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.webkit.JavascriptInterface;
import android.webkit.ValueCallback;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("xdhdvkq");
    }

    private native void initialize();

    private native void launchEvent( String Digest );

    private WebView webView;

    class Callback implements ValueCallback<String> {
        @Override
        public native void onReceiveValue( String Value );
    }

    class LaunchEventRunnable implements Runnable
    {
        private String Digest_;

        public LaunchEventRunnable( String Digest )
        {
               Digest_ = Digest;
        }

        @Override
        public void run() {
            launchEvent( Digest_ );
        }
    }

    public Callback callback;

    class JsObject {
        private MainActivity mainActivity_;
        public JsObject( MainActivity mainActivity )
        {
            mainActivity_ = mainActivity;
        }
        @JavascriptInterface
        public void launchEvent( String Digest )
        {
           mainActivity_.runOnUiThread( new LaunchEventRunnable( Digest ) );
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initialize();

        callback = new Callback();

        webView = (WebView) findViewById(R.id.activity_main_webview);

        webView.setWebChromeClient(new WebChromeClient());

        webView.addJavascriptInterface(new JsObject(this), "xdhdvkq");

        // Enable Javascript
        WebSettings webSettings = webView.getSettings();
        webSettings.setJavaScriptEnabled(true);
        webSettings.setDomStorageEnabled(true);

        webView.loadUrl( "file:///android_asset/WebView/xdhdvkq.html" );

        Log.println( Log.ASSERT, "XDHDVK", "6" );
    }
}
