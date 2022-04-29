# Only for Replit.

import os, sys

if not "REPL_SLUG" in os.environ:
  print("This file is only of use on Replit.")
  print("See the 'README.md' file.")
  sys.exit()

PROGRAMS = [
  "Flooder",
  "Four in a Row",
  "Mancala"
]

while True:
  print()
  
  for i, p in enumerate(PROGRAMS):
    print( f"{i+1}: {p}")
  
  rawChoice = input(f"\nPlease choose one of above program (1 … {len(PROGRAMS)}): ")

  try:
    choice = int(rawChoice) - 1
    
    if int(choice) in range(len(PROGRAMS)):
      label = PROGRAMS[choice]
      program = label.translate(label.maketrans("", "", " '")).lower() + ".py"
  
      print(f"Launching '{program}'!\n")
  
      os.system(f"ATK=None python {program}")
    else:
      raise ValueError 
  except ValueError:
    print(f"\n'{rawChoice}' is not a valid value.")  