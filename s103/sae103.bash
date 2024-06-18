#!/bin/bash

#--------------------------------------------------------------------------------#
# Les images ne peuvent pas être éxécutés dans le bash à l'IUT c'est pourquoi elles sont en commentaire

# docker image pull sae103-php
# docker image pull sae103-html2pdf
# docker image pull clock

#--------------------------------------------------------------------------------#

docker volume create sae103

docker container run --name sae103-forever -v sae103:/work/ -d clock

#--------------------------------------------------------------------------------#
# On copie chaque fichier.c
docker container cp src1.c sae103-forever:/work
docker container cp src2.c sae103-forever:/work
docker container cp src3.c sae103-forever:/work
docker container cp doc.md sae103-forever:/work
docker container cp gendoc-tech.php sae103-forever:/work
docker container cp gendoc-user.php sae103-forever:/work

#--------------------------------------------------------------------------------#
#Version
config_file="config"

# Vérifier si le fichier config existe
if [ -e "$config_file" ]; then
    # Extraire la valeur de VERSION
    nomclient=$(grep "^CLIENT=" "$config_file" | awk -F "=" '{print $2}')
    version=$(grep "^VERSION=" "$config_file" | awk -F "=" '{print $2}')
    # Décomposer la version en major, minor et build
    major=$(echo "$version" | cut -d'.' -f1)
    minor=$(echo "$version" | cut -d'.' -f2)
    build=$(echo "$version" | cut -d'.' -f3)

    # Afficher les composantes de la version
    echo "================================================================="
    echo "La version est : $version"
    echo "Major : $major"
    echo "Minor : $minor"
    echo "Build : $build"
else
    echo "Le fichier config n'existe pas."
fi

echo "================================================================="
echo "Que voulez-vous faire ?"
echo "0. Pour ne rien changer à la version"
echo "1. Major, évolution majeure"
echo "2. Minor, évolution mineure, correction de bugs important"
echo "3. Build, évolution très légère, correctif esthétique par exemple"
echo "================================================================="
echo "Écrivez le numéro de votre choix."
read reponse
echo "================================================================="

# Affectation en fonction du choix
if [ "$reponse" -eq 1 ]; then
    major=$((major+1))
    minor=0
    build=0
elif [ "$reponse" -eq 2 ]; then
    minor=$((minor+1))
    build=0
elif [ "$reponse" -eq 3 ]; then
    build=$((build+1))
else
    echo "Vous avez choisi de ne pas changer la version."
fi

# Changement pour la nouvelle version
    new_version="$major.$minor.$build"

    # Mettre à jour le fichier config avec la nouvelle version
    awk -v new_version="$new_version" '/^VERSION=/ {$0="VERSION=" new_version} 1' "$config_file" > temp_config && mv temp_config "$config_file"
    # Afficher la nouvelle version
    echo "Le fichier config a été mis à jour avec la nouvelle version : $new_version"

    nommodif=$(echo "$nomclient" | tr [A-Z] [a-z] | tr " " "_")
#--------------------------------------------------------------------------------#
docker container cp config sae103-forever:/work
# Execution du conteneur php
docker container run --rm -v sae103:/work/ sae103-php php /work/gendoc-tech.php
docker container run --rm -v sae103:/work/ sae103-php php /work/gendoc-user.php

#--------------------------------------------------------------------------------#
# Execution du conteneur html2pdf

docker container run --rm -v sae103:/work/ sae103-html2pdf 'html2pdf doc-tech.html doc-tech.pdf'
docker container run --rm -v sae103:/work/ sae103-html2pdf 'html2pdf doc-user.html doc-user.pdf'

#--------------------------------------------------------------------------------#
# Mise en dossier
docker container cp sae103-forever:/work/ ./Rendu
mkdir ./Final
mv Rendu/*.pdf ./Final/
rm Rendu/*
rmdir Rendu/

#--------------------------------------------------------------------------------#
# Compression
tar -czvf $nommodif-$new_version.tgz Final/
rm Final/*
rmdir Final/
echo .tgz*
#--------------------------------------------------------------------------------#

# Vérification de la génération des fichiers
echo "Fichiers générés"
docker container exec sae103-forever ls work/
echo " "

# Procédures de fin de programme
echo "Fin du programme, PDF disponible"

docker container stop sae103-forever
docker container rm sae103-forever
docker volume prune -f
docker volume rm sae103

echo " "
docker volume ls
