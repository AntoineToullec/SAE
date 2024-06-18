<?php
//=====================================================================================================================================================
//------------------------------------------------------------------A REMPLIR--------------------------------------------------------------------------
//=====================================================================================================================================================

$configFile = __DIR__ . '/config';
// Vérifier si le fichier config existe
    // Lire les valeurs depuis le fichier config
    $version = trim(shell_exec("grep '^VERSION=' $configFile | cut -d'=' -f2"));
    $nomclient = trim(shell_exec("grep '^CLIENT=' $configFile | cut -d'=' -f2"));
    $produit = trim(shell_exec("grep '^PRODUIT=' $configFile | cut -d'=' -f2"));
    $date = trim(shell_exec("date '+%d/%m/%Y'"));

    // Construire la description
    $description = "Version: $version | Client: $nomclient | Produit: $produit | Date: $date";
//=====================================================================================================================================================
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//=====================================================================================================================================================

// nom du fichier html qui va être créé
$fichierHTML = "doc-tech.html";

function variable($handleC, $handleHTML,$var, $lenvar, $textvar){
    $varNb = 0;
    $ecrirevraName = false;
    $ecrirevarText = false;
    $varFind = false;
    $finligne = false;
    $varText = "";
    rewind($handleC);

    $caractereAvant ='';
    $caractereAvantAvant ='';
    $caractere = fgetc($handleC);


    while (!feof($handleC)) {


        if ($varNb == $lenvar) {
            $caractereAvantAvant = $caractereAvant;
            $caractereAvant = $caractere;
            $caractere = fgetc($handleC);
            while ($caractere == ' ' || $caractere == '\t') {
                $caractereAvantAvant = $caractereAvant;
                $caractereAvant = $caractere;
                $caractere = fgetc($handleC);
            }
            fwrite($handleHTML," ");
            
            
            $varText = "";
            while ($caractere != '/') {
                $varText .= $caractere;
                if ($caractere != '(') {
                    $caractereAvantAvant = $caractereAvant;
                    $ecrirevarName = true;
                    $varFind = true;
                }
                else {
                    $ecrirevarName = false;
                    $varFind = false;
                    break;
                }
                $caractereAvant = $caractere;
                $caractere = fgetc($handleC);
            }
            if ($ecrirevarName == true) {
                fwrite($handleHTML,"<article><h3>");
                fwrite($handleHTML,$textvar);
                fwrite($handleHTML,$varText);
                $ecrirevarName = false;
                fwrite($handleHTML,"</h3> <p class='indent'>");
                $caractere = fgetc($handleC);
                $caractere = fgetc($handleC);
                $caractere = fgetc($handleC);
                
                
                while ($caractereAvant != '*' && $caractere != '/') {
                    if ($caractere != '*' && $caractere != '/') {
                        fwrite($handleHTML,$caractere);
                    }
                    $caractereAvantAvant = $caractereAvant;
                    $caractereAvant = $caractere;
                    $caractere = fgetc($handleC);
                }
                fwrite($handleHTML,"</p><br><br></article><hr>");
            }
            $varNb = 0;
        }
        
        if ($varNb < $lenvar) {
            if ($finligne == true && $varNb == 0) {
                $varNb = $varNb + 1;
            }
            if ($var[$varNb] == $caractere) {
                $varNb = $varNb + 1;
            }
            else {
                $varNb = 0;
            }
        }
        $caractereAvantAvant = $caractereAvant;
        $caractereAvant = $caractere;
        $caractere = fgetc($handleC);
        if ($caractere == "\n") {
            $finligne = true;
        }
        elseif ($caractere == "\t" || $caractere == " " ) {
            $finligne = false;
        }

    }
}

