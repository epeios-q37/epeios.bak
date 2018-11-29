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

module Atlas
	require 'XDHq'

	class DOM < XDHq::DOM
	end

	def Atlas::thread(dom,callbacks)

		while true
			action, id = dom.getAction()

			callbacks[action].call(dom,id)
		end
	end


	def Atlas::launch(newSessionAction,callbacks,headContent,dir)
		XDHq.launch(newSessionAction,headContent,dir)

		while true
			Thread.new(Atlas::DOM.new(),callbacks) do |dom,callbacks| thread(dom, callbacks) end
		end
	end
end