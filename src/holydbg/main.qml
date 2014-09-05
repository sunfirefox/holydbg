import QtQuick 2.1
import QtQuick.Window 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.0

ApplicationWindow {
  id: root
  title: "HolyDbg"
  width: 640
  height: 480
  visibility: Window.AutomaticVisibility
  
  Action {
    id: execAction
    text: "&Exec"
    shortcut: "Ctrl+E"
  }
  
  Action {
    id: attachAction
    text: "&Attach"
    shortcut: "Ctrl+A"
  }
  
  Action {
    id: exitAction
    text: "Exit"
    shortcut: "Ctrl+Q"
    onTriggered: Qt.quit()
  }
  
  Action {
    id: runAction
    text: "Run"
    shortcut: "F9"
  }
  
  Action {
    id: runThreadAction
    text: "Run thread"
    shortcut: "Ctrl+F9"
  }
  
  Action {
    id: stepIntoAction
    text: "Step into"
    shortcut: "F7"
  }
  
  Action {
    id: stepOverAction
    text: "Step over"
    shortcut: "F8"
  }
  
  Action {
    id: stepOutAction
    text: "Step out"
    shortcut: "Ctrl+F8"
  }
  
  Action {
    id: toggleSwBpAction
    text: "Toggle breakpoint"
    shortcut: "F2"
  }
  
  Action {
    id: toggleHwBpAction
    text: "Toggle hardware breakpoint"
  }
  
  Action {
    id: restartAction
    text: "Restart debuggee"
    shortcut: "Ctrl+F2"
  }
  
  Action {
    id: detachAction
    text: "Detach debuggee"
  }
  
  Action {
    id: killAction
    text: "Kill debuggee"
    shortcut: "Alt+F2"
  }
  
  Action {
    id: aboutAction
    text: "About"
  }
  
  menuBar: MenuBar {
    Menu {
      title: "File"
      MenuItem { action: execAction   }
      MenuItem { action: attachAction }
      MenuItem { action: exitAction   }
    }
    
    Menu {
      title: "Debug"
      MenuItem { action: runAction        }
      MenuItem { action: runThreadAction  }
      MenuSeparator {                     }
      MenuItem { action: stepIntoAction   }
      MenuItem { action: stepOverAction   }
      MenuItem { action: stepOutAction    }
      MenuSeparator {                     }
      MenuItem { action: toggleSwBpAction }
      MenuItem { action: toggleHwBpAction }
      MenuSeparator {                     }
      MenuItem { action: detachAction     }
      MenuItem { action: restartAction    }
      MenuItem { action: killAction       }
    }
    
    Menu {
      title: "Help"
      MenuItem { action: aboutAction   }
    }
  }
  
  toolBar: ToolBar {
    RowLayout {
      Label { text: "Process:" }
      ComboBox {
        id: dbgProcBox
        model: []
        enabled: model.count > 0
      }
      
      Label { text: "Thread:" }
      ComboBox {
        id: dbgThrBox
        model: []
        enabled: model.count > 0
      }
      
      Label { text: "State:" }
      Label {
        id: dbgStateLabel
        text: "Running..."
      }
    }
  }
  
  statusBar: StatusBar {
    RowLayout {
      anchors.fill: parent
      Label { text: ">" }
      TextField {
        id: txtCmd
        Layout.fillWidth: true
        placeholderText: "Type command here"
        style: TextFieldStyle {}
        focus: true
      }
      Label { text: "v0.3.0-alpha" }
    }
  }
}
