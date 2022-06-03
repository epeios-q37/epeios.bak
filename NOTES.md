# Developer's reminder (hence written in french)

## La section `RTW` dans les fichiers `Project.xml`.

Cette section est pour la publication *web* (plus précisément, sur *Github*) d'un composant logiciel.

```xml
<RTW Description="DESCRIPTION" Keywords="KEYWORDS">
 <Dir>DIR</Dir>
</RTW>
```

* `DESCRIPTION` : la description du composant,
* `KEYWORDS` : une liste de mots-clefs associés au composant, séparés par des virgules (`,`), sans espaces (ex.: `electron,XDHTML,wrapper,native,desktop,ui`),
* `DIR` : le nom du *repository* *GitHub* (sera également le nom du répertoire dans `H:\RTW\`).

`DESCRIPTION` et `KEYWORDS` ne seront pas systématiquement utilisés, mais `DIR` doit être correctement défini. Pour certains types de composants, en plus des mots-clefs définis dans `KEYWORD`, d'autres mots-clefs peuvent être automatiquement ajoutés.

## *hg-git*

*hg-git* permet de mettre à jour le dépôt *GitHub* automatiquement à chaque mise à jour du dépôt *GitHub*.

Aller à <https://bitbucket.org/durin42/hg-git/src/default/>, sélectionner un *tag* (à priori, le dernier), et vérifier dans le *Makefile* si la version de *mercurial* est  gérée. Changer de *tag* si nécessaire. Puis aller dans <https://bitbucket.org/durin42/hg-git/downloads/?tab=tags>, télécharger l'archive souhaitée, et remplacer le répertoire `~/hg/hg-extensions/hg-git` par celui contenu dans l'archive.

*Dulwich* peut être chargé à partir du gestionnaire de logiciel.

*NOTA* : il semblerait que le *plugin* soit disponible tel quel dans le gestionnaire de logiciels (*Mercurial-git*)…

## Utilisation contenu du dépôt *mercurial* sur *Savannah* (ou du dépôt *Git* sur *Github*)

*NOTA* : la définition de certaines variables d'environnement sera nécessaire. Cela devra se faire avec `export <nom>=<valeur>` sous *linux*-like et `setenv <nom> <valeur>` sous *FreeBSD* (le `setenv…` devrair également fonctionner sous *Linux*-like).

### Récupération du dépôt

- Depuis *Savannah* (https://savannah.nongnu.org/hg/?group=epeios) : `hg clone https://hg.savannah.nongnu.org/hgweb/epeios/` ;
- depuis *GitHub* (https://github.com/epeios-q37/epeios) : `git clone https://github.com/epeios-q37/epeios.git`.

Il faut placer le dépôt dans un répertoire nommé `hg` (voir ci-dessous).

### Compilation

Pour que la compilation fonctionne, il faut définir la variable d'environnement suivante : `Q37_EPEIOS` : `<path-to-hg>/hg/epeios/`.

Pour la compilation proprement dite, il y a un *Makefile* pour chaque binaire. Ce *Makefile* doit être traité par *GNU* *make* (`gmake` sour *FreeBSD*).

### Exécution

Pour que les fichiers `.xlcl` et `.xcfg` propres à chaque binaire soient traités correctement, il faut définir la variable d'environnement suivante : `Q37_XPP`, `'<xpp:bloc> <xpp:expand href="<path-to-hg>/hg/epeios/corpus/q37.xpp"/> </xpp:bloc>'`.