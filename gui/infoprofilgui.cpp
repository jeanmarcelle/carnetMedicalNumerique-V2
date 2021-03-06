#include "infoprofilgui.h"
#include "ui_infoprofilgui.h"
#include <qinputdialog.h>
#include <QFileDialog>
#include "../fonctions/fonctions.h"


InfoProfilGui::InfoProfilGui(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::InfoProfilGui)
{
	ui->setupUi(this);

	// imposer la saisie de nombre dans les champs "N° de tél"
	ui->lineEdit_tel->setValidator(new QIntValidator(0, 9999999999, this));
	ui->lineEdit_telContact->setValidator(new QIntValidator(0, 9999999999, this));
	ui->lineEdit_telMed->setValidator(new QIntValidator(0, 9999999999, this));
	ui->spinBox_taille->setMaximum(300); // taille maximale
	ui->doubleSpinBox_poids->setMaximum(300.00); // poids maximum
	ui->dateEdit->clear();

	adminProfil = true; // pour le profil admin
	EnableModif = false; // utiliser pour remplir la fenetre lorsqu'on clique sur modifier profil
}


InfoProfilGui::~InfoProfilGui()
{
	delete ui;
}


void InfoProfilGui::setInAdminProfil(bool b){
	adminProfil = b;
}


void InfoProfilGui::setEnableModif(bool b){
	EnableModif = b;
}


void InfoProfilGui::on_pushButton_Ok_clicked()
{
	if(verifieChampDeSaisi()){
		ProfilPrive nouveauProfil;
		bool pseudoExistDeja=false;
		if(!EnableModif && !adminProfil){ // profil user

			QString pseudoProfil = QInputDialog::getText (this, "nom du profil", "Entrer le nom de votre profil");
			pseudoExistDeja = pseudoExist(pseudoProfil);

			if(!pseudoExistDeja){// test si le profil existe déjà
				nouveauProfil.creerFichierPublic(pseudoProfil);
				emit newprofil(pseudoProfil); // envoi l'information de creation de profil
			}else{
				QMessageBox::warning(this, tr("Création de profil"),
											tr("Ce nom de profil existe déjà!"),
											QMessageBox::Cancel,
											QMessageBox::Cancel);
			}
		}

		if(!pseudoExistDeja){
			// recuperation des infos
			if(adminProfil){
				nouveauProfil.creerFichierPublic(ui->lineEdit_prenom->text()); // specifie que c'est la creation du profil admin avec le "true"
				ecrireDansFichierTemp(ui->lineEdit_prenom->text()); // enregistrement du prenom de l'admin dans le fichier temp
				nouveauProfil.setInAdmin(true); // signifie que le profil est admin
			}else{
				nouveauProfil.setInAdmin(false); // signifie que le profil n'est pas admin
			}

			nouveauProfil.setNom(ui->lineEdit_nom->text(), ui->lineEdit_prenom->text(), ui->checkBox_nom->isChecked());

			if(ui->radioButton_M->isChecked())
				nouveauProfil.setSexe("M", ui->checkBox_sexe->isChecked());
			if(ui->radioButton_F->isChecked())
				nouveauProfil.setSexe("F", ui->checkBox_sexe->isChecked());

			nouveauProfil.setCorps(ui->spinBox_taille->value(), ui->doubleSpinBox_poids->value(), ui->checkBox_corps->isChecked());
			nouveauProfil.setDateNaissance(ui->dateEdit->text(), ui->checkBox_date->isChecked());
			nouveauProfil.setAdresse(ui->lineEdit_adresse->text(), ui->checkBox_Adress->isChecked());
			nouveauProfil.setTel(ui->lineEdit_tel->text(), ui->checkBox_Tel->isChecked());
			nouveauProfil.setProfession(ui->lineEdit_profession->text(), ui->checkBox_profession->isChecked());
			nouveauProfil.setMedecin(ui->lineEdit_nomMed->text(), ui->lineEdit_telMed->text(), ui->checkBox_medecin->isChecked());
			nouveauProfil.setGroupSanguin(ui->comboBox_groupe->currentText()+ui->comboBox_rhesus->currentText(), ui->checkBox_goupeSanguin->isChecked());
			nouveauProfil.setPersonContact(ui->lineEdit_nomContact->text(), ui->lineEdit_telContact->text());

			// TODO : recuperer les donnees privées

			nouveauProfil.saveProfilInFiles(); // enregistrement des données dans le fichier publique

			this->setResult(1);
			hide();
		}
	}else{
		QMessageBox::warning(this, tr("Création de profil"),
							tr("Des champs de saisie sont vide!"),
							QMessageBox::Cancel,
							QMessageBox::Cancel);
	}
}


