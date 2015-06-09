#ifndef EVENEMENT_H_INCLUDED
#define EVENEMENT_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <QString>
#include <QDate>
#include <QFile>
#include <QTextStream>
#include <QtXml>
#include "timing.h"
#include "gestionprojet.h"

using namespace std;

    class Evt {
	  protected :
        QDate date;
		QString sujet;
		QTime debut;
		Duree duree;
      public:
		Evt(const QDate & d, const QString & s, const QTime& deb, const Duree & dur): date(d), sujet(s), debut(deb), duree(dur) {}
		virtual ~Evt() {}
		const QString & getDescripteur() { return sujet; }
		const QDate & getDate() { return date; }
		const QTime & getDebut() const { return this->debut; }
		const Duree & getDuree() const { return this->duree; }
        virtual QString toString() const
        {
                std::stringstream f;
                f<<"******** Evenement ********"<<"\n\n"<<"Date = "<<date.toString()<<"\n"<<" Sujet = "<<sujet<<"\n\n";
                return f.str();
        }
        virtual void afficher(std::ostream & f) const
		{
		    	std::stringstream f;
		    	f<<toString();
		}
        virtual bool imEvtTache() = 0;
    };

    class EvtRDV: public Evt{
		QString lieu;
        QString personne;
      public:
        EvtRDV(const QDate & d, const QString & s, const QTime & deb, const Duree & dur, const QString & l, const QString & pers):
                Evt(d, s, deb, dur), lieu(l), personne(pers) {}
		~EvtRDV() {}
		/*RDV(const RDV& r): Evt1jDur(r.getDate(), r.getDescription(), r.getDebut(), r.getDuree()),
		        lieu(r.getLieu()), personne(r.getPersonne())
		        {} */
		EvtRDV & operator =(const EvtRDV & r)
		{
		    	this->lieu = r.lieu;
		    	this->personne = r.personne;

                EvtRDV *x = this;
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
    };

    std::ostream & operator <<(std::ostream & f, Evt & j);		///A quoi ca sert ca ?

	class EvtTache: public Evt {
        Tache * tache;
      public:
        EvtTache(Tache * t): Evt(),tache(t) {}          //A finir !
		void afficher(std::ostream & f = std::cout);
	};

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
        Evt ** getEvt() { return evt; }
        virtual void addEvt(const Evt * e, const QDate & date, const QTime & h);
        virtual Evt & addNewEvt(const QDate & d, const QString & s, const QTime & deb, const Duree & dur, const QString & l, const QString & pers);
        virtual void supprimerEvt(const QString & s);
        Evt * trouverEvt(const QString & s);
        bool isEvtExistant(const Evt * e) { return trouverEvt(e->getDescripteur()) != NULL; }
        void load(QString & f);
        void save(QString & f);

        class Iterator{
            Evt ** currentEvt;
            unsigned int nbRemain;
            Iterator(Evt ** e, unsigned int nb): currentEvt(e), nbRemain(nb) {}
            friend class EvtManager;
        public:
            Iterator(): nbRemain(0), currentEvt(0) {}
            bool isDone() const { return nbRemain == 0; }
            void next() {
                if (isDone())
                    throw CalendarException("error, next on an iterator which is done");
                nbRemain--;
                currentEvt++;
            }
            Evt * current2() const {
                if (isDone())
                    throw CalendarException("error, indirection on an iterator which is done");
                return * currentEvt;
            }
        };

        Iterator * getIterator()const {
            Iterator * i = new Iterator(evt,nb);
            return i;
        }

        /*class iterator {
            Evt ** current;
            iterator(Evt ** e): current(e) {}
            friend class EvtManager;
          public:
            iterator(): current(0) {}
            Evt & operator * () const { return ** current; }
            bool operator != (iterator it) const { return current != it.current; }
            iterator & operator++(){ ++current; return * this; }
        };
        iterator begin() { return iterator(evt); }
        iterator end() { return iterator(evt+nb); }*/
    };   


#endif // EVENEMENT_H_INCLUDED








