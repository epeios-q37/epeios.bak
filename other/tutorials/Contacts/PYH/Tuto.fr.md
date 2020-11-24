 
# *Toolkit* *Atlas* : un tutoriel

Histoire de meubler mes longues soirées ~~d'hiver~~ de confinement, je me suis lancé dans la rédaction d'un tutoriel détaillant pas à pas la programmation d'une application basique de gestion de contacts avec la version *Python* du *toolkit* *Atlas* tutoriel, qui ressemble à ceci :

![](https://q37.info/download/assets/Contacts.png)

Le *toolkit* *Atlas* permet d'écrire des [applications web monopages](https://fr.wikipedia.org/wiki/Application_web_monopage), avec les différences suivantes par rapport à la plupart des *framework* utilisés pour ce type d'application :
- léger (quelques dizaines de Ko) et sans dépendances ;
- n'impose pas une architecture particulière (MVC ou consort) ;
- n'impose pas l'utilisation de *JavaScript* ou dérivé.

En outre, pour qu'elle soit accessible d'internet, il n'est pas nécessaire de déployer une application développée avec le *toolkit* *Atlas* sur un serveur distant. Il suffit que l'ordinateur à partir duquel vous lancez l'application soit connecté à internet pour qu'elle soit accessible de n'importe quel dispositif équipé d'un navigateur web moderne connecté à internet, sans rien avoir à configurer.

Ce tutorial est hébergé sur *GitHub* (https://github.com/epeios-q37/tutorial-python), mais également sur *Repl.it* (https://repl.it/@AtlasTK/tutorial-python).  
Si vous avez *Python* 3 d'installé sur votre ordinateur, vous pouvez récupérer le dépôt *GitHub* et visualiser/exécuter les différentes sections du tutoriel directement sur votre machine, aprés avoir installé le *toolkit* *Atlas* (`pip3 install atlastk`).  
Vous pouvez également, notamment si vous n'avez pas *Python* 3 d'installé sur votre ordinateur, visualiser/exécuter ces parties directement dans votre navigateur, en utilisant le lien ci-dessus sur *Repl.it*.

Pour ne pas allonger ce tutoriel outre mesure, d'une section à l'autre, seule les modifications apportées au code sont indiquée. Néanmoins, à la fin de chaque section, il y aura un lien vers le code source du programme, sur *GitHub*, dans sa version propre à cette section, ainsi que les actions à lancer pour l'exécuter sur *Repl.it*.


## Le ficher HTML principal (`Main.html`)

### Structure générale

Ce fichier `Main.hmtl` est un fichier au format *HTML* qui décrit l'interface, dont voici le contenu :

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
      <!-- Bouton de saisie.-->
    </div>
  </div>
</fieldset>
<div style="display: table; margin: 10px auto auto auto; border-collapse: collapse;">
  <!-- Liste des contacts -->
</div>
```

Il est aisément compréhensible pour ceux qui sont familier avec *HTML*. Ce fichier va prendre place dans la section *body* de la page HTML affichée par l'application. Ses différentes sous-parties vont être détaillées ci-dessous.

### Détail d'un contact

Voici le code dédié à l'affichage du détail d'un contact :

```html
<table style="margin: auto;">
  <tr>
    <td>
      <label for="Name">Full name:</label>
    </td>
    <td>
      <input id="Name" size="50" />
    </td>
  </tr>
  <tr>
    <td>
      <label for="Address">Address:</label>
    </td>
    <td>
      <input id="Address" size="50" />
    </td>
  </tr>
  <tr>
    <td>
      <label for="Phone">Phone:</label>
    </td>
    <td>
      <input id="Phone" type="tel" size="50" />
    </td>
  </tr>
  <tr>
    <td>
      <label for="Note">Note:</label>
    </td>
    <td>
      <textarea id="Note" style="width: 100%;"></textarea>
    </td>
  </tr>
</table>
```

On y trouve un tableau, avec, pour chaque ligne, un des champs constituant un contact, accompagné d'un libellé et d'un identifiant explicite.

### Boutons généraux

Ces boutons vont servir à créer/éditer/supprimer un contact. En voici le code :

```html
<button class="Display" data-xdh-onevent="New">New</button>
<span class="DisplayAndSelect">
  <button data-xdh-onevent="Edit">Edit</button>
  <button data-xdh-onevent="Delete">Delete</button>
</span>
```

À part l l'attribut `data-xdh-onevent`, on n'a là que du *HTML* des plus classique. Les différentes classes ont cependant un rôle bien particulier, qui sera révélé dans lse sections qui suivent.

L'attribut `data-xdh-onevent` prend ici la place de l'habituel attribut `onclick`, qui contient le code *JavaScript* à lancer lorsque l'on clique sur le bouton auquel est affecté cet attribut. Ici, à la place, on définit un libellé d'action, libellé que l'on retrouvera dans le code *Python*. Ainsi, on va pouvoir coder les actions à réaliser, non plus en *JavScript*, mais en *Python*.

### Boutons de saisie

Ces boutons sont affichés lors de la saisie d'un contact, et permette de valider ou d'annuler cette saisie. Voici le code correspondant :

```html
<button data-xdh-onevent="Cancel">Cancel</button>
<button data-xdh-onevent="Submit">Submit</button>
```

Là encore, rien de particulier, mis à part l'attribut `data-xdh-onevent` que l'on a déjà rencontré ci-dessus. Le contenu des attributs `data-xdh-onevent`, à savoir `Cancel` et `Submit`, vont être utilisé dans le code *Python* de l'application.   
Notes que le nom du bouton (la valeur de l'élément `button`) est identique à la valeur de son attribut `data-xdh-onevent` uniquement par commodité ; ce n'est en rien obligatoire.

### Liste de contacts

Cette partie affiche le tableau qui va accueillir la liste des contacts au sein de son élément `tbody`, dont le contenu va être construit par l'application. En voici le contenu :

```html
<table id="Contacts" style="cursor: default; border-collapse: collapse;">
  <thead>
    <th>Name</th>
    <th>Address</th>
    <th>Phone</th>
    <th>Note</th>
  </thead>
  <tbody id="Content" />
</table>
```

Notez l'identifiant `Content`, que l'on va retrouver dans le coe *Python*.

## Le fichier d'entête `Head.html`

Ce fichier, également au format *HYML*  prendra place dans la section *head* de la page *HTML* de l'application

### Apparence de l'application

La première partie de ce fichier définit le titre, l'icône, et diverses retouches au niveau de l'apparence de l'interface à l'aide de quelques règles *CSS*. En voici le contenu :

```html
<title>Address book</title>
<link rel="icon" type="image/png"
	href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
<style>
	#Contact label {
		font-weight: bold;
	}

	#Contact span {
		text-align: left;
	}

	#Contacts th,
	#Contacts td {
		border: 1px solid black;
		padding: 0% 5px;
	}

	#Contacts td:nth-child(3) {
		text-align: right;
	}

	#Contacts tr:nth-child(even)
	{
		background: #CCC
	}

	#Contacts tr:nth-child(odd)
	{
		background: #FFF
	}

	#Contact *:disabled {
		background-color: snow;
		color: initial;
	}

