# Descriptions.md

Descriptions ajoutées dans la page HTML principale de chacune des démonstrations.

- Sélectionner la description au format *Markdown* avec la souris ;
- <u>soit</u> :
  - lancer `ATKDescGen`,
  - (cette tâche n'est disponible qu'avec le *folder* *Meta*) *Shift-Ctrl-B* + *entrée* (ou *Command Palette…*, *Tasks: Run Task*, *FaaS Description generator*) ;
- Coller le presse-papier (*CTRL-V*) comme (en remplacement de la) première ligne du fichier *HTML*.


## `15-puzzle` ([file](./15-puzzle/Main.html))

As the *Scalable Vector Graphics* ([*SVG*](https://en.wikipedia.org/wiki/Scalable_Vector_Graphics)) image format is now supported by all major web browser, you can also use it with the *Atlas* *toolkit*, as you can see with this example.  
Take a look at the source code for [*Node.js*](https://repl.it/@AtlasTK/atlas-node#examples/15-puzzle/main.js) or [*Python*](https://repl.it/@AtlasTK/atlas-python#examples/15-puzzle/main.py) of this popular game to see how easy it is…

## `Blank` ([file](./Blank/Main.html))

A very simple example, to show the overall structure of an application made with the *Atlas* *toolkit*.  
Take a look at the source code for [*Java*](https://repl.it/@AtlasTK/atlas-java#examples/Blank/main.java), [*Node.js*](https://repl.it/@AtlasTK/atlas-node#examples/Blank/main.js) [*Perl*](https://repl.it/@AtlasTK/atlas-perl#examples/Blank/main.pl), [*Python*](https://repl.it/@AtlasTK/atlas-python#examples/Blank/main.py) or [*Ruby*](https://repl.it/@AtlasTK/atlas-ruby#examples/Blank/main.rb).

## `Chatroom` ([file](./Chatroom/Main.html))

An example of a basic chatroom, to show how data can be shared between several sessions (source code available for [*Java*](https://repl.it/@AtlasTK/atlas-java#examples/Chatroom/main.java), [*Node.js*](https://repl.it/@AtlasTK/atlas-node#examples/Chatroom/main.js), [*Perl*](https://repl.it/@AtlasTK/atlas-perl#examples/Chatroom/main.pl), [*Python*](https://repl.it/@AtlasTK/atlas-python#examples/Chatroom/main.py) or [*Ruby*](https://repl.it/@AtlasTK/atlas-ruby#examples/Chatroom/main.rb)). So, when you send some text from one session, this text will appear on all other sessions.
You can open as many sessions as you want by clicking the below displayed [QR code](https://en.wikipedia.org/wiki/QR_code), or by scanning it with a smartphone or similar devices, like a tablet…

## `Chatrooms` ([file](./Chatrooms/Admin.html))

In the [*Chatroom* example](https://repl.it/@AtlasTK/atlas-python#examples/Chatroom/main.py) (without the final `s`), you could see how to share data between all the sessions. With this example, only sessions from the same room will share the same messages.  
To achieve this, you create links with a user defined parameter, which value you can retrieve on the creation of a session. Take a look to the *Python* source files [here](https://repl.it/@AtlasTK/atlas-python#examples/Chatrooms/main.py) to see how to do this.

## `Excel-1` ([file](./Excel-1/Main.html))

This example is loosely based on [chapter 13](https://automatetheboringstuff.com/2e/chapter13/) of the [*Automate the Boring Stuff with Python*](https://automatetheboringstuff.com/) book by Al Sweigart.  
Clicking on a state takes you to its list of counties, and clicking on a county takes you to its details.


## `Excel-3` ([file](./Excel-3/Main.html))

This example is loosely based on [chapter 13](https://automatetheboringstuff.com/2e/chapter13/) of the [*Automate the Boring Stuff with Python*](https://automatetheboringstuff.com/) book by Al Sweigart. It shows an application that helps to correct a handwritten *Excel* file with misspelled entries.    
Select the misspelled entries corresponding to the same item by clicking their checkboxes, click the *Jump* button, select the correctly spelled item by clicking its radio button, click the *Coll./Exp.* button to see the entries you have selected, and click the *Apply* button to apply the corrections.

## `Hello` ([file](./Hello/Main.html))

The famous ["Hello, World!" program](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) made with the *Atlas* *Toolkit*.  
Source code is available for [*Java*](https://repl.it/@AtlasTK/atlas-java#examples/Hello/Hello.java), [*Node.js*](https://repl.it/@AtlasTK/atlas-node#examples/Hello/Hello.js), [*Perl*](https://repl.it/@AtlasTK/atlas-perl#examples/Hello/Hello.pl), [*Python*](https://repl.it/@AtlasTK/atlas-python#examples/Hello/Hello.py) and [*Ruby*](https://repl.it/@AtlasTK/atlas-ruby#examples/Hello/Hello.rb)!

## `MatPlotLib`  ([file](../../examples/PYH/MatPlotLib/main.py))

Graphics drawn with [*matplotlib*](https://matplotlib.org/) can be used with the *Atlas* *Toolkit*.

## `PigGame` ([file](./PigGame/Main.html))

A multi-player version of the [Pig game](https://en.wikipedia.org/wiki/Pig_(dice_game)#Gameplay).  
You can choose to play against the computer, or with a partner.

## `TodoMVC` ([file](./TodoMVC/Main.html))

The [*TodoMVC* app](http://todomvc.com/), but **not** using the [*MVC*](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller) pattern.
Source code is available for [*Java*](https://repl.it/@AtlasTK/atlas-java#examples/TodoMVC/main.java), [*Node.js*](https://repl.it/@AtlasTK/atlas-java#examples/TodoMVC/main.java), [*Python*](https://repl.it/@AtlasTK/atlas-python#examples/TodoMVC/main.py) and [*Ruby*](https://repl.it/@AtlasTK/atlas-ruby#examples/TodoMVC/main.rb)!

## `Widgets` ([file](./Widgets/Main.html))

You will see here some widgets in action. Corresponding HTML code will be displayed.

## Misc

### Libellés des langages

*Java* *Node.js* *Perl* *Python* *Ruby*

### `tasks.json`

Commande utilisée dans `~/.config/VSCodium/User/tasks.json` :

```json
{
  "label": "FaaS description generator",
  "type": "shell",
  "command": "echo '<iframe style=\"border: none;\" src=\"FaaSDesc.php?text='$(xsel -o -p | base64 -w 0 - | sed s/+/%2B/g)'\"></iframe>' | xsel -i -b",
  "problemMatcher": []
}
```

*NOTA* :
- le clipboard primaire (texte sélectionné avec la souris) ne fonctionne pas ;
- `xclip -i -selection clipboard` ne fonctionne ps, d'où l'utilisation de `xsel`.