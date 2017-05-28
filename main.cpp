#include <noteeditor.h>
#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPushButton bouton("Chaud pour coder ? C'est parti !");
    bouton.show();

    QString fichier = "./notes.xml";
    NotesManager& m = NotesManager::getManager();
    m.setFilename(fichier);
    m.load();
    Note& n = m.getNewNote("id:citation_babage");
    NoteEditor fenetre(n);
    fenetre.show();

    return app.exec();
}
