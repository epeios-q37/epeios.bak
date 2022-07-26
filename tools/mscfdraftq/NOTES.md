# Notes concernant l'utilitaire *mscfdraftq* (toutes variantes)

## Divers

Commandes *Linux* utile :
- `aplay` permet de jouer un fichier audio ; l'option `-l` liste les dispositifs audio ;
- `aplaymidi` permet de jouer un fichier *MIDI* ; l'option `-l`' liste les dipositifs *MIDI* de sortie ;
- `cat /proc/asound/cards` permet de lister les cartes audios présentes sur le système ;
- `fluidsynth` permet de lancer un*serveur* *MIDI* (voir ci-dessous).

## *fluidsynth*

Permet de lancer un serveur *MIDI*.

`fluidsynth --audio-driver=alsa -o audio.alsa.device=hw:<c>[,<d>] <soundfont> <midifile>` permet de tester les parmètres avant de lancer le serveur. `<c>` et `<d>` sont fournis par `aplay -l`, respectivement les identifiants de *card* et *device*. `<soundfont>` est un fichier à l'extension `sf2` (voire `sf3`). Ce genre de fichier se situe généralement dans `/usr/share/sounds/sf(2,3)/` (NOTA : à part celui indiqué ci-dessous, les fichiers *soundfont* présents dans les répertoires indiqués ne semblent pas fonctionner). `<midifile>` est un fichier à l'extension `.mid`.


Ce qui fonctionne avec mon système est : `fluidsynth --audio-driver=alsa -o audio.alsa.device=hw:0,3 /usr/share/sounds/sf2/FluidR3_GM.sf2 <midifile>`.

Pour lancer le serveur : `fluidsynth --server --audio-driver=alsa -o audio.alsa.device=hw:<c>[,<d>] <soundfont>`, ce qui donne, pour mon serveur, `fluidsynth --server --audio-driver=alsa -o audio.alsa.device=hw:0,3 /usr/share/sounds/sf2/FluidR3_GM.sf2`.

`aplaymidi -l` affiche maintenant le dispositif *MIDI* ainsi créer, et `aplaymidi <midifile>` interprète le fichier *MIDI* donné en paramètre.

`sudo modprobe snd-virmidi midi_devs=1`

`aconnect 24 128`
