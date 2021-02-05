# Descriptions.md

Descriptions ajoutées dans la page HTML principale de chacune des démonstrations.

- Sélectionner la description au format *Markdown* avec la souris ;
- *Shift-Ctrl-B* + *entrée* (ou *Command Palette…*, *Tasks: Run Task*, *FaaS Description generator*) ;
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

## `Hello` ([file](./Hello/Main.html))

The famous ["Hello, World!" program](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) made with the *Atlas* *Toolkit*.  
Source code is available for [*Java*](https://repl.it/@AtlasTK/atlas-java#examples/Hello/Hello.java), [*Node.js*](https://repl.it/@AtlasTK/atlas-node#examples/Hello/Hello.js), [*Perl*](https://repl.it/@AtlasTK/atlas-perl#examples/Hello/Hello.pl), [*Python*](https://repl.it/@AtlasTK/atlas-python#examples/Hello/Hello.py) and [*Ruby*](https://repl.it/@AtlasTK/atlas-ruby#examples/Hello/Hello.rb)!

## `MatPlotLib`  ([file](../../examples/PYH/MatPlotLib/main.py))

Graphics drawn with [*matplotlib*](https://matplotlib.org/) can be used with the *Atlas* *Toolkit*.

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