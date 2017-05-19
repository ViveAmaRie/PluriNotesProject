#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPushButton bouton("Chaud pour coder? c'est parti!");
    bouton.show();

    return app.exec();
}
