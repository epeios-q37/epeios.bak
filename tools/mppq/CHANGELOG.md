# *MPPq* changelog

## 2019-06-20

- adaptation to modification in underlying modules,
- adding an empty line before code blocks (or they will not be displayed correctly under some circumstances),
- heading spaces in slide separators (`---`) and block delimiters (` ``` `) do no more produce messy results,

## 2018-09-24

- blank lines inside code blocks are now preserved,

## 2018-05-02

- skipping tailing slide separator, to avoid empty slides,

## 2018-05-01

- adaptation to changes in underlying libraries,
- preceding the slide separator (`---`) with a new line, because if previous line have its last character being a colon (`:`), the slide separator is ignored,
- text after last list item is no more displayed simultaneously with last item, but added on a new slide,
- *HTML* comment after a list item do no more generate two different slides (one without the comment, then next with comment).

