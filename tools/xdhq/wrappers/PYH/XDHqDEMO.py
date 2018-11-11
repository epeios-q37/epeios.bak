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

import XDHqSHRD

import os
import socket

_protocolLabel = "712a58bf-2c9a-47b2-ba5e-d359a99966de"
_protocolVersion = "0"

_newSessionAction = ""
_headContent = ""

def launch(newSessionAction,headContent):
	global _newSessionAction,_headContent

	_newSessionAction = newSessionAction
	_headContent = headContent


class XDHqDOM_DEMO:
	_firstLaunch = True
	_token = ""

	def _isTokenEmpty(this):
		return not this._token or this._token[0] == "&"

	def _writeSize(this, size):
		result = chr(size & 0x7f)
		size >>= 7

		while size != 0:
			result = chr((size & 0x7f) | 0x80) + result
			size >>= 7

		this._socket.send(result)

	def _writeString(this, string):
		this._writeSize(len(string))
		this._socket.send(string)

	def _writeStrings(this, strings):
		this._writeSize(len(strings))

		for string in strings:
			this._writeString(string)

	def _getByte(this):
		return ord(this._socket.recv(1))

	def _getSize(this):
		byte = this._getByte()
		size = byte & 0x7f

		while byte & 0x80:
			byte = this._getByte()
			size = (size << 7) + byte & 0x7f

		return size

	def _getString(this):
		size = this._getSize()

		if size:
			return this._socket.recv(size)
		else:
			return ""

	def _getStrings(this):
		amount = this._getSize()
		strings = []

		while amount:
			strings.append(this._getString())
			amount -= 1

		return strings


	def _getQuery(this):
		c = this._socket.rec(1)
		query = ""


	def __init__(this):
		global _protocolLabel, _protocolVersion, _newSessionAction,_headContent
		address = "atlastk.org"
		httpPort = ""
		cgi = "xdh"
		port = 53800
		if "ATK" in os.environ:
			atk = os.environ["ATK"]
			if atk == "DEV":
				address = "localhost"
				httpPort = ":8080"
				print("\tDEV mode !")
			elif atk == "TEST":
				cgi = "xdh_"
				print("\tTEST mode!")
			else:
				sys.exit("Bad 'ATK' environment variable value : should be 'DEV' or 'TEST' !")

		if this._isTokenEmpty():
			if "ATK_TOKEN" in os.environ:
				token = os.environ["ATK_TOKEN"].strip()

				if token:
					this._token = "&" + token
		
		this._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		this._socket.connect((address,port))

		print("\tAv.",this._token)
		this._writeString(this._token)

		if this._isTokenEmpty():
			this._writeString(_headContent)

			this._token = this._getString()
			print("\tAp.",this._token)

			if this._isTokenEmpty():
				sys.exit("Invalid connection information !!!")

			url = "http://" + address + httpPort + "/" + cgi + ".php?_token=" + this._token

			print(url)
			print("Open above URL in a web browser. Enjoy!\n")
			XDHqSHRD.open(url)
		else:
			if this._getString() != this_.token:
				sys.exit("Unmatched token !!!")

		this._getString()	# Language.
		this._writeString(_protocolLabel)
		this._writeString(_protocolVersion)

	def getAction(this):
		if this._firstLaunch:
			this._firstLaunch = False
		else:
			this._socket.send("StandBy_1\0")

		id = this._getString();
		action = this._getString()

		if not action:
			action = _newSessionAction

		return [action,id]

	def call(this, command, type, *args):
		i=0
		this._socket.send( command + "\0" )

		amount = args[i]
		i += 1

		while amount:
			this._writeString(args[i])
			i += 1
			amount -= 1

		amount = args[i]
		i += 1

		while amount:
			this._writeStrings(args[i])
			i += 1
			amount -= 1

		if type == XDHqSHRD.RT_STRING:
			return this._getString();
		elif type == XDHqSHRD.RT_STRINGS:
			return this._getStrings()
		elif type != XDHqSHRD.RT_VOID:
			sys.exit("Unknown return type !!!")

		
