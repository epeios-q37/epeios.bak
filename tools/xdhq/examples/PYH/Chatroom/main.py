"""
MIT License

Copyright (c) 2018 Claude SIMON (https://q37.info/s/rmnmqd49)

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
"""

import os, sys, threading

sys.path.append("./atlastk")
sys.path.append("../atlastk")

import atlastk as Atlas

messages = []
pseudos = []
lock = threading.Lock()

def read_asset(path):
	return Atlas.read_asset(path, "Chatroom")

class Chatroom:
	def __init__(self):
		self.last_message = 0
		self.pseudo = ""

	def build_xml(self):
		xml = Atlas.create_XML("XDHTML")
		xml.push_tag( "Messages" )
		xml.put_attribute( "pseudo", self.pseudo )

		global messages, pseudos, lock

		lock.acquire()

		index = len( messages ) - 1

		while index >= self.last_message:
			message = messages[index]

			xml.push_tag( "Message" )
			xml.put_attribute( "id", index )
			xml.put_attribute( "pseudo", message['pseudo'] )
			xml.put_value( message['content'] )
			xml.pop_tag()

			index -= 1

		self.last_message = len(messages)

		lock.release()

		xml.pop_tag()

		return xml

	def display_messages(self, dom):
		global messages
		
		if len(messages) > self.last_message:
			dom.prepend_layout_XSL("Board", self.build_xml(), "Messages.xsl")

	def handle_pseudo(self, pseudo):
		global pseudos, lock

		lock.acquire()

		if pseudo in pseudos:
			result = False
		else:
			pseudos.append(pseudo)
			result= True

		lock.release()

		return result

	def add_message(self, pseudo, message):
		global messages, lock
		message = message.strip()

		if message:
			print("'" + pseudo + "': " + message)
			lock.acquire()
			messages.append({'pseudo': pseudo, 'content': message})
			lock.release()

def ac_connect(chatroom, dom):
	dom.set_layout("", read_asset("Main.html"))
	dom.focus("Pseudo")
	chatroom.display_messages(dom)
	
def ac_submit_pseudo(chatroom, dom):
	pseudo = dom.get_content("Pseudo").strip()

	if not pseudo:
		dom.alert("Pseudo. can not be empty !")
		dom.set_content("Pseudo", "")
		dom.focus("Pseudo")
	elif chatroom.handle_pseudo(pseudo.upper()):
		chatroom.pseudo = pseudo
		dom.add_class("PseudoButton", "hidden")
#		dom.disable_elements(["Pseudo", "PseudoButton"])
		dom.disable_element("Pseudo")
		dom.enable_elements(["Message", "MessageButton"])
#		dom.set_content("Pseudo", pseudo)
		dom.focus("Message")
		print("\t>>>> New user: " + pseudo)
	else:
		dom.alert("Pseudo. not available!")
		dom.set_content("Pseudo", pseudo)
		dom.focus("Pseudo")

def ac_submit_message(chatroom, dom):
	message = dom.get_content("Message")
	dom.set_content("Message", "")
	dom.focus("Message")
	chatroom.add_message(chatroom.pseudo, message)
	chatroom.display_messages(dom)
	Atlas.broadcast_action("Update")

callbacks = {
		"": ac_connect,
		"SubmitPseudo": ac_submit_pseudo,
		"SubmitMessage": ac_submit_message,
		"Update": lambda chatroom, dom: chatroom.display_messages(dom),
	}
		
Atlas.launch(callbacks, Chatroom, read_asset("Head.html"), "Chatroom")
