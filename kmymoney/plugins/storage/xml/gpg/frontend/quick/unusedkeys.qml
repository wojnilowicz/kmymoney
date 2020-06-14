import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

Window {
    id: dialog
    width: 400
    height: 130
    visible: true
    title: "Select keys to add"
    modality: Qt.ApplicationModal
    flags: Qt.Dialog
    color: palette.window

    property var keyIDs: []
    property var selectedKeyIDs: []

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            id : keysListBackground
            Layout.fillHeight: true
            Layout.fillWidth: true
            color : palette.base

            ListView {
                id: keysList
                anchors.fill: parent
                focus: true
                model: keyIDs
                delegate: Item {
                    width: parent.width
                    height: keyEntry.font.pixelSize * 1.5
                    Text { id: keyEntry ; text: modelData ; color: palette.text  }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: keysList.currentIndex = index
                    }
                }
                highlight: Rectangle { color: palette.highlight }
            }
        }

        RowLayout {
            id: keysListButtons
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true

            Button {
                id: okButton
                text: qsTr("Ok")
                onClicked: {
                    selectedKeyIDs.push(keyIDs[keysList.currentIndex])
                    dialog.close()
                }
            }

            Button {
                id: cancelButton
                text: qsTr("Cancel")
                onClicked: dialog.close()
            }
        }
    }

    SystemPalette {
        id: palette
        colorGroup: SystemPalette.Active
    }

}



/*##^##
Designer {
    D{i:3;anchors_height:160;anchors_width:110;anchors_x:0;anchors_y:0}D{i:1;anchors_height:100;anchors_width:100}
}
##^##*/
