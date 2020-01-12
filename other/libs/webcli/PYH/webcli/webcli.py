# coding: utf-8
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

import os, sys, threading, time, html

import atlastk as Atlas

head = """
<title>builtins overload</title>
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
<style type="text/css">
 html, body {height: 100%; padding: 0; margin: 0; width: 100%;}
 .vcenter-out, .hcenter { display: table; height: 100%; margin: auto;}
 .vcenter-in {display: table-cell; vertical-align: middle;}
 .hidden {display: none;}
</style>
"""

body = """
<div class="vcenter-out">
	<div class="vcenter-in">
		<fieldset style="font-family: monospace;" id="Output"/>
	</div>
</div>
"""

_print = ""
_printRead = threading.Lock()
_printRead.acquire()
_printWrite = threading.Lock()
_printWrite.acquire()

_input = ""
_inputRead = threading.Lock()
_inputRead.acquire()
_inputWrite = threading.Lock()
_inputWrite.acquire()

def handleNewLines_(text):
	return text.replace('\n', "<br/>").replace(" ","&nbsp;")

def print(*args, sep=" ", end="\n"):
	global _print, _printRead, _printWrite

	first = True
	text = ""

	sep = handleNewLines_(sep)

	for arg in args:
		if first:
			first = False
		else:
			text += sep

		text += handleNewLines_(html.escape(arg))

	text += handleNewLines_(end)

	_printWrite.acquire()
	_print = text
	_printRead.release()


def input(prompt=""):
	global _print, _printRead,_printWrite, _input, _inputRead,_inputWrite

	if prompt:
		print(prompt,end="")

	result = ""
	_printWrite.acquire()
	_printRead.release()
	_inputRead.acquire()
	result = _input
	_input = ""
	_inputWrite.release()

	return result

def scrollToBottom_(dom):
	dom.execute("window.scrollTo(0,document.getElementById('Output').scrollHeight);")

def loop_(dom):
	global _print, _printRead,_printWrite, _input, _inputRead,_inputWrite
	cont = True
	while cont:
		_printRead.acquire()

		if _print:
			dom.appendLayout("Output", "<span>" + _print + "</span>")
			if '<br/>' in _print:
				scrollToBottom_(dom)
			_print = ""
		else:
			cont = False
		_printWrite.release()

	dom.appendLayout("Output","<span><input type='text' id='Input' data-xdh-onevent='Submit' size='10' maxlength='10'/><br/></span>")
	scrollToBottom_(dom)
	dom.focus("Input")

def acConnect(dom):
	global _printWrite, _inputWrite
	dom.setLayout("", body)
	_printWrite.release()
	_inputWrite.release()
	loop_(dom)

def acSubmit(dom, id):
	global _print, _printRead,_printWrite, _input, _inputRead,_inputWrite

	_inputWrite.acquire()
	_input = dom.getContent("Input")
	dom.disableElement("Input")
	dom.removeAttribute("Input","id")
	_inputRead.release()
	loop_(dom)


callbacks = {
	"": acConnect,
	"Submit": acSubmit,
}

class Loop (threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)

	def run(self):
		Atlas.launch(callbacks, None, head, "Blank")

Loop().start()		
