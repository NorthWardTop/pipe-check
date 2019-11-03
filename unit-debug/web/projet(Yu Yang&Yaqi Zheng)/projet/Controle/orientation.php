<?php

	class Orientation{
		public $alpha="";
		public $beta="";
		public $gamma="";
	}
	class Motion{
		public $accx="";
		public $accy="";
		public $accz="";
	}
	class Data{
		public $nom="";
		public $motcle="";
		public $tempdebut="";
		public $tempfin="";
		public $orientation=array();
		public $motion=array();
	}

	$informationOri = new Orientation();
	$informationMotion = new Motion();
	$information=new Data();
	$new = array();
	$vectors=array();
if (isset($_GET["alpha"])&&isset($_GET["beta"])&&isset($_GET["gamma"])){
		
		$informationOri->alpha=$_GET["alpha"];
		$informationOri->beta=$_GET["beta"];
		$informationOri->gamma=$_GET["gamma"];
		$new=json_encode($informationOri);
		$tabb=array_merge((array)$new,(array)json_decode(file_get_contents("../Modele/orientation.json")));
		file_put_contents("../Modele/orientation.json",json_encode($tabb,JSON_UNESCAPED_SLASHES));
}
if (isset($_GET["accx"])&&isset($_GET["accy"])&&isset($_GET["accz"])){
		echo "<script>alert('111');</script>";
		$informationMotion->accx=$_GET["accx"];
		$informationMotion->accy=$_GET["accy"];
		$informationMotion->accz=$_GET["accz"];
		$new=json_encode($informationMotion);
		$tabb=array_merge((array)$new,(array)json_decode(file_get_contents("../Modele/motion.json")));
		file_put_contents("../Modele/motion.json",json_encode($tabb,JSON_UNESCAPED_SLASHES));
}
if (isset($_GET["nom"])&&isset($_GET["motcle"])&&isset($_GET["tempdebut"])&&isset($_GET["tempfin"])){
	$information->nom=$_GET["nom"];
	$information->motcle=$_GET["motcle"];
	$information->tempdebut=$_GET["tempdebut"];
	$information->tempfin=$_GET["tempfin"];
	$information->orientation=(array)json_decode(file_get_contents("../Modele/orientation.json"));
	$information->motion=(array)json_decode(file_get_contents("../Modele/motion.json"));
	$new=json_encode($information);
	$tabb=array_merge((array)$new,(array)json_decode(file_get_contents("../Modele/movement.json")));
	file_put_contents("../Modele/movement.json",json_encode($tabb));
	file_put_contents("../Modele/orientation.json",json_encode(array()));
	file_put_contents("../Modele/motion.json",json_encode(array()));
	echo "<script>alert('bien enregistre');</script>";
}
if (isset($_GET["enregistrer"])){
	file_put_contents("../Modele/orientation.json",json_encode(array()));
	file_put_contents("../Modele/motion.json",json_encode(array()));
}

?>