</style>
```

### Visibilité des boutons

Le bas du fichier permet de gérer la visibilité des boutons. En voici le contenu :

```html
<style id="HideDisplay">
	.Display {
		display: none;
	}
</style>
<style id="HideDisplayAndSelect">
	.DisplayAndSelect {
		display: none;
	}
</style>
<style id="HideEdition">
	.Edition {
		display: none;
	}
</style>
```

On y voit des élement `style` affublés d'un identifiant. Ces éléments vont permettre de cacher/afficher certains boutons.  
Chaque élément `style` définit, en effet, une règle pour une classe. En activant/désactivant, on ajoute/retire, à cette classe, la règle *CSS* qu'elle contient, et par conséquent on agit également sur les éléments, en l'occurrence des boutons, auxquels cette classe est affectée.

On retrouvera les différents identifiants de ces éléments `style` dans le code détaillé dans les sections qui suivent.

## Rendu de l'interface (`part1.py`)

### Affichage de la page *HTML*

En premier, on va définir la fonction qui sera appelée à chaque ouverture de session :

```python
def ac_connect(dom):
  dom.inner("",open("Main.html").read())
```

`dom` est un objet fournit par le *toolkit* *Atlas* ; chaque session a sa propre instance de cette objet. En l'occurrence, l'appel de la méthode `inner(…)`va remplacer la totalité de la page web par le contenu du fichier `Main.html` précédemment décrit.  
Le premier paramètre de cette méthode est l'identifiant de l'élément dont on va remplacer le contenu. La chaîne vide est une valeur spéciale qui fait référence à l'élément racine de la page.

On va ensuite affecter cette fonction à une action, à l'aide d'un dictionnaire nommé, par convention, `CALLBACKS` :

```python
CALLBACKS = {
  "": ac_connect
 }
