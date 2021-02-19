import QtQuick 2.9
import TCP 1.0
import chat.gui 1.0

ChatWindow {
    width: 300
    height: 200

    type: TcpConnection.Client
    port: 4242
}
