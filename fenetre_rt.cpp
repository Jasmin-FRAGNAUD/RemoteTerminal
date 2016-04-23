#include "fenetre_rt.h"
#include "frame.h"


// adresse ip (127.0.0.1 ; LocalHost) et port (7800) du BC

fenetre_rt::fenetre_rt()
{
    setupUi(this);

    boutonEnvoyer->setEnabled(false);
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    connect(socket, SIGNAL(connected()), this, SLOT(connecte()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));

    connect(this, SIGNAL(message_recieved()), this, SLOT(traitement_messages()));  // on execute la fonction creation_liste quand le signal signal_creation_liste est émis

    tailleMessage = 0;
}

// destructeur de fenetre_bm
fenetre_rt::~fenetre_rt(){
}

// Tentative de connexion au serveur
void fenetre_rt::on_bouton_demarrer_clicked()
{
    // On annonce sur la fenêtre qu'on est en train de se connecter
    listeMessages->append("Tentative de connexion en cours...");
    bouton_demarrer->setEnabled(false);
    boutonEnvoyer->setEnabled(true);
    socket->abort();  // Désactivation des connexions précédentes
    // On se connecte au bc
    socket->connectToHost(QHostAddress::LocalHost,7800);

}


// preparation d'un message a envoyer au serveur
void fenetre_rt::on_boutonEnvoyer_clicked(){
    // On prépare le paquet à envoyer
    if (adresse_destinataire->text()=="0"  || adresse_destinataire->text()=="1"  || adresse_destinataire->text()=="2"  ||
        adresse_destinataire->text()=="3"  || adresse_destinataire->text()=="4"  || adresse_destinataire->text()=="5"  ||
        adresse_destinataire->text()=="6"  || adresse_destinataire->text()=="7"  || adresse_destinataire->text()=="8"  ||
        adresse_destinataire->text()=="9"  || adresse_destinataire->text()=="10" || adresse_destinataire->text()=="11" ||
        adresse_destinataire->text()=="12" || adresse_destinataire->text()=="13" || adresse_destinataire->text()=="14" ||
        adresse_destinataire->text()=="15" || adresse_destinataire->text()=="16" || adresse_destinataire->text()=="17" ||
        adresse_destinataire->text()=="18" || adresse_destinataire->text()=="19" || adresse_destinataire->text()=="20" ||
        adresse_destinataire->text()=="21" || adresse_destinataire->text()=="22" || adresse_destinataire->text()=="23" ||
        adresse_destinataire->text()=="24" || adresse_destinataire->text()=="25" || adresse_destinataire->text()=="26" ||
        adresse_destinataire->text()=="27" || adresse_destinataire->text()=="28" || adresse_destinataire->text()=="29" ||
        adresse_destinataire->text()=="30" || adresse_destinataire->text()=="bc" ||
        adresse_destinataire->text()=="BC" || adresse_destinataire->text()=="Bc" ){

        if(adresse_destinataire->text()=="bc" || adresse_destinataire->text()=="Bc"){
            adresse_destinataire->setText("BC");
        }
    QString messageAEnvoyer =adresse_destinataire->text()+tr("_")+QString::number(choix_rt->value())+"_"+message->text();

    message_a_envoyer.enqueue(messageAEnvoyer);
    boutonEnvoyer->setEnabled(false);
    listeMessages->append("");
    listeMessages->append("Message mis en attente d'envoi :  "+messageAEnvoyer);
    listeMessages->append("");
    adresse_destinataire->clear(); // On vide la zone d'écriture du destinatairesss
    message->clear(); // On vide la zone d'écriture du message
    }

}