```

Ici, `ac_connect` est affecté à une action dont le libellé est une chaîne vide. Cette valeur correspond à l'action qui est lancée à chaque nouvelle session.

### La boucle évènementielle

On va ensuite lancer la boucle évènementielle de l'application, en lui passant le dictionnaire des actions, ainsi que le contenu du fichier `Head.html` décrit précédemment :

```python
atlastk.launch(CALLBACKS,None,open("Head.html").read())
```

Le paramètre dont la valeur est `None` sera abordé plus tard.

## Liste des contacts (`part2.py`)

### Liste fictive

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
    "Note": "Creator of the Bianca rose."
  }
]
```

que l'on va affecter à une variable qui fera office de base de données :

```python
contacts = EXAMPLE
```

### Affichage

Créons une fonction dédiée à l'affichage de cette liste :

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

Dans cette fonction, on récupère chaque contact de la liste, et, pour chacun de ces contacts, le contenu de chacun de ses champs. On va s'en servir pour créer le contenu du corps du tableau dédié à l'affichage de la liste, qui sera stocké dans la variable `html`.  
Le contenu de cette variable est ensuite injecté dans le corps de la table, plus précisément dans l'élément `tbody` d'identifiant `Content` (voir le fichier `Main.html`), grâce à la méthode `inner(…)` que l'on a déjà rencontré.  
Chaque ligne du tableau à son propre identifiant, et un attribut `data-xdh-onevent="Select"` qui fera l'objet de la prochaine section.

Enfin, on ajoute l'appel à cette fonction dans la fonction `ac_connect(…)` ,appelée appelée à chaque nouvelle session :

```python
def ac_connect(dom):
  dom.inner("",open("Main.html").read())
  display_contacts(dom)
```

## Détail d'un contact (`part3.py`)

### Fonction générale d'affichage

On va maintenant remplir les champs au sommet de l'interface avec les valeurs du contact sélectionné dans la liste. On va définir la fonction correspondante :

```python
def display_contact(contactId,dom):
  dom.set_values(contacts[contactId])
```

La fonction `set_values(…)` prend un dictionnaire avec, pour clefs, l'identifiant d'un élément, et, pour valeurs, le contenu que doit prendre cet élément.  
Comme, dans la page *HTML*, les éléments ont pour identifiants les clefs correspondants aux champs d'un contact, le dictionnaire existe déjà et n'est plus à construire. On l'utilise donc tel quel dans l'appel de la méthode `set_values(…)`.  
`contactId` est l'index, dans la liste `contacts`, du contact à afficher. 

### Sélection d'un contact

On va maintenant créer la fonction que l'on va affecter à l'action `Select` définit dans l'attribut `data-xdh-onevent` du code *HTML* qui est crée dans la précedente section :

```python
def ac_select(dom,id):
  display_contact(int(id),dom)
```

Le paramètre `id` contient l'identifiant de l'élément recevant l'évènement ayant lancé l'action à laquelle cette fonction a été affectée.  
Dans la section précédente, on a vu que, pour le tableau *HTML* contenant la liste des contacts, chaque ligne à pour identifiant l'index, dans la table `contacts`, du contact correspondant. On peut donc utiliser directement `id`, aprés l'avoir convertit en entier (on le récupère sous forme d'une chaîne de caractères), pour le passer à la fonction `display_contact(…)`

On met à jour la table `CALLBACKS`, en affectant à l'action `Select` (définie comme valeur de l'attribut `data-on-event` dans le code *HTML* généré dans la précédente section) cette fonction:

```python
CALLBACKS = {
  "": ac_connect,
  "Select": ac_select
}
```

## Désactivation des champs + bouton *New* (`part4.py`)

Comme vous l'aurez sans doute remarqué, le contenu des champs dans lesquels s'affichent les détails sont modifiables, ce qui n'est pas le comportement voulu dans ce contexte. On va donc écrire le code permettant de désactiver ces champs.

