Files in this directory are directly red by the httpd server ; the 'xdhbrwq' tool doesn't read them.

----

FOR THE MAINTAINER :

Créer un lien ('mklink /j') en tant qu'administrateur à partir du répertoire 'htdocs' de l'installation d'Apache vers ce répertoire.
Ex : 'C:\wamp\www>mklink /j xdh h:\hg\epeios\tools\xdhbrwq\htdocs'

Le répertoire 'js' est un 'mklink /j' vers le répertoire de même nom dans 'corpus'.