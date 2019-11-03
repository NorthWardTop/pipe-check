<?php
class Person{
	public $nomutilisateur="";
	public $mdp="";
}

$information=new Person();
$information->nomutilisateur="";
$information->mdp="";
$newutilisateur=array();

$data=(array)json_decode(file_get_contents("../Modele/data.json")); 

if (isset($_GET["nomutilisateur"])&&isset($_GET["mdp"])){
	$information->nomutilisateur=$_GET["nomutilisateur"];
	$information->mdp=$_GET["mdp"];
	$newutilisateur=json_encode($information);
	$tabb=array_merge((array)json_decode(file_get_contents("../Modele/data.json")),(array)$newutilisateur);
	file_put_contents("../Modele/data.json", json_encode($tabb));
}


?>