### Champs à désactiver

Pour cela, on va d'abord créer une liste contenant les identifiants des différents  champs à désactiver :

```python
FIELDS = [
  "Name",
  "Address",
  "Phone",
  "Note"
]
 ```

### Fonction générale de gestion des éléments interactifs

 On va créer une fonction qui va gérer l'état de ces champs, et qui sera complété ultérieurement pour gérer d'autre éléments :

 ```python
def update_outfit(dom):
  dom.disable_elements(FIELDS)
 ```

Ici, cette fonction fait appel à la méthode `disable_elements`, dont le nom est assez explicite.

On va également utiliser cette fonction pour faire apparaître le bouton *New*, qui permet de saisir un nouveau contact.  
La classe `Display` étant affectée à ce bouton (voir le fichier `Main.html`), on va désactiver l'élément `style` d'identifiant `HideDisplay` (voir le fichier `Head.html`) qui définit la règle cachant les éléments de la classe, en utilisant la méthode `disable_element` (notez l'absence du `s` final). La fonction `update_outfit(…)` se présente alors de la manière suivante.

```python
def update_outfit(dom):
  dom.disable_elements(FIELDS)
  dom.disable_element("HideDisplay")
```

On pourrait éf=galement ajouter l'identifiant `HideDisplay` à la liste passée à `disable_elements(…)`, pour économiser un appel de fonction. 
 
### Mise en oeuvre de cette fonction

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

## Saisie d'un nouveau contact (`part5.py`)

On va maintenant gérer l'action affectée au bouton *New*. Pour cela, on va utiliser un objet qui va stocker dans quel mode est placé le logiciel, à savoir *édition* ou *affichage*.

### Les différents états de l'application

On va d'abord crée un *enum* relatifs à ces deux états, à l'aide du module *enum*, que l'on va importer en modifiant l'instruction d'importation existante :

```python
 import atlastk, enum
```

et pour créer l'*enum* proprement dit :

```python
class State(enum.Enum):
  DISPLAY = enum.auto()
  EDIT = enum.auto()
```

### Classe dédiée à chaque session

On va maintenant créer une classe `Board` dans laquelle on va stocker les différentes variables propres à chaque session :

```python
class Board:
  def __init__(self):
    self.state = State.DISPLAY
```

Le constructeur de cette classe (`__init__(…)`) va stocker l'état initial de l'application, à savoir `DISPLAY` (affichage), dans la variable membre `state`.

Il va falloir maintenant créer une instance de cette classe pour chaque nouvelle session. Ceci est realisé automatiquement par le *toolkit* *Atlas* ; il suffit de modifier l'appel à la fonction fonction `launch(…)` en remplaçant le paramètre de valeur `None` par le constructeur de cette classe, ce qui donne :

```python
atlastk.launch(CALLBACKS,Board,open("Head.html").read())
```

Ce faisant, toutes les fonctions référencées dans `CALLBACKS`, qui, je le rappelle, associe fonctions et actions, vont recevoir l'instance de l'objet `Board` correspondant à la session à l'origine de l'appel. Il faut donc modifier le prototype de ces fonctions :

```python
def ac_connect(board,dom):
  …

def ac_select(board,dom,id):
  …
```

Notez l'jout du paramètre `board`.

### Adaptation de la gestion des contrôles interactifs

On va passer ce paramètre à la fonction `update_outfit(…)` pour qu'elle puisse tenir compte de l'état dans lequel se trouve l'application, et réagir en conséquence, ce qui donne :

```python
def update_outfit(board,dom):
  if board.state == State.DISPLAY:
    dom.disable_elements(FIELDS)
    dom.disable_element("HideDisplay")
  elif board.state == State.EDIT:
    dom.enable_elements(FIELDS)
    dom.enable_elements("HideDisplay")
```

On voit l'apparition des méthodes `enable_element[s](…)`, qui sont le pendant des méthodes `enable_element[s](…)`.

### Autres adaptations

Il faut, bien entendu, également modifier les appels à cette fonction en conséquence ; on va également, par précaution, mettre à jour, dans l'instance `board`, l'état de m'application pour être sûr qu'il correspond à l'action lancée :

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

On notera que donner la valeur `None` au paramètre `contactId` entraînera maintenant le vidage des champs.

### Activation de la saisie

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
- affectation du focus (méthode `focus(…)`) au premier champs éditable (d'identifiant `Name`, qui correspond au nom du contact), de manière à ce que l'utilisateur puisse procéder immédiatement à la saisie du nouveau contact.

## Boutons de saisie (`part6.py`)

### Adaptation de la gestion des contrôles interactifs

On peut maintenant saisir un nouveau contact, mais il manque les boutons pour valider ou annuler cette saisie. Cela va se faire en désactivant l'élément `style` d'identifiant `HideEdition` (dans le fichier `Head.html`) qui définit une règle cachant les éléments auxquels on a affecté la classe `Edition`, ce qui est le cas de l'élément `div` qui contient les deux boutons d'édition `Submit` et `Cancel` (voir le fichier `Main.html`).  
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
```

### Confirmation/annulation d'une saisie

Maintenant que les boutons sont affichés, on va créer les fonctions associées.

Pour le bouton *Cancel*, on va demander confirmation et, en fonction de la réponse, ne rien faire, ou repasser en mode d'affichage aprés avoir vider les champs de saisie :

```python
def ac_cancel(board,dom):
  if dom.confirm("Are you sure?"):
    display_contact(None,dom)
    board.state = State.DISPLAY
    update_outfit(board,dom)
```

Comme l'on peut s'en douter, le méthode `confirm(…)` ouvre une boîte de dialogue affichant la chaîne de caractère passés en paramètre, et retourne `True` lorsque l'on clique sur le bouton *OK* (ou ce qui en tient lieu), ou `False` si on clique sur le bouton `Cancel` (ou ce qui en tent lieu), tout en fermant ladite boîte de dialogue.

Pour le bouton `Submit`, il s'agit de récupérer les valeurs des champs de saisie, de stocker lesdites valeurs dans ce qui tient lieu de base de donnée, à savoir la variable `contacts`, de rafraîchir la liste des contacts, et de rebasculer en mode saisie, tout cela sous condition que le champs `Name` contienne une valeur :

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

La méthode `get_values(…)` prend une liste de chaîne de caractères correspondants à des identifiants d'éléments, et retourne un dictionnaire avec, pour clefs, ces identifiants, et pour valeurs, le contenu des éléments correspondants. Comme les identifiants correspondent au nom des clefs d'un contact, ou peut stocker le dictionnaire obtenu tel quel.

La méthode `alert(…)` affiche simplement une boîte de dialogue contenant, comme message, la chaîne passée en paramètre, avec un bouton *OK* (ou équivalent) permettant de la fermer.

On termine en mettant à jour `CALLBACKS` pour affecter ces nouvelles fonctions aux actions adéquates :

```python
CALLBACKS = {
  "": ac_connect,
  "Select": ac_select,
  "New": ac_new,
  "Cancel": ac_cancel,
  "Submit": ac_submit
}
```

## Les autres boutons (`part7.py`)

Il nous restent deux boutons à afficher : le bouton d'édition (*Edit*) et le bouton de suppression.

### Adaptation de la classe `Board`

Avant toute chose, nous allons modifier la classe `Board` pour lui ajouter une variable (`contactId`) stockant l'index du contact sélectionné dans la liste, qui sera mis à `None` lorsqu'aucun contact n'est sélectionné :

```python
class Board:
  def __init__(self):
    self.state = State.DISPLAY
    self.contactId = None
```

Nous allons modifier `ac_select(…)` pour gérer cette nouvelle variable membre :

```python
def ac_select(board,dom,id):
  board.contactId = int(id)
  display_contact(board.contactId,dom)  
  …
```

### Adaptation de la gestion des contrôles interactifs

La variable ajoutée à la classe `Boars` va également nous servir pour l'affichage des boutons manquants. La classe `DisplayAndSelect` est affecté à ces boutons (voir le fichier `Main.html`), dont la règle *CSS* pour cacher les éléments de cette classe est défini dans l'élément *style* d'identifiant `HideDisplayAndSelect` (voir le fichier `Head.html`. On obtient donc cela :

```python
def update_outfit(board,dom):
  if board.state == State.DISPLAY:
    …
    if board.contactId == None:
      dom.enable_element("HideDisplayAndSelect")
    else:
      dom.disable_element("HideDisplayAndSelect")
  elif board.state == State.EDIT:
    …
    dom.enable_elements(("HideDisplay","HideDisplayAndSelect"))
    …
```

### Modification d'un contact

Passons à la fonction qui sera associée au bouton *Edit*. Elle reprendra en grande partie le contenu de la fonction `ac_new(…)` (on pourrait d'ailleurs en factoriser une partie) :

```python
def ac_edit(board,dom):
  board.state = State.EDIT
  display_contact(board.contactId,dom)
  update_outfit(board,dom)
  dom.focus("Name")
```

Il faut aussi modifier la fonction `ac_submit(…)`, pour tenir compte de son exécution dans le cadre du modification d'un contact :

```python
def ac_submit(board,dom):
  …
  else:
    board.state = State.DISPLAY
    if board.contactId == None:
      contacts.append(idsAndValues)
    else:
      contacts[board.contactId] = idsAndValues
    display_contact(board.contactId,dom)
    display_contacts(dom)
    …
```

Et également la fonction `ac_cancel(…)` pour la même raison : 

```python
  if dom.confirm("Are you sure?"):
    display_contact(board.contactId,dom)
    board.state = State.DISPLAY
    update_outfit(board,dom)
```

Et mettons à jour `CALLBACKS` :

```python
CALLBACKS {
  …
  "Edit": ac_edit
}
```

### Suppression d'un contact

Implémentons maintenant la fonction qui sera associée au bouton *Delete*, qui ne présente rien de particulier, au regard de ce qui a été abordé dans les précédentes sections :

```python
def ac_delete(board,dom):
  contacts.pop(board.contactId)
  board.contactId = None;
  display_contact(None,dom)
  display_contacts(dom)
  update_outfit(board,dom)
```

Et mettons à jour `CALLBACKS` :

```python
CALLBACKS {
  …
  "Delete": ac_delete
}
```

## Bonus (`part8.py`)

Comme vous avez pu le constater, la variable `contacts` est globale, ce qui signifie qu'elle est commune à toutes les sessions. Cependant, une modification apportée à cette variable par une session n'est pas immédiatement visible dans toutes les sessions. L'objet de cette section est d'apporter les modifications au code pour remédier à cela.

On va se limiter à rafraîchir la liste des contacts, dés qu'une modification y est apportée, dans l'ensemble des sessions. À noter qu'il y aurait bien plus à faire, notamment protéger l'accès à la variable `contacts` pour contrôler les tentatives d'accès simultanée, mais cela sort du cadre de ce tutoriel.

Pour commencer, on va créer une fonction qui va rafraîchir la liste des contacts :

```python
def ac_refresh(board,dom):
  display_contacts(dom)
```

Comme vous pouvez le constater, elle présente des similitudes, concernant les paramètres qu'elle reçoit, avec les fonctions associées à des actions (`ac_edit(…)`, `ac_submit(…)`…). Cela n'a rien d'étonnant, car on va effectivement l'associer à une action :

```python
CALLBACKS = {
  …
  "Refresh": ac_refresh
}
```

Et maintenant, on va remplacer, dans les fonctions qui modifient la liste des contacts, à savoir `ac_submit(…)` et `ac_delete(…)`, chaque appel à la fonction `display_contacts(dom)` par un appel à `atlastk.broadcast_action("Refresh")`.

```python
def ac_submit(board,dom):
  …
    display_contact(board.contactId,dom)
    atlastk.broadcast_action("Refresh")
    update_outfit(board,dom)


def ac_delete(board,dom):
  …
  display_contact(None,dom)
  atlastk.broadcast_action("Refresh")
  update_outfit(board,dom) 

```

Cette fonction va lancer l'action dont le libellé est passé en paramètre, à savoir `Refresh` ici, dans toutes les sessions, ce qui va provoquer l'appel à la fonction `display_contacts(…)`, et ainsi la liste des contacts sera rafraîchie dans toutes les sessions.

Encore une fois, cela ne suffit pas pour gérer correctement le partage de la liste des contacts entre les sessions, et le programme est facile à mettre en défaut en l'état, mais le code nécessaire pour remédier à cela n'étant pas directement lié au *toolkit* *Atlas*, il ne sera pas abordé dans ce tutoriel.


