import QtQuick 2.9
import TCP 1.0

ChatWindow {
    width: 300
    height: 200

    type: TcpConnection.Server

    port: 4242

}
