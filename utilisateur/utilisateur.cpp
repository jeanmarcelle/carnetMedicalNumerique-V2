#include "utilisateur.h"

Utilisateur::Utilisateur()
{
	profilChoisi = new ProfilPrive;
}


Utilisateur::~Utilisateur(){
	delete profilChoisi;
}


void Utilisateur::selectCurrentProfil(QString pseudo){

	QFile file;
	file.setFileName(PROFILPATH+pseudo+"/"+pseudo+EXTPUBLIC);

	// charger les données du profil avec le nom pseudo
	QDomDocument xmlBOM;
	file.open(QFile::ReadOnly | QFile::Text);

	xmlBOM.setContent(&file);
	QDomElement element = xmlBOM.documentElement().firstChild().toElement();
	QDomElement sousElement;

	while(!element.isNull())
	{
		if(element.tagName()=="admin"){
			if(element.text() == "true")
				profilChoisi->setInAdmin(true);
			else
				profilChoisi->setInAdmin(false);
		}
		else if(element.tagName()=="nom")
			profilChoisi->setNom(element.text());
		else if(element.tagName()=="prenom")
			profilChoisi->setPrenom(element.text());
		else if(element.tagName()=="sexe")
			profilChoisi->setSexe(element.text());
		else if(element.tagName()=="taille")
			profilChoisi->setTaille(element.text().toInt());
		else if(element.tagName()=="poids")
			profilChoisi->setPoids(element.text().toDouble());
		else if(element.tagName()=="date")
			profilChoisi->setDateNaissance(element.text());
		else if(element.tagName()=="tel")
			profilChoisi->setTel(element.text());
		else if(element.tagName()=="adress")
			profilChoisi->setAdresse(element.text());
		else if (element.tagName()=="profession")
			profilChoisi->setProfession(element.text());
		else if(element.tagName()=="medecin"){
			sousElement = element.firstChild().toElement();
			while(!sousElement.isNull())
			{
				if(sousElement.tagName()=="nom")
					profilChoisi->setMedecinNom(sousElement.text());
				else if(sousElement.tagName()=="tel")
					profilChoisi->setMedecinTel(sousElement.text());
				sousElement = sousElement.nextSibling().toElement();
			}
		}
		else if (element.tagName()=="contact"){
			sousElement = element.firstChild().toElement();
			while(!sousElement.isNull())
			{
				if(sousElement.tagName()=="nom")
					profilChoisi->setPersonContactNom(sousElement.text());
				else if(sousElement.tagName()=="tel")
					profilChoisi->setPersonContactTel(sousElement.text());
				sousElement = sousElement.nextSibling().toElement();
			}
		}

		element = element.nextSibling().toElement();
	}

	file.close();
}


ProfilPrive* Utilisateur::getProfil(){
	return profilChoisi;
}
