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

import threading

import atlastk

class Room:
  def __init__(self):
    self.messages = []
    self.pseudos = []
    self.lock = threading.Lock()

  def build_xml(self,session):
    xml = atlastk.create_XML("XDHTML")
    xml.push_tag("Messages")
    xml.put_attribute("pseudo",session.pseudo)

    self.lock.acquire()

    index = len(self.messages) - 1

    while index >= session.last_message:
      message = self.messages[index]

      xml.push_tag( "Message" )
      xml.put_attribute( "id", index )
      xml.put_attribute( "pseudo", message['pseudo'] )
      xml.put_value( message['content'] )
      xml.pop_tag()

      index -= 1

    session.last_message = len(self.messages)

    self.lock.release()

    xml.pop_tag()

    return xml

  def display_messages(self,session,dom):
   
    if len(self.messages) > session.last_message:
      dom.begin("Board", self.build_xml(session), "Messages.xsl")
      
      
  def handle_pseudo(self,pseudo):
    self.lock.acquire()

    if pseudo in self.pseudos:
      result = False
    else:
      self.pseudos.append(pseudo)
      result= True

    self.lock.release()

    return result

  def add_message(self,pseudo,message):
    message = message.strip()

    if message:
      print("'" + pseudo + "': " + message)
      self.lock.acquire()
      self.messages.append({'pseudo': pseudo, 'content': message})
      self.lock.release()    