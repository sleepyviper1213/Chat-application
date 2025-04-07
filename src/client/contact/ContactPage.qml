import QtQuick 2.15
import QtQuick.Controls 2.15
import ChatClient 1.0

Page {
	id: root

	header: ToolBar {
		Label {
			anchors.centerIn: parent
			font.pixelSize: 20
			text: qsTr("Contacts")
		}
	}

	ListView {
		id: listView

		anchors.fill: parent
		bottomMargin: 48
		leftMargin: 48
		rightMargin: 48
		spacing: 20
		topMargin: 48

		delegate: ItemDelegate {
			id: contactDelegate

			height: avatar.implicitHeight
			leftPadding: avatar.implicitWidth + 32
			text: model.name
			width: listView.width - listView.leftMargin - listView.rightMargin

			onClicked: root.StackView.view.push(
						   "../conversation/ConversationPage.qml", {
							   "inConversationWith": model.actor // Access the 'actor' role
						   })

			Image {
				id: avatar

				source: Qt.resolvedUrl(
							`../assets/avatars/${contactDelegate.text.replace(
								" ", "_")}.png`)
			}
		}
		model: SqlContactModel {}
	}
}
