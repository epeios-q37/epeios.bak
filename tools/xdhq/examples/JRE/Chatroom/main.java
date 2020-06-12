
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

	static private String readAsset_( String path )  {
		return readAsset( path, "Chatroom" );
	}

	private XML buildXML_() {
		XML xml = Atlas.createXML("XDHTML");
		xml.pushTag( "Messages" );
		xml.putAttribute( "pseudo", pseudo );

		synchronized (Shared.messages) {
			ListIterator<Message> li = Shared.messages.listIterator(Shared.messages.size());

			int i = li.previousIndex();

			while (i >= this.lastMessage) {
				Message message = li.previous();

				xml.pushTag( "Message" );
				xml.putAttribute( "id", i );
				xml.putAttribute( "pseudo", message.pseudo );
				xml.putValue( message.content );
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
			dom.prependLayoutXSL("Board", buildXML_(), "Messages.xsl");
		}
	}

	private void connect_(DOM dom, String id) {
		dom.setLayout("", readAsset_("Main.html"));
		dom.focus("Pseudo");
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
		broadcastAction("Update");
	}

	private void update_(DOM dom, String id) {
		displayMessages_(dom);
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

	public static void main(String[] args) {
		launch(() -> new Chatroom(),readAsset_("Head.html"), "Chatroom", GUI.DEFAULT, args);
	}
}
