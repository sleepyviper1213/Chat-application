qt_standard_project_setup(REQUIRES 6.7)

set(APP_ID "github.sleepyviper1213.chat")

# Conditional executable setup
if(WIN32)
	set(app_icon_resource_windows
		"${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/photosurface.ico")
	qt_add_executable(ChatClient main.cpp ${app_icon_resource_windows})
elseif(APPLE)
	set(application_icon
		"${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/7124142_chat_icon.icns")
	set_source_files_properties(${application_icon}
		PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

	qt_add_executable(ChatClient MACOSX_BUNDLE main.cpp ${application_icon})
	set_target_properties(
		ChatClient
		PROPERTIES MACOSX_BUNDLE True
		MACOSX_BUNDLE_GUI_IDENTIFIER ${APP_ID}
		MACOSX_BUNDLE_ICON_FILE "7124142_chat_icon.icns")
else()
	qt_add_executable(ChatClient main.cpp)
endif()

qt_add_qml_module(
	ChatClient
	URI ChatClient
	VERSION 1.0
	QML_FILES
	Main.qml
	    contact/ContactPage.qml
		conversation/ConversationPage.qml
		conversation/ChatToolBar.qml
		theme/+Material/ChatToolBar.qml
		theme/+Universal/ChatToolBar.qml
	SOURCES
	    contact/sqlcontactmodel.cpp
		contact/sqlcontactmodel.hpp
		conversation/sqlconversationmodel.cpp
		conversation/sqlconversationmodel.hpp
		messaging/messagebody.cpp
		messaging/messagebody.hpp
		messaging/actor.cpp
		messaging/actor.hpp
		messaging/chatmessage.cpp
		messaging/chatmessage.hpp
		messaging/MessageBoardAttachedType.cpp
		messaging/MessageBoardAttachedType.hpp
		messaging/MessageBoard.cpp
		messaging/MessageBoard.hpp
		utilities/connectDatabase.hpp
	RESOURCES
	    "assets/avatars/Albert_Einstein.png"
		"assets/avatars/Albert_Einstein@2x.png"
		"assets/avatars/Albert_Einstein@3x.png"
		"assets/avatars/Albert_Einstein@4x.png"
		"assets/avatars/Ernest_Hemingway.png"
		"assets/avatars/Ernest_Hemingway@2x.png"
		"assets/avatars/Ernest_Hemingway@3x.png"
		"assets/avatars/Ernest_Hemingway@4x.png"
		"assets/avatars/Hans_Gude.png"
		"assets/avatars/Hans_Gude@2x.png"
		"assets/avatars/Hans_Gude@3x.png"
		"assets/avatars/Hans_Gude@4x.png"
		"theme/qtquickcontrols2.conf"
)

target_include_directories(
	ChatClient
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/messaging
	    ${CMAKE_CURRENT_SOURCE_DIR}/contact
		${CMAKE_CURRENT_SOURCE_DIR}/conversation
		${CMAKE_CURRENT_SOURCE_DIR}/utilities
)

target_link_libraries(ChatClient PRIVATE Qt::QuickControls2 Qt::Sql)

# Installation and deployment
install(
	TARGETS ChatClient
	BUNDLE DESTINATION .
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_qml_app_script(
	TARGET ChatClient
	OUTPUT_SCRIPT deploy_script
	MACOS_BUNDLE_POST_BUILD
	NO_UNSUPPORTED_PLATFORM_ERROR
	DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
)

install(SCRIPT ${deploy_script})
