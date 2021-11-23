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

package info.q37.xdhq.dom;

import info.q37.xdhq.XDH_SLFH;
import info.q37.xdhq.dom.DOM_SHRD;

public class DOM_SLFH extends DOM_SHRD {
	private Object core;

	public DOM_SLFH() {
		core = XDH_SLFH.call(2);
	}

	public void finalize() {
		XDH_SLFH.call(3, core);
	}

	@Override public void getAction(Event event) {
		XDH_SLFH.call(4, core, event);
	}

	@Override public Object call(String command, Type type, info.q37.xdhq.ARG ...args )  {
		return XDH_SLFH.call( 5, core, command, type.getValue(), args );
	}
}
