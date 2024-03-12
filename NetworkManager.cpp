//
// Created by Parker on 2/23/2024.
// I don't have much experience in networking, so I just tried to set this up as best as I could figure out based on
// some resources online. I figure we'll tweak this later once the main game is working
//

#include "NetworkManager.h"

//Constructor for the NetworkManager class. Initializes the TCP socket and data stream
NetworkManager::NetworkManager(QObject *parent) :
        QObject(parent), socket(new QTcpSocket(this)) {

    setupSocketSignals();

    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_5_10);
}

//Destructor ensures that the network connection is closed properly
NetworkManager::~NetworkManager() {
    disconnectFromServer();
}

//Attempts to establish a connection to the server at the specified host and port
void NetworkManager::connectToServer(const QString &host, quint16 port) {
    socket->connectToHost(host, port);
}

//Disconnects the socket from the server
void NetworkManager::disconnectFromServer() {
    socket->disconnectFromHost();
}

//Sends the provided data to the server over the established connection
void NetworkManager::send(const QByteArray &data) {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(data);
    }
}

//Emits the connected signal when the socket connection is successfully established
void NetworkManager::onConnected() {
    emit connected();
}

//Emits the disconnected signal when the socket has disconnected from the server
void NetworkManager::onDisconnected() {
    emit disconnected();
}

//Reads all available data from the socket and emits the dataReceived signal
void NetworkManager::onReadyRead() {
    QByteArray data = socket->readAll();
    emit dataReceived(data);
}

//Emits the errorOccurred signal with the error message when a network error occurs
void NetworkManager::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError)
    emit errorOccurred(socket->errorString());
}

//Private method to connect the QTcpSocket's signals to the NetworkManager's slots
void NetworkManager::setupSocketSignals() {
    connect(socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &NetworkManager::onErrorOccurred);
}