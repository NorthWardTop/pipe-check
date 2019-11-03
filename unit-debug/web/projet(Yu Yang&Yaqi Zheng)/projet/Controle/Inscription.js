var go=false;
var nomu="";
var tempdebut="";
var tempfin="";
var motcle="";
function CreerUser(){
	//alert(nameInput);
	if(document.getElementById("nomutilisateur").value==""||document.getElementById("mdp").value==""||document.getElementById("verifmot").value==""){
		alert("Il faut remplir tous les tableau");
	}
	else if(document.getElementById("mdp").value!=document.getElementById("verifmot").value){
		alert("Il faux verifier ton mot de passe");
	}
	else 
	{
		$.ajax({
			type:'post',
			url:'../Modele/data.json',
			datatype: 'json',
			error:function (XMLHttpRequest, textStatus, errorThrown) 
			{
				alert("a"+XMLHttpRequest.status+"\n"+textStatus+"\n"+errorThrown);
			},
			success: function(data)
			{
				var trouve=false;
				$.each(data, function (infoIndex, info) 
				{
					var utilisateur = JSON.parse(info);
					if (document.getElementById('nomutilisateur').value==utilisateur["nomutilisateur"])
					{
						alert("Il existe deja ce nom");
						trouve=true;
					}
				});
				if(!trouve){
					$.ajax({
					type:'post',
					url:'../Controle/Inscription.php?nomutilisateur='+document.getElementById("nomutilisateur").value+'&mdp='+document.getElementById("mdp").value,
					error:function (XMLHttpRequest, textStatus, errorThrown) 
					{
        			alert("a"+XMLHttpRequest.status+"\n"+textStatus+"\n"+errorThrown);
					},
					success: function(data)
					{
					alert("Vous compte a ete enregistrer!");					
					$("#forminscription").hide();
        			$("#formlogin").show();
					}		
				});
			}
		}
	});
	}
}


function showP(param){
    if(param == 1){
        $("#formlogin").hide();
        $("#forminscription").show();
   	} 
   	else if(param == 2 ){
   		$("#formlogin").show();
        $("#forminscription").hide();
   	}
}

function Login(){
	if(document.getElementById("nomutilisateurlogin").value==""||document.getElementById("mdplogin").value==""){
		alert("Il faut remplir tous les tableau");
	}
	else{
		$.ajax({
			type:'post',
			url:'../Modele/data.json',
			datatype: 'json',
			error:function (XMLHttpRequest, textStatus, errorThrown) {
				alert("a"+XMLHttpRequest.status+"\n"+textStatus+"\n"+errorThrown);
			},
			success: function(data){
				var trouve=false;
				$.each(data, function (infoIndex, info) {
					var userdata = JSON.parse(info);
					if (document.getElementById('nomutilisateurlogin').value==userdata["nomutilisateur"]&&document.getElementById('mdplogin').value===userdata["mdp"]){
						window.location.href='../Vue/Principale.php?nomutilisateur='+document.getElementById('nomutilisateurlogin').value; 
						trouve=true;
					}

				});
				if(!trouve){
					alert("nom ou mot de passe incorrect");
				}
			}
		});
	}
}
		//e1.addEventListener("click",changerLeStyle,false);
function changerLeStyle(nom){
		nomu=nom;
		let e1 = document.getElementById("boutonGo");
		let e2 = document.getElementById("boutonEnregistrer");
		if(e1.innerHTML=="Stop"){
			$("#dessin").show();
			e2.style.display="";
			e1.innerHTML="Restart";
			window.removeEventListener("devicemotion", motionHandler, false);
			window.removeEventListener("deviceorientation", orientationHandler, false);
			tempfin=new Date().toLocaleString();
			showimg();
		}
		else{
			if(e1.innerHTML=="Restart"){
				var c1=document.getElementById("myCanvas1"); 
				var c2=document.getElementById("myCanvas2");
				c1.height=c1.height; 
				c2.height=c2.height; 
					$.ajax({
						type: 'post',
						url: '../Controle/orientation.php?enregistrer=1',
						async: false,
						error:function (XMLHttpRequest, textStatus, errorThrown) {
							alert("a"+XMLHttpRequest.status+"\n"+textStatus+"\n"+errorThrown);
						}
			});
					alert("On commence d'enregistrer les donnes apres 3 secondes");
					temps=3;
				function countTime(){
					//alert("1");
			if(temps>0){
				//alert("2");
				e1.innerHTML=temps;
				temps--;
				t=setTimeout(countTime,1000);
				//alert("3");
			}
			else{
				//alert("4");
				e1.innerHTML="Stop";
				window.addEventListener("devicemotion", motionHandler, false);
				window.addEventListener("deviceorientation", orientationHandler, false);
				tempdebut=new Date().toLocaleString();
				//alert("5");
			}
			}
			t=setTimeout(countTime,1000);
			}

			if(e1.innerHTML=="Go")
			{
				$.ajax({
					type: 'post',
					url: '../Controle/orientation.php?enregistrer=1',
					async: false,
					error:function (XMLHttpRequest, textStatus, errorThrown) {
					alert("a"+XMLHttpRequest.status+"\n"+textStatus+"\n"+errorThrown);
				}
			});
				alert("On commence d'enregistrer les donnes apres 3 secondes");
				temps=3;
				function countTime(){
					//alert("1");
			if(temps>0){
				//alert("2");
				e1.innerHTML=temps;
				temps--;
				t=setTimeout(countTime,1000);
				//alert("3");
			}
			else{
				//alert("4");
				e1.innerHTML="Stop";
				window.addEventListener("devicemotion", motionHandler, false);
				window.addEventListener("deviceorientation", orientationHandler, false);
				tempdebut=new Date().toLocaleString();
				//alert("5");
			}
			}
			t=setTimeout(countTime,1000);
			}
			e2.style.display="none";
			//e1.innerHTML="Stop";
			$("#dessin").hide();
			//window.addEventListener("devicemotion", motionHandler, false);
			//window.addEventListener("deviceorientation", orientationHandler, false);
			//tempdebut=new Date().toLocaleString();
		}
}

