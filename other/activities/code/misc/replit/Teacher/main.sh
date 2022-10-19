#!/bin/bash

# À renommer en 'main.sh' et à placer dans le dépôt 'GitHub' 'coding-workshops-replit'.

REPO="coding-workshops"
REPO_URL="https://github.com/epeios-q37/$REPO"

L_LABEL=("English" "français")
L_ID=("en" "fr")

A_ID=("Hangman")
A_MILESTONE=("l")

[ -d $REPO ] || ( git ls-remote -h $REPO_URL HEAD &> /dev/null && git clone $REPO_URL )

export PYTHONPATH=$PYTHONPATH:.:..:./../EduTK.zip:./../atlastk.zip

STATE="language"

languageId () {
  echo ${L_ID[$LANGUAGE]}
}

displayLanguage() {
  echo "Selected language: ${L_LABEL[$LANGUAGE]}"
}

activityId() {
  echo ${A_ID[$ACTIVITY]}
}

displayActivity() {
  echo "Selected activity: $(activityId)"
}

milestoneId() {
  echo $(chr $(( $MILESTONE + $(ord "a") )) )
}

displayMilestone() {
  echo "Selected milestone: $(milestoneId)"
}

chr() {
  [ "$1" -lt 256 ] || return 1
  printf "\\$(printf '%03o' "$1")"
}

ord() {
  LC_CTYPE=C printf '%d' "'$1"
}

testAndSetState () {
  local answ=$1
  local last=$2
  local prev=$3
  local next=$4

  if [[ "$answ" == "0" ]]; then STATE=$prev; fi

  if [[ $(ord ${answ,}) -ge $(ord "a") && $(ord ${answ,}) -le $last ]]; then
    STATE=$next
  fi

  RESULT=$(( $(ord ${answ,}) - $(ord "a") ))
}

choose () {
  local prev=$1; shift
  local next=$1; shift
  local array=("$@")

  local choice=0

  echo

  for entry in "${array[@]}"; do
    echo -e "\t$(chr $(( $choice + $(ord 'a') )) )) ${array[$choice]}"
    choice=$(( $choice + 1 ))
  done

  echo -e "\n\t0) back\n"

  read -n 1 -p "Please enter your choice: " answer

  echo

  testAndSetState $answer $(( ${#array[@]} + $(ord 'a') - 1 )) "$prev" "$next"
}

selectLanguage () {
  reset
  echo "Select language:"

  choose "quit" "activity" "${L_LABEL[@]}"
  LANGUAGE=$RESULT
}

selectActivity() {
  reset
  echo
  displayLanguage
  echo

  echo "Select activity:"

  choose "language" "milestone" "${A_ID[@]}"
  ACTIVITY=$RESULT
}

function selectMilestone() {
  local last=$(ord $1)

  reset
  echo
  displayLanguage
  displayActivity
  echo

  read -n 1 -p "Select milestone ('a'…'$(chr $last)', '0': back): " answer

  echo

  testAndSetState $answer $last "activity" "launch"

  MILESTONE=$RESULT
}

launch () {
  echo
  displayLanguage
  displayActivity
  displayMilestone
  echo

  pushd $REPO/$(activityId)
  ATK=none python3 $(languageId)/$(milestoneId).py
  popd
}

while [ "$STATE" != "quit" ]; do
  case $STATE in
  "language") selectLanguage;;
  "activity") selectActivity;;
  "milestone") selectMilestone "${A_MILESTONE[$ACTIVITY]}";;
  "launch") STATE="milestone"; launch;;
  "quit") break;;
  esac
done