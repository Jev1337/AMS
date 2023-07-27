import QtQuick 2.1
import QtLocation 5.9
import QtPositioning 5.9




Rectangle {
    id: window

    property double oldLat: 25.6585
    property double oldLng: 100.3658
    property Component comMarker: mapMarker
    property Component comHospitalMarker: hospitalMarker
    property Component comPatientMarker: patientMarker
    property Component comAmbulanceMarker: ambulanceMarker

    property Component comPatientOccupiedMarker: patientOccupiedMarker
    property Component comAmbulanceOccupiedMarker: ambulanceOccupiedMarker

    property var existingMarker: null

    RouteModel {
                id: rm
                plugin: mapPlugin
                query: RouteQuery {id: routeQuery }
                //Component.onCompleted: {
                //    routeQuery.addWaypoint(QtPositioning.coordinate(36.866822, 10.182740));
                //    routeQuery.addWaypoint(QtPositioning.coordinate(36.862064, 10.164594));
                //    update();
                //}

                onStatusChanged: console.debug("current route model status", status, count, errorString)


            }



    Plugin {
        id: mapPlugin
        name: "osm"
    }

    signal coordinateSelected(double lat, double lng)

    signal ambulanceToHospital(double distance)

    signal asignAmbulanceToEmergency(int ambulanceID,int emergencyID)
    signal asignEmergencyToHospital(int hospitalID,int emergencyID)

    Map {
        id: mapView
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(oldLat, oldLng);
        zoomLevel: 10

        MapItemView {
            model: rm
            delegate: Component{
                MapRoute {
                    route: routeData
                    line.color: "red"
                    line.width: 5
                    smooth: true
                }
            }
        }

        MapPolyline {
                id:mapPolyline
        }
        MouseArea {
            anchors.fill: parent
            onDoubleClicked: {
                var coordinate = mapView.toCoordinate(Qt.point(mouse.x, mouse.y))
                if(existingMarker !== null) {
                    mapView.removeMapItem(existingMarker)
                }
                var item = comMarker.createObject(window, {
                                                   coordinate: coordinate
                                                  })
                mapView.addMapItem(item)
                existingMarker = item
                var lat = coordinate.latitude
                var lng = coordinate.longitude
                coordinateSelected(lat, lng)
            }
        }
        Timer {
           id: timer
           interval: 5000 // 5 second
           repeat: true
           running: true

        }

    }

    function setRoute(lat1, lng1, lat2,lng2) {
        routeQuery.clearWaypoints();
        routeQuery.addWaypoint(QtPositioning.coordinate(lat1, lng1)); //A
        routeQuery.addWaypoint(QtPositioning.coordinate(lat2,lng2)); //B
        rm.update();
    }

    function setCenter(lat, lng) {
        mapView.pan(oldLat - lat, oldLng - lng)
        oldLat = lat
        oldLng = lng
    }

    function addMarker(lat, lng) {
        if (existingMarker !== null) {
            mapView.removeMapItem(existingMarker)
        }
        var item = comMarker.createObject(window, {
            coordinate: QtPositioning.coordinate(lat, lng)
        })
        mapView.addMapItem(item)
        existingMarker = item
        //console.log("Marker added: " + lat + ", " + lng)
        coordinateSelected(lat, lng)
    }

    function addHospitalMarker(lat, lng) {
        if (existingMarker !== null) {
            mapView.removeMapItem(existingMarker)
        }
        var item = comHospitalMarker.createObject(window, {
            coordinate: QtPositioning.coordinate(lat, lng)
        })
        mapView.addMapItem(item)
    }

    function addAmbulanceOccupiedMarker(lat, lng) {
        if (existingMarker !== null) {
            mapView.removeMapItem(existingMarker)
        }
        var item = comAmbulanceOccupiedMarker.createObject(window, {
            coordinate: QtPositioning.coordinate(lat, lng)
        })
        mapView.addMapItem(item)
    }

    function addPatientOccupiedMarker(lat, lng) {
        if (existingMarker !== null) {
            mapView.removeMapItem(existingMarker)
        }
        var item = comPatientOccupiedMarker.createObject(window, {
            coordinate: QtPositioning.coordinate(lat, lng)
        })
        mapView.addMapItem(item)
    }

    function addAmbulanceMarker(lat, lng) {
        if (existingMarker !== null) {
            mapView.removeMapItem(existingMarker)
        }
        var item = comAmbulanceMarker.createObject(window, {
            coordinate: QtPositioning.coordinate(lat, lng)
        })
        mapView.addMapItem(item)
    }


    function addPatientMarker(lat, lng,idPatient) {
        if (existingMarker !== null) {
            mapView.removeMapItem(existingMarker)
        }
        var item = comPatientMarker.createObject(window, {
            coordinate: QtPositioning.coordinate(lat, lng)
        })
        mapView.addMapItem(item)

        var ambulanceMarkers = mapView.mapItems.filter(function(item) {
            return item.name === "ambulance";
        });

        var patientMarker = item;
    }
    Component {
        id: mapMarker
        MapQuickItem {
            id: markerImg
            property string name: "marker"
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: position

            sourceItem: Image {
                id: image
                source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png"
            }

        }
    }
    Component {
        id: hospitalMarker
        MapQuickItem {
            id: markerImg
            property string name: "hospital"
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: position

            sourceItem: Image {
                id: image
                source: "https://media.discordapp.net/attachments/929827215791779890/1088940058914066502/Asset_1.png?width=40&height=63"
            }

        }
    }

    Component {
        id: patientMarker
        MapQuickItem {
            id: markerImg
            property string name: "patient"
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: position

            sourceItem: Image {
                id: image
                source: "https://media.discordapp.net/attachments/929827215791779890/1088940818259263639/Asset_3.png?width=40&height=63"
            }

        }
    }

    Component {
        id: ambulanceMarker
        MapQuickItem {
            id: markerImg
            property string name: "ambulance"
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: position

            sourceItem: Image {
                id: image
                source: "https://media.discordapp.net/attachments/929827215791779890/1088940818519306240/Asset_2.png?width=40&height=63"
            }

        }
    }

    Component {
        id: ambulanceOccupiedMarker
        MapQuickItem {
            id: markerImg
            property string name: "ambulanceOccupied"
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: position

            sourceItem: Image {
                id: image
                source: "https://media.discordapp.net/attachments/929827215791779890/1089161901130391662/Asset_1.png?width=40&height=62"
            }

        }
    }
    Component {
        id: patientOccupiedMarker
        MapQuickItem {
            id: markerImg
            property string name: "patientOccupied"
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: position

            sourceItem: Image {
                id: image
                source: "https://media.discordapp.net/attachments/929827215791779890/1089161901344292934/Asset_3.png?width=40&height=62"
            }

        }
    }

}
