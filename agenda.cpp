#include "agenda.h"

using namespace std;
using namespace TIME;

std::ostream& operator <<(std::ostream& f, TIME::Evt1j& j){
        j.afficher(f);
        return f;
}
/// ********** Fonction pour ajouter une tache déjà existante ********** ///

void EvtManager::addEvt(const Evt * e, const QDate & date, const QTime & h) {			/// Je peux mettre que c'est un EvtTache en parametre ?
	//evt -> tableau de tous les evt existants (taches et RDV)
	//evt[] pointe sur un evenement particulier et evt[][] est l'evenement en lui meme
	//evt[].getTruc pour les données

	if (nb == nbMax)
	{			
		Evt ** newtab = new Evt * [nbMax + 10];
		nbMax += 10;
		for(unsigned int i = 0; i<nb; i++)
		{
			newtab[i]=evt[i];
		}		
		Evt ** old = evt;
		evt = newtab;
		delete[] old;
	}
    	
	for (unsigned int i = 0; i<nb; i++)
	{
		if (evt[i].getDate() == e.getDate())
		{
			unsigned int fin = evt[i].getHoraire() + evt[i].getDuree;		// L'opérateur + doit pas etre defini entre QTime et Duree
			if((evt[i].getHoraire() < e.getHoraire()) && (e.getHoraire() < fin))
			{
				cout<<"Erreur, la date et l'horaire démandés sont déjà occupés par un autre evenement !"<<endl;
				return;
			}
		}
	}
	if (date < e.getDateDisponibilite())				/// Comment récupérer la dispo de cette tache ?
	{
		cout<<"Erreur, la date demandée est antérieure à la date de disponibilité de la tache"<<endl;
		return;
	}
	if (date > e.getDateEcheance())					/// Comment récupérer l'échéance de cette tache ?
	{
		cout<<"Erreur, la date demandée est postérieure à la date d'échéance de la tache"<<endl;
		return;
	}
		
	//précédence a checker	

	evt[nb++]=e;	
}

/// ********** Fonction pour créer puis ajouter une tache ou un RDV ********** ///

virtual Evt & addNewEvt(const QDate & d, const QString & s, const QTime & deb, const Duree & dur, const QString & l, const QString & pers){

}
