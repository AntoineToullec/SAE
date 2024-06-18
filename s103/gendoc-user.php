#!/usr/bin/php

<?php
    $markdownFiles = glob('*.md'); // Get all .md files in the current directory

    $fichierhtml = fopen('doc-user.html', 'w');

    $stylesCSS = "
    <style>
        body{
            background-color: #1f1f1f;
            color: white;
        }
        #infos{
            color: #888888;
            font-size: small;
        }
        .center{
            text-align: center;
        }

        h3 {
            text-decoration: underline;
        }
        h2{
            color : rgb(255, 255, 255);
            text-decoration: underline;
        }
        h1{
            color : rgb(255, 255, 255);
            text-decoration: underline;
        }
        a{
            color : rgb(116, 192, 255);
        }
        section,header,footer,.title,.other{
            background-color: rgb(49, 49, 49);
            border-radius: 0 20px ;
            padding: 0 40px;
        }

        section > hr{
            height: 1px;
            background-color: black;
        }
        code {
            background-color: #eee;
            border-radius: 3px;
            padding: 0 3px;
            color: #1f1f1f;
        }
        span {
            border-radius: 3px;
            padding: 0 3px;
            color: orange;
        }
        table {
        border-collapse: collapse;
        width: 40%;
        }

        th, td {
            border: 2px solid rgb(255, 255, 255);
            padding: 8px;
        }
    </style>
    ";

    fwrite($fichierhtml, $stylesCSS);
    
    foreach ($markdownFiles as $markdownFile) {
        $fichier = file_get_contents($markdownFile);
        $lignes = explode("\n", $fichier);
    
        foreach ($lignes as $ligne) {
            $lignepur = trim($ligne);
            $texte = null;
    
            /*Recherche des titres de niveau 4 à 1*/
            for ($titre = 4; $titre >= 1; $titre--) {
                $hashtags = str_repeat("#", $titre);
    
                if (substr($lignepur, 0, $titre) === $hashtags) {
                    $texte = trim(substr($lignepur, $titre));
                    if ($titre == 1){
                        fwrite($fichierhtml, "<section><h$titre class=\"center\">$texte</h$titre>\n");
                    }
                    elseif ($titre == 2) {
                        fwrite($fichierhtml, "</section><section><h$titre>$texte</h$titre>\n");
                    }
                    else {
                        fwrite($fichierhtml, "<h$titre>$texte</h$titre>\n");
                    }
                    break;
                }
            }
    
            if (substr($lignepur, 0, 1) === '-') {
                $texte = trim(substr($lignepur, 1));
                fwrite($fichierhtml, "<ul><li>$texte</li></ul>\n");
            }

            if (preg_match('/^Version : (.+)/', $lignepur, $versionMatches)) {
                $versionInfo = $versionMatches[1];
                $configFile = __DIR__ . '/config';
                $version = trim(shell_exec("grep '^VERSION=' $configFile | cut -d'=' -f2"));
                $nomclient = trim(shell_exec("grep '^CLIENT=' $configFile | cut -d'=' -f2"));
                $produit = trim(shell_exec("grep '^PRODUIT=' $configFile | cut -d'=' -f2"));
                $date = trim(shell_exec("date '+%d/%m/%Y'"));
            
                $texte = "Version : $version | Client : $nomclient | Produit : $produit | Date : $date";
                fwrite($fichierhtml, "$texte\n");
            }


    
            if (strpos($lignepur, '*') !== false || strpos($lignepur, '`') !== false) {
                $texte = $lignepur;
                $texte = preg_replace('/\*{2}([^*]+)\*{2}/', '<strong>$1</strong>', $texte);
                $texte = preg_replace('/\*([^*]+)\*/', '<em>$1</em>', $texte);
                $texte = preg_replace('/`([^`]+)`/', '<span>$1</span>', $texte);
                fwrite($fichierhtml, "<p>$texte</p>\n");
            }
    
            if (preg_match('/\[(.*?)\]\((.*?)\)/', $lignepur, $matches)) {
                $texte = $matches[1];
                $lien = $matches[2];
                fwrite($fichierhtml, "<a href=\"$lien\">$texte</a>\n");
            }
    
            if ($texte === null || $texte === "") {
                fwrite($fichierhtml, "<p>$lignepur</p>\n");
            }
    
        }

        fwrite($fichierhtml, "</section>\n");
    }
?>
