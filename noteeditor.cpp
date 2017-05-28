#include "noteeditor.h"

#include <QMessageBox>

NoteEditor::NoteEditor(Note &n, QWidget *parent)
    : note(&n), QWidget(parent)
{
    id_l = new QLabel("ID : ");
    title_l = new QLabel("Titre : ");
    text_l = new QLabel("Texte : ");
    id_t = new QLineEdit;
    title_t = new QLineEdit;
    text_t = new QTextEdit;
    save = new QPushButton("Sauver");
    id_hbox = new QHBoxLayout();
    title_hbox = new QHBoxLayout();
    text_hbox = new QHBoxLayout();
    fenetre_vbox = new QVBoxLayout();

    id_hbox->addWidget(id_l);
    id_hbox->addWidget(id_t);
    title_hbox->addWidget(title_l);
    title_hbox->addWidget(title_t);
    text_hbox->addWidget(text_l);
    text_hbox->addWidget(text_t);
    fenetre_vbox->addLayout(id_hbox);
    fenetre_vbox->addLayout(title_hbox);
    fenetre_vbox->addLayout(text_hbox);
    fenetre_vbox->addWidget(save);

    id_t->setText(a.getId()); id_t->setReadOnly(true);
    title_t->setText(a.getTitle());
    text_t->setText(a.getText());
    QObject::connect(title_t, SIGNAL(textChanged(QString)), this, SLOT(activerSave()));
    QObject::connect(text_t, SIGNAL(textChanged()), this, SLOT(activerSave()));
    QObject::connect(save, SIGNAL(clicked()), this, SLOT(saveArticle()));
    save->setDisabled(true);
    this->setLayout(fenetre_vbox);
}

void NoteEditor::activerSave()
{
    save->setEnabled(true);
}

void NoteEditor::saveNote()
{
    article->setText(text_t->toPlainText());
    article->setTitle(title_t->text());
    QMessageBox::information(this, "Sauvegarde", "Note sauvegardée !");
    save->setDisabled(true);
}