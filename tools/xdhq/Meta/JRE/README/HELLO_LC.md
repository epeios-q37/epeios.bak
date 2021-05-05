- on *Windows* (pay attention to the `;` on last line):
```bash
# You can replace 'github.com' with 'framagit.org'.
# DON'T copy/paste this and above line!
git clone https://github.com/epeios-q37/atlas-java
cd atlas-java/examples/Hello
java -cp .;../../Atlas.jar Hello
```

- on other platforms (pay attention to the `:` on last line):
```bat
REM You can replace 'github.com' with 'framagit.org'.
REM DON'T copy/paste this and above line!
git clone https://github.com/epeios-q37/atlas-java
cd atlas-java/examples/Hello
java -cp .:../../Atlas.jar Hello
```