function maindesc($handleC, $handleHTML){
    rewind($handleC);
    $caractereAvant ='';
    $caractereAvantAvant ='';
    $caractere = fgetc($handleC);
    while ($caractere == '/' || $caractere == '*' || $caractere == ' '|| $caractere == "\n") {
        $caractere = fgetc($handleC);
    }
    
    fwrite($handleHTML,'<p>');
    while (!feof($handleC)) {
        if ($caractere == '*') {
            if ($caractereAvantAvant == "\n" || $caractereAvant == "\n") {
                fwrite($handleHTML,'<br>');
            }
            else{
                fwrite($handleHTML,'*');
            }
        }
        else {
            if ($caractereAvant == '*') {
                if ($caractere == '/') {
                    fwrite($handleHTML,'</p>');
                    break;
                }
            }
            fwrite($handleHTML,$caractere);
        }
        $caractereAvantAvant = $caractereAvant;
        $caractereAvant = $caractere;
        $caractere = fgetc($handleC);
    }
    fwrite($handleHTML,'</section>');
}

function appelevariable($handleC, $handleHTML){
    $define = ["\n",'#', 'd', 'e', 'f', 'i', 'n', 'e'];
    $lendefine = 8;
    $textdefine = 'define ';
    $int =["\n",'i', 'n', 't'];
    $lenint =4;
    $textint ='int ';
    $char =["\n",'c','h', 'a', 'r'];
    $lenchar =5;
    $textchar ='char ';
    $bool =["\n",'b','o', 'o', 'l'];
    $lenbool =5;
    $textbool ='bool ';
    $float =["\n",'f','l','o', 'a', 't'];
    $lenfloat =6;
    $textfloat ='float ';
    
    fwrite($handleHTML,'<hr><hr><section> <h2 > defines : </h2>');
    variable($handleC, $handleHTML,$define, $lendefine, $textdefine);
    fwrite($handleHTML,'</section>');
    
    fwrite($handleHTML,'<hr><hr><section> <h2 > variables globales : </h2>');
    variable($handleC, $handleHTML,$int, $lenint, $textint);
    variable($handleC, $handleHTML,$char, $lenchar, $textchar);
    variable($handleC, $handleHTML,$bool, $lenbool, $textbool);
    variable($handleC, $handleHTML,$float, $lenfloat, $textfloat);
    fwrite($handleHTML,'</section>');

}


