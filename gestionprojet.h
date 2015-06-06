#ifndef GESTIONPROJET
#define GESTIONPROJET
#include <QString>
#include <QDate>
#include <QTextStream>
#include "timing.h"


class CalendarException{
public:
    CalendarException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};

/**
 * @brief The Duree class
 * Classe de définition d'une durée. Une duree peut être construite à partir de sa valeur en minutes ou heures et minutes
 */
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
    Duree operator+(const Duree& d2)const;
    Duree div(unsigned int n) const;
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
class TacheExplorer;

class Tache {
protected:
    const QDate DATE_MIN = QDate(0,1,1);
    const QDate DATE_MAX = QDate(9999,1,1);
    QString identificateur;
    QString titre;
    Duree duree;
    QDate disponibilite;
    QDate echeance;
    bool programmee;
    Tache(const Tache& t);
    Tache& operator=(const Tache&);
    friend class TacheExplorer;
public:
    Tache(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline,bool program=false):
        identificateur(id),titre(t),duree(dur),disponibilite(dispo),echeance(deadline),programmee(program){}
    virtual ~Tache();
    TacheExplorer* tachesPrecedentesTraitement;
    TacheExplorer* tachesPrecedentesAffichage;
    virtual void ajouterTachePrecedenteA(Tache* u)=0;
    QString getId() const { return identificateur; }
    void setId(const QString& str);
    QString getTitre() const { return titre; }
    void setTitre(const QString& str) { titre=str; }
    Duree getDuree() const { return duree; }
    QDate getDateDisponibilite() const {return disponibilite; }
    QDate getDateEcheance() const {  return echeance; }
    bool getProgrammee()const{return programmee;}
    TacheExplorer* getTachesPrecedentesPourAffichage()const{return tachesPrecedentesAffichage;}
    TacheExplorer* getTachesPrecedentesPourTraitement()const{return tachesPrecedentesTraitement;}
};

class TacheExplorer {
protected:
    Tache** taches;
    unsigned int nb;
    unsigned int nbMax;

    void ajouterTacheUnitaire(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt=false);
    void ajouterTacheComposite(const QString& id, const QString& t);
    TacheExplorer& operator=(const TacheExplorer& um);
public:
    TacheExplorer(){}
    ~TacheExplorer();
    TacheExplorer(const TacheExplorer& um);
    Tache* trouverTache(const QString& id) const;
    Tache& getTache(const QString& id);
    bool isTacheExistante(const QString& id) const { return trouverTache(id)!=0; }
    //const Tache& getTache(const QString& code) const;
    void addItem(Tache* t);
    void concatSansRedondance(const TacheExplorer* tE);
    class Iterator{
        friend class TacheExplorer;
        Tache** currentTache;
        unsigned int nbRemain;
        Iterator(Tache** u, unsigned int nb):currentTache(u),nbRemain(nb){}
    public:
        Iterator():nbRemain(0),currentTache(0){}
        bool isDone() const { return nbRemain==0; }
        void next() {
            if (isDone())
                throw CalendarException("error, next on an iterator which is done");
            nbRemain--;
            currentTache++;
        }
        Tache* current2() const {
            if (isDone())
                throw CalendarException("error, indirection on an iterator which is done");
            return *currentTache;
        }
    };
    Iterator getIterator()const {
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

class TacheComposite:public Tache{
public:
    TacheComposite (const QString& id, const QString& t):
        Tache(id,t,findDuree(),findDispo(),findEcheance(),findAllProgrammed()),sousTaches(new TacheExplorer()){
        tachesPrecedentesTraitement = new TacheExplorer();
        tachesPrecedentesAffichage = new TacheExplorer();
    }
    virtual ~TacheComposite(){}
    //TacheExplorer* tachesPrecedentes;
    TacheExplorer* sousTaches;
    //void accept(Visiteur* v);
    void ajouterTacheComposite(const QString& id, const QString& t);
    void ajouterTacheUnitaire(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt);
    Duree findDuree() const;
    QDate findDispo() const;
    QDate findEcheance() const;
    bool getAllInproject()const;
    bool findAllProgrammed()const;
    void updateTachesPrecedentes();
    void updateAttributs();
    void ajouterTachePrecedenteA(Tache* u);
    void ajouterTacheExistante(Tache* t);

};

class Projet:public TacheComposite{
public:
    QString description;
    Projet(const QString& id, const QString& t,const QString& des):
        TacheComposite(id,t),description(des){}
    virtual ~Projet(){}
};

class TacheUnitaire:public Tache{
    bool preemptive;
    bool inTree;
public:
    //void accept(Visiteur* v);
    TacheUnitaire (const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool program=false,bool preemp=false):
        Tache(id,t,dur,dispo,deadline,program),preemptive(preemp){
        tachesPrecedentesAffichage = new TacheExplorer();
        tachesPrecedentesTraitement = new TacheExplorer();
    }
    virtual ~TacheUnitaire();
    void setDuree(const Duree& d) {
        Duree d2(720);
        if(!preemptive && d.getDureeEnMinutes()>d2.getDureeEnMinutes()) throw CalendarException("Duree trop elevee (>12h) pour une tache non-preemptive");
        duree=d;
    }
    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e) {
        if (e<disp) throw CalendarException("erreur Tache : date echeance < date disponibilite");
        if (disp>DATE_MAX || disp<DATE_MIN) throw CalendarException("erreur Tache : disponibilite hors limites de temps");
        if (e>DATE_MAX || e<DATE_MIN) throw CalendarException("erreur Tache : echeance hors limites de temps");
        disponibilite=disp; echeance=e;}
    void ajouterTachePrecedenteA(Tache* u);
    void ajouterTachePrecedente(Tache* tP);
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
suppression de taches.
/*
 * S'occuper de la sauvegarde!!!
 *
}*/
#endif // GESTIONPROJET

