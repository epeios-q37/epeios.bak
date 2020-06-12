/*
MIT License

Copyright (c) 2017 Claude SIMON (https://q37.info/s/rmnmqd49)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

import info.q37.atlas.*;

class Blank extends Atlas {
	static private String readAsset_( String path )  {
		return readAsset( path, "Blank" );
	}

	@Override
	public void handle(String action, String id ) {
		if ( action.equals( "" ) ) {
			dom.setLayout("",readAsset_( "Main.html") );
			dom.addClass("Input", "hidden");
		} else if ( action.equals( "Submit" ) ) {
			dom.setContent("Pattern", dom.getContent("Pattern").toUpperCase());
		} else if ( action.equals( "HideInput" ) ) {
			dom.addClass("Input", "hidden");
		} else if ( action.equals( "ShowInput" ) ) {
			dom.removeClass("Input", "hidden");
			dom.focus("Pattern");
		} else {
			System.out.println("No or unknown action !");
			System.exit(1);
		}
	}

	public static void main(String[] args) {
		String dir;

		launch(() -> new Blank(), readAsset_("Head.html"), "Blank", GUI.DEFAULT, args);
	}
}
