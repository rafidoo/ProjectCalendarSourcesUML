#ifndef GESTIONPROJET
#define GESTIONPROJET
#include <QString>
#include <QDate>
#include <QTextStream>
#include "timing.h"
using namespace std;




class CalendarException{
public:
    CalendarException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};

class Duree{
public:
    //! Constructeur ? partir de heure et minute
    /*! \param h heure avec h>=0
        \param m minute avec 0<=m<=59
        */
    Duree(unsigned int h, unsigned int m):nb_minutes(h*60+m) {if (m>59) throw CalendarException("erreur: initialisation duree invalide");}
    //! Constructeur ? partir de minute
    /*! \param m minute avec m>=0
        */
    Duree(unsigned int m=0):nb_minutes(m) {}
    void setDuree(unsigned int minutes) { nb_minutes=minutes; }
    void setDuree(unsigned int heures, unsigned int minutes) { if (heures==0 && minutes>59) throw CalendarException("erreur: initialisation duree invalide"); nb_minutes=heures*60+minutes; }
    unsigned int getDureeEnMinutes() const { return nb_minutes; } //<!Retourne la duree en minutes
    double getDureeEnHeures() const { return double(nb_minutes)/60; } //<!Retourne la duree en heures
    unsigned int getMinute() const { return nb_minutes%60; }
    unsigned int getHeure() const { return nb_minutes/60; }
    void afficher(QTextStream& f) const; //<!Affiche la duree sous le format hhHmm
private:
    unsigned int nb_minutes;
};

QTextStream& operator<<(QTextStream& f, const Duree & d);
QTextStream& operator>>(QTextStream&, Duree&); //lecture format hhHmm


/*class Visiteur{
public:
    visitObjetDeTypeTacheUnitaire (TacheUnitaire* tacheUnitaire);
    visitObjetDeTypeTacheComposite (TacheComposite* tacheComposite);
};

class Visiteur{};*/


class Tache {
protected:
    QString identificateur;
    QString titre;
    Duree duree;
    QDate disponibilite;
    QDate echeance;
    bool programmee;
    bool terminee;
    Tache(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline,bool program=false, bool ter=false):
            identificateur(id),titre(t),duree(dur),disponibilite(dispo),echeance(deadline),programmee(program),terminee(ter){}
    Tache(const Tache& t);
    Tache& operator=(const Tache&);
    friend class TacheManager;
public:
    ~Tache();
    QString getId() const { return identificateur; }
    void setId(const QString& str);
    QString getTitre() const { return titre; }
    void setTitre(const QString& str) { titre=str; }
    Duree getDuree() const { return duree; }
    void setDuree(const Duree& d) { duree=d; }
    QDate getDateDisponibilite() const {return disponibilite; }
    QDate getDateEcheance() const {  return echeance; }
    bool getProgrammee()const{return programmee;}
    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e) {
        if (e<disp) throw CalendarException("erreur Tache : date echeance < date disponibilite");
        disponibilite=disp; echeance=e;
    }

};
class TacheExplorer {
protected:
    virtual Tache& ajouterTacheUnitaire(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt=false);
    virtual Tache& ajouterTacheComposite(const QString& id, const QString& t);

    Tache** taches;
    unsigned int nb;
    unsigned int nbMax;
    Tache* trouverTache(const QString& id) const;
    TacheExplorer& operator=(const TacheExplorer& um);

public:
    TacheExplorer();
    virtual ~TacheExplorer();
    TacheExplorer(const TacheExplorer& um);
    Tache& getTache(const QString& id);
    void addItem(Tache* t);
    bool isTacheExistante(const QString& id) const { return trouverTache(id)!=0; }
    const Tache& getTache(const QString& code) const;