function fonction($handleC, $handleHTML){
    $varNb = 0;
    $ecrirevarText = false;
    $varFind = false;
    $varText = "";
    $varName = "";
    $count = 0;
    rewind($handleC);
    fwrite($handleHTML, "<hr><hr><section><h2 > fonctions : </h2>");
    
    $caractereAvant ='';
    $caractereAvantAvant ='';
    $caractere = fgetc($handleC);
    
    
    while (!feof($handleC)) {
        if ($caractere == '*' && $caractereAvant == '*' && $caractereAvantAvant == '/') {
            $ecrirevarText = true;
            $varText = "";
        }
        if ($caractere == '/' && $caractereAvant == '*') {
            $ecrirevarText = false;
            $varText .= '</p>';
        }
        if ($ecrirevarText == true) {
            if ($caractere != '*' && $caractere != '/') {
                if ($caractere == '\\') {
                    $caractereAvantAvant = $caractereAvant;
                    $caractereAvant = $caractere;
                    $caractere = fgetc($handleC);
                    if ($caractere == 'b') {
                        for ($i=0; $i < 5; $i++) { 
                            $caractereAvantAvant = $caractereAvant;
                            $caractereAvant = $caractere;
                            $caractere = fgetc($handleC);
                        }
                        $varText .= '<h4>';
                        while ($caractere != "\n") {
                            $varText .= $caractere;
                            $caractereAvantAvant = $caractereAvant;
                            $caractereAvant = $caractere;
                            $caractere = fgetc($handleC);
                        }
                        $varText .= '</h4>';
                        $varText .= "<p class='indent'>";
                    }
                    if ($caractere == 'd') {
                        for ($i=0; $i < 6; $i++) { 
                            $caractereAvantAvant = $caractereAvant;
                            $caractereAvant = $caractere;
                            $caractere = fgetc($handleC);
                        }
                    }
                    if ($caractere == 'r') {
                        $varText .= $caractere;
                    }
                    if ($caractere == 'p') {
                        for ($i=0; $i < 6; $i++) {
                            $caractereAvantAvant = $caractereAvant;
                            $caractereAvant = $caractere;
                            $caractere = fgetc($handleC);
                        }
                        $varText .= '<strong>';
                        while ($caractere != ' ') {
                            $varText .= $caractere;
                            $caractereAvantAvant = $caractereAvant;
                            $caractereAvant = $caractere;
                            $caractere = fgetc($handleC);
                            while ($caractere != ' ') {
                                $varText .= $caractere;
                                $caractereAvantAvant = $caractereAvant;
                                $caractereAvant = $caractere;
                                $caractere = fgetc($handleC);
                            }
                            $varText .= '</strong>';
                        }
                    }
                }
                else{
                    $varText .= $caractere;
                }
            }
            elseif ($caractere == '*' && $caractereAvant != '*' && $caractereAvant != '/') {
                $varText .= '<br>';
            }
        }
        if ($count == 2) {
            if ($caractere == " ") {
                $count = 0;
            }
        }
        
        if ($count == 1){
            if ($caractere == " ") {
                $count = 2;
            }
        }
        if ($count == 4) {
            if ($caractere == "\n" || $ecrirevarText == true) {
                $count = 0;
            }
        }
        if ($count == 3) {
            if ($caractere == "\n") {
                $count = 4;
            }
        }
        if ($count == 3) {
            if ($ecrirevarText == true) {
                $count = 0;
            }
        }
        if ($count == 4 || $count == 3) {
            if ($caractere == "{") {
                if ($varName != " main") {
                    fwrite($handleHTML,'<article><h3> fonction :');
                    fwrite($handleHTML,$varName);
                    fwrite($handleHTML,'</h3>');
                    fwrite($handleHTML,$varText);
                    fwrite($handleHTML, '</article><hr>');
                    $varName = "";
                    $varText = "";
                }
                $count = 0;
            }
        }
        if ($ecrirevarText == false && $caractereAvant == "\n" && $caractere != '/'){
            $count = 1;
            $varName = "";
        }
        
        if ($count == 2) {
            if ($caractere == "(") {
                $count = 3;
            }
        }
        if ($count == 2) {
            $varName .= $caractere;
        } 

        $caractereAvantAvant = $caractereAvant;
        $caractereAvant = $caractere;
        $caractere = fgetc($handleC);
    }
    fwrite($handleHTML, "</section>");
}

