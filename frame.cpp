#include "frame.h"

frame::frame()
{
}

// construit une trame a partir des 3 elements qui la constitue
QString frame::create_frame(QString e_destinataire,QString e_emetteur,QString e_message){
    QString frame=e_destinataire+"_"+e_emetteur+"_"+e_message;
    return frame;
}

// on extrait de la trame reçue le destinataire
QString frame::get_frame_destinataire(QString r_frame){
    QStringList r_destinataire=r_frame.split("_");
    return r_destinataire[0];
}

// on extrait de la trame reçue l'emmeteur
QString frame::get_frame_emetteur(QString r_frame){
    QStringList r_emetteur=r_frame.split("_");
    return r_emetteur[1];
}

// on extrait de la trame reçue le message
QString frame::get_frame_message(QString r_frame){
    QStringList r_message=r_frame.split("_");
    return r_message[2];
}