    class Iterator{
        friend class TacheExplorer;
        Tache** currentTache;
        unsigned int nbRemain;
        Iterator(Tache** u, unsigned nb):currentTache(u),nbRemain(nb){}
    public:
        Iterator():nbRemain(0),currentTache(0){}
        bool isDone() const { return nbRemain==0; }
        void next() {
            if (isDone())
                throw CalendarException("error, next on an iterator which is done");
            nbRemain--;
            currentTache++;
        }
        Tache& current() const {
            if (isDone())
                throw CalendarException("error, indirection on an iterator which is done");
            return **currentTache;
        }
    };
    Iterator getIterator() {
        return Iterator(taches,nb);
    }
    class iterator {
        Tache** current;
        iterator(Tache** u):current(u){}
        friend class TacheExplorer;
    public:
        iterator():current(0){}
        Tache& operator*() const { return **current; }
        bool operator!=(iterator it) const { return current!=it.current; }
        iterator& operator++(){ ++current; return *this; }
    };
    iterator begin() { return iterator(taches); }
    iterator end() { return iterator(taches+nb); }

};

class TacheManager:public TacheExplorer {
private:

    QString file;
    TacheManager();
    ~TacheManager();
    TacheManager(const TacheManager& um);
    TacheManager& operator=(const TacheManager& um);
    struct Handler{
        TacheManager* instance;
        Handler():instance(0){}
        // destructeur appel� � la fin du programme
        ~Handler(){ if (instance) delete instance; }
    };
    static Handler handler;
public:
    void load(const QString& f);
    void save(const QString& f);
    static TacheManager& getInstance();
    static void libererInstance();

    class DisponibiliteFilterIterator {
        friend class TacheManager;
        Tache** currentTache;
        unsigned int nbRemain;
        QDate dispo;
        DisponibiliteFilterIterator(Tache** u, unsigned nb, const QDate& d):currentTache(u),nbRemain(nb),dispo(d){
            while(nbRemain>0 && dispo<(*currentTache)->getDateDisponibilite()){
                nbRemain--; currentTache++;
            }
        }
    public:
        DisponibiliteFilterIterator():nbRemain(0),currentTache(0){}
        bool isDone() const { return nbRemain==0; }
        void next() {
            if (isDone())
                throw CalendarException("error, next on an iterator which is done");
            do {
                nbRemain--; currentTache++;
            }while(nbRemain>0 && dispo<(*currentTache)->getDateDisponibilite());
        }
        Tache& current() const {
            if (isDone())
                throw CalendarException("error, indirection on an iterator which is done");
            return **currentTache;
        }
    };
    DisponibiliteFilterIterator getDisponibiliteFilterIterator(const QDate& d) {
        return DisponibiliteFilterIterator(taches,nb,d);
    }
};

class TacheComposite:public Tache,public TacheExplorer{
    public:
    TacheComposite (const QString& id, const QString& t):
        Tache(id,t,getDuree(),getDispo(),getDeadline(),getAllProgrammed()),TacheExplorer(){}
    ~TacheComposite();

    //void accept(Visiteur* v);
    Tache& ajouterTacheUnitaire(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt);
    Duree getDuree() const;
    QDate getDispo() const;
    QDate getDeadline() const;
    bool getAllInproject();
    bool getAllProgrammed()const;

};

class Projet:public TacheComposite{
public:
    QString description;
    Projet(const QString& id, const QString& t,const QString& des):
        TacheComposite(id,t),description(des){}
};

class TacheUnitaire:public Tache, public TacheExplorer{
    bool preemptive;
    bool inTree;

public:
    //void accept(Visiteur* v);
    TacheUnitaire (const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline,  bool program=false,bool preemp=false):
        Tache(id,t,dur,dispo,deadline,program),preemptive(preemp){}
    ~TacheUnitaire();
    bool isPreemptive() const { return preemptive; }
    void setPreemptive() { preemptive=true; }
    void setNonPreemptive() { preemptive=false; }
    bool isInTree() const { return inTree; }
    void setInTree() { inTree=true; }
    void setNonInTree() { inTree=false; }
};



/*class FabriqueTaches
//plus tard
}*/

/*class FacadeGP {
//Obtenir une tache (On a alors accès à tous ses attributs)
//Obtenir l'ensemble des taches unitaires d'un projet
//Obtenir l'ensemble des taches unitaires non-programmees d'un projet
//Ajouter une tache unitaire à une tache existante
//Créer et ajouter tache à une tache composite
/*
 * S'occuper de la sauvegarde!!!
 *
}*/
#endif // GESTIONPROJET