void InfoProfilGui::on_pushButton_Annul_clicked()
{
	this->setResult(0);
	hide();
}


bool InfoProfilGui::pseudoExist(QString pseudo){

	bool exist = false;
	// lister tous les noms de profils dans le dossier profil
	QDir repertoireProfil(PROFILPATH);
	QStringList list = repertoireProfil.entryList();

	for(int i=2; i<list.length(); i++){ // comparaison avec les autres pseudo
		if(pseudo == list[i]){
			exist = true;
			break; // si le pseudo entrer existe déjà retourner true
		}
	}
	return exist;
}


bool InfoProfilGui::verifieChampDeSaisi(){
	bool champRempli = true;

	if(ui->lineEdit_nom->text().isEmpty() || ui->lineEdit_prenom->text().isEmpty()){
		champRempli = false;
	}else if(ui->spinBox_taille->value() == 0 || ui->doubleSpinBox_poids->value() == 0.0){
		champRempli = false;
	}

	// TODO : tester tous les champ de saisi

	return champRempli;
}


void InfoProfilGui::setQlineEditWithDatas(ProfilPrive* profil){

	ui->checkBox_nom->setChecked(profil->getPriveNom());
	ui->lineEdit_nom->setText(profil->getNom());
	ui->lineEdit_prenom->setText(profil->getPrenom());

	ui->checkBox_sexe->setChecked(profil->getPriveSexe());
	if(profil->getSexe() == "F")
		ui->radioButton_F->setChecked(true);
	else
		ui->radioButton_M->setChecked(true);

	ui->checkBox_corps->setChecked(profil->getPriveCorps());
	ui->spinBox_taille->setValue(profil->getTaille());
	ui->doubleSpinBox_poids->setValue(profil->getPoids());

	ui->checkBox_Tel->setChecked(profil->getPriveTel());
	ui->lineEdit_tel->setText(profil->getTel());

	ui->checkBox_date->setChecked(profil->getPriveDate());
	ui->dateEdit->setDate(QDate::fromString(profil->getDateNaissance(), "dd/MM/yyyy"));

	ui->checkBox_Adress->setChecked(profil->getPriveAdresse());
	ui->lineEdit_adresse->setText(profil->getAdresse());

	ui->checkBox_medecin->setChecked(profil->getPriveMedecin());
	ui->lineEdit_nomMed->setText(profil->getMedecinNom());
	ui->lineEdit_telMed->setText(profil->getMedecinTel());

	ui->checkBox_profession->setChecked(profil->getPriveProfession());
	ui->lineEdit_profession->setText(profil->getProfession());

	ui->lineEdit_nomContact->setText(profil->getPersonContactNom());
	ui->lineEdit_telContact->setText(profil->getPersonContactTel());

	ui->checkBox_goupeSanguin->setChecked(profil->getPriveGroupe());
	// TODO: groupe sangin
}


void InfoProfilGui::clearAllQlineEdit(){
	// initialiser tous les champs
	ui->lineEdit_nom->clear();
	ui->lineEdit_prenom->clear();
	ui->radioButton_M->setChecked(true);
	ui->spinBox_taille->setValue(0);
	ui->doubleSpinBox_poids->setValue(0.00);
	ui->dateEdit->clear();
	ui->lineEdit_adresse->clear();
	ui->lineEdit_tel->clear();
	ui->lineEdit_profession->clear();
	ui->lineEdit_nomMed->clear();
	ui->lineEdit_telMed->clear();
	ui->comboBox_groupe->setCurrentIndex(0);
	ui->comboBox_rhesus->setCurrentIndex(0);
	ui->lineEdit_nomContact->clear();
	ui->lineEdit_telContact->clear();

	// desactivé tous les checkbox "prive"
	ui->checkBox_nom->setChecked(false);
	ui->checkBox_sexe->setChecked(false);
	ui->checkBox_Tel->setChecked(false);
	ui->checkBox_date->setChecked(false);
	ui->checkBox_corps->setChecked(false);
	ui->checkBox_Adress->setChecked(false);
	ui->checkBox_medecin->setChecked(false);
	ui->checkBox_profession->setChecked(false);
	ui->checkBox_goupeSanguin->setChecked(false);
}


void InfoProfilGui::on_toolButton_clicked()
{
	// TODO: ajouter chemin vers photo comme attribut de profil
	// TODO: copie la photo dans le repertoire du profil

	QString cheminPhoto = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
	QPixmap *photo = new QPixmap(cheminPhoto);
	photo->scaled(ui->label_Photo->size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
	ui->label_Photo->setPixmap(*photo);
}


void InfoProfilGui::desactivePage(int numPage, bool etat){
	ui->tabWidget->setTabEnabled(numPage, etat);
}
