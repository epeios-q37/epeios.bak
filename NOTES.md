# Developer's reminder (hence written in french)

## La section `RTW` dans les fichiers `Project.xml`.

Cette section est pour la publication *web* (plus précisément, sur *Github*) d'un composant logiciel.

``` xml
<RTW Description="DESCRIPTION" Keywords="KEYWORDS">
 <Dir>DIR</Dir>
</RTW>
```

  * `DESCRIPTION` : la description du composant,
  * `KEYWORDS` : une liste de mots-clefs associés au composant, séparés par des virgules (`,`), sans espaces (ex.: `electron,XDHTML,wrapper,native,desktop,ui`),
  * `DIR` : le nom du *repository* *GitHub* (sera également le nom du répertoire dans `H:\RTW\`).

  `DESCRIPTION` et `KEYWORDS` ne seront pas systématiquement utilisés, mais `DIR` doit être correctement défini. Pour certains types de composants, en plus des mots-clefs définis dans `KEYWORD`, d'autres mots-clefs peuvent être automatiquement ajoutés.


  <file://h:/hg/epeios/notes.md>

  ## *hg-git*

  Aller à <https://bitbucket.org/durin42/hg-git/src/default/>, sélectionner un *tag* (à priori, le dernier), et vérifier dans le *Makefile* si la version de *mercurial* est  gérée. Changer de *tag* si nécessaire. Puis aller dans <https://bitbucket.org/durin42/hg-git/downloads/?tab=tags>, télécharger l'archive souhaitée, et remplacer le répertoire `~/hg/hg-extensions/hg-git` par celui contenu dans l'archive.

  *Dulwich* peut être chargé à partir du gestionnaire de logiciel.

  *NOTA* : il semblerait que le *pligin* soit disponible tel quel dans le gestionnaire de logiciels (*Mercurial-git*)…