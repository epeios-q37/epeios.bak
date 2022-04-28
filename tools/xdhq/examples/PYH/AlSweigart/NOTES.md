Codes source à <https://replit.com/@Q37Info/AlSweigartTheBigBookPython#/FromBook>.

L'autre répertoire contient le dépôt http://github.com/asweigart/PythonStdioGames qui semble reprendre les mêmes programmes, et en contient quelques-uns en plus.

Livre en ligne : <https://inventwithpython.com/bigbookpython/>

Code à placer en début de fichier pour importer la bibliothèque `atlastk` : 

```python
# The 'atlastk' module will be imported from the provided
# 'atlastk.zip' archive, to avoid having to install it.  
__import__("sys").path.append("./atlastk.zip")

import atlastk
```

- Images :
  - *Four in a row* :
    - *CLI* : !['Four in a row' with CLI](http://q37.info/s/v3hbxwbt.png)
    - *ATK* : !['Four in a row' with GUI](http://q37.info/s/dwgn9ckg.png)
  - *Mancala* :
    - *CLI* : !['Mancala' with CLI](http://q37.info/s/kmwf9rsn.png)
    - *ATK* : !['Mancala' with GUI](http://q37.info/s/c4rcfh4c.png)

Utiliser `__import__("sys").path.extend(["./atlastk.zip",…])` pour ajouter plusieurs *path*.

La commande `ATKAtlasPack` génère le fichier `atlastk.zip` contenant la bibliothèque. Ce fichier doit être placé à la racine des dépôts *GitHub* et *Replit*.
