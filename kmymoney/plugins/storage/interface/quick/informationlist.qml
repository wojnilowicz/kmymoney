import QtQuick 2.7
import QtQuick.Dialogs 1.2
import MessageBoxAnswers 0.1

MessageDialog {
    property bool firedOnce: false
    title: issueTitle
    icon: StandardIcon.Question
    text: issueMessage
    detailedText: issueList
    standardButtons: StandardButton.OK
    Component.onCompleted: visible = true
    onAccepted: {if (!firedOnce) { firedOnce = true; messageBackend.slotIssueAnswered(Answer.Yes)}}
    onRejected: {if (!firedOnce) { firedOnce = true; messageBackend.slotIssueAnswered(Answer.Yes)}}
} 
