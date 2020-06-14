import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

Window {
    id: dialog
    property int implicitHeight: 100
    minimumWidth: 500
    minimumHeight: implicitHeight
    visible: true
    modality: Qt.WindowModal
    flags: Qt.Dialog
    title: dialogTitle
    color: palette.window

    ColumnLayout {
        id: mainColumn
        anchors.fill: parent
        anchors.margins: 9
        spacing: 6

        Frame {
            id: storageTypePart
            padding: 0
            Layout.fillWidth: true
            background: Rectangle {
                color: "transparent"
            }

            ColumnLayout {
                id: columnLayout
                anchors.fill: parent

                Label {
                    id: storageTypeSelectorLabel
                    text: qsTr("Storage type")
                    font.weight: Font.Bold
                }

                ComboBox {
                    id: storageTypeSelector
                    Layout.fillWidth: true
                    model : storageTypesList
                    onActivated: slotStorageTypeChanged(currentText)

                    delegate: ItemDelegate {
                        width: parent.width
                        text: model.modelData
                    }
                }
            }
        }

        Frame {
            id: dialogPartContainer
            padding: 0
            Layout.fillHeight: true
            Layout.fillWidth: true
            background: Rectangle {
                color: "transparent"
            }


        }

        Frame {
            id: dialogButtonsPart
            padding: 0
            Layout.fillWidth: true
            background: Rectangle {
                color: "transparent"
            }

            Row {
                anchors.right: parent.right
                Button {
                    id: okButton
                    text: qsTr("OK")
                    enabled: false
                    onClicked: {
                        backend.accepted()
                    }
                }

                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        backend.rejected()
                    }
                }
            }
        }


    }

    Component.onCompleted: {
        storageTypeSelector.currentIndex = defaultTypeIndex
        backend.userBasedValidityChanged.connect(slotValidityChanged)
        slotStorageTypeChanged(storageTypeSelector.currentText)
    }

    function slotStorageTypeChanged(sStorageType) {
        // remove old dialog parts
        let dialogParts = dialogPartContainer.contentItem.children
        for (let i = 0; i < dialogParts.length; i++)
            dialogParts[i].parent = null

        backend.slotStorageTypeChanged(sStorageType)
        let uiPart = backend.dialogPartUIPart()

        let uiElementsHeight =  mainColumn.anchors.margins * 2 + mainColumn.spacing * 3 + dialogButtonsPart.implicitHeight + storageTypePart.implicitHeight
        if (uiPart) {
            dialog.minimumHeight = uiPart.implicitHeight + uiElementsHeight
            dialog.height = dialog.minimumHeight
            uiPart.parent = dialogPartContainer.contentItem
        } else {
            simpleLoader.item.parent = dialogPartContainer.contentItem
            dialog.minimumHeight = implicitHeight + simpleLoader.item.implicitHeight + uiElementsHeight
            dialog.height = dialog.minimumHeight
        }
    }
    
        Component {
            id: fakeLabel
            Label {
                anchors.fill:  parent
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Not available.")
            }
        }
        Loader {
            id: simpleLoader
            anchors.fill:  parent
            visible: false
            sourceComponent: fakeLabel
        }

    function slotValidityChanged(isValid) {
        okButton.enabled = isValid
    }
    SystemPalette {
        id: palette
        colorGroup: SystemPalette.Active
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:4;anchors_x:"-6";anchors_y:456}D{i:3;anchors_height:100;anchors_width:100;anchors_x:"-6";anchors_y:187}
D{i:8;anchors_x:228;anchors_y:"-109"}
}
##^##*/
