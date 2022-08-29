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

/*
	This is the one-file version, which does not need the 'Head.html' and the 'Main.html' files.
*/

import info.q37.atlas.*;

class Accumulator extends Atlas {
	private static String BODY =
	"<span style=\"margin: 5px; color:white; background-color: grey; padding: 5px;\">ATK Accumulator</span>" +
	"<fieldset style=\"display: grid; gap: 10px; background-color: lightgrey;\">" +
	"  <span style=\"margin: auto 0;\">Enter an integer:</span>" +
	"  <input style=\"grid-column: 2;\" id=\"Input\" xdh:onevent=\"Action\" size=\"10\"/>" +
	"  <span style=\"grid-row: 2; margin: auto 0;\" >The Accumulated sum is:</span>" +
	"  <input id=\"Output\" disabled=\"disabled\" style=\"font-weight: bolder;\" size=\"10\"/>" +
	"</fieldset>";
	private int sum = 0; 
	
	@Override
	public void handle(String action, String id )
	{
		switch( action) {
		case "":
			dom.inner("", BODY);
			dom.focus("Input");
			break;
		case "Action":
			int numberIn = Integer.parseInt(dom.getValue("Input"));
			sum += numberIn;      // Accumulate numbers entered into sum
			dom.setValue("Input", "");  // Clear input TextField
			dom.setValue("Output", sum + "");
			break;
		default:
			throw new RuntimeException( "Unknown action '" + action + "' !!!");
		}

		dom.focus( "input");
	}

	public static void main(String[] args) {
		launch(() -> new Accumulator(), "<title>Accumulator</title>");
	}
}