#include "interface.h"
#include "gestionprojet.h"
#include "calendrier.h"
#include <QCoreApplication>




int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);

    Duree d1=Duree(180);
    QDate date1= QDate(1925,7,3);
    QDate date2= QDate (1925,11,3);
    Projet* p = new Projet ("0","Construire une maison","C'est pas très dur");
    TacheManager::getInstance().addItem(p);
    TacheManager::getInstance().trouverTache("0")->ajouterTacheUnitaire("1","Faire les courses chez castorama",d1,date1,date2,false);
    TacheManager::getInstance().trouverTache("0")->ajouterTacheComposite("2","Assembler les morceaux");
    TacheManager::getInstance().trouverTache("1")->ajouterTacheUnitaire("12","Assembler la charpente",d1,date1,date2,false);
    TacheManager::getInstance().trouverTache("1")->ajouterTacheComposite("11","Assembler les briques");
    TacheManager::getInstance().trouverTache("11")->ajouterTacheUnitaire("111","Assembler la charpente",d1,date1,date2,false);
    TacheManager::getInstance().trouverTache("11")->ajouterTacheUnitaire("112","Assembler la charpente",d1,date1,date2,false);
    VisiteurSauvegarde* vS = new VisiteurSauvegarde ("Test1");
    //std::cout<<"Sauvegarde commence";
    vS->visiterProjet(p);
    //return a.exec();
}
