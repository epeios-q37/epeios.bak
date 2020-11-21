 
*Toolkit* *Atlas* : un tutoriel

Plutôt qu'une simple journal annonçant une nouvelle version du *toolkit* *Atlas*, et pour meubler les longues soirées ~~d'hiver~~ de confinement, je me suis lancé dans la rédaction de ce tutoriel, que je vous livre dans cette dépêche.

Ce tutorial est hébergé sur *GitHub* (https://github.com/epeios-q37/tutorial-python), mais également sur *Repl.it* (https://repl.it/@AtlasTK/tutorial-python). Si vous avez *Python* 3 installé en local, vous pouvez récupérer le dépôt *GitHub* et visualiser/exécuter les différentes parties du tutoriel directement sur votre machine. Vous pouvez également, notamment si vous n'avez pas *Python* 3 installé sur votre ordinateur, visualiser/exécuter ces parties dans votre navigateur, en utilisant le lien ci-dessus sur *Repl.it*.

Chaque section de ce tutoriel a, dans le titre, la référence du fichier correspondant. Lorsqu'il s'agit d'un fichier source, il se présente sous la forme `part<x>.py`, ou `<x>` est un nombre. Pour exécuter ce fichier, lancer `python3 main.py` (ou, sur *Repl.it*, cliquez sur le bouton vert *Run*), saisissez le nombre `<x>`, puis valider avec la touche *entrée*. Pour sortir du logiciel, appuyer simultanément (et plusieurs fois si nécessaire) sur les touches *Ctrl* et *C* dans le terminal d'où vous avez lancé le programme (ou cliquez sur le bouton *Stop* dans *Repl.it*)

Techniquement, le *toolkit* *Atlas* permet de développer des applications web mono-pages, avec les différences suivantes par rapport aux *frameworks* traditionnellement utilisés pour cela :
- il n'y a pas de séparation front-end/back-end,

Si vous avez *Python* 3 installé sur votre ordinateur, vous pouvez également récupérer le dépôt *GitHub* https://github.com/epeios-q37/tuotorial-python en local, et lancer `python3 main.py`.

## La page principale de l'interface

```html
<fieldset>
  <fieldset id="Contact">
  <!-- Détail d'un contact -->
  </fieldset>
  <div style="display: table; margin: 10px auto auto auto;">
    <div>
      <!-- Boutons généraux. -->  
    </div>
    <div class="Edition">
      <!-- Bouton d'édition.-->
    </div>
  </div>
</fieldset>
<div style="display: table; margin: 10px auto auto auto; border-collapse: collapse;">
  <!-- Liste des contacts -->
</div>
```

## La page de départ (`part1.py`)

On va commencer par afficher la page de base.

En premier, on va définir la fonction qui sera appelée à chaque nouvelle ouverture de session :

```python
def ac_connect(dom):
  dom.inner("",open("Main.html").read())
```

`dom` est un objet fournit par le *toolkit* *Atlas* ; chaque session a sa propre instance de cette object. En l'occurrence, l'appelle de la méthode `inner(…)`va remplacer la totalité de la page web par le contenu du fichier `Main.html` précédemment décrit.  
Normalement, le premier paramètre est l'identifiant de l'élement dont on va remplacer le contenu. La chaîne vide est une valeur spéciale qui fait référence à l'élément racine de la page.

On va ensuite affecter cette fonction à une action, à l'aide d'un dictionnaire par convention nommé `CALLBACKS` :

```python
CALLBACKS = {
  "": ac_connect,
 }
```

Ici, `ac_connect` est affecté à une action dont le libellé est une chaîne vide. Cette valeur correspond à l'action qui est lancée à chaque nouvelle session.

On va ensuite lancer la boucle évènementielle du programme en lui passant la table des programmes, ainsi que le contenu du fichier `Head.html` précédemment décrit :

```python
atlastk.launch(CALLBACKS,None,open("Head.html").read())
```

Le paramètre dont la valeur est `None` sera abordé plus tard.

Au final, on a donc :

```python
import atlastk

def ac_connect(dom):
  dom.inner("",open("Main.html").read())

CALLBACKS = {
  "": ac_connect,
 }

atlastk.launch(CALLBACKS,None,open("Head.html").read())
```

Par la suite, le code complet correspondant à chaque section ne sera plus incluse dans le tutorial, mais pourra être visualisé sur *Repl.it* en suivant, comme déjà mentionné, le lien associé au nombre entre parenthèses dans le titre de la section.

En lançant ce programme ( en suivant le lien mentionné ci-dessus, bouton vert *run*, selectionner `1` puis valider avec la touche *entrée*), une fenêtre  va s'ouvrir dans un navigateur web affichant l'interface de l'application. En utilisant le code QR qui s'afficher en dessous de l'application, on va pouvoir ouvrir plusieurs sessions indépendantes de cette application.

## La liste des contacts (`part2.py`)

On va d'abord créer une liste de contacts fictives :

```python
EXAMPLE = [
  {
    "Name": "Holmes, Sherlock",
    "Address": "221B Baker Street, Londres",
    "Phone": "(use telegraph)",
    "Note": "Great detective!"
  },
  {
    "Name": "Holmes, Mycroft",
    "Address": "Diogenes Club, Pall Mall, Londres",
    "Phone": "(use telegraph)",
    "Note": "Works for the British government.\nBrother of Holmes, Sherlock."
  },
  {
    "Name": "Tintin",
    "Address": "Château de Moulinsart",
    "Phone": "421",
    "Note": "Has a dog named Snowy."
  },
  {
    "Name": "Tournesol, Tryphon (prof.)",
    "Address": "Château de Moulinsart",
    "Phone": "421",
    "Note": "Loves Loch Lomond whiskey."
  }
]
```

que l'on va affecter à une variable qui fera office de base de données:

```python
contacts = EXAMPLE
```

Et l'on va créer une fonction dédiée à l'affichage de cette liste :

```python
def display_contacts(dom):
  html = ""

  for contactId in range(len(contacts)):
    contact = contacts[contactId]
    html += f'<tr id="{contactId}" data-xdh-onevent="Select" style="cursor: pointer;">'
    for key in contact:
      html += f'<td>{contact[key]}</td>'
    html += '</td>'

  dom.inner("Content", html)
```

Dans cette fonction, on récupère chaque contact de la liste, et, pour chacun de ces contacts, le contenu de chacun de ses champs, et on va s'en servir pour créer le corps du tableau dédié à l'affichage de la liste, dont le code HTML est stocké dans la variable `html`. Le contenu de cette variable est injecté dans la table d'identifiant `Content` de l'interface grâce à la méthode `inner(…)` que l'on a déjà rencontré.  
Chaque ligne du tableau à son propre identifiant, et un attribut `data-xdh-onevent="Select"` qui fera l'objet de la prochaine section.

Et enfin, on ajoute l'appel à cette fonction dans la focntion appelée appelée à chque nouvelle session :

```python
def ac_connect(dom):
  dom.inner("",open("Main.html").read())
  display_contacts(dom)
```

Le [code résultant](part2.py) permet donc maintenant d'afficher une liste de contacts.


## Le détail d'un contact (`part3.py`)

On va maintenant remplir les champs au sommet de l'interface avec les valeurs du contact sélectionné dans la liste. On va définir la fonction correspondante.

```python
def display_contact(contactId,dom):
  dom.set_values(contacts[contactId])
```

La fonction `set_values(…)` prend un dictionnaire avec pour clef l'identifiant d'un élément, et, pour valuer, le contenu que doit prendre cet élément. Comme dans la page *HTML*, les éléments ont pour identifiant la clef des champs d'un contact, le dictionnaire existe déjà et n'est plus à construire. On l'utilise donc tel quel dans l'appel de la méthode `set_values(…)`.  
`contactId` est l'index du contact à afficher dans la liste `contacts`. 

On va maintenant créer la fonction que l'on va affecter à l'action `Select` définit dans l'attribut `data-xdh-onevent` du code HTML dont il est question dans la précedent section.

```python
def ac_select(dom,id):
  display_contact(int(id),dom)
```

Le paramètre `id` contient l'identifiant de l'élément recevant l'évènement ayant lancé l'action à laquelle cette fonction a été affectée. Dans la section précédente, on a vu que, pour le tableau contenant la liste des contacts, chaque ligne à pour identifiant l'index du contact dans la table `contacts`. On peut donc utiliser directement `id`, aprés l'avoir convertit en entier, puisqu'on le récupère sous forme d'une chaîne de caractère, pour le passé à la fonction `display_contact(…)`

On met à jour la table `CALLBACKS`, en affectant à l'action `Select` (qui a été définit dans un attribut `data-on-event` dans la précédente section de ce tutoriel) cette fonction:

```python
CALLBACKS = {
  "": ac_connect,
  "Select": ac_select
}
```

Le [code résultant](part3.py) permet maintenant, à chaque fois que l'on sélectionne un contact dans le liste, d'en afficher le contenu dans la partie de l'interface dédiée au détail d'un contact.

## Désactivation des champs + bouton *New* (`part4.py`)

Comme vous l'aurez sans doute remarqué, le contenu des champs dans lesquels s'affichent les détails sont modifiables, ce qui n'est pas le comportement voulu dans ce contexte. On va donc écrire le code permettant de désactiver ces champs.

Pour cela, on va d'abord créer une liste contenant les identifiants des différents  champs à désactiver :

```python
FIELDS = [
  "Name",
  "Address",
  "Phone",
  "Note"
]
 ```

 On va créer une fonction qui va gérer l'état de ces champs, et qui sera complété ultérieurement pour gérer d'autre éléments :

 ```python
 def update_outfit(dom):
    dom.disable_elements(FIELDS)
 ```

Ici, cette fonction fait appel à la méthode `disable_elements`, dont le nom est assez explicite.

 On va également utiliser cette fonction pour faire apparaître le bouton *New* qui permet de saisir un nouveau contact. La classe *Display* étant affecté à ce bouton (voir le fichier HTML), on va désactiver l'élément *style* d'identifiant `HideDisplay` (voir le fichier `Head.html`) qui définit une règle cachant les éléments de la classe, en utilisant la méthode `disable_element` (notez l'absence du `s` final). La fonction `update_outfit(…)` se présente alors de la manière suivante.

 ```python
 def update_outfit(dom):
    dom.disable_elements(FIELDS)
    dom.disable_element("HideDisplay")
 ```

 On pzut naturellement ajouter l'identifiant `HideDisplay` à la liste passée au `disable_elements(…)`, pour économiser un appel de fonction. 
 
 On va appeler cette fonction à chaque action de l'utilisateur, ce qui peut sembler ne pas être approprié vu son contenu, mais c'est en prévision de ce qu'elle contiendra une fois qu'elle sera enrichie au fur et à mesure de l'avancement dans ce tutoriel.

```python
def ac_connect(dom):
  dom.inner("",open("Main.html").read())
  display_contacts(dom)
  update_outfit(dom)

def ac_select(dom,id):
  display_contact(int(id),dom)  
  update_outfit(dom)
```

En lançant le programme, on verra que les champs affichant le détail d'un contact ont changé d'apparence, que leur contenu n'est plus modifiable, et que le bouton *New* fait son apparition.

## Saisie d'un nouveau contact (`part5.py`)

On va maintenant gérer l'action affecté au bouton *New*. Pour cela, on va utiliser un objet qui va stocker dans quel mode est placé le logiciel, à savoir *édition* ou *affichage*.

On va d'abord crée un *enum*relatifs à ces deux états, à l'aide du module *enum*, que l'on va importer en modifiant l'instructoin d'importation existante :

```python
 import atlastk, enum
```

et pour créer l'*enum* proprement dit :

```python
class State(enum.Enum):
  DISPLAY = enum.auto()
  EDIT = enum.auto()
```

On va maintenant créer une classe `Board` qui permet de stocker différentes variables propres à chaque session :

```python
class Board:
  def __init__(self):
    self.state = State.DISPLAY
```

Le constructeur d cette classe (`__init__(…)`) va stocker l'état initial du logiciel, à savoir `DISPLAY` (affichage), dans la variable membre `state`.

Il va falloir maintenant créer une instance de cette classe pour chaque nouvelle session. Ceci est realisé automatiquement par le *toolkit* *Atlas* ; il suffit de modifier l'appel fonction `launch(…)` déjà existante en remplaça,t le parmaètre de valeur `None` par le constructeur de cette classe, ce qui donne :

```python
atlastk.launch(CALLBACKS,Board,open("Head.html").read())
```

Ce faisant, toutes les fonctions référencées dans `CALLBACKS`, qui, je le rappelle, définit la fonction à appeler pour chaque action, vont recevoir l'instance de l'objet `Board` correspondant à la session à l'origine de l'appel de cette fonction. Il faut donc modifier le prototype de cette fonction. On obtient donc :

```python
def ac_connect(board,dom):
  …

def ac_select(board,dom,id):
  …
```

Notez l'apparition du paramètre `board`.

On va passer ce paramètre à la fonction `update_outfit(…)` pour qu'elle puisse tenir compte de l'état dans lequel se trouve le logiciel, et réagir en conséquence, ce qui donne :

```python
def update_outfit(board,dom):
  if board.state == State.DISPLAY:
    dom.disable_elements(FIELDS)
    dom.disable_element("HideDisplay")
  elif board.state == State.EDIT:
    dom.enable_elements(FIELDS)
    dom.enable_elements("HideDisplay")
  else:
    raise Exception("Unknown state!")
```

On voit l'apparition des méthodes `enable_element[s](…)`, qui sont le pendant des méthodes `enable_element[s](…)`.

Il faudra bien entendu modifier les appels à cette fonction en conséquence ; on va également, par précaution, mettre à jour dans l'instance `board` l'état du logiciel pour être sûr qu'il correspond à l'action lancée :

```python
def ac_connect(board,dom):
  …
  board.state = State.DISPLAY
  update_outfit(board,dom)


def ac_select(board,dom,id):
  …
  board.state = State.DISPLAY
  update_outfit(board,dom)
```

On va également modifier la fonction `display_contact(…)` de manière à pouvoir l'utiliser pour qu'elle vide les champs avant de donner la main pour la saisie du nouveau contact. Pour cela on va créer un dictionnaire correspondant à un contact vide :

```python
EMPTY_CONTACT = {
  "Name": "",
  "Address": "",
  "Phone": "",
  "Note": ""
}
```

qui va être utilisé de la manière suivante dans la fonction `displayContact`.

```python
def display_contact(contactId,dom):
  dom.set_values(EMPTY_CONTACT if contactId == None else contacts[contactId])
```

On constate que le passage de la valeur `None` au paramètre `contactId` entraînera maintenant le vidage des champs.

Ne reste plus qu'à définir la fonction qui sera appelée lors d'un clic sur le bouton *New* :

```python
def ac_new(board,dom):
  board.state = State.EDIT
  display_contact(None,dom)
  update_outfit(board,dom)
  dom.focus("Name")
```

Cette fonction réalise les opérations suivantes :
- stockage dans l'instance de l'objet `board` du nouvel état du logiciel, à savoir `EDIT` (édition) ;
- vidage des champs de saisie ;
- mise à jour de l'apparence de l'interface ;
- donner le focus (méthode `focus(…)`) au premier champs éditable (d'idetifiant `Name`, qui correspond au nom du contact), de manière à ce que l'utilisateur puisse procéder immédiatement à la saisie du nouveau contact.

Lors d'un clic que le bouton *New*, le programme cache maintenant ce bouton, vide les différents champs et permet la saisie d'un nouveau contact.

## Boutons d'édition (`part6.py`)

On peut maintenant saisir un nouveau contact, mais il manque les boutons pour valider ou annuler cette saisie. Cela va se faire en désactivant l'élément *style* d'identifiant `HideEdition` (dans le fichier `Head.html`) qui définit une règle cachant les éléments auxquels on a affecté la classe edition, ce qui est le cas de l'élément `div` qui contient les deux boutons d'édition `Submit` et `Cancel`.

Désactiver ce style pour faire apparaître les boutons d'éditions ne suffit pas ; il faut également l'activer pour cacher ces boutons lorsque requis. On va, pour cela, modifier la fonction `update_outfit(…)` pour obtenir cela :

```python
def update_outfit(board,dom):
  if board.state == State.DISPLAY:
    dom.disable_elements(FIELDS)
    dom.disable_element("HideDisplay")
    dom.enable_element("HideEdition")
  elif board.state == State.EDIT:
    dom.enable_elements(FIELDS)
    dom.enable_element("HideDisplay")
    dom.disable_element("HideEdition")
  else:
    raise Exception("Unknown state!")
```

Maintenant que les boutons sont affichés, on va créer les fonctions associés.

Pour le bouton *Cancel*, on va demander confirmation et, en fonction de la réponse, ne rien faire, ou repasser en mode d'affichage aprés avoir vider les champs de saisie :

```python
def ac_cancel(board,dom):
  if dom.confirm("Are you sure?"):
    display_contact(None,dom)
    board.state = State.DISPLAY
    update_outfit(board,dom)
```

Comme l'on peut s'en douter, le méthode `confirm(…)` ouvre une boîte de dialogue affichant la chaîne de caractère passés en paramètre, et retourne `True` lorsque l'on clique sur le bouton *OK* (ou ce qui en tien lieu), ou `False` si on clique sur le bouton `Cancel` (ou ce qui en tent lieu) tout en fermant ladite boîte de dialogue.

Pour le bouton `Submit`, il s'agit de récupérer les valeurs des champs de saisie, de stocker lesdites valeurs dans ce qui tient lieu de base de donnée, à savoir la variable `contacts`, de rafraîchir la liste des contacts, et de rebasculer en mode sasie, tout cela sous condition que le champs `Name` contienne une valeur :

```python
def ac_submit(board,dom):
  idsAndValues = dom.get_values(FIELDS)

  if not idsAndValues['Name'].strip():
    dom.alert("The name field can not be empty!")
  else:
    board.state = State.DISPLAY
    contacts.append(idsAndValues)
    display_contact(None,dom)
    display_contacts(dom)
    update_outfit(board,dom)
```

La méthode `get_values(…)` prend une liste de chaîne de caractères qui correspondent à des identifiants d'éléments, et retourne un dictionnaire avec pour clef, ces identifiants, etn pour valeurs, le contenu des éléments correspondants. Comme les identifiants correspondent au nom des clefs d'un contact, ou peut stocker le dictionnaire obtenu tel quel.

La méthode `alert(…)` affiche simplement une boîte de dialogue contenant comme message la chaîne passée en paramètre, avec un bouton *OK* (ou équivalent) permettant de la fermer.

On termine en metant à jour `CALLBACKS` pour affecter ces nouvelles fonctions aux actions adéquates :

```python
CALLBACKS = {
  "": ac_connect,
  "Select": ac_select,
  "New": ac_new,
  "Cancel": ac_cancel,
  "Submit": ac_submit
}
```