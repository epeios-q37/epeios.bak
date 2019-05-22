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

import os
import sys
from enum import Enum, auto

sys.path.append("./Atlas.python.zip")
sys.path.append("../Atlas.python.zip")

import atlastk as Atlas


class State(Enum):
    SHOW = auto()
    CREATE = auto()
    EDIT = auto()


state = State.SHOW

# contacts = []
contacts = [
    {
        "Name": "Holmes, Sherlock",
        "Address": "221B Baker Street, Londres",
        "Phone": "(use telegraph)"
    },
    {
        "Name": "Holmes, Mycroft",
        "Address": "Diogenes Club, Pall Mall, Londres",
        "Phone": "(use telegraph)"
    },
    {
        "Name": "Tintin",
        "Address": "Château de Moulinsart",
        "Phone": "421"
    },
    {
        "Name": "Tournesol, Tryphon (prof.)",
        "Address": "Château de Moulinsart",
        "Phone": "421"
    }
]

current = 1


def readAsset(path):
    return Atlas.readAsset(path, "Contacts")


def displayContact(dom, contact):
    global state
    dom.setContents(contact)

    if ((state == State.CREATE) or (state == State.EDIT)):
        dom.enableElement("Contact")
    elif (state == State.SHOW):
        dom.disableElement("Contact")
    else:
        raise Exception("Unknown state!")


def displayContacts(dom, contacts):
    #   html="<tr><td>a</td><td>b</td><td>c</td></tr>"
    html=Atlas.createHTML()

    for i in range(len(contacts)):
        contact=contacts[i]
        html.pushTag("tr")
        html.setAttribute("id", i)
        html.setAttribute("data-xdh-onevent", "Select")
        for key in contact:
            print(key)
            html.setTagAndValue("td", contact[key])
        html.popTag()

    dom.setLayout("Content", html)


def display(dom, contacts):
    global current

    if (len(contacts)):
        displayContacts(dom, contacts)
        dom.removeClass("Contacts", "hidden")
    else:
        dom.addClass("Contacts", "hidden")

    if (current):
        displayContact(dom, contacts[current])


def acConnect(self, dom, id):
    global contacts, state
    dom.setLayout("", readAsset("Main.html"))
    display(dom, contacts)
    state = State.SHOW


def acSelect(self, dom, id):
    global contacts

    displayContact(dom, contacts[int(id)])
    state = State.SHOW


def acSubmit(self, dom, id):
    global contacts
    [name, address, phone]=dom.getContents(
        ["Name", "Address", "Phone"]).values()

    name=name.strip()
    address=address.strip()
    phone=phone.strip()

    if (not name):
        dom.alert("The name can not be empty!")

    contacts.append({
        "Name": name,
        "Address": address,
        "phone": phone
    })

    display(dom, contacts)


callbacks={
    "": acConnect,
    "Select": acSelect,
    "Submit": acSubmit,


}

Atlas.launch(callbacks, lambda: None, readAsset("Head.html"), "Contact")
