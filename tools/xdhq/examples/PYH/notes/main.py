# -*- coding: utf-8 -*-
""" 
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
 """

import Atlas

import os

_viewModeElements = ["Pattern", "CreateButton", "DescriptionToggling", "ViewNotes"]

def _readAsset(path):
	return Atlas.readAsset(path, "notes")

def _put(note, id, xml ):
	xml.pushTag("Note")
	xml.setAttribute("id", id)

	for key in note:
		xml.pushTag(key)
		xml.setValue(note[key])
		xml.popTag()

	xml.popTag()

class Notes(Atlas.DOM):
	def __init__(this):
		Atlas.DOM.__init__(this,this)
		this._pattern = ""
		this._hideDescriptions = False
		this._index = 0
		this._notes = [
			{
			'title': '',
			'description': '',
			},
			{
			'title': 'Improve design',
			'description': "Tastes and colors... (aka «CSS aren't my cup of tea...»)",
			},
			{
			'title': 'Fixing bugs',
			'description': "There are bugs ? Really ?",
			},
			{
			'title': 'Implement new functionalities',
			'description': "Although it's almost perfect..., isn't it ?",
			},
		]

	def _handleDescriptions(this,dom):
		if this._hideDescriptions:
			dom.disableElement("ViewDescriptions")
		else:
			dom.enableElement("ViewDescriptions")

	def _displayList(this,dom):
		xml = Atlas.XML("XDHTML")
		i = 1 # 0 skipped, as it serves as buffer for the new ones.
		contents = {}

		xml.pushTag("Notes")

		count = len(this._notes)

		while i < count:
			if this._notes[i]['title'][:len(this._pattern)].lower() == this._pattern:
				_put(this._notes[i], i, xml)
				contents["Description." + str(i)] = this._notes[i]['description']
			i += 1

		dom.setLayoutXSL("Notes", xml, "Notes.xsl")
		dom.setContents(contents)
		dom.enableElements(_viewModeElements)

	def _view(this,dom ):
		dom.enableElements(_viewModeElements)
		dom.setContent("Edit." + str(this._index), "")
		this._index = -1

	def _edit(this,dom,id):
		this._index = int(id)
		note = this._notes[this._index]

		dom.setLayout("Edit." + id, _readAsset( "Note.html") )
		dom.setContents({ "Title": note['title'], "Description": note['description'] })
		dom.disableElements(_viewModeElements)
		dom.dressWidgets("Notes")
		dom.focus("Title")

	def _submit(this,dom):
		result = dom.getContents(["Title", "Description"])
		title = result["Title"].strip()
		description = result["Description"]

		if title:
			this._notes[this._index] = { "title": title, "description": description }

			if this._index == 0:
				this._notes.insert(0, { 'title': '', 'description': ''})
				this._displayList( dom )
			else:
				dom.setContents( { "Title." + str(this._index): title, "Description." + str(this._index): description })
				this._view( dom )
		else:
			dom.alert("Title can not be empty !")
			dom.focus("Title")

	def _cancel(this,dom):
		note = this._notes[this._index]

		result = dom.getContents(["Title", "Description"])
		title = result["Title"].strip()
		description = result["Description"]

		if (title != note['title']) or (description != note['description']):
			if dom.confirm("Are you sure you want to cancel your modifications ?"):
				this._view( dom )
		else:
			this._view( dom )
		
	def handle(this,dom,action,id):
		if action == "Connect":
			dom.setLayout("", _readAsset( "Main.html") )
			this._displayList(dom)
		elif action == "ToggleDescriptions":
			this._hideDescriptions = dom.getContent(id)=="true"
			this._handleDescriptions(dom)
		elif action == "Search":
			this._pattern = dom.getContent("Pattern").lower()
			this._displayList(dom)
		elif action == "Edit":
			this._edit(dom, dom.getContent(id))
		elif action == "Delete":
			if dom.confirm("Are you sure you want to delete this entry ?"):
				this._notes.pop(int(dom.getContent(id)))
				this._displayList(dom)
		elif action == "Submit":
			this._submit(dom)
		elif action == "Cancel":
			this._cancel(dom)
		else:
			print("???")
			os._exit(1)

def new():
	return Notes()

Atlas.launch("Connect", _readAsset("Head.html"), "notes", new)
