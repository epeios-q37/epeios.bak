# rvlq-cli

[![MacOS](https://github.com/epeios-q37/rvlq-cli/actions/workflows/MacOS.yml/badge.svg)](https://github.com/epeios-q37/rvlq-cli/actions/workflows/MacOS.yml) [![MacOS](https://github.com/epeios-q37/rvlq-cli/actions/workflows/MacOS.yml/badge.svg)](https://github.com/epeios-q37/rvlq-cli/actions/workflows/MacOS.yml) [![Windows](https://github.com/epeios-q37/rvlq-cli/actions/workflows/Windows.yml/badge.svg)](https://github.com/epeios-q37/rvlq-cli/actions/workflows/Windows.yml)

*RVLq* replaces tags with text, file content or command output.

Let's take a file named `Source.txt` which contains tags (delimited by `$`):

```
Text 1: $text1$
Text 2: $text2$
Text 3: $text3$
Text 4: $text4$
Text 1 again followed by text 5: $text1$, $text5$
File content: $file$
Command output: $command$
```

a file named `Tags.txt` which contains the definition of the tags of above file (nothing or `&` for text tags, `%` for file content tags and `@` for command output tags; `#` are for comments, and blank line or fields are ignored):

```
text1		&Simple text
text2		Text on 2 lines in the tags file, \
but displayed on one line thanks to the '\\' before the newline

# A comment.
text3		Text on one line in the tags file,\nbut displayed on 2 lines thanks to '\\n'.
text4		Text with a tabulation here:>\t< thanks to '\\t'
text5       and another simple texte
file		%File.txt
command		@echo -n Current date and time: `date`
```

and a file named `File.txt`, which is referenced in above file:
```
This is the content of a file!
```

Launching `rvlq Tags.txt Source.txt` gives following output:
```
Text 1: Simple text
Text 2: Text on 2 lines in the tags file, but displayed on one line thanks to the '\' before the newline
Text 3: Text on one line in the tags file,
but displayed on 2 lines thanks to '\n'.
Text 4: Text with a tabulation here:>	< thanks to '\t'
Text 1 again followed by text 5: Simple text, and another simple texte
File content: This is the content of a file!
Command output: Current date and time: mardi 11 janvier 2022, 09:39:05 (UTC+0100)
```
