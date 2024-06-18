Version : 2.7.4 | Client : Les génies du ponant | Produit : Machine à courber les bananes | Date : 08/12/2023
# COMPILER ET LANCER UN PROGRAMME DEPUIS LE TERMINAL LINUX

Bonjour à toi jeune informaticien, ce tuto consiste à te montrer comment 
- Compiler
- Exécuter
- Utiliser un programme codé en C   

*Et tout cela depuis un terminal Linux.*  

**Vous devez tout de même avoir installé le compilateur `GCC` au préalable.**

## Trouver le programme

*La première étape consiste  à **aller chercher votre programme**. Pour cela, vous devez vous placer à l'endroit où se trouve votre programme.*

### Etape 1
#### Ouvrir le terminal :
Appuyez sur la touche Windows et saisissez `terminal` et ouvrez le.
### Etape 2
#### Se déplacer jusqu'à son fichier :
À l'aide de la commande `cd` déplacez vous jusqu'au dossier où se trouve votre code en C.

## Compiler le programme 

*Vous devez maintenant compiler votre programme afin de pouvoir l'exécuter par la suite.*

### Etape 1
#### Trouver le nom de votre programme
*Pour compiler votre programme, il vous faut le nom de votre programme *CQFD*.*  

Pour cela, si vous ne connaissez pas le nom de votre programme, vous pouvez utiliser la commande `ls` afin de lister les fichiers du répertoire.  

Pour notre exemple, nous utiliserons un ficher nommé `exemple.c`.

### Etape 2
#### Compiler le programme

Pour compiler le programme, il vous suffit de rentrer cette commande.
```
cc programme.c -o programme -Wall
```
Plusieurs cas apparaissent alors.

|Cas 1        |Cas 2                |Cas 3          |
|-------------|---------------------|---------------|
|Aucune erreur|Erreurs dans le code |Autres erreurs |
|C'est parfait| Modifiez le programme et rééssayez | Courage...  |       

### Etape 3
#### Exécuter et utiliser le programme
Une fois que votre programme est bien compilé **sans erreurs donc** il ne reste plus qu'à l'utiliser, *enfin...*

Pour cela, toujours dans le terminal, utilisez la commande suivante 
```
./programme
```

**Il ne faut pas inclure l'extension dans la requête, c'est-à-dire le `.c`.**

Votre programme devrait donc pouvoir être utilisé sans problème `:)`.

*A condition évidemment que vous soyez un programmeur digne de ce nom...*

## Le petit plus

Si toutefois tu ne comprends rien à ce que je viens de dire, je te mets à disposition une petite vidéo qui illustre bien ce que je viens d'expliquer !!

[Voir la vidéo](https://www.youtube.com/watch?v=M04I-RPhYwQ)

**@copyright** *FAVER Sasha - GOURLAOUEN Cyril - TOULLEC Antoine - BROSSARD Lilian*





