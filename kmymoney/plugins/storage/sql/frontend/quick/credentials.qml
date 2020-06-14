import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

Window {
    id: dialog
    
    property int implicitWidth: 400
    height: 10
    width: 10
    minimumHeight: 0
    maximumHeight: 0

    visible: true
    modality: Qt.ApplicationModal
    flags: Qt.Dialog
    title: qsTr("Credentials")
    color: palette.window

    property bool userAccepted: false

    ColumnLayout {
        id: mainColumn
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left

        Label {
            id: requestLabel
            text: requestLabelText
            Layout.fillWidth: false // keep it disabled to get scaling properly
        }

        Label {
            id: usernameLabel
            text: qsTr("User name")
            font.weight: Font.Bold
            visible: isUserNameVisible
        }


        TextField {
            id: usernameInput
            Layout.fillWidth: true
            font.pixelSize: 12
            visible: isUserNameVisible
        }


        Label {
            id: passphraseLabel
            text: qsTr("Passphrase")
            font.weight: Font.Bold
        }

        TextField {
            id: passphraseInput
            Layout.fillWidth: true
            font.pixelSize: 12
            echoMode : TextInput.Password
        }

        RowLayout {
            id: dialogButtons

            Button {
                id: okButton
                text: qsTr("Ok")
                Layout.fillWidth: true
                onClicked: {
                    userAccepted = true
                    dialog.close()
                }
            }

            Button {
                id: cancelButton
                text: qsTr("Cancel")
                Layout.fillWidth: true
                onClicked: {
                    userAccepted = false
                    dialog.close()
                }
            }
        }
    }
    SystemPalette {
        id: palette
        colorGroup: SystemPalette.Active
    }
    onClosing : {
        if (userAccepted)
            backend.credentials(usernameInput.text, passphraseInput.text)
        else
            backend.credentials("", "")
    }
    Component.onCompleted : {
        let calculatedHeight = mainColumn.anchors.margins * 2 + mainColumn.spacing * 3 + requestLabel.implicitHeight +  passphraseLabel.implicitHeight + passphraseInput.implicitHeight + okButton.implicitHeight
        if (isUserNameVisible)
          calculatedHeight += usernameLabel.implicitHeight + usernameInput.implicitHeight + mainColumn.spacing * 2
      
        dialog.height = dialog.contentItem.children[0].height
        dialog.minimumHeight = calculatedHeight
        dialog.maximumHeight = calculatedHeight
        if (dialog.implicitWidth < requestLabel.width)
            dialog.width = requestLabel.width
        else
            dialog.width = dialog.implicitWidth
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;anchors_height:100;anchors_width:100;anchors_x:608;anchors_y:6}
}
##^##*/
