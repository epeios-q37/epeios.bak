# encoding: UTF-8 
=begin
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
=end

require 'Atlas'

$viewModeElements = ["Pattern", "CreateButton", "DescriptionToggling", "ViewNotes"]

def readAsset(path)
	return Atlas::readAsset(path, "notes")
end

def put(note, id, xml)
	xml.pushTag("Note")
	xml.setAttribute("id", id)

	note.each do |key,value|
		xml.pushTag(key)
		xml.setValue(value)
		xml.popTag()
	end

	xml.popTag()
end

class Notes
	def initialize()
		@pattern=""
		@hideDescription = false
		@index = 0
		@notes = [
			{
				'title' => '',
				'description' => '',
			},
			{
				'title' => 'Improve design',
#				'description' => "Tastes and colors... (aka CSS aren't my cup of tea...)",
				'description' => "Tastes and colors... (aka «CSS aren't my cup of tea...»)",
			},
			{
				'title' => 'Fixing bugs',
				'description' => "There are bugs ? Really ?",
			},
			{
				'title' => 'Implement new functionalities',
				'description' => "Although it's almost perfect..., isn't it ?",
			},
		]
	end

	def handleDescriptions(dom, hide)
		@hideDescription = hide
		if hide
			dom.disableElement("ViewDescriptions")
		else
			dom.enableElement("ViewDescriptions")
		end
	end

	def displayList(dom)
		xml = Atlas.createXML("XDHTML")
		contents = {}

		xml.pushTag("Notes")

		index = 1

		while index < @notes.length()
			Atlas::l
			if @notes[index]['title'][0,@pattern.length()].downcase() == @pattern
				Atlas::l
				put(@notes[index], index, xml)
				Atlas::l
				contents["Description." + index.to_s()] = @notes[index]['description']
				Atlas::l
			end

			index += 1
		end

		Atlas::l
		pp(xml.toString())
		Atlas::l
		dom.setLayoutXSL("Notes", xml, "Notes.xsl")
		Atlas::l
		dom.setContents(contents)
		Atlas::l
		dom.enableElements($viewModeElements)
		Atlas::l
	end
end

def acConnect(notes, dom, id)
	dom.setLayout("", readAsset("Main.html"))
	notes.displayList(dom)
end

def acToggleDescriptions(notes, dom, id)
	notes.handleDescriptions(dom,dom.getContent(id)=="true")
end

callbacks = {
	"Connect" => method(:acConnect),
	"ToggleDescriptions" => method(:acToggleDescriptions),
}

Atlas.launch("Connect",callbacks, -> () {Notes.new()}, readAsset("Head.html"),"notes")
