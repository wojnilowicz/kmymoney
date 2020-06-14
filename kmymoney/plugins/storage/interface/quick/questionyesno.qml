import QtQuick 2.7
import QtQuick.Dialogs 1.2
import MessageBoxAnswers 0.1

MessageDialog {
    property bool firedOnce: false
    title: issueTitle
    icon: StandardIcon.Question
    text: issueMessage
    standardButtons: StandardButton.Yes | StandardButton.No
    Component.onCompleted: visible = true
    onYes: {if (!firedOnce) { firedOnce = true; messageBackend.slotIssueAnswered(Answer.Yes)}}
    onNo: {if (!firedOnce) { firedOnce = true; messageBackend.slotIssueAnswered(Answer.No)}}
    onRejected: {if (!firedOnce) { firedOnce = true; messageBackend.slotIssueAnswered(Answer.Cancel)}}
} 
