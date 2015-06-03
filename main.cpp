#include "interface.h"
#include "gestionprojet.h"
#include "calendrier.h"
#include <QCoreApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //ExplorateurProjet expProj1;
    //expProj1.open();

    //GestionnaireDeProjet gP ();
    //gP.ajouter

    Duree d1=Duree(180);
    QDate date1= QDate(1925,7,3);
    QDate date2= QDate (1925,11,3);
    Projet* p = new Projet ("0","Construire une maison","C'est pas très dur");
    TacheManager::getInstance().addItem(p);
    p->ajouterTacheUnitaire("1","Faire les courses",d1,date1,date2,false);
    /*
    TacheUnitaire t12 = new TacheUnitaire ("12","Prendre son cartable",d1,date1,date2,false,false);
    TacheUnitaire t111 = new TacheUnitaire ("112","Faire les courses",d1,date1,date2,false,false);
    p.ajouterTacheUnitaire("2","Faire les courses à Castorama",d1,date1,date2,false);
    p.ajouterTacheComposite("1","Construire");
    dynamic_cast<TacheComposite>(p.getTache("1")).ajouterTacheComposite("11","Couler le ciment");
    dynamic_cast<TacheComposite>(p.getTache("1")).ajouterTacheComposite("11","Couler le ciment");
    */
    return a.exec();
}
