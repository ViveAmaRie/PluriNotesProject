#ifndef PLURINOTES_H
#define PLURINOTES_H


#include<string>
#include<iostream>
#include<typeinfo>
#include<QObject>
#include<QFile>
#include<QXmlStreamWriter>
#include "timing.h"

using namespace std;
using namespace TIME;


/*********************************************************************
***                          NotesException                        ***
**********************************************************************/

class NotesException{
public:
    NotesException(const string& message):info(message) {}
    string getInfo() const {
        return info;
    }
private:
    string info;
};
/*********************************************************************/


/*********************************************************************
***                           NotesManager                         ***
**********************************************************************/
class NotesManager
{
    //! Tableau de pointeurs sur des Notes
    Note** notes;
    //! Nombre de Notes dans le tableau
    unsigned int nbNotes;
    //! Taille allouée au tableau
    unsigned int nbMaxNotes;
    //! Ajoute la Note n au tableau notes
    void addNote(Note* n);
    //! Nom du fichier
    string filename;

    //! Renvoie la Note correspondant à id si elle dans le tableau notes
    Note* findNote(const string& id) const;


    //! Constructeur qui initialise le tableau et sa taille à 0
    NotesManager():notes(NULL), nbNotes(0),
                   nbMaxNotes(0), filename("tmp.dat"){}

    //! Constructeur de recopie interdite
    NotesManager(const NotesManager& n);

    //! Destructeur de NotesManager, détruit les Notes puis le tableau notes
    ~ProjetManager(){
        for(unsigned int i = 0 ; i < nb ; i++)
            delete projets[i];
        delete[] projets;
    }

    //! Surcharge de =, recopie par operator= interdite
    NotesManager& operator=(const NotesManager&);

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
    string genererId();

public:
    //! Renvoie la seule instance de NotesManager
    static NotesManager& getInstance();
    //! Renvoie la seule instance de NotesManager
    static void libererInstance();

    //! Crée une Note et garde son pointeur dans le tableau notes, à partir d'un nom, d'un nom de fichier et d'une date de disponibilité
    Note& addNote(const string& nom, const string& filename, const Date& dispo);
    //! Renvoie une référence vers la Note correspondant à id si elle existe dans le tableau notes
    Note& getNote(const string& id);
    //! Renvoie une référence const vers la Note correspondant à id si elle existe dans le tableau notes
    const Note& getNote(const string& code) const;

    Note& getNewNote(const string& id);

// Class Iterator
    // Dans la partie public car on doit pouvoir y accéder depuis le main
    class Iterator {
        friend class NotesManager; // Car la méthode Iterator getIterator() const; va utiliser le constructeur Iterator(Note** a, int nbR); qui est dans la zone privée
        // On a besoin d'un pointeur qui pointe sur l'Note courant
        Note** currentNote;
        // On a besoin d'une variable qui va compter le nombre d'éléments parcourus, pour voir si on a parcouru toute la collection ou pas
        int nbRemain;

        Iterator(Note** n, int nbR) : currentNote(n), nbRemain(nbR){}

    public:
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
            indice_projet = 0;
        }
        Note& current() const {
            return **currentNote;
        }
    };

    Iterator getIterator() const {
        return Iterator(notes, nbNotes);
    }
};

/*********************************************************************/




/*********************************************************************
***                             Note                               ***
**********************************************************************/

class Note {
    friend class NotesManager; // La classe NotesManager peut utiliser les méthodes privées d'Note
    string id;
    string title;
    string text;
    Date dateCreation;
    Date dateLastModif;
    string status;

    Note(const Note& n): id(n.id), title(n.title), text(n.text){}
    Note& operator=(const Note& n) const;

  public:
    const string& getId() const {return id;}
    const string& getTitle() const {return title;}
    const string& getText() const {return text;}
    const string& getDateCreation() const {return dateCreation;}
    const string& getDateLastModif() const {return dateLastModif;}

    void edit() {

    }

    Note& getCurrentVersion() {

    }

    Note& getOlderVersion(int numVersion) {

    }

    void saveOlderVersion(int numVersion) {

    }

    void restoreVersion(int numVersion) {

    }

    void createRelation() {

    }

    void setTitle(const string& t) {title = t;}
    void setText(const string& t) {text = t;}

    Note(const string& _id, const string& _title, const string& _text) :
        id(_id), title(_title), text(_text) {}

};
/*********************************************************************/



/*********************************************************************
***                            Surcharge                           ***
**********************************************************************/
ostream& operator<<(ostream& f, const Note& n);
/*********************************************************************/


/*! \enum TaskStatus
        \brief Classe d'ensembles de taches, héritant de Tache
        La classe possède un pointeur de Taches qui représente les composants de la tache composite.
*/
enum TaskSTatus{

};

class Task : public Notes {
private:
    //!action d'une tache
    string action;
    //!priorité d'une tache
    int priority;
    //!deadline d'uune tache
    Date deadline;
    //!état de la tache
    STATUS taskStatus;


public:

};

#endif // PLURINOTES_H
