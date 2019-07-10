# coding: utf-8

import workshop._._ as _

_.store('html', _.Atlas.createHTML())

def _acConnect(c,dom):
  dom.setLayout("",_.readBody("ab"))
  dom.setLayout("output", _.recall('html'))


def display(text):
  _.recall('html').putTagAndValue("h1", text)

def main(title):
    _.main("ab", lambda : None,
    {
      "" : _acConnect,
    }, title )
