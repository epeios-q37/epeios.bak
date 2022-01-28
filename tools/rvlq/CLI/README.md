<!-- À part la fin, ce document est une copie du début de la page dédiée du site epeios.q37.info  -->

# The *RVLq* command-line tool

[![MacOS](https://github.com/epeios-q37/rvlq-cli/actions/workflows/MacOS.yml/badge.svg)](https://github.com/epeios-q37/rvlq-cli/actions/workflows/MacOS.yml) [![Ubuntu](https://github.com/epeios-q37/rvlq-cli/actions/workflows/Ubuntu.yml/badge.svg)](https://github.com/epeios-q37/rvlq-cli/actions/workflows/Ubuntu.yml) [![Windows](https://github.com/epeios-q37/rvlq-cli/actions/workflows/Windows.yml/badge.svg)](https://github.com/epeios-q37/rvlq-cli/actions/workflows/Windows.yml)

## Description

*RVLq* is a command-line tool which replaces tags, mostly in a file, with text, content of a file or the output of a command.

## Example

A source file, named `Source.txt`, which contains tags, delimited by `$`:
```text
Text 1: $text1$
Text 2: $text2$
Text 3: $text3$
Text 4: $text4$
Text 1 again followed by text 5: $text1$, $text5$
File content: $file$
Command output: $command$
```

The tag file, named `Tags.txt`, containing the definition of the tags in above file:
```text
text1	&Simple text
text2	Text on 2 lines in the tags file, \
but displayed on one line thanks to the '\\' before the newline

# A comment.
text3	Text on one line in the tags file,\nbut displayed on 2 lines thanks to '\\n'
text4	Text with a tabulation here:>\t< thanks to '\\t'
text5	and another simple texte
file  	%File.txt
command	@echo Text issued by the echo command
```

Another file, named `File.txt`, which is referenced in above file:
```text
This is the content of a file!
```

Launching `rvlq Tags.txt Source.txt` outputs following:
```text
Text 1: Simple text
Text 2: Text on 2 lines in the tags file, but displayed on one line thanks to the '\' before the newline
Text 3: Text on one line in the tags file,
but displayed on 2 lines thanks to '\n'
Text 4: Text with a tabulation here:>	< thanks to '\t'
Text 1 again followed by text 5: Simple text, and another simple texte
File content: This is the content of a file!
Command output: Text issued by the echo command
```

<!-- Ici se termine la partie copiée de la page du site epeios.q37.info -->

## Resources

More information about *RVLq* : <https://q37.info/s/3zhxvhdx>.