function enregistrer(){
	if(document.getElementById("motcle").value==""){
		alert("il faut remplir le motcle")}
	else{
		motcle=document.getElementById('motcle').value;
		$.ajax({
			type:'post',
			url:'../Modele/movement.json',
			datatype: 'json',
			error:function (XMLHttpRequest, textStatus, errorThrown) 
			{
				alert("a"+XMLHttpRequest.status+"\n"+textStatus+"\n"+errorThrown);
			},
			success: function(data){
				var trouve=false;
				$.each(data, function (infoIndex, info) {
					var motcledata = JSON.parse(info);
					if (document.getElementById('motcle').value==motcledata["motcle"])
					{
						alert("Il existe deja ce motcle");
						trouve=true;
					}
				});
				if(!trouve){
					$.ajax({
					type: 'post',
					url: '../Controle/orientation.php?nom='+nomu+'&motcle='+motcle+'&tempdebut='+tempdebut+'&tempfin='+tempfin,
					async: false,
					error:function (XMLHttpRequest, textStatus, errorThrown) 
					{
					alert("a"+XMLHttpRequest.status+"\n"+textStatus+"\n"+errorThrown);
					}		
					});
				}
			}
		});
	}
}

function getformChercher(){
	$("#draw").hide();
	$("#cherche").show();
}

function orientationHandler(event) {
$.ajax({
	type: 'post',
	url: '../Controle/orientation.php?alpha='+event.alpha.toFixed(5)+'&beta='+event.beta.toFixed(5)+'&gamma='+event.gamma.toFixed(5)+'&nom='+nomu,
	async: false,
	error:function (XMLHttpRequest, textStatus, errorThrown) {
        alert("a"+XMLHttpRequest.status+"\n"+textStatus+"\n"+errorThrown);

    }
});
}


function motionHandler(event) {
	var acc = event.acceleration;
	$.ajax({
		type: 'post',
		url: '../Controle/orientation.php?accx='+acc.x.toFixed(5)+'&accy='+acc.y.toFixed(5)+'&accz='+acc.z.toFixed(5)+'&nom='+nomu,
		async: false,
		error:function (XMLHttpRequest, textStatus, errorThrown) {
			alert("b"+XMLHttpRequest.status+"\n"+textStatus+"\n"+errorThrown);
		}
	});
}

