#ifndef HEADER_FENCLIENT
#define HEADER_FENCLIENT

#include <QtGui>
#include <QtNetwork>
#include <QtWidgets/QtWidgets>
#include "ui_fenetre_rt.h"


class fenetre_rt : public QWidget, private Ui::fenetre_rt
{
    Q_OBJECT

    public:
        fenetre_rt();
        ~fenetre_rt();
        int number_rt;
        QString donnees_message;
        //variable qui recoit les donn�es du message

        void send(QString &contenu_envoi);
        // fonction qui envoie le message sur le bus


    public slots:
        void on_bouton_demarrer_clicked();
        //fonction appell�e en cas de clic sur le bouton demarrer

        void on_boutonEnvoyer_clicked();
        //fonction appell�e en cas de clic sur le bouton envoyer

        void on_message_returnPressed();
        //fonction pour qu'un appui sur la touche entr�e ait le m�me
        // effet qu'un clic sur le bouton envoyer

        void donneesRecues();
        // fonction appell�e en cas de reception de messages

        void connecte();
        // fonction appel�e en cas de connection reussie au bc

        void erreurSocket(QAbstractSocket::SocketError erreur);
        // fonction appel�e en cas d'erreur de connection au bc

        void traitement_messages();
        // fonction appel�e en cas de reception q'un message qui permet de le traiter

    private:
        QTcpSocket *socket; // Repr�sente le serveur
        quint16 tailleMessage;
        QQueue<QString> message_a_envoyer;
        // contient le message entr�e par l'utilisateur a envoyer

    signals:
        void message_recieved();
        // signal emis en cas de reception d'un message
};

#endif
