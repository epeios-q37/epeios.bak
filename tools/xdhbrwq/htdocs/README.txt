Files in this directory are directly red by the httpd server ; the 'xdhbrwq' tool doesn't read them.

----

FOR THE MAINTAINER :

Créer un lien ('mklink /j') en tant qu'administrateur à partir du répertoire 'htdocs' de l'installation d'Apache vers ce répertoire.
Ex : 'C:\Program Files (x86)\Apache Software Foundation\Apache2.2\htdocs>mklink /j xdh h:\hg\epeios\tools\xdhbrwq\httpd\htdocs'

Le répertoire 'js' est un 'mklink /j' vers le répertoire de même nom dans 'corpus'.