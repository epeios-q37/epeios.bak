# coding: utf-8

import workshop._._ as _

def display(text):
    html = _.recall('html')
    html.putTagAndValue("h1", text)


def _acConnect(c, dom):
    _.store('html', _.Atlas.createHTML())
    dom.setLayout("", _.readBody("c", c.bodyI18n))
    dom.focus("input")


def _acSubmit(c, dom):
    input=dom.getContent("input").strip()

    if (len(input)) != 0:
        globals()['_function'](dom.getContent("input"))
        dom.setContent( "input", "")
        dom.setLayout("output", _.recall('html'))
        dom.removeClass("output", "hidden")
        _.store('html', _.Atlas.createHTML())
    else:
        dom.alert(c.i18n["NameIsMandatory"])
        dom.setContent("input", "")
        dom.focus("input")


def main(function, callback, title=None):
  globals()['_function']=function
  _.main("c", callback,
    {
      "": _acConnect,
      "Submit": _acSubmit,
    }, title)
