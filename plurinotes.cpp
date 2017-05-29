#include "plurinotes.h"

void NotesManager::addNote(Note* n){
    if (nbNotes==nbMaxNotes){
        Note** newtab=new Note*[nbMaxNotes+10];
        for(unsigned int i=0; i<nbNotes; i++) newtab[i]=notes[i];
        nbMaxNotes+=10;
        Note** old=notes;
        notes=newtab;
        delete[] old;
    }
    notes[nbNotes++]=n;
}
void NotesManager::Iterator::suppr(){
    if(allowSuppr && (nbNotes>0) && (nbNotes != indice_note)){
        tab[indice_note] = tab[nbNotes];
        --indice_note;
        --nbNotes;
    } else throw ("Suppression de tache impossible");
}

void NotesManager::deleteNote(Note* n){
    for(Iterator it = getIterator(); !it.isDone(); it.next()){
        if( it.current().getId() == n->getId() ){
            it.suppr();
            --nbNotes;
            return;
        }
    }
    throw NotesException("Suppression impossible : notes non trouvée");
}

Note* NotesManager::findNote(const string& id)const{
    for(unsigned int i=0; i<nbNotes; i++)
        if (id==notes[i]->getId()) return notes[i];
    return 0;
}
