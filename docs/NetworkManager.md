# NetworkManager

_Primary Author: Parker Hyde, SPRING 2024_

The `NetworkManager` class is responsible for managing the network communication aspects of the game. It handles connecting to and communicating with a game server, managing both incoming and outgoing data. Built using Qt's networking modules, it provides network operations integrated with Qt's signal-slot mechanisms.

## Methods

Here are the main methods provided by the `NetworkManager` class:

```c++
// Constructor initializes the network components and sets up signal handlers
explicit NetworkManager(QObject *parent = nullptr);

// Destructor to ensure clean disconnection and resource release
virtual ~NetworkManager();

// Initiates a connection to the specified server address and port
void connectToServer(const QString &host, quint16 port);

// Disconnects from the server
void disconnectFromServer();

// Sends data to the server
void send(const QByteArray &data);
```

## Signals

These are the signals emitted by the `NetworkManager`:

```c++
// Emitted when a connection to the server is successfully established
void connected();

// Emitted when the connection to the server is lost
void disconnected();

// Emitted when data is received from the server
void dataReceived(const QByteArray &data);

// Emitted when a network error occurs
void errorOccurred(const QString &errorMessage);
```

## Slots

These slots handle network events:

```c++
// Called when the socket connects to the server
void onConnected();

// Called when the socket disconnects from the server
void onDisconnected();

// Called when there is new data to read from the server
void onReadyRead();

// Called when a network error occurs
void onErrorOccurred(QAbstractSocket::SocketError socketError);
```

## Internal Details

### Network Communication

- **QTcpSocket**: Manages the TCP network connection.
- **QDataStream**: Facilitates reading and writing data from/to the network socket.

### Method: `setupSocketSignals`

This method connects the QTcpSocket's signals to the appropriate slots in the `NetworkManager`, setting up the handlers needed for network events. This integration allows the `NetworkManager` to respond dynamically to network state changes and data transmission events.

### Connection Management

The `connectToServer` method is used to initiate a connection to the server using the provided host address and port. The `disconnectFromServer` method ensures that the connection is terminated gracefully.

### Data Handling

Data transmission is handled by the `send` method, which writes the given QByteArray of data to the server if the connection is active. Incoming data is managed in the `onReadyRead` slot, which reads all available data from the socket and emits a `dataReceived` signal.

## Other Notes

- **Error Handling**: The `NetworkManager` emits an `errorOccurred` signal with a descriptive message whenever a network error is encountered, facilitating debugging and robust error management.
- **Resource Management**: The destructor ensures that all network connections are properly closed and resources are released to prevent leaks and ensure clean application shutdowns.


## Potential Future Enhancements:

### Enabling Host and Play Functionality

The current implementation of the `NetworkManager` is designed primarily for client-side operations. It handles connections to a server, sending and receiving data, and managing network events. However, it lacks the functionality to act as a server that can manage incoming connections from different clients.

Some changes that would need to be made:

1. **Server Capabilities**:
   - Develop a new class or extend the existing `NetworkManager` to handle server-side functions such as listening for incoming connections, managing multiple client connections, and distributing game state updates to all clients.

2. **Connection Management**:
   - Implement methods to manage multiple connections efficiently, including the ability to accept new clients, handle client disconnections gracefully, and broadcast messages to all connected clients.

3. **Game State Synchronization**:
   - Design mechanisms to ensure game state is synchronized across all clients, handling scenarios where different clients may have slightly out-of-sync views of the game state due to network delays or packet loss.
   
### Changing from TCP to UDP

The current `NetworkManager` uses TCP for its network communications, which provides reliable delivery of data. However, for many real-time multiplayer games, UDP can be a better option.

### Potential Modifications

1. **Protocol Change**: Implement UDP socket creation and management within the `NetworkManager`.
2. **Custom Reliability**: Develop methods for ensuring data reliability and order, if necessary, such as implementing sequence numbers and acknowledgments.
3. **Error Handling**: Design error handling to manage packet loss and ordering issues effectively.
4. **Packet Resending**: Determine if/when and how often to resend critical data packets that have not been acknowledged.