function structure($handleC, $handleHTML){
    $var = ['t','y','p','e','d','e','f',' ','s','t','r','u','c','t'];
    $varNb = 14;
    $varText = "";
    $count = 0;
    rewind($handleC);
    fwrite($handleHTML, "<hr><hr><section><h2 > structures : </h2>");
    
    $caractereAvant ='';
    $caractereAvantAvant ='';
    $caractere = fgetc($handleC);
    
    
    while (!feof($handleC)) {

        if ($count == $varNb) {
            $varText = "";
            $varText .= "<p class='indent'>";
            while ($caractere != "}") {
                if ($caractere == "/"&& $caractereAvant == "*") {
                    $varText .= "<br>";
                }
                else if ($caractere != "{"&& $caractere != "*"&& $caractere != "/") {
                    $varText .= $caractere;
                }
                
                $caractereAvantAvant = $caractereAvant;
                $caractereAvant = $caractere;
                $caractere = fgetc($handleC);
            }
            $varText .= "</p>";
            $caractereAvantAvant = $caractereAvant;
            $caractereAvant = $caractere;
            $caractere = fgetc($handleC);
            fwrite($handleHTML, "<article><h3>");
            while ($caractere != ";") {
                fwrite($handleHTML, $caractere);
                $caractereAvantAvant = $caractereAvant;
                $caractereAvant = $caractere;
                $caractere = fgetc($handleC);
            }
            fwrite($handleHTML, "</h3><br><h4>");
            $caractereAvantAvant = $caractereAvant;
            $caractereAvant = $caractere;
            $caractere = fgetc($handleC);
            while ($caractere != "/" || $caractereAvant != "*") {
                if ($caractere != "/" && $caractere != "*") {
                    fwrite($handleHTML, $caractere);
                }
                $caractereAvantAvant = $caractereAvant;
                $caractereAvant = $caractere;
                $caractere = fgetc($handleC);
            }
            fwrite($handleHTML, "</h4>");
            fwrite($handleHTML, $varText);
            fwrite($handleHTML, "</article><hr>");
            $count = 0;
            
        }
        
        

        if ($var[$count] == $caractere) {
            $count ++;
        }
        else {
            $count = 0;
        }

        $caractereAvantAvant = $caractereAvant;
        $caractereAvant = $caractere;
        $caractere = fgetc($handleC);
    }
    fwrite($handleHTML, "</section>");
}

#main=================================================================================================================================================






$dir = __DIR__; // Obtient le chemin du dossier courant

// Ouvre un répertoire bien connu, et liste tous les fichiers
$handleHTML = fopen($fichierHTML, 'w');
fwrite($handleHTML, '<!DOCTYPE html><html lang="fr"><head><title>documentation technique </title><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1.0"></head><body><header><p class="center" >');
fwrite($handleHTML, $description);
fwrite($handleHTML, '</p><hr></header><main>');
if (is_dir($dir)){
    if ($dh = opendir($dir)){
        while (($file = readdir($dh)) !== false){
            $file_parts = pathinfo($file);
            if ($file_parts['extension'] == 'c') {
                // $FileName = $file_parts['filename'] . '.html';
                // touch($FileName); 
                // echo "Fichier créé: $FileName\n"; // affichage d'un message de succès

                //ouvre les fichier
                $handleC = fopen($file_parts['filename'].'.c', 'r');
                
                if ($handleC) {
                    fwrite($handleHTML, '<hr><div class="title"><h1 class="center">Documentation Technique</h1><p class="center">du programme :</p><h2 class="center">');
                    fwrite($handleHTML, $file_parts['filename']);
                    fwrite($handleHTML, '</h2></div><hr>');
                    fwrite($handleHTML,'<section class="main_prog"><h2>Programme ');
                    fwrite($handleHTML, $file_parts['filename']);
                    fwrite($handleHTML,':</h2>');
                
                    #écriture de la description du programme
                    maindesc($handleC, $handleHTML);
                    #écriture des defines et des variables gloables (int bool float char)
                    appelevariable($handleC, $handleHTML);
                    #écriture des structure
                    structure($handleC, $handleHTML);
                    #écriture des fonctions
                    fonction($handleC, $handleHTML);
                    
                    fwrite($handleHTML,'<hr><hr>');
                    // Fermer les fichiers
                    fclose($handleC);
                }
            }
        }
        closedir($dh);
    }
}
fwrite($handleHTML, '</main></body><style>body{background-color: #1f1f1f;color: white;}#infos{color: #888888;font-size: small;}.center{text-align: center;}.indent{padding-left: 5em;}h2{color : rgb(103, 214, 248);text-decoration: underline;}h1{color : #9900FF;}a{color : rgb(255, 255, 255)}section,.title{background-color: rgb(49, 49, 49);border-radius: 0 20px ;padding: 0 40px;}hr{border-radius: 2px;height: 2px;background-color: rgb(88, 88, 88);}section > hr{height: 1px;background-color: black;}</style></html>');
fclose($handleHTML);

// Ouvrir le fichier C en lecture seule
?>
