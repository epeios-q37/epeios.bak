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

import os, sys
from enum import Enum, auto

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import atlastk as Atlas


class State(Enum):
    DISPLAY = auto()
    EDIT = auto()


state = State.DISPLAY

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
        "Note": "Works for the British government."
    },
    {
        "Name": "Tintin",
        "Address": "Château de Moulinsart",
        "Phone": "421",
        "Note": "Has a dog named <span style=\"font-style: italic\">Snowy</span>."
    },
    {
        "Name": "Tournesol, Tryphon (prof.)",
        "Address": "Château de Moulinsart",
        "Phone": "421",
        "Note": "Loves <span style=\"font-style: italic\">Loch Lomond</span> whiskey."
    }
]

current = None

def displayContact(dom, contact):
    global state
    dom.set_contents(contact)

    if (state == State.EDIT):
        dom.enable_element("Contact")
    elif (state == State.DISPLAY):
        dom.disable_element("Contact")
    else:
        raise Exception("Unknown state!")


def displayContacts(dom, contacts):
    #   html="<tr><td>a</td><td>b</td><td>c</td></tr>"
    html = Atlas.createHTML()
    notes = {}

    for i in range(len(contacts)):
        contact = contacts[i]
        html.push_tag("tr")
        html.put_attribute("id", i)
        html.put_attribute("data-xdh-onevent", "Select")
        for key in contact:
            if (key == 'Note'):
                id = "Note." + str(i)
                html.push_tag("td")
                html.put_attribute("id", id)
                notes[id] = contact[key]
            else:
                html.put_tag_and_value("td", contact[key])
        html.pop_tag()

    dom.set_layout("Content", html)

    dom.set_contents(notes)


def display(dom, contacts):
    global current

    if (len(contacts)):
        displayContacts(dom, contacts)
        dom.remove_class("Contacts", "hidden")
    else:
        dom.add_class("Contacts", "hidden")

    if (current):
        displayContact(dom, contacts[current])


def handleButtonVisibility(dom):
    global state, current

    if state == State.DISPLAY:
        dom.add_class("EditionButtons", "hidden")
        dom.disable_element("HideConsultation")
        if current is None:
            dom.enable_element("HideConsultationAndSelection")
        else:
            dom.disable_element("HideConsultationAndSelection")
    elif State == State.EDIT:
        dom.remove_class("EditionButtons", "hidden")
        dom.enable_elements(("HideConsultation", "HideConsultationAndSelection"))
    else:
        raise Exception("Unknown state!")


def acConnect(dom):
    global contacts, state
    dom.set_layout("", open("Main.html").read())
    display(dom, contacts)
    state = State.DISPLAY
    handleButtonVisibility(dom)


def acSelect(dom, id):
    global contacts, state, current

    current = int(id)

    displayContact(dom, contacts[current])
    state = State.DISPLAY
    handleButtonVisibility(dom)

def acEdit(dom):
    global contacts, current

    if current is None:
        dom.alert("No selected item!")
    else:
        contact = contacts[current]

def acSubmit(dom):
    global contacts
    [name, address, phone, note] = dom.get_contents(
        ["Name.edit", "Address.edit", "Phone.edit", "Note.edit"]).values()

    name = name.strip()
    address = address.strip()
    phone = phone.strip()
    note = note.strip()

    if (not name):
        dom.alert("The name can not be empty!")

    contacts.append({
        "Name": name,
        "Address": address,
        "Phone": phone,
        "Note": note
    })

    display(dom, contacts)


callbacks = {
    "": acConnect,
    "Select": acSelect,
    "Edit": acEdit,
    "Submit": acSubmit,
}

Atlas.launch(callbacks, None, open("Head.html").read())
