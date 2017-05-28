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
    Article& a=m.getArticle("id:citation_babage");
    ArticleEditeur fenetre(a);
    fenetre.show();

    return app.exec();
}
