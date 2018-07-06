# *TodoMVC* tutorial

## Progress

0. Only `Main`, **no** `Todos`
1. Submission of new entry (`Main`, `Todos`)
2. Deletion of entry (`Todos`)
3. Toggling *Active*/*Completed* (`Todos`)
4. Displaying of completed tasks when task list refreshed (`Todos`)
5. Displaying of the amount of active items
6. Displaying of *Clear completed* when needed
7. Handling of the *All*/*Active*/*Achieved* buttons (`Main`)
8. Handling of the *Clear completed* button (`Main`)
9. Edition of an entry (`Todos`)
10. Handling submitting/dismissing of the modification of an entry (`Todos`)

## Commands

### Global, for *XSL* files

`code -n -d <file1>, <file2>.`

### For *Java*

- `n=1;code -n -d main.$(expr $n - 1 ).java main.$n.java`
- `javac main.$n.java;java TodoMVC` 

### For *Node.js*

- `n=1;code -n -d main.$(expr $n - 1 ).js main.$n.js`
- `node main.$n.js` 

### For *PHP*

- `n=1;code -n -d main.$(expr $n - 1 ).php main.$n.ph`
- `php main.$n.php` 

## *VSCode* settings :

### For *diff* (*settings*)
- `"diffEditor.renderSideBySide": false`

### Font zoom (*keyboard shortcuts*)

``` json
{ "key": "ctrl+numpad_add",      "command": "editor.action.fontZoomIn" },
{ "key": "shift+ctrl+=",         "command": "editor.action.fontZoomIn" },
{ "key": "ctrl+=",               "command": "editor.action.fontZoomIn" },
{ "key": "ctrl+numpad_subtract", "command": "editor.action.fontZoomOut" },
{ "key": "shift+ctrl+-",         "command": "editor.action.fontZoomOut" },
{ "key": "ctrl+-",               "command": "editor.action.fontZoomOut" },
{ "key": "ctrl+numpad0",         "command": "editor.action.fontZoomReset" },
{ "key": "ctrl+0",               "command": "editor.action.fontZoomReset" },
```