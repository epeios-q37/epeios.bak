# Notes pour le développeur

- `DVLGenGyp` créer le fichier `binding.gyp` nécessité par la commande `node-gyp` (actuellement, certaines variables doivent être ajoutée pour ce que ce fichier soit utilisable) ;

- pour créer les fichiers nécessaires à la compilation  : `node-gyp configure`  ;

- pour créer le binaire (`njss.node`)  : `node-gyp build`.

`node-gyp configure` créer un `.vcxproj` (dans le répertoire `build`). De ce fait, il est plus facile d'utilisre *Windows* pour mettre cet utilitaire au point.
