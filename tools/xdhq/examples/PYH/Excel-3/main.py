# -*- coding: utf-8 -*-
"""
MIT License

Copyright (c) 2020 Claude SIMON (https://q37.info/s/rmnmqd49)

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

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

# Note to the dev: 'openpyxl' must be imported before 'atlastk',
# as the 'load_workbook' function from 'openpyxl' is overloaded.
# Applies only in DEV context.
import openpyxl
import atlastk as Atlas

item = """
<tr id="tr.{Produce}" data-xdh-mark="{Produce}">
	<td>{Produce}</td>
	<td>{Count}</td>
	<td class="checkbox">
		<input id="checkbox.{Produce}" type="checkbox" data-xdh-onevent="CheckboxClick"/>
	</td>
	<td class="radio">
		<input id="radio.{Produce}" type="radio" name="radio" data-xdh-onevent="RadioClick"/>
	</td>
</tr>
"""

source = ""
target = []
unsortedCounts = {}
sortedCounts = {}
expanded = True
workbook = None

def count(dom):
	global unsortedCounts

	sheet = workbook['Sheet']

	dom.set_content('output', 'Reading rows...')

	limit = sheet.max_row - 1	# This takes time, so it is stored.

	index = 0	

	unsortedCounts = {}

	for row in sheet.iter_rows(min_row=2, min_col=1,max_col=1,values_only=True):
		index += 1

		produce = row[0]

		unsortedCounts.setdefault(produce, 0)
		unsortedCounts[produce] += 1

		if not ( index % 2000 ) or ( index == limit ):
			dom.set_content('output', 'Reading rows {}/{}'.format(index,limit))

def sort():
	global sortedCounts

	sortedCounts = {}

	for count in sorted(unsortedCounts.items(), key = lambda item: item[1]):
		sortedCounts[count[0]]=count[1]

def display(dom):
	layout = ""

	for produce,count in sortedCounts.items():
		layout += item.format(Id=produce,Mark=produce,Produce=produce,Count=count)

	dom.set_layout("counts",layout)

def launch(dom):
	global target, source
	dom.set_layout("counts","")
	dom.set_content("output", "Initialization…")
	dom.remove_class("output", "hidden")
	dom.set_content('output', 'Sorting…')
	count(dom)
	sort()
	dom.set_content('output', 'Displaying…')
	display(dom)
	dom.disable_element("HideCheckbox")
	dom.disable_element("HideRadio")
	dom.set_content("output", "Done")
	dom.add_class("output", "hidden")
	target = []
	source = ""

def ac_connect(dom):
	global workbook

	dom.set_layout("", open("Main.html").read())
	workbook = openpyxl.load_workbook("produceSales.xlsx")

	launch(dom)

def ac_checkbox_click(dom,id):
	global target

	mark = dom.get_mark(id)

	if mark in target:
		target.remove(mark)
		dom.enable_element("radio.{}".format(mark))
	else:
		target.append(mark)
		dom.disable_element("radio.{}".format(mark))

def ac_radio_click(dom,id):
	global source

	if source != "":
		dom.enable_element("checkbox.{}".format(source))

	source = dom.get_mark(id)

	dom.disable_element("checkbox.{}".format(source))

def collapse(dom):
	toHide = {}
	
	for produce in unsortedCounts:
		if (not produce in target) and (produce != source):
			toHide["tr.{}".format(produce)] = "hidden"

	dom.add_classes(toHide)

def expand(dom):
	elements = {}
	
	for produce in unsortedCounts:
		elements["tr.{}".format(produce)] = "hidden"

	dom.remove_classes(elements)

def ac_collapse_expand(dom):
	global expanded

	expanded = not expanded

	if expanded:
		expand(dom)
	else:
		collapse(dom)

def ac_apply(dom):
	global workbook

	workbook.save("Result.xlsx")
	
callbacks = {
	"": ac_connect,
	"Refresh": lambda dom: launch(dom),
	"CheckboxClick": ac_checkbox_click,
	"RadioClick": ac_radio_click,
	"CollapseExpand": ac_collapse_expand,
	"Apply": ac_apply
}

Atlas.launch(callbacks, None, open("Head.html").read())
