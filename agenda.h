#ifndef EVENEMENT_H_INCLUDED
#define EVENEMENT_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "timing.h"

namespace TIME{

    	class Evt {
	  protected :
		Date date;
		std::string sujet;
      	  public:
		Evt(const Date& d, const std::string& s): date(d), sujet(s) {}
		virtual ~Evt() {}
		const std::string getDescripteur() { return sujet; }
		virtual void afficher(std::ostream &) const
		{
		    std::stringstream f;
		    f<<toString();
		}
		virtual std::string toString() const
		{
		    std::stringstream f;
		    f<<"***** Evt ********"<<"\n"<<" sujet = "<<sujet<<"\n\n";
		    return f.str();
		}
		virtual Evt * clone() const=0;
		const Date & getDate() { return date; }
		virtual bool operator <(Evt & e)
		{
		    return date < e.date;
		}
    	};

    	class Agenda{
		typedef std::vector<Evt*> contEvt;
		contEvt evts;
		Agenda(const Agenda &);
		Agenda & operator =(const Agenda &);
      	  public:
		Agenda() { evts.reserve(10); }
		Agenda & operator <<(const Evt & e)
		{
		        Evt * nevt = e.clone();
		        // nevt pointe sur une copie créée dynamiquement de
		        // l'objet e transmis en parametre
		        evts.push_back(nevt);
		        return *this;
		}
		virtual ~Agenda()
		{
		        for (contEvt::iterator it=evts.begin(); it!=evts.end(); ++it)
		        // On pourrait faire une boucle sur un tableau avec un i
		        // qui va jusqu'a la taille du vecteur et puis delete evts[i]
		            delete *it;
		}
        	virtual void afficher(std::ostream& f=std ::cout)const
           	{
		        for (contEvt::const_iterator it=evts.begin(); it!=evts.end(); ++it)
		            (*it)->afficher(f);
            	}

        	class iterator{
            		contEvt::iterator it;
          	  public:
			iterator(contEvt::iterator i): it(i) {}
			Evt& operator *() const { return **it; }
			iterator & operator ++() { ++it; return *this; }
			iterator & operator --() { --it; return *this; }
			bool operator !=(iterator i) const { return i.it!=it; }
		};
		iterator begin()  { return iterator(evts.begin()); }
		iterator end() { return iterator(evts.end()); }
    	};

    	class Evt1j : public Evt {
      	  public:
		Evt1j(const Date& d, const std::string& s): Evt(d,s) {}
		virtual ~Evt1j() {}
		/*virtual void afficher(std::ostream& f= std::cout) const{
		    f<<"***** Evt ********"<<"\n"<<"Date = "<<date<<" sujet = "<<sujet<<"\n\n";
		}*/
		Evt1j * clone() const { return new Evt1j(*this); }
		std::string toString() const
		{
		    std::stringstream f;
		    f<<"***** Evt ********"<<"\n"<<"Date = "<<date<<" sujet = "<<sujet<<"\n\n";
		    return f.str();
		}
    	};

    	class Evt1jDur: public Evt1j{
		Horaire debut;
		Duree duree;
      	  public:
		Evt1jDur(const Date& d, const std::string& s, const Horaire& deb, const Duree& dur):
		        Evt1j(d, s) ,debut(deb), duree(dur) {}
		~Evt1jDur() {}
		const Horaire& getDebut() const { return this->debut; }
		const Duree& getDuree() const { return this->duree; }
		/*virtual void afficher(std::ostream& f = std::cout) const{
		    Evt1j::afficher(f);
		    this->debut.afficher(f);
		    this->duree.afficher(f);
		}*/
		std::string toString() const
		{
		    std::stringstream f;
		    f<<"***** Evt ********"<<"\n"<<"Debut = "<<debut<<"Duree = "<<duree<<" sujet = "<<sujet<<"\n\n";
		    return f.str();
		}
		Evt1jDur * clone() const { return new Evt1jDur(*this); }
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

    	class RDV: public Evt1jDur{
		std::string lieu;
		std::string personne;
	      public:
		RDV(const Date& d, const std::string& s, const Horaire& deb, const Duree& dur, const std::string& l, const std::string& pers):
		        Evt1jDur(d, s, deb, dur), lieu(l), personne(pers) {}
		~RDV() {}
		/*RDV(const RDV& r): Evt1jDur(r.getDate(), r.getDescription(), r.getDebut(), r.getDuree()),
		        lieu(r.getLieu()), personne(r.getPersonne())
		        {} */
		RDV& operator =(const RDV& r)
		{
		    this->lieu = r.lieu;
		    this->personne = r.personne;

		    Evt1j *x = this;
		    *x = r;
		}
		const std::string& getLieu() const { return this->lieu; }
		const std::string& getPersonne() const { return this->personne; }
		/*virtual void afficher(std::ostream& f = std::cout) const{
		    Evt1jDur::afficher(f);
		    f<<"Lieu = "<<lieu<<std::endl;
		    f<<"Personne = "<<personne<<std::endl;
		}*/
        	std::string toString() const
		{
		    std::stringstream f;
		    f<<"***** Evt ********"<<"\n"<<"Lieu = "<<lieu<<"Personne = "<<personne<<" sujet = "<<sujet<<"\n\n";
		    return f.str();
		}
		RDV * clone() const { return new RDV(*this); }
		void afficher(std::ostream & f = std::cout) const {
		    Evt1jDur::afficher(f);
		    f<<personne<<std::endl;
		    f<<lieu<<std::endl;
        	}
    	};

    	std::ostream& operator <<(std::ostream& f, TIME::Evt1j& j);


	class EvtManager {
	  protected:
		Evt ** evt;
		unsigned int nb;
		unsigned int nbMax;
		QString file;
	  public:
		virtual void addEvt(Evt * e);
		virtual Evt & addNewEvt();
		Evt * trouverEvt(QSring & s);
		void load(QString & f);
		void save(QString & f);
	};

	class EvtManagerTache: public EvtManager {	// Attribut : une tache à mettre ou pas ?
	  public:
		void addEvt(Tache * T)			// Je dois mettre en paramtre une tache ou un Evt ?
		{
			if (nb == nbMax)
			// A continuer ...
		}
	};

	class EvtManagerRDV: public EvtManager {	// Attribut : un RDV à mettre ou pas ?
	  public:
		void addEvt(Evt * e);
		Evt & addNewEvt();
	};

	class EvtTache: public Evt1jDur {
	  public:
		void afficher(std::ostream & f = std::cout);
	};
}

#endif // EVENEMENT_H_INCLUDED








