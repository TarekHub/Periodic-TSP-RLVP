#!/bin/bash

# Fichier d'entrée
input_file="./calais312_init.tours"

# Fichier de sortie
output_file="./calais312_init_null.tours"

# Traitement du fichier
awk '
    /^DAY/ {print $0 > "'$output_file'"; next}
    /^LENGTH/ {print "LENGTH = 0" >> "'$output_file'"}
    /^DIMENSION/ {print "DIMENSION = 1" >> "'$output_file'"}
    /^[0-9]+$/ {if ($1 == "1") print "1" >> "'$output_file'"}
    END {print "" >> "'$output_file'"} # Ajoute une ligne vide à la fin
' $input_file