// fonction de traitement des messages recus
void fenetre_rt::traitement_messages(){
    frame m_frame;
    listeMessages->append("");
    listeMessages->update();
    if(m_frame.get_frame_message(donnees_message)=="Status"){
        listeMessages->append("Demande de status de la part du BC.");
        QString contenu_message="Status OK";
        QString contenu_envoi=m_frame.create_frame("BC",QString::number(choix_rt->value()),contenu_message);
        listeMessages->append("Trame envoyé : "+contenu_envoi);
        listeMessages->update();
        send(contenu_envoi);
    }
    if(message_a_envoyer.isEmpty()){
        if(m_frame.get_frame_message(donnees_message)=="Message a transmettre?"){
            listeMessages->append("Le BC demande si il y a un message a envoyer.");
            listeMessages->append("Le RT n'a pas de message a envoyer.");
            QString contenu_message="Pas de message";
            QString contenu_envoi=m_frame.create_frame("BC",QString::number(choix_rt->value()),contenu_message);
            send(contenu_envoi);
            listeMessages->append("Trame envoyé : "+contenu_envoi);
            listeMessages->update();
        }
    }
    else{
        if (m_frame.get_frame_message(donnees_message)=="Transmit Data"){
            listeMessages->append("Envoi des données.");
            QString contenu_message=message_a_envoyer.dequeue();
            listeMessages->append("Trame envoyé : "+contenu_message);
            send(contenu_message);
            boutonEnvoyer->setEnabled(true);

        }
        if (m_frame.get_frame_message(donnees_message)=="Message a transmettre?"){
            listeMessages->append("Le BC demande si il y a un message a envoyer.");
            listeMessages->append("Le RT a un messsage a envoyer.");
            QString contenu_message="Message a envoyer";
            QString contenu_envoi=m_frame.create_frame("BC",QString::number(choix_rt->value()),contenu_message);
            send(contenu_envoi);
        }
    }
}


// appuyer sur la touche Entrée aura le même effet que cliquer sur le bouton "Envoyer"
void fenetre_rt::on_message_returnPressed(){
    on_boutonEnvoyer_clicked();
}


void fenetre_rt::donneesRecues()
{

    QDataStream in(socket);

    if (tailleMessage == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
             return;

        in >> tailleMessage;
    }

    if (socket->bytesAvailable() < tailleMessage)
        return;

    // on peut récupérer le message entier
    in >>donnees_message;

    // On remet la taille du message à 0 pour pouvoir recevoir de futurs messages
    tailleMessage = 0;

    frame m_frame;
    // on affiche le message si il a été recus de la part d'un autre rt et destiné a ce rt
    if(m_frame.get_frame_emetteur(donnees_message)!="BC" && m_frame.get_frame_destinataire(donnees_message)==QString::number(choix_rt->value())){
        listeMessages->append("Le RT a reçu un message :  "+donnees_message);
    }
    // on emet le signal "message_recieved" si on recoit un message destiné a ce rt et provenant du bc
    if(m_frame.get_frame_emetteur(donnees_message)=="BC" && m_frame.get_frame_destinataire(donnees_message)==QString::number(choix_rt->value())){
        emit message_recieved();
    }
}

// Ce slot est appelé lorsque la connexion au serveur a réussi
void fenetre_rt::connecte()
{
    listeMessages->append("Connexion réussie !");
    listeMessages->update();
    bouton_demarrer->hide();
    number_rt=choix_rt->value();
    listeMessages->append(tr("Lancement du Remote terminal N° %1").arg(number_rt));
    listeMessages->update();
    setWindowTitle(tr("Remote Terminal N° %1").arg(number_rt));
    choix_rt->hide();
    label->hide();
    setWindowTitle(tr("Remote Terminal N° %1").arg(number_rt));
    label_rt->setText(tr("RT N° %1").arg(number_rt));
    listeMessages->append(tr("Le Remote terminal N° %1").arg(number_rt)+" à été lancé avec succes.");
    listeMessages->update();
}

// Ce slot est appelé lorsqu'il y a une erreur
void fenetre_rt::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur) // On affiche un message différent selon l'erreur qu'on nous indique
    {
        case QAbstractSocket::HostNotFoundError:
            listeMessages->append("ERREUR : le serveur n'a pas pu être trouvé.");
            break;
        case QAbstractSocket::ConnectionRefusedError:
            listeMessages->append("ERREUR : Vérifiez si le programme \"BC\" a bien été lancé.");
            break;
        case QAbstractSocket::RemoteHostClosedError:
            listeMessages->append("ERREUR : le serveur a coupé la connexion.");
            break;
        default:
            listeMessages->append("ERREUR : "+socket->errorString());
    }
    bouton_demarrer->setEnabled(true);
}

void fenetre_rt::send(QString &contenu_envoi)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);


    out << (quint16) 0;
    out << contenu_envoi;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet); // On envoie le paquet
}
