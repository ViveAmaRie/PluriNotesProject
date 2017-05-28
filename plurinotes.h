#ifndef PLURINOTES_H
#define PLURINOTES_H

#include <string>
#include <iostream>
#include <typeinfo>
#include <QObject>
#include <QFile>
#include <QXmlStreamWriter>
#include <QString>
#include "timing.h"

using namespace std;
using namespace TIME;


/*********************************************************************
***                          NotesException                        ***
**********************************************************************/
/*! \class NotesException
        \brief Classe d'ensemble d'exceptions générées par les Notes
*/

class NotesException {
public:
    //! Constructeur à partir d'un QString
    NotesException(const QString& message): info(message) {}
    //! Retourne le message d'erreur
    QString getInfo() const {
        return info;
    }
private:
    QString info;
};
/*********************************************************************/










/*********************************************************************
***                           Relation                             ***
**********************************************************************/
/*! \class Relation
        \brief Classe qui gère les relations et les couples qui constituent les relations
*/

class Relation {
    QString title;
    QString description;

public:
    //! Constructeur d'une Relation à partir d'un titre et d'une description
    Relation(const QString& titre, const QString& description): title(titre), description(description){}

    //! Retourne le titre de la Note
    const QString& getTitle() const {return title;}
    //! Retourne le titre de la Note
    const QString& getDescription() const {return description;}
};







/*********************************************************************
***                             Note                               ***
**********************************************************************/
/*! \class Note
        \brief Classe de Notes composant NotesManager
*/

class Note {
    friend class NotesManager; // La classe NotesManager peut utiliser les méthodes privées d'Note
    //! Identifiant de la note
    QString id;
    //! Titre de la note
    QString title;
    //! Texte de la note
    QString text;
    //! Date de création de la note
    Date dateCreation;
    //! Date de dernière modification de la note
    Date dateLastModif;
    //! Etat de la note : active ou archivée
    QString noteStatus;

    //! Constructeur de Note par recopie interdit
    Note(const Note& n): id(n.id), title(n.title), text(n.text){}
    //! Affectation, Recopie par operator= interdite
    Note& operator=(const Note& n) const;

  public:
    //! Constructeur de Note à partir d'un id, d'un titre et d'un texte
    Note(const QString& id, const QString& title, const QString& text) : id(id), title(title), text(text) {}

    //! Retourne l'identificateur de la Note
    const QString& getId() const {return id;}
    //! Retourne le titre de la Note
    const QString& getTitle() const {return title;}
    //! Retourne le texte de la Note
    const QString& getText() const {return text;}
    //! Retourne la date de création de la Note
    const Date& getDateCreation() const {return dateCreation;}
    //! Retourne la date de dernière modification de la Note
    const Date& getDateLastModif() const {return dateLastModif;}

    //! Affecte une valeur au titre de la Note
    void setTitle(const QString& t) { title = t; }
    //! Affecte une valeur au texte de la Note
    void setText(const QString& t) { text = t; }

    //! Edite de la Note
    void edit();

    //! Retourne une référence sur la version courante de la Note
    Note& getCurrentVersion();

    //! Retourne une référence sur la version de la Note ayant le numéro numVersion
    Note& getOlderVersion(int numVersion);

    //! Sauvegarde une version de la Note ayant le numéro numVersion
    void saveVersion(int numVersion);

    //! Restaure une ancienne version de la Note ayant le numéro numVersion
    void restoreVersion(int numVersion);

    //! Crée une nouvelle Relation r composée des Notes n1 et n2
    void createRelation(Relation r, Note n1, Note n2) ;

};
/*********************************************************************/



/*********************************************************************
***                            Surcharge                           ***
**********************************************************************/
ostream& operator<<(ostream& f, const Note& n);
/*********************************************************************/


/*! \enum TaskStatus
        \brief Classe d'ensemble de taches, héritant de Notes
*/
enum TaskStatus{
    pending,
    inProgress,
    done,
};

class Task : public Note {
private:
    //! Action d'une tache
    QString action;
    //! Priorité d'une tache
    int priority;
    //! Deadline d'uune tache
    Date deadline;
    //! Etat de la tache
    TaskStatus taskStatus;
public:
    //! Retourne l'action d'une tache
    QString getAction() const {return action;}
    //! Retourne la priorité d'une tache
    int getPriority() const {return priority;}
    //! Retourne la deadline d'une tache
    Date getDeadline() const {return deadline;}
    //! Retourne l'état de la tache
    TaskStatus getTaskStatus() const {return taskStatus;}

};

/*! \brief Classe d'ensemble d'Articles, héritant de Notes
*/
class Article : public Note {
private:
    //!texte d'un article
    QString text;
public:
    //!retourne le texte d'un article
    QString getText() const {return text;}
};

/*! \brief Classe d'ensemble de mutltimédia, héritant de Notes
*/

