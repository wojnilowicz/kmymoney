import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    implicitHeight: mainColumn.spacing * 3 + hostNameLabel.implicitHeight + hostNameInput.implicitHeight + databaseNameLabel.implicitHeight + databaseNameInput.implicitHeight
    anchors.fill: parent

    ColumnLayout {
        id: mainColumn
        anchors.left: parent.left
        anchors.right: parent.right

        Label {
            id: hostNameLabel
            text: qsTr("Host name")
            font.weight: Font.Bold
        }

        TextField {
            id: hostNameInput
            Layout.fillWidth: true
            readOnly: false
            text: openBackend.hostName
            placeholderText : "localhost"
            onTextChanged: {
                openBackend.hostName = text
            }
            Layout.minimumHeight: contentHeight + topPadding + bottomPadding
        }

        Label {
            id: databaseNameLabel
            text: qsTr("Database name")
            font.weight: Font.Bold
        }

        TextField {
            id: databaseNameInput
            Layout.fillWidth: true
            readOnly: false
            text: openBackend.databaseName
            placeholderText : "KMyMoneyNEXT"
            onTextChanged: {
                openBackend.databaseName = text
            }
            Layout.minimumHeight: contentHeight + topPadding + bottomPadding
            ToolTip {
                id: databaseNameTooltip
                visible: databaseNameInput.hovered
                text: openBackend.databaseNameTooltip
            }
        }
    }
    Component.onCompleted: {
        openBackend.databaseNameTooltipChanged.connect(slotStorageTypeChanged)
    }

    function slotStorageTypeChanged(newTooltip) {
        if (newTooltip.length === 0)
            databaseNameTooltip.visible = false
        else
            databaseNameTooltip.visible = true
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;anchors_height:100;anchors_width:100;anchors_x:608;anchors_y:6}
}
##^##*/
