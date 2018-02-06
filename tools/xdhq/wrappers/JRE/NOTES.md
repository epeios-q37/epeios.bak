 [Epeios NOTES.md](<file://h:/hg/epeios/notes.md>)

 Script pour *recompiler* l'ensemble des sources *JAVA*  :

 `pushd /cygdrive/h/hg/epeios/tools/jreq;rm -rf classes/*;javac -d classes *.java;cd /cygdrive/h/hg/epeios/tools/xdhq/wrappers/JRE;rm -rf classes/*;javac -d classes *.java;popd`
 