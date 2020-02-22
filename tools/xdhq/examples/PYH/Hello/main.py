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

import os, sys

sys.path.append("./atlastk")
sys.path.append("../atlastk")

import atlastk as Atlas

def read_asset(path):
	return Atlas.read_asset(path, "Hello")

def ac_connect(dom):
	dom.set_layout("", read_asset( "Main.html") )
	dom.focus( "input")

def ac_submit(dom):
	dom.alert("Hello, " + dom.get_content("input") + "!")
	dom.focus( "input")

def ac_clear(dom):
	if ( dom.confirm("Are you sure?" ) ):
		dom.set_content("input", "" )
	dom.focus( "input")

callbacks = {
	"": ac_connect,
	"Submit": ac_submit,
	"Clear": ac_clear,
}
		
Atlas.launch(callbacks, None, read_asset("Head.html"))
