<!DOCTYPE html>
<head>
	<meta content="charset=utf-8"/>
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
	<script src="../Controle/Inscription.js" type="text/javascript"></script>
	<link rel="stylesheet" href="index.css" type="text/css">
	<meta name="viewport" content="width=device-width,initial-scale=1">
		
</head>

<div class ="top">
<img src="projet.jpg">
</div>

<header>
<h1>Bienvenue a la page principale.</h1>
</header>

<body>

<div class="draw" id="draw">


<Expliquer>
<h3>Mouvez votre telephone pour enregistrer les donnees.</h3>
</Expliquer>


<Principale>
	
<button type="button" id="boutonGo" onclick="changerLeStyle(<?php echo $_GET['nomutilisateur'] ?>)" >Go</button>
<button type="button" id="boutonEnregistrer" onclick="enregistrer()" style="display:none">Enregistrer</button>
<p>Je voudrais trouver les dessins anciens. <a href="#" id="getformChercher" onclick="getformChercher()">Chercher</a></p>
<br>

</Principale>

<Dessin id="dessin" style="display:none">
	<canvas id="myCanvas1" width="150" height="150"
	style="border:1px solid #000000;"></canvas>
	<canvas id="myCanvas2" width="150" height="150"
	style="border:1px solid #000000;"></canvas>
	<br>
	<label for="motcle">Motcle:</label>
		<input type="text" id="motcle" name="motcle" placeholder="Entrer un mot cle pour votre dessin">
		<br>
</Dessin>


</div>

<div class="chercher" id="cherche" style="display:none">
	 <form id="formchercher" action="../Controle/Inscription.php" method="post">
		
		<label for="nom">MotCle:</label>
		<input type="text" id="nomdessin" name="nomdessin" placeholder="Saisir le nom de dessin.">
		<br>
		<label for ="date">Date:</label>
		<input type="date" id= "date" name="date">
	</form>

	<button type="button" id="boutonChercher" onclick="chercher()">Chercher</button>
<div class ="histoire" id="histoire">

</div>
<div class="histoirecanvas" id="histoirecanvas" style="display:none">
	<canvas id="myCanvas3" width="150" height="150"
	style="border:1px solid #000000;"></canvas>
	<canvas id="myCanvas4" width="150" height="150"
	style="border:1px solid #000000;"></canvas>
</div>

</div>
</body>



</html>

