#ifndef NOTEEDITOR_H
#define NOTEEDITOR_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <plurinotes.h>

class NoteEditor : public QWidget
{
    Q_OBJECT
    Note *note;
    QLabel *id_l;
    QLabel *title_l;
    QLabel *text_l;
    QLineEdit *id_t;
    QLineEdit *title_t;
    QTextEdit *text_t;
    QPushButton *save;
    QHBoxLayout *id_hbox;
    QHBoxLayout *title_hbox;
    QHBoxLayout *text_hbox;
    QVBoxLayout *fenetre_vbox;

public:
    NoteEditor(Note &n, QWidget *parent = 0);

public slots:
    void activerSave();
    void saveNote();

};

#endif // NOTEEDITOR_H
