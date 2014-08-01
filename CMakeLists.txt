PROJECT(PANDEMIC)
CMAKE_MINIMUM_REQUIRED (VERSION 1.4)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES(${CMAKE_CURENT_BINARY_DIR})

OPTION(CMAKE_VERBOSE_MAKEFILE "if all commands will be echoed to the console during the make" ON)
MARK_AS_ADVANCED(CLEAR CMAKE_VERBOSE_MAKEFILE DEBUG)

OPTION(DEBUG "Add debug symbols to executable" OFF)

## GUI ##
OPTION(ENABLE_GUI "enable GUI build" ON)

IF(ENABLE_GUI)
	FIND_PACKAGE(Qt4 REQUIRED)
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGUI ")
	SET(GUI_SOURCES 
	    gui/mainwindow.cpp)
	SET(GUI_HEADERS 
	    gui/mainwindow.h)
	QT4_WRAP_CPP(HEADERS_MOC ${GUI_HEADERS})
	SET(FORMS 
		gui/mainwindow.ui)
	QT4_WRAP_UI(FORMS_HEADERS ${FORMS})
	INCLUDE(${QT_USE_FILE})
	ADD_DEFINITIONS(${QT_DEFINITIONS})
	SET(GUI_EXEC ${HEADERS_MOC} ${FORMS_HEADERS})
	SET(GUI_LIBRARIES ${GUI_LIBRARIES} ${QT_LIBRARIES})
ENDIF()

IF(DEBUG)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
ENDIF()

SET(PANDEMIC_SOURCES 


main/main.cpp 

main/enums/object_enums.cpp

main/helpers/rng.cpp

main/mapping/map.cpp
main/mapping/map_attributes.cpp
main/mapping/mRegion.cpp
main/mapping/map_server.cpp

main/networking/networking.cpp

main/objects/character.cpp
main/objects/obstacle.cpp
main/objects/Building.cpp
main/objects/Human.cpp
main/objects/Zombie.cpp
main/objects/objects.cpp
main/objects/attributes.cpp

main/settings/character_stats.cpp
main/settings/map_settings.cpp

main/supervisor/supervisor.cpp


)

ADD_EXECUTABLE(pandemic ${PANDEMIC_SOURCES} ${GUI_SOURCES} ${GUI_EXEC})
TARGET_LINK_LIBRARIES(pandemic ${GUI_LIBRARIES})
SET_TARGET_PROPERTIES(pandemic PROPERTIES LINKER_LANGUAGE CXX)

INSTALL_TARGETS(./ pandemic)


