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


/**
 * @brief NetworkManager handles the network communication for the game, managing the connection to the server
 * and handling incoming and outgoing data.
 * @author Parker Hyde
 * @date SPRING 2024
 */
class NetworkManager : public QObject {
Q_OBJECT

public:
    /**
     * @brief Constructor that sets up the network socket and connects signal handlers.
     * @param parent The QObject parent of this network manager.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    explicit NetworkManager(QObject *parent = nullptr);

    /**
     * @brief Destructor that ensures the network connection is properly closed.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    virtual ~NetworkManager();

    /**
     * @brief Initiates a connection to a server with the specified host address and port number.
     * @param host The host address of the server to connect to.
     * @param port The port number on the server to connect to.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    void connectToServer(const QString &host, quint16 port);

    /**
     * @brief Disconnects from the server if currently connected.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    void disconnectFromServer();

    /**
     * @brief Sends data to the server if a connection is established.
     * @param data The data to send to the server.
     * @author Parker Hyde
     * @date SPRING 2024
     */
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


    /**
    * @brief Configures signal-slot connections for the QTcpSocket instance.
    * This method connects the socket's signals to the appropriate slots in this class,
    * setting up the necessary event handlers for network communication.
    * @author Parker Hyde
    * @date SPRING 2024
    */
    void setupSocketSignals();
};


#endif //TANKS_NETWORKMANAGER_H
