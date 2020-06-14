import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Item {
    id: mainContainer
    implicitHeight: 10
    anchors.fill: parent

    ColumnLayout {
        id: mainColumn
        anchors.fill: parent

        Label {
            id: fileSelectorLabel
            text: qsTr("Location to save storage")
            font.weight: Font.Bold
        }

        RowLayout {
            id : fileSelector
            Layout.fillWidth: true
            Layout.fillHeight: false
            spacing: 5

            TextField {
                id: filePathInput
                Layout.fillHeight: true
                Layout.fillWidth: true
                readOnly: false
                text: saveAsEncryptedBackend.filePath
                onTextChanged: {
                    saveAsEncryptedBackend.filePath = text
                }
            }

            Button {
                id: fileDialogButton
                Layout.fillHeight: true
                icon.name: "document-open"
                onClicked: fileDialog.visible = true
            }
        }

        Label {
            id: keysSelectorLabel
            text: qsTr("Keys used to encrypt storage")
            font.weight: Font.Bold
        }

        Frame {
            id : keysSelectorPlaceholder
            padding: 0
            Layout.fillHeight: true
            Layout.fillWidth: true
            background: Rectangle {
                color: "transparent"
            }
        }

        FileDialog {
            id: fileDialog
            title: qsTr("Select a file")
            selectMultiple: false
            selectExisting: false
            selectFolder: false
            folder: startDir
            nameFilters: [ "KMyMoneyNEXT files (*.kmy)", "All files (*)" ]
            selectedNameFilter: "KMyMoneyNEXT files (*.kmy)"
            onAccepted: {
                filePathInput.text = fileUrl.toString().replace("file://", "")
            }
        }

        SystemPalette {
            id: palette
            colorGroup: SystemPalette.Active
        }

        Component.onCompleted: {
            let uiPart = keysSelector.uiPart()
            if (uiPart) {
                mainContainer.implicitHeight += uiPart.implicitHeight + mainColumn.spacing * 3 + fileSelectorLabel.implicitHeight + fileSelector.implicitHeight + keysSelectorLabel.implicitHeight + keysSelectorPlaceholder.implicitHeight
                uiPart.parent = keysSelectorPlaceholder.contentItem
            }
        }
    }
}

/*##^##
Designer {
    D{i:1;anchors_height:100;anchors_width:100;anchors_x:608;anchors_y:6}
}
##^##*/
