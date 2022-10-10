---
title: Accueil
weight: 1
---

# Bienvenue aux ateliers de programmation du *Epeios* project

## Présentation

De nos jour, la plupart des jeunes possèdent un smartphone. Il est donc plus facile de les intéresser au numérique en général et à la programmation en particulier en impliquant leur smartphone.

C'est pourquoi les programmes proposés ici ont une véritable interface graphique, au lieu de l'habituelle et austère interface texte. De plus, à chaque étape de la réalisation d'un programme, un [code QR](https://fr.wikipedia.org/wiki/Code_QR) donnant la possibilité d'y accéder à partir d'un smartphone est affiché. Ainsi, ils s'apercevront qu'ils peuvent utiliser leurs smartphones pour utiliser leurs propres applications, et non pas seulement à des applications comme *Facebook*, *YouTube*, *Twitter*….

En outre, ils pourront facilement donner accès à leurs programmes de n'importe quel dispositif équipé d'un navigateur web moderne connecté à internet, nptamment les smartphones de leurs proches. Ces derniers auront ainsi plus d'occasion de les encourager, et les inciter ainsi à progresser.

Vous trouverez plus de détails sur ce type d'exercices à l'adresse <https://q37.info/s/knf9hdwp>.

Les exercices présentés ici reproduisent les Étape s menant à l'élaboration d'un programme, chaque exercice s'appuyant sur le précédent. Ils renforcent l'intérêt des élèves pour la programmation par une utilisation concrète des concepts abordés par les nombreux exercices de programmations que l'on trouve par ailleurs.

Contrairement à la plupart des autres exercices de programmation, chaque exercice ne porte pas sur un concept informatique, mais sur l'élaboration d'une fonctionnalité du programme. L'assimilation des concepts en est ainsi facilitée, car n'étant pas une fin en soi, mais une Étape dans la réalisation du programme.

Pour confèrer une dimension ludique à ces exercices, le programme en question est un jeu, le [jeu du pendu](https://fr.wikipedia.org/wiki/Le_Pendu_(jeu)).

Bien que s'appuyant sur le langage *Python*, ces exercices portent sur la programmation en générale, et non pas sur la programmation en *Python*. C'est pour cela que les solutions générales seront données avant les solutions propres à *Python*.

Si vous avez des questions/suggestions/remarques/… concernant ces exercices, vous pouvez contacter l'auteur via le formulaire de contact à l'adresse <https://q37.info/s/ggq7x4w7>, ou utiliser les système de suivi de *GitHub* du dépôt principal à l'adresse <https://github.com/epeios-q37/hangman-exercises>.

## Utilisation

Pour aborder ces exercices, seules des notions de base en développement *Python* sont nécessaires. Ils sont particulièrement indiqués pour faire découvrir ou initier à la programmation.

Le dépôt https://github.com/epeios-q37/hangman-fr est destiné aux élèves. https://github.com/epeios-q37/hangman-fr-teacher reprend le contenu de ce dépôt, en y ajoutant des informations utiles aux enseignants, notamment les solutions des exercices. De ce fait, ce dernier dépôt est réservé aux enseignants.

Pour distribuer ces exercices aux élèves, vous pouvez directement utiliser le dépôt https://github.com/epeios-q37/hangman-fr ou, si vous êtes familiarisés avec *GitHub*, en utiliser les fonctionnalités pour créer votre propre version de ce dépôt.

Les solutions aux exercices présentées dans cette notice peuvent être vue à l'œuvre en allant à l'adresse <https://repl.it/github/epeios-q37/hangman-fr-teacher>. Les fichiers contenus dans le répertoire `Student` sont ceux mis à disposition des étudiants.

Si vous avez des retours à faire concernant ces exercices, vous pouvez contacter l'auteur à l'adresse <https://q37.info/s/ggq7x4w7>, ou passer par le dépôt maître, d'où sont dérivés les deux dépôts présentés ici, à l'adresse <https://github.com/epeios-q37/hangman-exercises>.

Il existe deux façons pour les élèves de travailler sur ces exercices. Soit en utilisant un ordinateur sur lequel est installé *Python* (utilisation en local), soit dans un navigateur web (utilisation en ligne).

### Utilisation en local

Pour une utilisation en local sur un ordinateur équipé de *Python*, il suffit aux élèves de télécharger et désarchiver le fichier suivant : <https://github.com/epeios-q37/hangman-fr/archive/master.zip>. Alternativement, s'ils possèdent les connaissances adéquates, ils peuvent aussi faire un `git clone https://github.com/epeios-q37/hangman-fr` à partir d'une console.

Les exercices consistent à écrire du code dans le fichier `pendu.py` situé à la racine du dépôt. Pour le lancement, il suffira d'exécuter la commande `python3 pendu.py` dans une console, à partir de la racine du dépôt. S'ouvrira alors un navigateur web leur donnant accès à leur exercice. Le programme sera arrêté avec un *CTRL-C*.

Ce dépôt contient, à la racine, un fichier `LISEZMOI.html` qui leur donne les instructions pour chaque exercice.

### Utilisation en ligne

Pour une utilisation en ligne en utilisant un navigateur web, sans rien avoir à installer, il suffit de suivre le lien suivant : https://repl.it/github/epeios-q37/hangman-fr. Cela ouvre une session dans *Repl.it*, un environnement de développement en ligne.

Le fichier `LISEZMOI.html` leur donnant les instructions pour chaque exercice se situe à l'adresse https://q37.info/s/zmg4m3fx. Il faut l'ouvrir via cette adresse, car l"accès à ce fichier via *GitHub* ou *Repl.it* n'en affichera que le code source.

Comme ci-dessus, l'exercice consistera à écrire du code dans le fichier `pendu.py`. Pour lancer l'exercice, il suffira de cliquer sur le bouton vert. Cela provoquera l'affichage d'une URL que l'on cliquera pour accèder à l'interface de l'exercice.

Pour revenir au code source, on fermera l'onglet (ou la fenêtre) qui s'est ouverte suite au click sur l'URL. Pour relancer l'application, il suffira de cliquer à nouveau sur le bouton vert (dont le libellé change en fonction du contexte).

*Repl.it* ne nécessite pas la création d'un compte pour être utilisé. Néanmoins, ouvrir un compte permet d'y stocker ses projets et de les retrouver d'une session à l'autre. On peut aussi, sans compte, sauvegarder en local l'ensemble d'un projet.

## Les exercices

Les informations données ici viennent en complément de celles indiquées dans le fichier `Student/LISEZMOI.html`, qui est la notice destinée aux étudiants.

**ATTENTION** : ne pas oublier de modifier la ligne `from workshop.fr.… import *` en fonction de l'exercice.

