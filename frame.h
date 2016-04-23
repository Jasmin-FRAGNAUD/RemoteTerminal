#ifndef FRAME_H
#define FRAME_H
#include <QString>

#include <QtGui>
#include <QtNetwork>

class frame
{
public:
    frame();
    QString create_frame(QString e_destinataire,QString e_emetteur,QString e_message);  // contruit une trame
    QString get_frame_destinataire(QString r_frame);    //extrait le destinataite de la trame donnée
    QString get_frame_emetteur(QString r_frame);          //extrait l'emetteur de la trame donnée
    QString get_frame_message(QString r_frame);          //extrait le message de la trame donnée

};

#endif // FRAME_H
