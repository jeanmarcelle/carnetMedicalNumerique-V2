#include "profilprive.h"
#include "../chemin.h"
#include "../fonctions/fonctions.h"

ProfilPrive::ProfilPrive()
{

}


ProfilPrive::~ProfilPrive(){

}


ProfilPrive::ProfilPrive(const ProfilPrive&){

}


void ProfilPrive::setMotDePasse(QString pwd){
	mdp = pwd;
}


void ProfilPrive::saveMotDePasse(QString pseudo, QString pwd){ // enregistre le mot de passe d'un profil dans un fichier crypter

	QFile file;
	file.setFileName(PROFILPATH+pseudo+FICHIERMOTDEPASSE);

	if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream out(&file);
		out << crypter(pwd); // crypter le texte
		file.close();
	}
}


QString ProfilPrive::getMotDePasse(QString pseudo){ // lit le fichier du mot de passe et renvoi le mot de passe décrypter

	QString pwd, readPwd;
	QFile file(PROFILPATH+pseudo+FICHIERMOTDEPASSE);

	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream out(&file);
		out >> readPwd;
		file.close();
	}
	pwd = decrypter(readPwd); // decrypte le texte
	return pwd;
}
