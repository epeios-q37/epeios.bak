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

module XDHq
	require 'XDHqDEMO'

	$VOID=XDHqSHRD::VOID
	$STRING=XDHqSHRD::STRING
	$STRINGS=XDHqSHRD::STRINGS

	def XDHq::split(keysAndValues)
		keys = []
		values = []

		keysAndValues.each do |key, value|
			keys.push(key)
			values.push(value)
		end

		pp(keysAndValues, keys, values)

		return keys, values
	end

	def XDHq::unsplit(keys, values)
		i = 0
		keysAndValues = {}
		length = keys.length()

		while i < length
			keysAndValues[keys[i]] = values[i]
			i += 1
		end

		return keysAndValues
	end

	class DOM
		def initialize()
			@dom = XDHqDEMO::DOM.new()
		end

		def unsplit(*args)
			return XDHq.unsplit(*args)
		end

		def split(*args)
			return XDHq.split(*args)
		end

		def call(command, type, *args)
			return @dom.call(command,type,*args)
		end

		def getAction()
			return @dom.getAction()
		end

		def setLayout(id, xml, xslFilename = "")
			call("SetLayout_1", $VOID, 3, id, xml, xslFilename, 0)
		end

		def confirm?(message)
			return call("Confirm_1", $STRING, 1, message, 0) == "true"
		end

		def getContents(ids)
			return unsplit(ids, call("GetContents_1", $STRINGS, 0, 1, ids))
		end

		def getContent(id)
			return getContents([id])[id]
		end

		def setContents(idsAndContents)
			ids, contents = split(idsAndContents)
			pp(idsAndContents, ids, contents)
			call("SetContents_1", $VOID, 0, 2, ids, contents)
		end

		def setContent(id, content)
			setContents({id => content})
		end

		def focus(id)
			call("Focus_1", $VOID, 1, id, 0)
		end
	end

	def XDHq::launch(newSessionAction, headContent, dir)
		XDHqDEMO.launch(newSessionAction, headContent)
	end

end