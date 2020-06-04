/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHq.

	XDHq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHq. If not, see <http://www.gnu.org/licenses/>.
*/

package info.q37.xdhq;

public class XDH_FAAS extends XDH_SHRD {
	static public String headContent;
	static public void launch(String headContent, info.q37.xdhq.XDH_SHRD.Callback callback ) {
		XDH_FAAS.headContent = headContent;
		info.q37.xdhq.dom.DOM_FAAS.launch(callback);
	}
	static public void broadcastAction(String action, String id) {
		info.q37.xdhq.dom.DOM_FAAS.broadcastAction(action, id);
	}
}