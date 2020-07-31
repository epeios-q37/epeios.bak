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

def get_asset_filename(path):
	return Atlas.get_asset_filename(path, "Excel")

def read_asset(path):
	return Atlas.read_asset(path, "Excel")

def reading(dom):
	countyData = {}
	prevCounty = ""

	dom.set_content('output', 'Opening workbook...')
	wb = openpyxl.load_workbook(get_asset_filename('censuspopdata__.xlsx'))

	sheet = wb['Population by Census Tract']

	table = Atlas.create_HTML()

	dom.set_layout("table", tableFrame)
	dom.set_content('output', 'Reading rows...')

	limit = sheet.max_row	# This takes time, so it is stored.

	for row in range(2, sheet.max_row + 1):
		state  = sheet['B' + str(row)].value
		county = sheet['C' + str(row)].value
		pop    = sheet['D' + str(row)].value

		countyData.setdefault(state, {})
		countyData[state].setdefault(county, {'tracts': 0, 'pop': 0})
		countyData[state][county]['tracts'] += 1
		countyData[state][county]['pop'] += pop

		table.push_tag("tr")
		if prevCounty != county:
			table.put_attribute("id", "Table.{}.{}".format(state,county))
			prevCounty = county
		# Each row in the spreadsheet has data for one census tract.
		table.put_tag_and_value('td',row-1)
		table.put_tag_and_value('td', state)
		table.put_tag_and_value('td', county)
		table.put_tag_and_value('td', pop)
		table.pop_tag()

		if not (row % 2500 ) or ( row == limit):
			dom.append_layout('Body', table)
#			dom.execute_void("getElement('table').scrollTo(0,getElement('table').scrollHeight);")
			dom.execute_void("getElement('Body').lastChild.scrollIntoView({behavior: 'smooth', block: 'center'});")

			dom.flush()
			dom.set_content('output', 'Reading rows {}/{}'.format(row-1, limit-1))
			table = Atlas.create_HTML()

	dom.set_content('output', 'Building tree...')
	
	tree = Atlas.create_HTML("ul")
	for state in countyData:
		tree.push_tag("li")
		tree.push_tag("input")
		tree.put_attribute("type", "checkbox")
		tree.put_attribute("id", state)
		tree.pop_tag()
		tree.push_tag("span")
		tree.put_attribute("class","tree fold")				
		tree.put_value("⊖")
		tree.pop_tag()
		tree.push_tag("span")
		tree.put_attribute("class","tree unfold")				
		tree.put_value("⊕")
		tree.pop_tag()
		tree.push_tag("label")
		tree.put_attribute("for", state)
		tree.put_value(state)
		tree.pop_tag()
		tree.push_tag("table")
		tree.push_tag("thead")
		tree.push_tag("tr")
		tree.put_tag_and_value("th", "County")
		tree.put_tag_and_value("th", "Pop")
		tree.put_tag_and_value("th", "Tracts")
		tree.pop_tag()
		tree.pop_tag()
		tree.push_tag("tbody")			

		for county in countyData[state]:
			tree.push_tag("tr")
			tree.put_attribute("id","{}.{}".format(state,county))
			tree.put_attribute("data-xdh-onevent","View")
			tree.put_tag_and_value("td",county)
			tree.put_tag_and_value("td",countyData[state][county]['pop'])
			tree.put_tag_and_value("td",countyData[state][county]['tracts'])
			tree.pop_tag()			

		tree.pop_tag()	# </table>
		tree.pop_tag()	# </tbody>
		tree.pop_tag()	# </li>

	dom.set_layout("tree", tree)
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

