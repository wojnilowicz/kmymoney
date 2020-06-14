import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Item {
    anchors.fill: parent
    implicitHeight: mainColumn.spacing * 2 + addKeyButton.implicitHeight + 50

    ColumnLayout {
        id: mainColumn
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
                model: keysModel
                delegate: Item {
                    id: keyEntry
                    width: parent.width
                    height: keyEntryText.font.pixelSize * 1.5

                    Text { id: keyEntryText ; text: keyId ; color: palette.text  }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: keysList.currentIndex = index
                    }
                }
                highlight: Rectangle { color: palette.highlight }

            }
        }

        RowLayout {
            id : keysListButtons
            Layout.fillWidth: true

            Button {
                id: addKeyButton
                text: qsTr("Add")
                Layout.fillWidth: true
                onClicked: {
                    unusedKeysDialog.active = true
                    unusedKeysDialog.item.keyIDs = keysModel.notSelectedKeys()
                }
                Loader {
                    id : unusedKeysDialog
                    active : false
                    source: "unusedkeys.qml"
                }

                Connections {
                    target: unusedKeysDialog.item
                    onClosing: {
                        keysModel.appendKeyIDs(unusedKeysDialog.item.selectedKeyIDs)
                        unusedKeysDialog.active = false
                        keyselectorbackend.keysListChanged(keysModel.selectedKeys(false))
                    }
                }
            }

            Button {
                id: removeKeyButton
                text: qsTr("Remove")
                Layout.fillWidth: true
                onClicked: {
                    keysModel.removeKeyIDs([keysList.currentIndex])
                    keyselectorbackend.keysListChanged(keysModel.selectedKeys(false))
                }
            }
        }

        SystemPalette {
            id: palette
            colorGroup: SystemPalette.Active
        }
    }
}

/*##^##
Designer {
    D{i:1;anchors_height:100;anchors_width:100;anchors_x:608;anchors_y:6}
}
##^##*/
