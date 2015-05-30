#include "gestionprojet.h"
#include <QFile>
#include <QTextCodec>
#include <QtXml>
//#include <QMessageBox>



/*void Tache::setId(const QString& str){
  if (TacheManager::getInstance().isTacheExistante((str))) throw CalendarException("erreur TacheManager : tache id d�j� existante");
  identificateur=str;
}
*/


QTextStream& operator<<(QTextStream& f, const Duree & d){ d.afficher(f); return f; }

QTextStream& operator>>(QTextStream& flot, Duree& duree){
    unsigned int h,m;
    bool ok=true;
    flot>>h;
    if (flot.status()!=QTextStream::Ok) ok=false;

    if(flot.read(1)=="H") {
        flot>>m;
        if (flot.status()!=QTextStream::Ok) ok=false;
    }
    else {
        ok=false;
    }
    if (ok) duree=Duree(h,m);
    return flot;
}

 void Duree::afficher(QTextStream& f) const {
     f.setPadChar('0');
     f.setFieldWidth(2);
     f<<nb_minutes/60;
     f.setFieldWidth(0);
     f<<"H";
     f.setFieldWidth(2);
     f<<nb_minutes%60;
     f.setFieldWidth(0);
     f.setPadChar(' ');
 }

QTextStream& operator<<(QTextStream& fout, const Tache& t){
    fout<<t.getId()<<"\n";
    fout<<t.getTitre()<<"\n";
    fout<<t.getDuree()<<"\n";
    fout<<t.getDateDisponibilite().toString()<<"\n";
    fout<<t.getDateEcheance().toString()<<"\n";
    return fout;
}

/*Visiteur::visiterObjetDeTypeTacheUnitaire (TacheUnitaire* tU)
{}

Visiteur::visiterObjetDeTypeTacheComposite (TacheComposite* tC)
{}

TacheUnitaire::accept(Visiteur *v)
{
    v->visitObjetDeTypeTacheUnitaire(this);
}

TacheComposite::accept(Visiteur *v)
{
    v->visitObjetDeTypeTacheComposite(this);
}*/
Tache::~Tache(){}
TacheComposite::~TacheComposite(){}
TacheUnitaire::~TacheUnitaire(){}

TacheExplorer::TacheExplorer():taches(),nb(0),nbMax(0){}


void TacheExplorer::addItem(Tache* t){
    if (nb==nbMax){
        Tache** newtab=new Tache*[nbMax+10];
        for(unsigned int i=0; i<nb; i++) newtab[i]=taches[i];
        // ou memcpy(newtab,taches,nb*sizeof(Tache*));
        nbMax+=10;
        Tache** old=taches;
        taches=newtab;
        delete[] old;
    }
    taches[nb++]=t;
}

Tache* TacheExplorer::trouverTache(const QString& id)const{
    for(unsigned int i=0; i<nb; i++)
        if (id==taches[i]->getId()) return taches[i];
    return 0;
}

Tache& TacheExplorer::ajouterTacheUnitaire(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt){
    if (trouverTache(id)) throw CalendarException("erreur, TacheManager, tache deja existante");
    Tache* newt=new TacheUnitaire(id,t,dur,dispo,deadline,preempt);
    addItem(newt);
    return *newt;
}

Tache& TacheExplorer::ajouterTacheComposite(const QString& id, const QString& t){
    if (trouverTache(id)) throw CalendarException("erreur, TacheManager, tache deja existante");
    Tache* newt=new TacheComposite(id,t);
    addItem(newt);
    return *newt;
}
Tache& TacheComposite::ajouterTacheUnitaire(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt){
    if (trouverTache(id)) throw CalendarException("erreur, TacheManager, tache deja existante");
    Tache* newt=new TacheUnitaire(id,t,dur,dispo,deadline,preempt);
    addItem(newt);
    TacheManager::getInstance().addItem(newt);
    return *newt;
}

Tache& TacheExplorer::getTache(const QString& id){
    Tache* t=trouverTache(id);
    if (!t) throw CalendarException("erreur, TacheManager, tache inexistante");
    return *t;
}

/*const Tache& TacheManager::getTache(const QString& id)const{
    return const_cast<TacheManager*>(this)->getTache(id);
}
*/
TacheExplorer::~TacheExplorer(){
    //if (file!="") save(file);
    for(unsigned int i=0; i<nb; i++) delete taches[i];
    delete[] taches;
    //file="";
}

