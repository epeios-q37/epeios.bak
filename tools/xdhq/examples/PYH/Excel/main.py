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

view_mode_elements = ["Pattern", "CreateButton", "DescriptionToggling", "ViewNotes"]

def get_asset_filename(path):
	return Atlas.get_asset_filename(path, "Excel")

def read_asset(path):
	return Atlas.read_asset(path, "Excel")

def reading(dom):
	dom.set_content('output', 'Opening workbook...')
	wb = openpyxl.load_workbook(get_asset_filename('censuspopdata__.xlsx'))

	sheet = wb['Population by Census Tract']

	countyData = {}

	tbody = Atlas.create_HTML()

	dom.set_layout("@frame", """					<table>
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
					""")
	dom.set_content('output', 'Reading rows...')

	limit = sheet.max_row	# This takes time, so it is stored.

	for row in range(2, sheet.max_row + 1):
		tbody.push_tag("tr")
		# Each row in the spreadsheet has data for one census tract.
		tbody.put_tag_and_value('td',row-1)
		state  = sheet['B' + str(row)].value
		tbody.put_tag_and_value('td', state)
		county = sheet['C' + str(row)].value
		tbody.put_tag_and_value('td', county)
		pop    = sheet['D' + str(row)].value
		tbody.put_tag_and_value('td', pop)
		tbody.pop_tag()

		if not (row % 2500 ) or ( row == limit):
			dom.append_layout('Body@frame', tbody)
			dom.execute_void("getElement('@frame').scrollTo(0,getElement('@frame').scrollHeight);undefined;")
			dom.flush()
			dom.set_content('output', 'Reading rows {}/{}'.format(row-1, limit-1))
			tbody = Atlas.create_HTML()

def ac_connect(dom):
	dom.set_layout("", read_asset("Main.html"))
	
	reading(dom)

callbacks = {
	"": ac_connect,
}

Atlas.launch(callbacks, None, read_asset("Head.html"), "Excel")

