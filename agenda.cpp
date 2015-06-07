#include "agenda.h"

using namespace std;
using namespace TIME;

std::ostream& operator <<(std::ostream& f, TIME::Evt1j& j){
        j.afficher(f);
        return f;
}

void EvtManagerTache::addEvt(Tache * T) {
	//evt -> tableau de tous les evt existants (taches et RDV)
	//evt[] pointe sur un evenement particulier et evt[][] est l'evenement en lui meme
	//evt[].getTruc pour les données

	if (nb == nbMax)
	{			
		// Augmenter la taille du tableau
	}
	for (unsigned int i = 0; i<nb; i++)
	{
		if (evt[i].getDate() == T.getDate())						//Verifier que l'operateur est bien défini
		{										// Une tache n'a pas de getDate() ?!!
			unsigned int fin = evt[i].getHoraire() + evt[i].getDuree;		//Verifier que l'operateur est bien défini
			if(evt[i].getHoraire() < T.getHoraire() && T.getHoraire() < fin)	//Verifier que l'operateur est bien défini et le && ?
			{
				cout<<"Erreur, evenement déjà programmé à cette heure ce jour la"<<endl;
			}
		}
	}
	
		
			




}

