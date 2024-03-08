//
// Created by Parker on 2/23/2024.
// I don't have much experience in networking, so I just tried to set this up as best as I could figure out based on
// some resources online. I figure we'll tweak this later once the main game is working
//

#ifndef TANKS_NETWORKMANAGER_H
#define TANKS_NETWORKMANAGER_H


#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

//NetworkManager handles the network communication for the game, managing the connection to the server
//and handling incoming and outgoing data
class NetworkManager : public QObject {
Q_OBJECT

public:
    //Constructor that sets up the network socket and connects signal handlers
    explicit NetworkManager(QObject *parent = nullptr);

    //Destructor that ensures the network connection is properly closed
    virtual ~NetworkManager();

    //Initiates a connection to a server with the specified host address and port number
    void connectToServer(const QString &host, quint16 port);

    //Disconnects from the server if currently connected
    void disconnectFromServer();

    //Sends data to the server if a connection is established
    void send(const QByteArray &data);

signals:
    //Signal emitted when a connection to the server is successfully established
    void connected();

    //Signal emitted when the connection to the server is lost
    void disconnected();

    //Signal emitted when data is received from the server
    void dataReceived(const QByteArray &data);

    //Signal emitted when a network error occurs
    void errorOccurred(const QString &errorMessage);

private slots:
    //Slot invoked when the socket successfully connects to the server
    void onConnected();

    //Slot invoked when the socket is disconnected from the server
    void onDisconnected();

    //Slot invoked when there is data ready to be read from the server
    void onReadyRead();

    //Slot invoked when an error occurs with the network connection
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    //The TCP socket for network communication
    QTcpSocket *socket;
    //The data stream for reading data from the socket
    QDataStream in;

    //Sets up the connections for the QTcpSocket signals to the appropriate slots in this class
    void setupSocketSignals();
};


#endif //TANKS_NETWORKMANAGER_H
