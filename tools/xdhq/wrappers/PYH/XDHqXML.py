"""
MIT License

Copyright (c) 2018 Claude SIMON (https://q37.info/s/rmnmqd49)

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

class XML:
	def _write(self,value):
		self._xml += str(value) + "\0"

	def __init__(self,rootTag):
		self._xml = ""
		self._write("dummy")
		self._write(rootTag)

	def pushTag(self,tag):
		self._xml += ">"
		self._write(tag)

	def popTag(self):
		self._xml += "<"

	def setAttribute(self,name,value):
		self._xml += "A"
		self._write(name)
		self._write(str(value))

	def setValue(self,value):
		self._xml += "V"
		self._write(str(value))

	def setTagAndValue(self,tag,value):
		self.pushTag(tag)
		self.setValue(value)
		self.popTag()		

	def toString(self):
		return self._xml
