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

# This file is dedicated to modification of the loaded workbook
# to simulate errors occuring on a hand-written workbook.

import random

errors = {
    'Apples': ['apples', 'Aples', 'apels'],
    'Apricots': ['apricots', 'Appriccots', 'Apricot', 'Aspricot', 'appricots'],
    'Asparagus': ['asparagus', 'Aspparagus', 'asparragus', 'Aspragus'],
    'Avocados': ['Avacados', 'avocados'],
    'Bananas': ['bananas', 'Banananas'],
    'Beets': ['beets', 'Bets', 'bets'],
    'Bok choy': ['Bock choy', 'Boc Choy', 'Bocchoy', 'Bauk choy'],
    'Brussels sprouts': ['Brussels prouts', 'Brussel sprouts', 'Brusel sprouts'],
    'Butternut squash': ['Butenut squash', 'Butternut Squash', 'butternut squash'],
    'Carrots': ['carrots', 'Carots'],
    'Celery': ['celry', 'Celry', 'celery'],
    'Cherries': [],
    'Coconuts': ['Coconut', 'coconuts'],
    'Corn': ['Korn'],
    'Cucumber': ['Cumcumber', 'Cucuber', 'cucumber'],
    'Daikon': [],
    'Eggplant': ['Eggpalnt', 'eggplant'],
    'Fava beans': ['Faya beans'],
    'Garlic': ['garlic', 'Gralic'],
    'Ginger': ["Gingre"],
    'Grapefruit': ["Grape fruit"],
    'Grapes': [],
    'Green beans': ['Grenn beans'],
    'Green cabbage': ['Green cabage', 'green cabage'],
    'Green peppers': ["Green poppers"],
    'Kale': [],
    'Lemon': [],
    'Lettuce': ['Letuce'],
    'Lime': [],
    'Okra': ['Ocra'],
    'Orange': [],
    'Papaya': ['Papya', 'papya'],
    'Parsnips': ['PArsnip'],
    'Potatoes': ['Poatatoes', 'Patatos'],
    'Red onion': [],
    'Spinach': ['Spinac'],
    'Strawberries': ['Stawberies'],
    'Tomatoes': ['Tomatos'],
    'Watermelon': [],
    'Yellow peppers': ['Yellow pepers', 'Yelow peppers']
}

def scramble(workbook):
	sheet = workbook['Sheet']	

	for row in sheet.iter_rows(min_row=2, min_col=1,max_col=1):
		if random.randrange(200) == 0:
			produce = row[0].value

			if produce in errors:
				amount = len(errors[produce])

				if amount:
					row[0].value = errors[produce][random.randrange(amount)]