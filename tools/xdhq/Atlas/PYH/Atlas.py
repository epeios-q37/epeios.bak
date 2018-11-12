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

import XDHq
from threading import Thread

readAsset = XDHq.readAsset
XML = XDHq.XML

class DOM(Thread):
	def __init__(this, userObject):
		Thread.__init__(this)
		this._dom = XDHq.DOM()
		this._userObject = userObject

	def run(this):
		while True:
			[action,id] = this._dom.getAction()
			this._userObject.handle(this._dom,action,id)

def launch(newSessionAction, headContent, dir, new):
	XDHq.launch(newSessionAction,headContent,dir)

	while True:
		new().start()