function chercher(){
		$('#histoire').html("");
		let html='';
		var cherchemot=document.getElementById("nomdessin").value;
		var cherchedate=document.getElementById("date").value;
		var array=new Array();
		var datedebut;
		var datefin;
		var stringmot;
		$.ajax({
			async:false,
			type: "post",
			dataType: "json",
			url: "../Modele/movement.json",
			error:function (XMLHttpRequest, textStatus, errorThrown) {
				alert("b"+XMLHttpRequest.status+"\n"+textStatus+"\n"+errorThrown);
			},
			success: function (data) { 
				$.each (data, function (i, item){ 
					var json=JSON.parse(item);
					datedebut=json["tempdebut"];
					datefin=json["tempfin"];
					stringmot=json["motcle"];
					if(stringmot.indexOf(cherchemot)>=0){
						array.push(json);
					}
					else if (datefin.indexOf(cherchedate)>=0){
						array.push(json);
					}
				});  
				var len=array.length;
				if (len>0){	
					for(var i = 0; i < len; i++) {
						var stringtotal="nom: "+array[i]["nom"]+" mot-cle: "+array[i]["motcle"]+" date fin: "+array[i]["tempfin"];
						html+=('<input type="checkbox" value='+array[i]["motcle"]+' />'+stringtotal+"</br>");
					}
					$('#histoire').html(html);
					//document.getElementById("courbes").style.display="block";
				}
				else {
					alert("non trouve!");
				}
				return false;
			}
		});
		$('#histoire').html(html);
	
}
function showimg(){
		var array1=new Array();	
		var array2=new Array();	
		$.ajax({
			async:false,
			type: "get",
			dataType: "json",
			url: "../Modele/orientation.json",
			error:function (XMLHttpRequest, textStatus, errorThrown) {
				alert("XMLHttpRequest:"+XMLHttpRequest+"\ntextStatus"+textStatus+"\nerrorThrown:"+errorThrown);
			},
			success: function (data) {   
				var jsonstr=JSON.stringify(data);
				drawori(2,jsonstr);
			}  
		});
		$.ajax({
			async:false,
			type: "get",
			dataType: "json",
			url: "../Modele/motion.json",
			error:function (XMLHttpRequest, textStatus, errorThrown) {
				alert("XMLHttpRequest:"+XMLHttpRequest+"\ntextStatus"+textStatus+"\nerrorThrown:"+errorThrown);
			},
			success: function (data) {   
				var jsonstr=JSON.stringify(data);
				drawmotion(1,jsonstr);
			}  
		});

}

	function number(s){
		if(s.substr(0, 1) == "-"){return (-parseFloat(s));}
		else return (parseFloat(s));
	}
	
	function drawmotion(idimg,datajson){
		var arrx=[];
		var arry=[];
		var arrz=[];
		var length=JSON.parse(datajson).length;
		for (var i = 0; i <length; i++){
			arrx.push(number(eval("("+JSON.parse(datajson)[i]+")").accx));
			arry.push(number(eval("("+JSON.parse(datajson)[i]+")").accy));
			arrz.push(number(eval("("+JSON.parse(datajson)[i]+")").accz));
		}
		var canvas1 = document.getElementById("myCanvas"+idimg);
		var centerX = canvas1.width/2; 
		var centerY = canvas1.height/2;
		var lineacc = canvas1.getContext("2d");
		lineacc.beginPath();
		lineacc.lineWidth = 1;
		lineacc.strokeStyle = "red";
		var x = 0;
		var accy =75-arrx[i]*7.5;
		lineacc.moveTo(x, accy);
		for (var i = 1; i <length; i++){
			accy=75-arrx[i]*7.5;
			x=x+150/length;
			lineacc.lineTo(x,accy);	
		}
		lineacc.stroke();
		lineacc.beginPath();
		lineacc.lineWidth = 1;
		lineacc.strokeStyle = "blue";
		x = 0;
		accy = 75-arry[0]*7.5;
		lineacc.moveTo(x, accy);
		for (var i = 1; i <length; i++){
			accy=75-arry[i]*7.5;
			x=x+150/length;
			lineacc.lineTo(x,accy);	
		}
		lineacc.stroke();
		lineacc.beginPath();
		lineacc.lineWidth = 1;
		lineacc.strokeStyle = "black";
		x = 0;
		accy = 75-arrz[0]*7.5;
		lineacc.moveTo(x, accy);
		for (var i = 1; i <length; i++){
			accy=75-arrz[i]*7.5;
			x=x+150/length;
			lineacc.lineTo(x,accy);	
		}
		lineacc.stroke();
	}

	function drawori(idimg,datajson){
		var arrx=[];
		var arry=[];
		var arrz=[];
		var length=JSON.parse(datajson).length;
		for (var i = 0; i <length; i++){
			arrx.push(number(eval("("+JSON.parse(datajson)[i]+")").alpha));
			arry.push(number(eval("("+JSON.parse(datajson)[i]+")").beta));
			arrz.push(number(eval("("+JSON.parse(datajson)[i]+")").gamma));
		}
		var canvas1 = document.getElementById("myCanvas"+idimg);
		var centerX = canvas1.width/2; 
		var centerY = canvas1.height/2;
		var lineacc = canvas1.getContext("2d");
		lineacc.beginPath();
		lineacc.lineWidth = 1;
		lineacc.strokeStyle = "red";
		var x = 0;
		var accy = 150-arrx[0]*150/360;
		lineacc.moveTo(x, accy);
		for (var i = 1; i <length; i++){
			accy=150-arrx[i]*150/360;
			x=x+150/length;
			lineacc.lineTo(x,accy);	
		}
		lineacc.stroke();
		lineacc.beginPath();
		lineacc.lineWidth = 1;
		lineacc.strokeStyle = "blue";
		x = 0;
		accy = 150-arry[0]*150/360;
		lineacc.moveTo(x, accy);
		for (var i = 1; i <length; i++){
			accy=150-arry[i]*150/360;
			x=x+150/length;
			lineacc.lineTo(x,accy);	
		}
		lineacc.stroke();
		lineacc.beginPath();
		lineacc.lineWidth = 1;
		lineacc.strokeStyle = "black";
		x = 0;
		accy =150-arrz[0]*150/360;
		lineacc.moveTo(x, accy);
		for (var i = 1; i <length; i++){
			accy=150-arrz[i]*150/360;
			x=x+150/length;
			lineacc.lineTo(x,accy);	
		}
		lineacc.stroke();
	}
