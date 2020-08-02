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
import openpyxl

tableFrame = """
<table>
	<thead>
		<tr>
			<th class="sticky">Id</th>
			<th class="sticky">State</th>
			<th class="sticky">County</th>
			<th class="sticky">Pop</th>
		</tr>
	</thead>
	<tbody id="Body"/>
	</tbody>
</table>
"""

tableItem = """
<tr{}>
	<td>{}</td>
	<td>{}</td>
	<td>{}</td>
	<td>{}</td>
</tr>
"""

treeItemHeader = """
<li>
	<input type="checkbox" id="{State}">
	<label class="tree fold" for="{State}">⊖</label>
	<label class="tree unfold" for="{State}">⊕</label>
	<label for="{State}">{State}</label>
	<table>
		<thead>
			<tr>
				<th>County</th>
				<th>Pop</th>
				<th>Tracts</th>
			</tr>
		</thead>
		<tbody>
"""

treeItemContent = """
			<tr id="{State}.{County}" data-xdh-onevent="View">
				<td>{County}</td>
				<td>{Pop}</td>
				<td>{Tracts}</td>
			</tr>
"""

treeItemFooter = """
		</tbody>
	</table>
</li>
"""

def get_asset_filename(path):
	return Atlas.get_asset_filename(path, "Excel")

def read_asset(path):
	return Atlas.read_asset(path, "Excel")

def reading(dom):
	countyData = {}
	prevCounty = ""

	dom.set_content('output', 'Opening workbook...')
	wb = openpyxl.load_workbook(get_asset_filename('censuspopdata.xlsx'),read_only=True)

	sheet = wb['Population by Census Tract']

	tableLayout = ""

	dom.set_layout("table", tableFrame)
	dom.set_content('output', 'Reading rows...')

	limit = sheet.max_row - 1	# This takes time, so it is stored.

	index = 0

	for row in sheet.iter_rows(min_row=2, min_col=2,values_only=True):
		index += 1

		state = row[0]
		county = row[1]
		pop = row[2]

		countyData.setdefault(state, {})
		countyData[state].setdefault(county, {'tracts': 0, 'pop': 0})
		countyData[state][county]['tracts'] += 1
		countyData[state][county]['pop'] += pop


		attribute = ""
		
		if prevCounty != county:
			attribute = " id=\"Table.{}.{}\"".format(state,county)
			prevCounty = county
			
		tableLayout += tableItem.format(attribute,index,state,county,pop)

		if not (index % 2500 ) or ( index == limit):
			dom.append_layout('Body', tableLayout)
			dom.execute_void("getElement('Body').lastChild.previousSibling.scrollIntoView({behavior: 'smooth', block: 'center'});")
			dom.flush()
			dom.set_content('output', 'Reading rows {}/{}'.format(index, limit))
			tableLayout = ""

	dom.set_content('output', 'Building tree...')
	
	treeLayout = ""

	for state, stateData in countyData.items():
		treeLayout += treeItemHeader.format(State=state)

		for county, data in stateData.items():
			treeLayout += treeItemContent.format(State=state,County=county,Pop=data['pop'],Tracts=data['tracts'])

		treeLayout += treeItemFooter

	dom.set_layout("tree", treeLayout)
	dom.set_content('output', 'Done')
	

def ac_view(dom,id):
	dom.execute_void("getElement('Table.{}').scrollIntoView({{behavior: 'smooth', block: 'center'}});".format(id))


def ac_connect(dom):
	dom.set_layout("", read_asset("Main.html"))
	reading(dom)

callbacks = {
	"": ac_connect,
	"View": ac_view
}

Atlas.launch(callbacks, None, read_asset("Head.html"), "Excel")
