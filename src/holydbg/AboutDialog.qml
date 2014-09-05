import QtQuick 2.2
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ApplicationWindow {
  id: aboutDialog
  flags: Qt.Dialog
  modality: Qt.WindowModal
  title: "About HolyDbg"
  width: 300
  height: 200
  
  ColumnLayout {
    anchors.fill: parent
    
    Rectangle {
      anchors.horizontalCenter: parent.horizontalCenter
      width: 250
      height: 80
      color: "white"
    }
    
    Button {
      anchors.horizontalCenter: parent.horizontalCenter
      text: "Ok"
      onClicked: close()
    }
  }
}
