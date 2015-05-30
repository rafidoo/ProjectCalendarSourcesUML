#include "interface.h"


void ExplorateurProjet::open()
{
    QWidget fenetre;
    fenetre.setFixedSize(400, 400);

    QTreeView organigramme (&fenetre);
    organigramme.setFixedSize(600,400);

    QScrollArea scrollArea;
    scrollArea.setBackgroundRole(QPalette::Dark);
    scrollArea.setWidget(&organigramme);
    QVBoxLayout couche;
    couche.addWidget(&scrollArea);

    fenetre.setLayout(&couche);
    fenetre.show();

}

//Explorateur ExplorateurProjet::ExplorateurProjet()
//{}
