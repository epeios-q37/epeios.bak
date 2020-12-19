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

import os, sys, threading, uuid

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import atlastk

rooms = {}

def get_rooms():
  xml = atlastk.create_XML("Rooms")

  for room in rooms:
    xml.push_tag("Room")
    xml.put_attribute("id", room)
    xml.put_attribute("URL", atlastk.get_app_url(room))
    xml.put_value(rooms[room])
    xml.pop_tag()

  return xml

def display_rooms(dom):
  dom.inner("Rooms",get_rooms(), "Rooms.xsl")

def ac_connect(dom, id):
  if id:
    dom.inner("",open("Room.html").read())
    dom.set_content("Name", rooms[id])
  else:
    dom.inner("",open("Admin.html").read())
    dom.focus("Name")
    display_rooms(dom)

def ac_create(dom):
  global rooms

  room = dom.get_content("Name").strip()

  if not room:
    dom.alert(f"A room name can not be empty!")
    dom.focus("Name")
    return  

  if room in rooms.values():
    dom.alert(f"There is already a room named '{room}'")
    dom.focus("Name")
    return

  id = str(uuid.uuid4())
  url = atlastk.get_app_url(id)

  rooms[id]=room

  display_rooms(dom)

  # dom.end("Rooms",f'<fieldset style="margin: auto; width: min-content;"><legend>{room}</legend><a href="{url}" name="{id}" id="{id}"/></fieldset>')
  # dom.end("Rooms",f'<fieldset style="margin: auto; width: min-content;"><legend>{room}</legend><a href="{url}" name="{id}" id="{id}"><img src="https://api.qrserver.com/v1/create-qr-code/?size=150x150&data={url}"/></a></fieldset>')
  
  dom.set_content("Name", "")
  dom.focus("Name")

def ac_qrcode(dom,id):
  mark = dom.get_mark(id)

  if mark:
    url = atlastk.get_app_url(mark)
    dom.inner(dom.last_child(id), f'<a href="{url}" target="_blank"><img src="https://api.qrserver.com/v1/create-qr-code/?size=150x150&data={url}"/></a>')
    dom.set_mark(id,"")

CALLBACKS = {
  "": ac_connect,
  "Create": ac_create,
  "QRCode": ac_qrcode
}
    
atlastk.launch(CALLBACKS, None, open("Head.html").read())
