#!/usr/bin/python3

# If installed with 'pip install term2web',
# both above lines can be removed.
import sys
sys.path.append("./Atlas.zip")

import atlastk as Atlas

# Replace below lines by your own code.

head = """
<title>"Hello, World !" stub</title>
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
"""

body = """
<div style="display: table; margin: 50px auto auto auto;">
 <fieldset>
   <button data-xdh-onevent="Submit">Click me!</button>
  </div>
 </fieldset>
</div>
"""

def acConnect(dom):
	dom.setLayout("", body )


def acSubmit(dom):
	dom.alert("Hello, the World!")


callbacks = {
	"": acConnect,
	"Submit": acSubmit,
}


Atlas.launch(callbacks, None, head)