import QtQuick.Controls 2.15
import ChatClient 1.0

ApplicationWindow {
	id: window

	height: 960
	minimumHeight: 300
	minimumWidth: 400
	visible: true
	width: 540

	StackView {
		id: stackView

		anchors.fill: parent

		initialItem: ContactPage {}
	}
}
