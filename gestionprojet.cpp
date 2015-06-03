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

///On définit l'addition de 2 durées
Duree Duree::operator+(const Duree& d2)const{
    Duree dur=*this;
    dur.setDuree(dur.getDureeEnMinutes()+d2.getDureeEnMinutes());
    return dur;
}
///On définit la division d'une duree par un entier. La duree obtenue doit être multiple de 15.
Duree Duree::div(unsigned int n) const{
    Duree dur=*this;
    unsigned int u=dur.getDureeEnMinutes()/n;
    if (u%15<8) u=u-u%15;
    if (u%15>7) u=u+(15-u%15);
    dur.setDuree(u);
    return dur;

}
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
}

Tache::~Tache(){}
TacheComposite::~TacheComposite(){
    delete sousTaches;
    delete tachesPrecedentes;
}

*/
TacheUnitaire::~TacheUnitaire(){}


//TacheExplorer::TacheExplorer():taches(),nb(0),nbMax(0){}


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

void TacheExplorer::ajouterTacheUnitaire(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt){
    if (trouverTache(id)) throw CalendarException("erreur, TacheExplorer, tache deja existante");
    TacheUnitaire* newt=new TacheUnitaire(id,t,dur,dispo,deadline,preempt);
    addItem(newt);
}

void TacheExplorer::ajouterTacheComposite(const QString& id, const QString& t){
    if (trouverTache(id)) throw CalendarException("erreur, TacheExplorer, tache deja existante");
    TacheComposite* newt=new TacheComposite(id,t);
    addItem(newt);
}

///L'ajout d'une tache unitaire dans une tache composite implique la copie de la référence de cette tache dans le tache manager
void TacheComposite::ajouterTacheUnitaire(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt){
    if (sousTaches->trouverTache(id)) throw CalendarException("erreur, TacheExplorer, tache deja existante");
    TacheUnitaire* newt=new TacheUnitaire(id,t,dur,dispo,deadline,preempt);
    sousTaches->addItem(newt);
    updateAttributs();
    TacheManager::getInstance().addItem(newt);
}

void TacheComposite::ajouterTacheComposite(const QString& id, const QString& t){
    if (sousTaches->trouverTache(id)) throw CalendarException("erreur, TacheExplorer, tache deja existante");
    TacheComposite* newt=new TacheComposite(id,t);
    sousTaches->addItem(newt);
    updateAttributs();
}

void TacheComposite::ajouterTacheExistante(Tache* t){
    sousTaches->addItem(t);
    updateAttributs();
}

void TacheUnitaire:: ajouterTachePrec(Tache* t){
    tachesPrecedentes->addItem(t);
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

Duree TacheComposite::findDuree()const
{
    TacheExplorer::Iterator i = sousTaches->getIterator();
    Duree d=0;
    while(!i.isDone())
    {
        d=d+i.current2().getDuree();
        i.next();
    }
    return d;
}

QDate TacheComposite::findDispo() const{
    TacheExplorer::Iterator i=sousTaches->getIterator();
    QDate d=DATE_MAX;
    while(!i.isDone()){
        if(i.current2().getDateEcheance()<d)
        {
            d=i.current2().getDateEcheance();
        }
        i.next();
    }
    return d;
}

QDate TacheComposite::findEcheance() const{
    TacheExplorer::Iterator i=sousTaches->getIterator();
    QDate d=DATE_MIN;
    while(!i.isDone()){
        if(i.current2().getDateEcheance()>d)
        {
            d=i.current2().getDateEcheance();
        }
        i.next();
    }
    return d;
}


bool TacheComposite::findAllProgrammed()const{
    bool d=true;
    TacheExplorer::Iterator i = sousTaches->getIterator();
    while(!i.isDone()){
        if (i.current2().getProgrammee()==false)
        {
            d=false;
        }
        i.next();
    }
    return d;
}

void TacheComposite::updateAttributs(){
    duree=findDuree();
    disponibilite=findDispo();
    echeance=findEcheance();
    programmee=findAllProgrammed();
}
