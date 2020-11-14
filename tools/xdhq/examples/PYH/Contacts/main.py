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
    self.contactId = None

EMPTY_CONTACT = {
    "Name": "",
    "Address": "",
    "Phone": "",
    "Note": ""
}

EXAMPLE = [
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
    "Note": "Works for the British government.\nBrother of Holmes, Sherlock."
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

fields = []

# contacts = []
contacts = EXAMPLE


def display_contact(board, contact, dom):
  dom.set_values(contact)


def display_contacts(dom, contacts):
  html = ""

  for contactId in range(len(contacts)):
    contact = contacts[contactId]
    html += f"""
<tr id="{contactId}" data-xdh-onevent="Select" style="cursor: pointer;">
  <td>{contact["Name"]}</td>
  <td>{contact["Address"]}</td>
  <td>{contact["Phone"]}</td>
  <td>{contact["Note"]}</td>
</td>
"""

  dom.inner("Content", html)


def handle_outfit(board, dom):
  if board.state == State.DISPLAY:
    dom.disable_element("HideDisplay")
    dom.enable_element("HideEdition")
    dom.disable_elements(fields)
    if board.contactId != None:
      dom.disable_element("HideDisplayAndSelect")
    else:
      dom.enable_element("HideDisplayAndSelect")
  elif board.state == State.EDIT:
    dom.enable_elements(("HideDisplay", "HideDisplayAndSelect"))
    dom.disable_element("HideEdition")
    dom.enable_elements(fields)
  else:
    raise Exception("Unknown state!")


def ac_connect(board, dom):
  dom.inner("", open("Main.html").read())
  display_contacts(dom, contacts)
  board.state = State.DISPLAY
  handle_outfit(board, dom)


def ac_refresh(board,dom):
  display_contacts(dom,contacts)


def ac_select(board, dom, id):
  contactId = int(id)

  display_contact(board,contacts[contactId],dom)
  board.state = State.DISPLAY
  board.contactId = contactId

  handle_outfit(board, dom)


def ac_delete(board, dom):
  if board.contactId == None:
    raise Exception("No contact selected!")

  contacts.pop(board.contactId)
  board.contactId = None;

  display_contact(board, EMPTY_CONTACT, dom)

  handle_outfit(board,dom)

  atlastk.broadcast_action("Refresh")


def ac_new(board,dom):
  board.contactId = None

  board.state = State.EDIT

  display_contact(board, EMPTY_CONTACT, dom)

  handle_outfit(board,dom)


def ac_edit(board,dom):
  if board.contactId == None:
    raise Exception("No contact selected!")  

  board.state = State.EDIT

  handle_outfit(board,dom)

  dom.focus("Name")


def ac_submit(board,dom):
  idsAndValues = dom.get_values(fields)

  if not idsAndValues['Name'].strip():
    dom.alert("The name field can not be empty!")
    return

  if board.contactId == None or board.contactId >= len(contacts):
    contacts.append(idsAndValues)
  else:
    contacts[board.contactId] = idsAndValues

  atlastk.broadcast_action("Refresh")

  board.state = State.DISPLAY

  handle_outfit(board,dom)


def ac_cancel(board,dom):
  if not dom.confirm("Are you sure?"):
    return

  if board.contactId != None:
    display_contact(board,contacts[board.contactId],dom)
  else:
    display_contact(board, EMPTY_CONTACT, dom)

  board.state = State.DISPLAY

  handle_outfit(board,dom)


callbacks = {
  "": ac_connect,
  "Refresh": ac_refresh,
  "Select": ac_select,
  "Delete": ac_delete,
  "New": ac_new,
  "Edit": ac_edit,
  "Submit": ac_submit,
  "Cancel": ac_cancel
}

for key in EMPTY_CONTACT.keys():
  fields.append(key)

atlastk.launch(callbacks, Board, open("Head.html").read())
