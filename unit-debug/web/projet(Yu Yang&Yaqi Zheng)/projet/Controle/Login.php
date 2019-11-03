<?php
	$data=(array)json_decode(file_get_contents("../Modele/data.json"));
	$num = count($data); 
	$trouve=false;

	for($i=0;$i<$num;$i++){ 
		$json=(array)json_decode($data[$i]);
		if($_GET["nomutilisateurlogin"]==($json["nomutilisateur"])&&$_GET["mdplogin"]==($json["mdp"])){
			$trouve=true;
			echo "<script>window.location.href='../Vue/Principale.php?nomutilisateur='+document.getElementById('nomutilisateurlogin').value; </script>";
			break;
		}
	}
	if(!$trouve){
		echo "<script>alert('nom ou mot de passe uncorrecte11'); </script>";
	}
		
?>