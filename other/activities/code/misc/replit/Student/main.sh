#!/bin/bash

cd ..
ACTIVITY=Hangman
REPO=$ACTIVITY-activity-fr
git clone https://github.com/epeios-q37/$REPO
mv $REPO/* $REPL_SLUG
mv $REPO/.replit $REPL_SLUG
rm $REPL_SLUG/main.sh