TacheManager::TacheManager():TacheExplorer(){}
TacheManager::~TacheManager(){
    if (file!="") save(file);
    for(unsigned int i=0; i<nb; i++) delete taches[i];
    delete[] taches;
    file="";
}
void TacheManager::load(const QString& f){
    //qDebug()<<"debut load\n";
    this->~TacheManager();
    file=f;
    QFile fin(file);
    // If we can't open it, let's show an error message.
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw CalendarException("Erreur ouverture fichier t�ches");
    }
    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(&fin);
    //qDebug()<<"debut fichier\n";
    // We'll parse the XML until we reach end of it.
    while(!xml.atEnd() && !xml.hasError()) {
        // Read next element.
        QXmlStreamReader::TokenType token = xml.readNext();
        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {
            // If it's named taches, we'll go to the next.
            if(xml.name() == "taches") continue;
            // If it's named tache, we'll dig the information from there.
            if(xml.name() == "tache") {
                qDebug()<<"new tache\n";
                QString identificateur;
                QString titre;
                QDate disponibilite;
                QDate echeance;
                Duree duree;
                bool preemptive;

                QXmlStreamAttributes attributes = xml.attributes();
                // Let's check that Task has attribute.
                if(attributes.hasAttribute("preemptive")) {
                    QString val =attributes.value("preemptive").toString();
                    preemptive=(val == "true" ? true : false);
                }
                //qDebug()<<"preemptive="<<preemptive<<"\n";

                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named tache.


                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "tache")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        // We've found identificteur.
                        if(xml.name() == "identificateur") {
                            xml.readNext(); identificateur=xml.text().toString();
                            //qDebug()<<"id="<<identificateur<<"\n";
                        }

                        // We've found titre.
                        if(xml.name() == "titre") {
                            xml.readNext(); titre=xml.text().toString();
                            //qDebug()<<"titre="<<titre<<"\n";
                        }
                        // We've found disponibilite
                        if(xml.name() == "disponibilite") {
                            xml.readNext();
                            disponibilite=QDate::fromString(xml.text().toString(),Qt::ISODate);
                            //qDebug()<<"disp="<<disponibilite.toString()<<"\n";
                        }
                        // We've found echeance
                        if(xml.name() == "echeance") {
                            xml.readNext();
                            echeance=QDate::fromString(xml.text().toString(),Qt::ISODate);
                            //qDebug()<<"echeance="<<echeance.toString()<<"\n";
                        }
                        // We've found duree
                        if(xml.name() == "duree") {
                            xml.readNext();
                            duree.setDuree(xml.text().toString().toUInt());
                            //qDebug()<<"duree="<<duree.getDureeEnMinutes()<<"\n";
                        }
                    }
                    // ...and next...
                    xml.readNext();
                }
                //qDebug()<<"ajout tache "<<identificateur<<"\n";
                ajouterTacheUnitaire(identificateur,titre,duree,disponibilite,echeance,preemptive);

            }
        }
    }
    // Error handling.
    if(xml.hasError()) {
        throw CalendarException("Erreur lecteur fichier taches, parser xml");
    }
    // Removes any device() or data from the reader * and resets its internal state to the initial state.
    xml.clear();
    //qDebug()<<"fin load\n";
}

void  TacheManager::save(const QString& f){
    file=f;
    QFile newfile( file);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw CalendarException(QString("erreur sauvegarde t�ches : ouverture fichier xml"));
    QXmlStreamWriter stream(&newfile);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("taches");
    for(unsigned int i=0; i<nb; i++){
        stream.writeStartElement("tache");
        //stream.writeAttribute("preemptive", (taches[i]->isPreemptive())?"true":"false");
        stream.writeTextElement("identificateur",taches[i]->getId());
        stream.writeTextElement("titre",taches[i]->getTitre());
        stream.writeTextElement("disponibilite",taches[i]->getDateDisponibilite().toString(Qt::ISODate));
        stream.writeTextElement("echeance",taches[i]->getDateEcheance().toString(Qt::ISODate));
        QString str;
        str.setNum(taches[i]->getDuree().getDureeEnMinutes());
        stream.writeTextElement("duree",str);
        stream.writeEndElement();
    }
    stream.writeEndElement();
    stream.writeEndDocument();
    newfile.close();
}


TacheManager::Handler TacheManager::handler=TacheManager::Handler();

TacheManager& TacheManager::getInstance(){
    if (handler.instance==0) handler.instance=new TacheManager;
    return *(handler.instance);
}

void TacheManager::libererInstance(){
    if (handler.instance!=0) delete handler.instance;
    handler.instance=0;
}


Duree TacheComposite::getDuree() const
{
    return 0;
}
bool TacheComposite::getAllProgrammed()const
{
    return 0;
}
QDate TacheComposite::getDispo() const
{
    QDate qd (1995,2,1);
    return qd;
}

QDate TacheComposite::getDeadline() const
{
    QDate qd (2000,2,1);
    return qd;
}

/*Duree TacheComposite::getDuree() const
{
    Duree d=0;
    while(!iterateur->isDone())
    {
        iterateur->next();
        d=d+iterateur->iterateur->current().getDuree();
    }
    return d;
}
QDate TacheComposite::setDispo(QDate d) const
{
    while(!iterateur->isDone())
    {
        if(iterateur->current().getDateEcheance()<d)
        {
            throw TacheException
        }
        iterateur->next();

    }
    return d;
}
QDate TacheComposite::setDeadline(QDate d) const

    while(!iterateur->isDone())
    {
        if(iterateur->current().getDateDisponibilite()>d)
        {
            throw TacheException
        }
        iterateur->next();

    }
    return d;
{}

bool TacheComposite::getAllProgrammed()const
{
    bool d=true;
    while(!iterateur->isDone())
    {

        if (iterateur->current().getProgrammee()==false)
        {
            d=false;
        }
        iterateur->next();
    }
    return d;
}

*/
