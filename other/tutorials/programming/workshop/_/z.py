# coding: utf-8

import workshop._._ as _

_dir = "z"


def display(text):
    html = _.recall('html')
    html.putTagAndValue("h1", text)


def _acConnect(d,dom,id):
  _.store('html', _.Atlas.createHTML())
  dom.setLayout("",_.readBody(_dir,d.bodyI18n))
  dom.focus("a")


def _test(value):
  try:
    number = float(value)
    return number <100 and number >-100
  except:
    return False


def _test(value):
  number = float(value)

  if number <-100 or number > 100:
    raise ValueError("Out of range")

  return number
  
def _acSubmit(d,dom,id):
  [a,b,c,operator] = dom.getContents(['a','b','c','operator']).values()

  try:
    a = _test(a)
    b = _test(b)
    c = _test(c)
  except Exception as e:
    if _reportErrors:
      dom.alert(d.i18n["BadValue"])
    else:
      raise e
  else:
    if a == 0 and _reportErrors:
      dom.alert(d.i18n["ACannotBe0"])
    else:
      _function(a, b, c, operator)
      dom.setLayout("output", _.recall('html'))
      dom.disableElement("Hide")
      _.store('html', _.Atlas.createHTML())

  dom.focus('a')


def main(function,callback,title,reportErrors):
  globals()['_function'] = function
  globals()['_reportErrors'] = reportErrors
  _.main(_dir, callback,    {
      "" : _acConnect,
      "Submit": _acSubmit
    }, title )
