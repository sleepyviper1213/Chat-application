import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15
import ChatClient

Page {
	id: root

	required property Actor inConversationWith

	header: ToolBar {
		ToolButton {
			anchors.left: parent.left
			anchors.leftMargin: 10
			anchors.verticalCenter: parent.verticalCenter
			text: qsTr("Back")

			onClicked: root.StackView.view.pop()
		}
		Label {
			id: pageTitle

			anchors.centerIn: parent
			font.pixelSize: 20
			text: root.inConversationWith.firstName
		}
	}

	ColumnLayout {
		anchors.fill: parent

		ListView {
			id: listView

			Layout.fillHeight: true
			Layout.fillWidth: true
			Layout.margins: pane.leftPadding + messageField.leftPadding
			displayMarginBeginning: 40
			displayMarginEnd: 40
			spacing: 12
			verticalLayoutDirection: ListView.BottomToTop

			ScrollBar.vertical: ScrollBar {}
			delegate: Column {
				id: conversationDelegate

				required property ChatMessage message

				anchors.right: message.isSentByMe ? listView.contentItem.right : undefined
				spacing: 6

				Row {
					id: messageRow

					anchors.right: conversationDelegate.message.isSentByMe ? parent.right : undefined
					spacing: 6

					Image {
						id: avatar

						source: !conversationDelegate.message.isSentByMe ? `../assets/avatars/${conversationDelegate.message.author.firstName.replace(" ", "_")}.png` : ""
					}
					Rectangle {
						color: conversationDelegate.message.isSentByMe ? "lightgrey" : "steelblue"
						height: messageText.implicitHeight + 24
						width: Math.min(
								   messageText.implicitWidth + 24,
								   listView.width - (!conversationDelegate.isSentByMe ? avatar.width + messageRow.spacing : 0))

						Label {
							id: messageText

							anchors.fill: parent
							anchors.margins: 12
							color: conversationDelegate.message.isSentByMe ? "black" : "white"
							text: conversationDelegate.message.body.text
							wrapMode: Label.Wrap
						}
					}
				}
				Label {
					id: timestampText

					anchors.right: conversationDelegate.message.isSentByMe ? parent.right : undefined
					color: "lightgrey"
					text: Qt.formatDateTime(
							  conversationDelegate.message.sentAt,
							  "d MMM hh:mm")
				}
			}
			model: SqlConversationModel {
				recipient: root.inConversationWith
			}

			Text {
				anchors.centerIn: parent
				color: "lightgrey"
				font.pointSize: 18
				text: "No messages yet"
				visible: listView.count === 0
			}
		}
		Pane {
			id: pane

			Layout.fillHeight: false
			Layout.fillWidth: true

			RowLayout {
				width: parent.width

				TextArea {
					id: messageField

					Layout.fillWidth: true
					placeholderText: qsTr("Compose message")
					wrapMode: TextArea.Wrap
				}
				Button {
					id: sendButton

					Layout.fillWidth: false
					enabled: messageField.length > 0
					text: qsTr("Send")

					onClicked: {
						listView.model.sendMessageBody(messageField.text)
						messageField.text = ""
					}
				}
			}
		}
	}
}
