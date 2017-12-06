Files in this directory are directly red by the httpd server; the *XDHBrwQ* tool doesn't read them.

----

## FOR THE MAINTAINER

### Avec *lighttpd* (méthode préfèrée)

Voir <http://q37.info/computing/tools/lighttpd/> pour l'installation de *lighttpd* avec *PHP*.

Sous *WSL*, faire un lien (`ln -s`) :

 * de `/mnt/h/hg/epeios/xdhbrwq/htdocs` **vers** `/var/www/html/xdh/`,
 * de `/mnt/h/hg/q37/Auto/js` **dans** `/var/www/html/xdh/`. 


### Avec *WampServer*
Créer un lien (`mklink /j` ) en tant qu'administrateur à partir du répertoire  `htdocs` de l'installation d'Apache vers ce répertoire.
Ex : `C:\wamp\www>mklink /j xdh h:\hg\epeios\tools\xdhbrwq\htdocs`

Le répertoire `js` est un `mklink /j` vers le répertoire de même nom dans `corpus`.