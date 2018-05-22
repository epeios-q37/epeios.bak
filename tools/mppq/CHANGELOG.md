# *MPPq* *CHANGELOG*

## 2018-05-02

- skipping tailing slide separator, to avoid empty slides,

## 2018-05-01

- adaptation to changes in underlying libraries,
- preceding the slide separator (`---`) with a new line, because if previous line have its last character being a colon (`:`), the slide separator is ignored,
- text after last list item is no more displayed simultaneously with last item, but added on a new slide,
- *HTML* comment after a list item do no more display generate two different slides (one without the comment, then next with comment).

