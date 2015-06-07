#ifndef EVENEMENT_H_INCLUDED
#define EVENEMENT_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "timing.h"

using namespace std;

namespace TIME{

	class EvtManager {
	  protected:
		Evt ** evt;
		unsigned int nb;
		unsigned int nbMax;
		QString file;
		EvtManager & operator =(const EvtManager & um);
	  public:
		EvtManager() {}
		~ EvtManager() {}
		//Evt ** getEvt() { return evt; }			// Utile ?
		virtual void addEvt(Evt * e, QDate & date, QTime & h);
		virtual Evt & addNewEvt();
		Evt * trouverEvt(QSring & s);
		void load(QString & f);
		void save(QString & f);
	};

    	class Evt {
	  protected :
		DDate date;
		QString sujet;
      	  public:
		Evt(const QDate & d, const QString & s): date(d), sujet(s) {}
		virtual ~Evt() {}
		const QString getDescripteur() { return sujet; }
		const QDate & getDate() { return date; }
		virtual void afficher(std::ostream &) const
		{
		    	std::stringstream f;
		    	f<<toString();
		}
		virtual QString toString() const
		{
		    	std::stringstream f;
		    	f<<"******** Evenement ********"<<"\n\n"<<"Date = "<<date<<"\n"<<" Sujet = "<<sujet<<"\n\n";
		    	return f.str();
		}
		virtual Evt * clone() const = 0;
		//Evt * clone() const { return new Evt1j(*this); }	/// Cette ligne était dans Evt1j, laquelle guarder ?
		
		/*virtual bool operator <(Evt & e)			/// Fonction devenue inutile puisqu'on peut utiliser l'operateur < de QDate
		{
		    	return date < e.date;
		}*/
    	};

    	class Evt1jDur: public Evt{
		QTime debut;
		Duree duree;
      	  public:
		Evt1jDur(const QDate & d, const QString & s, const QTime& deb, const Duree & dur):
		        Evt(d, s) ,debut(deb), duree(dur) {}
		~Evt1jDur() {}
		const QTime & getDebut() const { return this->debut; }
		const Duree & getDuree() const { return this->duree; }
		/*virtual void afficher(std::ostream& f = std::cout) const{
		    	Evt1j::afficher(f);
		    	this->debut.afficher(f);
		    	this->duree.afficher(f);
		}*/
		QString toString() const
		{
		    	std::stringstream f;
			f<<"******** Evenement ********"<<"\n\n"<<"Date = "<<date<<"\n"<<" Sujet = "<<sujet<<"\n"<<"Debut = "<<debut<<"\n"<<"Duree = "<<duree<<"\n\n";		    
		    	return f.str();
		}
		Evt1jDur * clone() const { return new Evt1jDur(*this); }		/// A mettre ?
		bool operator <(Evt & e) {  						//Redefinition
		    if(Evt::operator <(e)) return true; 				//true si actuelle anterieure à e
		    if(e<*this) return false;           				//false si e anterieure a classe actuelle
		    const Evt1jDur * pt = dynamic_cast<Evt1jDur *>(&e); 		//date egales, check horaire !
		    if (pt!=NULL && this->debut < pt->getDebut())
		    {
		        return true;
		    }
		    return false;
		}
    	};

    	class EvtRDV: public Evt1jDur{
		QString lieu;
		QString personne;
	      public:
		EvtRDV(const QDate & d, const QString & s, const Qtime & deb, const Duree & dur, const QString & l, const QString & pers):
		        Evt1jDur(d, s, deb, dur), lieu(l), personne(pers) {}
		~EvtRDV() {}
		/*RDV(const RDV& r): Evt1jDur(r.getDate(), r.getDescription(), r.getDebut(), r.getDuree()),
		        lieu(r.getLieu()), personne(r.getPersonne())
		        {} */
		EvtRDV & operator =(const EvtRDV & r)
		{
		    	this->lieu = r.lieu;
		    	this->personne = r.personne;

		    	Evt1j *x = this;
		    	*x = r;
		}
		const QString & getLieu() const { return this->lieu; }
		const QString & getPersonne() const { return this->personne; }
		/*virtual void afficher(std::ostream& f = std::cout) const{
		   	 Evt1jDur::afficher(f);
		   	 f<<"Lieu = "<<lieu<<std::endl;
		   	 f<<"Personne = "<<personne<<std::endl;
		}*/
        	QString toString() const
		{
		   	 std::stringstream f;
			f<<"******** Evenement ********"<<"\n\n"<<"Date = "<<date<<"\n"<<" Sujet = "<<sujet<<"\n"<<"Debut = "<<debut<<"\n"<<"Duree = "<<duree<<"\n"<<"Lieu = 					"<<lieu<<"Personne = "<<personne<<"\n\n";
		 	   return f.str();
		}
		EvtRDV * clone() const { return new RDV(*this); }		/// A mettre ?
    	};

    	std::ostream & operator <<(std::ostream& f, TIME::Evt1j& j);		///A quoi ca sert ca ?

	class EvtTache: public Evt1jDur {
	  public:
		void afficher(std::ostream & f = std::cout);
	};
}

#endif // EVENEMENT_H_INCLUDED