class Multimedia : public Note {
private:
    //!description
    QString description;
    //!fichier image
    QString filePicture; //type à discuter
public:
    //!retourne la description d'un multimedia
    QString getDescription() const {return description;}
    //!retourne le fichier image que contient le multimedia
    QString getFilePicture() const {return filePicture;}
};
/*! \brief Classe d'ensembles d'enregistrements audio, héritant de Multimedia
*/
class Audio : public Multimedia {

};
/*! \brief Classe d'ensembles d'enregistrements video, héritant de Multimedia
*/
class Video : public Multimedia {

};
/*! \brief Classe d'ensembles d'image, héritant de Multimedia
*/
class Picture : public Multimedia {

};





/*********************************************************************
***                           NotesManager                         ***
**********************************************************************/
/*! \class NotesManager
        \brief Classe singleton qui gère les Notes, dont elle est composée
*/

class NotesManager {
    //! Tableau de pointeurs sur des Notes
    Note** notes;
    //! Nombre de Notes dans le tableau
    unsigned int nbNotes;
    //! Taille allouée au tableau
    unsigned int nbMaxNotes;
    //! Ajoute la Note n au tableau notes
    void addNote(Note* n);
    //! Nom du fichier
    mutable QString filename;

    //! Renvoie la Note correspondant à id si elle dans le tableau notes
    Note* findNote(const QString& id) const;


    //! Constructeur qui initialise le tableau et sa taille à 0
    NotesManager():notes(nullptr), nbNotes(0),
                   nbMaxNotes(0), filename("tmp.xml"){}

    //! Constructeur de recopie interdite
    NotesManager(const NotesManager& m);

    //! Destructeur de NotesManager, détruit les Notes puis le tableau notes
    ~NotesManager(){
            for(unsigned int i = 0 ; i < nbNotes ; i++)
                delete notes[i];
            delete[] notes;
        }

    //! Surcharge de =, recopie par operator= interdite
    NotesManager& operator=(const NotesManager& m);

    struct Handler {
        NotesManager* instance;

        Handler() : instance(nullptr){}
        ~Handler() {
            if(instance)
                delete instance;
            instance = nullptr;
        }

    };

    //! Handler permettant de rendre NotesManager singleton
    static Handler handler;

    //! Générateur d'identificateurs pour les Notes
    QString genererId();

    //! Garde le pointeur d'une note existante (à partir d'un nom, d'un nom de fichier et d'un texte) dans le tableau notes
    Note& addNote(const QString& nom, const QString& filename, const QString& text);
    //! Garde le pointeur d'une note existante n dans le tableau notes

public:
    //! Renvoie la seule instance de NotesManager
    static NotesManager& getManager();
    //! Renvoie la seule instance de NotesManager
    static void freeManager();

    //! Renvoie une référence vers la Note correspondant à id si elle existe dans le tableau notes
    Note& getNote(const QString& id);
    //! Renvoie une référence const vers la Note correspondant à id si elle existe dans le tableau notes
    const Note& getNote(const QString& code) const;
    //! Renvoie une référence vers la nouvelle Note créée avec l'id id
    Note& getNewNote(const QString& id);

    //! Renvoie le nom du fichier
    QString getFilename() const { return filename; }
    void setFilename(const QString& f) { filename = f; }

    //! Charge le fichier dont le nom correspond au filename
    void load();

    //! Sauvegarde la note dans le fichier appelé filename
    void save() const;

// Class Iterator
    // Dans la partie public car on doit pouvoir y accéder depuis le main
    class Iterator {
        //!
        friend class NotesManager; // Car la méthode Iterator getIterator() const; va utiliser le constructeur Iterator(Note** a, int nbR); qui est dans la zone privée
        // On a besoin d'un pointeur qui pointe sur l'Note courant
        Note** currentNote;
        // On a besoin d'une variable qui va compter le nombre d'éléments parcourus, pour voir si on a parcouru toute la collection ou pas
        int nbRemain;

        Iterator(Note** n, int nbR) : currentNote(n), nbRemain(nbR){}

    public:
        Iterator(): currentNote(nullptr), nbRemain(0){}
        bool isDone() const {
            return nbRemain == 0;
        }
        void next() { // Pas const car il va modifier l'iterator pour passer à l'élément suivant
            if(isDone())
                throw NotesException("ERREUR : Fin de la collection\n");
            currentNote++;
            nbRemain--;
        }
        void first() {
            //indice_projet = 0;
        }
        Note& current() const {
            if(isDone())
                throw NotesException("ERREUR : Fin de la collection\n");
            return **currentNote;
        }
    };

    //! Renvoie un itérateur sur le tableau notes
    Iterator getIterator() const {
        return Iterator(notes, nbNotes);
    }
};

/*********************************************************************/







#endif // PLURINOTES_H
