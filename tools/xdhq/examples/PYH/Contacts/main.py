"""
MIT License

Copyright (c) 2019 Claude SIMON (https://q37.info/s/rmnmqd49)

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

import os, sys, enum

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import atlastk


class State(enum.Enum):
  DISPLAY = enum.auto()
  EDIT = enum.auto()
  
class Board:
  def __init__(self):
    self.state = State.DISPLAY
    self.recordId = None

# contacts = []
contacts = [
  {
    "Name": "Holmes, Sherlock",
    "Address": "221B Baker Street, Londres",
    "Phone": "(use telegraph)",
    "Note": "Great detective!"
  },
  {
    "Name": "Holmes, Mycroft",
    "Address": "Diogenes Club, Pall Mall, Londres",
    "Phone": "(use telegraph)",
    "Note": "Works for the British government.\nBrother of Holmes, Sherlock"
  },
  {
    "Name": "Tintin",
    "Address": "Château de Moulinsart",
    "Phone": "421",
    "Note": "Has a dog named Snowy."
  },
  {
    "Name": "Tournesol, Tryphon (prof.)",
    "Address": "Château de Moulinsart",
    "Phone": "421",
    "Note": "Loves Loch Lomond whiskey."
  }
]

def display_contact(board, contact, dom):
  dom.set_values(contact)

  if (board.state == State.EDIT):
    dom.enable_element("Contact")
  elif (board.state == State.DISPLAY):
    dom.disable_element("Contact")
  else:
    raise Exception("Unknown state!")


def display_contacts(dom, contacts):
  html = ""

  for recordId in range(len(contacts)):
    contact = contacts[recordId]
    html += f"""
<tr id="{recordId}" data-xdh-onevent="Select">
  <td>{contact["Name"]}</td>
  <td>{contact["Address"]}</td>
  <td>{contact["Phone"]}</td>
  <td>{contact["Note"]}</td>
</td>
"""
  dom.inner("Content", html)


def display(board, contacts, dom):
  if (len(contacts)):
    display_contacts(dom, contacts)
    dom.remove_class("Contacts", "hidden")
  else:
    dom.add_class("Contacts", "hidden")

  if (board.recordId):
    display_ontact(dom, contacts[current])


def handle_button_visibility(board, dom):
  if board.state == State.DISPLAY:
    dom.add_class("EditionButtons", "hidden")
    dom.disable_element("HideConsultation")
    if board.recordId:
      dom.disable_element("HideConsultationAndSelection")
    else:
      dom.enable_element("HideConsultationAndSelection")
  elif board.state == State.EDIT:
    dom.remove_class("EditionButtons", "hidden")
    dom.enable_elements(("HideConsultation", "HideConsultationAndSelection"))
  else:
    raise Exception("Unknown state!")


def ac_connect(board, dom):
  dom.inner("", open("Main.html").read())
  display(board, contacts, dom)
  board.state = State.DISPLAY
  handle_button_visibility(board, dom)

def ac_select(board, dom, id):
  recordId = int(id)

  display_contact(board,contacts[recordId],dom)
  state = State.DISPLAY
  handle_button_visibility(board, dom)

callbacks = {
  "": ac_connect,
  "Select":  ac_select
}

atlastk.launch(callbacks, Board, open("Head.html").read())
