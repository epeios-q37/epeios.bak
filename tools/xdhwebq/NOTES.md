# Notes concernant *xdhwebq* (toutes versions)

Dans le répertoire `htdocs`, le fichier `qrcode.min.js` contient le code utilisé pour génèrer le code *QR* et `qrcode(.min).js.LICENSE` est la licence d'utilisation de ce code, dont la mise à dispsition est imposée par le code en question.

## Environnement de développement

### Configuration de *lighttpd*

*NOTA*:

- sous *Windows*, le *document root* de *lighttpd* se situe à `C:\Users\<user>\AppData\Local\lxss\rootfs\var\www\html` ;

- sous *GNU/Linux*, vérifier que tous les chemins référencés par les liens ci-dessous ont les bons droits (par exemple, `/media/csimon` n'est accessible que par l'utilisateur *csimon*) ;

Procédure :

- installer *php-cgi* (le nom peut varier) ;

- `sudo lighty-enable-mod fastcgi-php` ;

- `sudo service lighttpd force-reload` ;

- Dans le *document root*, crée un lien à partir de :
  
  - `…/epeios/tools/xdhwebq/htdocs/prolog.php` ;
  
  - `…/epeios/tools/xdhwebq/htdocs/xdh.php` ;

- Dans un répertoire nommé `xdh` dans le *document root**, crée un lien à partir de :
  
  - `…/epeios/tools/xdhwebq/htdocs/qrcode.min.js` ;
  
  - `…/epeios/corpus/js/xdhtml.js` ;
  
  - `…/epeios/tools/xdhwebq/htdocs/xdhwebq.js` ;


