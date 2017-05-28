#include "plurinotes.h"
#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>

void NotesManager::addNote(Note* n) {
    if (nbNotes==nbMaxNotes){
        Note** newtab=new Note*[nbMaxNotes+10];
        for(unsigned int i=0; i<nbNotes; i++) newtab[i]=notes[i];
        nbMaxNotes+=10;
        Note** old=notes;
        notes=newtab;
        delete[] old;
    }
    notes[nbNotes++] = n;
}

Note* NotesManager::findNote(const QString &id) const {
    for(unsigned int i=0; i<nbNotes; i++)
        if (id == notes[i]->getId())
            return notes[i];
    return 0;
}


NotesManager::Handler NotesManager::handler = Handler();

NotesManager& NotesManager::getManager(){
    if (!handler.instance) handler.instance = new NotesManager;
    return *handler.instance;
}

void NotesManager::freeManager(){
    delete handler.instance;
    handler.instance=nullptr;
}

Article& NotesManager::getNote(const QString& id){
    // si la note existe déjà, on en renvoie une référence
    for(unsigned int i = 0 ; i < nbNotes ; i++){
        if (notes[i]->getId() == id)
            return *notes[i];
    }
    // sinon elle est créée
    Note* n = new Note(id,"","");
    addNote(n);
}

void NotesManager::save() const {
    QFile newfile(filename);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw NotesException(QString("erreur sauvegarde notes : ouverture fichier xml"));
    QXmlStreamWriter stream(&newfile);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("notes");
    for(unsigned int i = 0; i < nbNotes; i++){
        stream.writeStartElement("note");
        stream.writeTextElement("id", notes[i]->getId());
        stream.writeTextElement("title", notes[i]->getTitle());
        stream.writeTextElement("text", notes[i]->getText());
        stream.writeEndElement();
    }
    stream.writeEndElement();
    stream.writeEndDocument();
    newfile.close();
}

void NotesManager::load() {
    QFile fin(filename);
    // If we can't open it, let's show an error message.
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw NotesException("Erreur ouverture fichier notes");
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
            if(xml.name() == "notes") continue;
            // If it's named tache, we'll dig the information from there.
            if(xml.name() == "note") {
                qDebug() << "New note\n";
                QString identificateur;
                QString titre;
                QString text;
                QXmlStreamAttributes attributes = xml.attributes();
                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named article.
                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "article")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        // We've found identificteur.
                        if(xml.name() == "id") {
                            xml.readNext(); identificateur=xml.text().toString();
                            qDebug() << "id = " << identificateur << "\n";
                        }

                        // We've found titre.
                        if(xml.name() == "title") {
                            xml.readNext();
                            titre = xml.text().toString();
                            qDebug() << "titre = " << titre << "\n";
                        }
                        // We've found text
                        if(xml.name() == "text") {
                            xml.readNext();
                            text=xml.text().toString();
                            qDebug() << "text = " << text << "\n";
                        }
                    }
                    // ...and next...
                    xml.readNext();
                }
                qDebug() <<"ajout note " << identificateur << "\n";
                addNote(identificateur, titre, text);
            }
        }
    }
    // Error handling.
    if(xml.hasError()) {
        throw NotesException("Erreur lecteur fichier notes, parser xml");
    }
    // Removes any device() or data from the reader * and resets its internal state to the initial state.
    xml.clear();
    qDebug() << "fin load\n";
}
