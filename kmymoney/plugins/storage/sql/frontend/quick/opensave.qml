import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Item {
    implicitHeight: fileSelectorLabel.implicitHeight + fileSelector.implicitHeight
    anchors.fill: parent

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right

        Label {
            id: fileSelectorLabel
            text: fileDialogLabel
            font.weight: Font.Bold
        }

        RowLayout {
            id: fileSelector
            Layout.fillWidth: true
            Layout.fillHeight: false
            spacing: 5

            TextField {
                id: filePathInput
                Layout.fillHeight: true
                Layout.fillWidth: true
                readOnly: false
                text: openBackend.filePath
                onTextChanged: {
                    openBackend.filePath = text
                }
                Layout.minimumHeight: contentHeight + topPadding + bottomPadding
            }

            Button {
                id: fileDialogButton
                Layout.fillHeight: true
                icon.name: "document-open"
                onClicked: fileDialog.visible = true
            }

        }

        FileDialog {
            id: fileDialog
            title: qsTr("Select a file")
            selectMultiple: false
            selectFolder: false
            selectExisting: selectExistingFile
            folder: startDir
            nameFilters: [ "KMyMoneyNEXT files (*.db)", "All files (*)" ]
            selectedNameFilter: "KMyMoneyNEXT files (*.db)"
            onAccepted: {
                filePathInput.text = fileUrl.toString().replace("file://", "")
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;anchors_height:100;anchors_width:100;anchors_x:608;anchors_y:6}
}
##^##*/
