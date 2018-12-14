
/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

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
	along with XDHq If not, see <http://www.gnu.org/licenses/>.
*/

import info.q37.atlas.*;
import java.util.*;

class Message {
	public String pseudo;
	public String content;

	Message(String pseudo, String content) {
		this.pseudo = pseudo;
		this.content = content;
	}
}

class Shared {
	public static List<Message> messages = new ArrayList<Message>();
	public static List<String> pseudos = new ArrayList<String>();
}

class Chatroom extends Atlas {
	private String pseudo ="";
	private int lastMessage;

	static private String readAsset_(String path) {
		String dir;

		if (System.getenv("EPEIOS_SRC") == null)
			dir = ".";
		else
			dir = "chatroom";

		return readAsset( path, dir );
	}

	private XML buildXML_() {
		XML xml = Atlas.createXML("XDHTML");
		xml.pushTag( "Messages" );
		xml.setAttribute( "pseudo", pseudo );

		synchronized (Shared.messages) {
			ListIterator<Message> li = Shared.messages.listIterator(Shared.messages.size());

			int i = li.previousIndex();

			while (i >= this.lastMessage) {
				Message message = li.previous();

				xml.pushTag( "Message" );
				xml.setAttribute( "id", i );
				xml.setAttribute( "pseudo", message.pseudo );
				xml.setValue( message.content );
				xml.popTag();

				i = li.previousIndex();
			}

			this.lastMessage = Shared.messages.size();
		}

		xml.popTag();

		return xml;
	}

	private void displayMessages_(DOM dom) {
		if ((Shared.messages.size() - 1) >= this.lastMessage) {
			String id = dom.createElement("span");
			dom.setLayoutXSL(id, buildXML_(), "Messages.xsl");
			dom.insertChild(id, "Board");
		}
	}

	private void connect_(DOM dom, String id) {
		dom.setLayout("", readAsset_("Main.html"));
		dom.focus("Pseudo");
		dom.setTimeout(1000, "Update");
		displayMessages_(dom);
	}

	private boolean handlePseudo_(String pseudo) {
		synchronized (Shared.pseudos) {
			if (Shared.pseudos.contains(pseudo))
				return false;
			else {
				Shared.pseudos.add(pseudo);
				return true;
			}
		}
	}

	private void submitPseudo_(DOM dom, String id) {
		String pseudo = dom.getContent("Pseudo").trim();

		if ("".equals(pseudo)) {
			dom.alert("Pseudo. can not be empty !");
			dom.setContent("Pseudo", "");
			dom.focus("Pseudo");
		} else if (handlePseudo_(pseudo.toUpperCase())) {
			this.pseudo = pseudo;
			dom.addClass("PseudoButton", "hidden");
			dom.disableElements(new String[] { "Pseudo", "PseudoButton" });
			dom.enableElements(new String[] { "Message", "MessageButton" });
			dom.setContent("Pseudo", pseudo);
			dom.focus("Message");
			System.out.println("\t>>>> New user: " + pseudo);
		} else {
			dom.alert("Pseudo. not available !");
			dom.setContent("Pseudo", pseudo);
			dom.focus("Pseudo");
		}
	}

	private void addMessage_(String pseudo, String message) {
		message = message.trim();

		if (!"".equals(message)) {
			System.out.println("'" + pseudo + "': " + message);
			synchronized (Shared.messages) {
				Shared.messages.add(new Message(pseudo, message));
			}
		}
	}

	private void submitMessage_(DOM dom, String id) {
		String message = dom.getContent("Message");
		dom.setContent("Message", "");
		dom.focus("Message");
		addMessage_(pseudo, message);
		displayMessages_(dom);
	}

	private void update_(DOM dom, String id) {
		displayMessages_(dom);
		dom.setTimeout(1000, "Update");
	}

	@Override
	public void handle(String action, String id) {
		switch (action) {
		case "":
			connect_(dom, id);
			break;
		case "SubmitPseudo":
			submitPseudo_(dom, id);
			break;
		case "SubmitMessage":
			submitMessage_(dom, id);
			break;
		case "Update":
			update_(dom, id);
			break;
		default:
			throw new RuntimeException("Unknown action '" + action + "' !!!");
		}
	}

	public static void main(String[] args) throws Exception {
		launch(readAsset_("Head.html"), "chatroom", GUI.DEFAULT, args);

		for (;;)
			new Chatroom();
	}
}