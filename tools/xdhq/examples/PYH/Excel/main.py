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

sys.path.append("./atlastk")
sys.path.append("../atlastk")

import atlastk as Atlas
import openpyxl, time

tableFrame = """
<table>
	<thead style="position: sticky; top: 0px; background-color: aliceblue;">
		<tr>
			<th>Id</th>
			<th>State</th>
			<th>County</th>
			<th>Pop</th>
		</tr>
	</thead>
	<tbody id="Body"/>
	</tbody>
</table>
"""

view_mode_elements = ["Pattern", "CreateButton", "DescriptionToggling", "ViewNotes"]

def get_asset_filename(path):
	return Atlas.get_asset_filename(path, "Excel")

def read_asset(path):
	return Atlas.read_asset(path, "Excel")

def reading(dom):
	currentState = ""
	currentCounty = ""
	cumulPop = 0

	dom.set_content('output', 'Opening workbook...')
	wb = openpyxl.load_workbook(get_asset_filename('censuspopdata.xlsx'))

	sheet = wb['Population by Census Tract']

	table = Atlas.create_HTML()
	tree = Atlas.create_HTML("ul")

	dom.set_layout("@frame", tableFrame)
	dom.set_content('output', 'Reading rows...')

	limit = sheet.max_row	# This takes time, so it is stored.

	for row in range(2, sheet.max_row + 1):
		table.push_tag("tr")
		# Each row in the spreadsheet has data for one census tract.
		table.put_tag_and_value('td',row-1)
		state  = sheet['B' + str(row)].value
		table.put_tag_and_value('td', state)
		county = sheet['C' + str(row)].value
		table.put_tag_and_value('td', county)
		pop    = sheet['D' + str(row)].value
		table.put_tag_and_value('td', pop)
		table.pop_tag()

		if ( state != currentState) or ( county != currentCounty ):
			if county != currentCounty:
				if currentState != '':
					tree.put_tag_and_value("li", "{}: {}".format(currentCounty, cumulPop))
				currentCounty = county
				cumulPop = pop
			else:
				cumulPop += pop
			
			if state != currentState:
				if currentState != "":
					tree.pop_tag()
					tree.pop_tag()

				tree.push_tag("li")
				tree.push_tag("input")
				tree.put_attribute("type", "checkbox")
				tree.put_attribute("id", row)
				tree.pop_tag()
				tree.push_tag("i")
				tree.put_attribute("class","fa fa-angle-double-right")				
				tree.put_value(" ")
				tree.pop_tag()
				tree.push_tag("i")
				tree.put_attribute("class","fa fa-angle-double-down")				
				tree.put_value(" ")
				tree.pop_tag()
				tree.push_tag("label")
				tree.put_attribute("for", row)
				tree.put_value(state)
				tree.pop_tag()
				tree.push_tag("ul")

				currentState = state

		if not (row % 2500 ) or ( row == limit):
			dom.append_layout('Body@frame', table)
			dom.execute_void("getElement('@frame').scrollTo(0,getElement('@frame').scrollHeight);undefined;")
			dom.flush()
			dom.set_content('output', 'Reading rows {}/{}'.format(row-1, limit-1))
			table = Atlas.create_HTML()

	print("coucou")
	print(tree.to_string())
	dom.set_layout("tree", tree)
	

def ac_connect(dom):
	dom.set_layout("", read_asset("Main.html"))
	
	reading(dom)

callbacks = {
	"": ac_connect,
}

Atlas.launch(callbacks, None, read_asset("Head.html"), "Excel")

