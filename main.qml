import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0
import QtQuick.Window 2.0



import "./resources/javascript/utils.js" as Utils;

ApplicationWindow {
    id: item;
    title: "Verication Tools 1.0"
    height: 650
    width:  1000
    visible: true

    property bool debuggerActive: false

    Rectangle {
        id: gv_
        width: parent.width
        height: parent.height
        color: "black"

        property bool menu_shown: false

        /* this rectangle contains the "menu" */
        Rectangle {
            id: menu_view_
            anchors.fill: parent
            color: "red";

            Image {
                anchors.fill: parent;
                source: "./resources/images/blur.jpg";
            }

            opacity: gv_.menu_shown ? 1 : 0
            Behavior on opacity { NumberAnimation { duration: 300 } }

            /* this is my sample menu content (TODO: replace with your own)  */

            Rectangle {
                width: parent.width
                height: 700
                color: "transparent"
                anchors.top: parent.top;
                anchors.topMargin: 10;

                Rectangle {
                    width: 150
                    height: 40
                    color: "#2CC185"
                    radius: 3
                    opacity: 0.8
                    anchors {
                        top: parent.top
                        right: parent.right
                        rightMargin: 500
                    }
                    Text {
                        anchors.centerIn: parent
                        color: "white"
                        font.pixelSize: 18
                        text: "Iniciar pruebas"
                    }

                    scale: initTests.pressed ? 1.1 : 1;

                    MouseArea {
                        id: initTests;
                        anchors.fill: parent;
                        onClicked: Utils.Verifier.fullInputs() ? Utils.Events.sendData() : Utils.Error.emptyData();
                    }
                }

                TabView {
                    height: parent.height
                    width: parent.width
                    id: lateralTabs

                    style: TabViewStyle {
                        frame: Rectangle {
                            color: "transparent"
                        }
                        frameOverlap: 1
                        tab: Rectangle {
                            color: "transparent"
                            border.color:  styleData.selected ? "white" : "#CCC"
                            anchors.left: parent.left
                            anchors.leftMargin: 15
                            implicitWidth: Math.min(textTitle.width + 20, 120);
                            implicitHeight: 40
                            radius: 2
                            Text {
                                id: textTitle
                                anchors.centerIn: parent
                                text: styleData.title
                                color: styleData.selected ? "white" : "#CCC"
                                font.pixelSize: 17
                            }
                        }
                    }

                    Tab {
                        active: true
                        title: "Verificación"
                        ListView {
                            anchors.fill: parent;
                            model: 5
                            delegate: Rectangle {
                                height: Utils.GUI.rectangleContainerSize(index);
                                width: parent.width;
                                color: "transparent"

                                Rectangle {
                                    Rectangle {
                                        width: 850;
                                        height: 30;
                                        color: "transparent";
                                        anchors {
                                            left: parent.left;
                                            top: parent.top;
                                            topMargin: 10;
                                            leftMargin: 15;
                                        }
                                        Text {
                                            text: Utils.GUI.assignLabel(index);
                                            font.pixelSize: 18;
                                            font.family: "Verdana";
                                            color: "white";
                                        }
                                    }

                                    Rectangle {
                                        height: Utils.GUI.textAreaSize(index);
                                        width: 850;
                                        color: "transparent"

                                        anchors {
                                            left: parent.left;
                                            top: parent.top;
                                            topMargin: 35;
                                            leftMargin: 12;
                                        }

                                        TextArea {
                                            height: Utils.GUI.textAreaSize(index);
                                            anchors.fill: parent
                                            anchors.margins: 3
                                            objectName: Utils.InputCollection.getId(index, this);

                                            style: TextAreaStyle {
                                                textColor: "white"
                                                selectionColor: "#9b59b6"
                                                selectedTextColor: "white"
                                                __stickyScrollbars: false;
                                                backgroundColor: "black";
                                            }

                                            opacity: 0.3;
                                            focus: true
                                            selectByMouse: true
                                            font.pixelSize: 20;
                                            onFocusChanged: Utils.GUI.cleanInputErrorState(this,index+1);
                                            onActiveFocusChanged: Utils.GUI.cleanInputErrorState(this,index+1);
                                            onActiveFocusOnPressChanged: Utils.GUI.cleanInputErrorState(this,index+1);
                                            onActiveFocusOnTabChanged: Utils.GUI.cleanInputErrorState(this,index+1);

                                        }/*END -> TextArea*/
                                    }

                                } /*END -> COLUMN LAYOUT.*/
                            }
                        }/*END ListView*/

                    } /*END -> Tab verificationMenu*/

                    Tab {
                        active: true
                        title: "Resultados"
                        Rectangle {
                            width: parent.width
                            height: 200
                            color: "transparent"
                            Rectangle {
                                width: 850
                                anchors {
                                    top: parent.top
                                    left: parent.left
                                    topMargin: 20
                                    leftMargin: 15
                                }
                                Text {
                                    font.pixelSize: 17
                                    color: "red"
                                    text: "Aún no se ha completado la depuración."
                                }

                                Text {
                                    id: firstResult
                                    font.pixelSize: 17
                                    state: ""
                                    states: [
                                        State {
                                            name: "closed"
                                            PropertyChanges {
                                                target: firstResult
                                                text: "El árbol de refutación de la prueba 1 es cerrado."
                                            }
                                        },
                                        State {
                                            name: "open"
                                            PropertyChanges {
                                                target: firstResult
                                                text: "El árbol de refutación de la prueba 1 es abierto."
                                            }
                                        },
                                        State {
                                            name: ""
                                            PropertyChanges {
                                                target: firstResult
                                                text: ""
                                            }
                                        }
                                    ]

                                    anchors {
                                        top: parent.top
                                        topMargin: 20
                                    }
                                }

                                Text {
                                    id: secondResult
                                    font.pixelSize: 17
                                    state: ""
                                    states: [
                                        State {
                                            name: "closed"
                                            PropertyChanges {
                                                target: secondResult
                                                text: "El árbol de refutación de la prueba 2 es cerrado."
                                            }
                                        },
                                        State {
                                            name: "open"
                                            PropertyChanges {
                                                target: secondResult
                                                text: "El árbol de refutación de la prueba 2 es abierto."
                                            }
                                        },
                                        State {
                                            name: ""
                                            PropertyChanges {
                                                target: secondResult
                                                text: ""
                                            }
                                        }
                                    ]

                                    anchors {
                                        top: parent.top
                                        topMargin: 40
                                    }
                                }

                                Text {
                                    id: thirdResult
                                    font.pixelSize: 17
                                    state: ""
                                    states: [
                                        State {
                                            name: "closed"
                                            PropertyChanges {
                                                target: thirdResult
                                                text: "El árbol de refutación de la prueba 3 es cerrado."
                                            }
                                        },
                                        State {
                                            name: "open"
                                            PropertyChanges {
                                                target: thirdResult
                                                text: "El árbol de refutación de la prueba 3 es abierto."
                                            }
                                        },
                                        State {
                                            name: ""
                                            PropertyChanges {
                                                target: thirdResult
                                                text: ""
                                            }
                                        }
                                    ]
                                    anchors {
                                        top: parent.top
                                        topMargin: 60
                                    }
                                }

                                Text {
                                    id: fourthResult
                                    font.pixelSize: 17
                                    state: ""
                                    states: [
                                        State {
                                            name: "closed"
                                            PropertyChanges {
                                                target: fourthResult
                                                text: "El árbol de refutación de la prueba 4 es cerrado."
                                            }
                                        },
                                        State {
                                            name: "open"
                                            PropertyChanges {
                                                target: fourthResult
                                                text: "El árbol de refutación de la prueba 4 es abierto."
                                            }
                                        },
                                        State {
                                            name: ""
                                            PropertyChanges {
                                                target: fourthResult
                                                text: ""
                                            }
                                        }
                                    ]
                                    anchors {
                                        top: parent.top
                                        topMargin: 80
                                    }
                                }

                                Text {
                                    id: fifthResult
                                    font.pixelSize: 17
                                    state: ""
                                    states: [
                                        State {
                                            name: "closed"
                                            PropertyChanges {
                                                target: fifthResult
                                                text: "El árbol de refutación de la prueba 5 es cerrado."
                                            }
                                        },
                                        State {
                                            name: "open"
                                            PropertyChanges {
                                                target: fifthResult
                                                text: "El árbol de refutación de la prueba 5 es abierto."
                                            }
                                        },
                                        State {
                                            name: ""
                                            PropertyChanges {
                                                target: fifthResult
                                                text: ""
                                            }
                                        }
                                    ]
                                    anchors {
                                        top: parent.top
                                        topMargin: 100
                                    }
                                }

                                Text {
                                    id: conclusion
                                    font.pixelSize: 17
                                    state: ""
                                    states: [
                                        State {
                                            name: "verified"
                                            PropertyChanges {
                                                target: conclusion
                                                color: "green"
                                                text: "Como los árboles de refutación de todas las pruebas son cerrados, el programa se verifica."
                                            }
                                        },
                                        State {
                                            name: "notVerified"
                                            PropertyChanges {
                                                target: conclusion
                                                color: "red"
                                                text: "Como al menos hay una prueba en donde el árbol de refutación es abierto, no se verifica el programa."
                                            }
                                        },
                                        State {
                                            name: ""
                                            PropertyChanges {
                                                target: conclusion
                                                text: ""
                                            }
                                        }
                                    ]
                                    anchors {
                                        top: parent.top
                                        topMargin: 120
                                    }
                                }

                                Component.onCompleted: Utils.DebuggerSetup.setResultsSection(this);
                            }
                        }
                    }

                    Tab {
                        active: true
                        title: "Tutorial"
                    }
                }

            }
        }

        /* this rectangle contains the "normal" view in your app */
        Rectangle {
            id: normal_view_
            anchors.fill: parent

            /* this is what moves the normal view aside */
            transform: Translate {
                id: game_translate_
                x: 0
                Behavior on x { NumberAnimation { duration: 400; easing.type: Easing.OutQuad } }
            }

            /* Menu shadow */
            BorderImage {
                id: menu_shadow_
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: -10
                z: -1 /* this will place it below normal_view_ */
                visible: gv_.menu_shown
                source: "./resources/images/shadow.png"
                border { left: 12; top: 12; right: 12; bottom: 12 }
            }

            Rectangle {
                id: menu_bar_
                anchors.top: parent.top
                width: parent.width;
                height: 70;
                color: "#DC554F"

                Rectangle {
                    id: menu_button_
                    anchors {
                        left: parent.left;
                        verticalCenter: parent.verticalCenter;
                        margins: 24
                    }
                    color: "transparent";
                    width: 30;
                    height: 30;
                    radius:3;
                    scale: ma_.pressed ? 1.2 : 1

                    Image {
                        anchors.fill: parent;
                        source: "./resources/images/menu.png";
                    }

                    MouseArea { id: ma_; anchors.fill: parent; onClicked: gv_.onMenu();  }
                }
            }

            Rectangle {
                height: 70
                width: parent.width
                color: "#EB7E7F"
                anchors {
                    top: parent.top
                    topMargin: 70
                }
                Rectangle {
                    height: parent.height
                    width: parent.width
                    color: "transparent"
                    Rectangle {
                        height: parent.height
                        width: 150
                        color: "transparent"
                        Text {
                            text: "Verificación"
                            color: "white"
                            font.pixelSize: 20
                            anchors.centerIn: parent
                        }
                    }
                    Rectangle {
                        id: testSelector
                        width: 700
                        height: parent.height
                        color: "transparent"
                        anchors {
                            left: parent.left
                            leftMargin: 300
                        }

                        Row {
                            id: testSelectorsGroup
                            ExclusiveGroup { id: testSelection }
                        }
                    }
                }
            }

            Rectangle {
                height: 2
                width: parent.width
                color: "#906094"
                anchors {
                    top: parent.top
                    topMargin: 140
                }
            }

            TabView {
                height: 508;
                anchors.bottom: parent.bottom;
                width: parent.width;
                id: mainContent;

                style: TabViewStyle {
                    tab:
                        Rectangle {
                            color: styleData.selected ? "white" :"#906094"
                            implicitWidth: mainContent.width/2
                            implicitHeight: 50

                            Text {
                                id: text
                                anchors.centerIn: parent
                                text: styleData.title
                                color: styleData.selected ? "#906094" : "white"
                                font.pixelSize: 19;
                                font.family: "Verdana";
                            }
                        }

                    frame: Rectangle {
                        color: "transparent"
                    }
                }

                Tab {
                    id: graphTab
                    title: "Gráfico"
                    active: true

                    MouseArea {
                        anchors.fill: parent
                        onClicked: debuggerActive = false
                    }

                    Rectangle {
                        width: parent.width
                        height: 500
                        anchors.bottom: parent.bottom
                        id: canvasSize
                        ScrollView {
                            width: parent.width
                            height: parent.height
                            Canvas {
                                height: 5000
                                width: canvasSize.width
                                Component.onCompleted: {
                                    Utils.TreeSetup.setValues(this, this.width, getContext("2d"));
                                }
                            }
                        }
                    }
                }

                Tab {
                    id: debugTab
                    title: "Debugger"
                    active: true

                    MouseArea {
                        anchors.fill: parent
                        onClicked: debuggerActive = true
                    }

                    Rectangle {
                        height: 500
                        width: parent.width

                        Rectangle {
                            width: parent.width
                            anchors.top: parent.top
                            height: 70
                            color: "lightblue"

                            Row {
                                width: parent.width
                                height: parent.height
                                Rectangle {
                                    width: parent.width/2
                                    height: 70
                                    anchors.left: parent.left

                                    Rectangle {
                                        width: 100
                                        height: 45
                                        color: "#333"
                                        radius: 3
                                        anchors {
                                            left: parent.left
                                            leftMargin: 10
                                            verticalCenter: parent.verticalCenter
                                        }

                                        Text {
                                            font.pixelSize: 18
                                            color: "white"
                                            anchors.centerIn: parent
                                            text: "Skip"
                                        }

                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: Utils.Debugger.skipBranch();
                                        }
                                    }

                                    Rectangle {
                                        width: 150
                                        height: 45
                                        color: "red"
                                        radius: 3
                                        anchors {
                                            left: parent.left
                                            leftMargin: 120
                                            verticalCenter: parent.verticalCenter
                                        }

                                        Text {
                                            font.pixelSize: 18
                                            color: "white"
                                            anchors.centerIn: parent
                                            text: "Cerrar rama"
                                        }

                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: Utils.Debugger.closeBranch()
                                        }
                                    }
                                }

                                Rectangle {
                                    width: parent.width/2
                                    height: 70
                                    anchors.right: parent.right
                                    Rectangle {
                                        width: parent.width
                                        height: parent.height
                                        anchors.left: parent.left

                                        Text {
                                            id: formulaHeader
                                            color: "#333"
                                            font.pixelSize: 18
                                            anchors.verticalCenter: parent.verticalCenter
                                            Component.onCompleted:  {
                                                Utils.DebuggerSetup.setFormulaHeader(this);
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        Rectangle {
                            height: 3
                            width: parent.width
                            color: "#906094"
                            anchors {
                                top: parent.top
                                topMargin: 70
                            }
                        }

                        ScrollView {
                            width: parent.width
                            height: 300
                            anchors.bottom : parent.bottom
                            Rectangle {
                                width: 5000
                                height: parent.height
                                /*Acá se insertan los nodos.*/

                                Component.onCompleted:  {
                                    Utils.DebuggerSetup.setScene(this);
                                }
                            }
                        }
                    }
                }
            }

            /* put this last to "steal" touch on the normal window when menu is shown */
            MouseArea {
                anchors.fill: parent
                enabled: gv_.menu_shown
                onClicked: gv_.onMenu();
            }
        }

        /* this functions toggles the menu and starts the animation */
        function onMenu() {
            game_translate_.x = gv_.menu_shown ? 0 : gv_.width * 0.9
            gv_.menu_shown = !gv_.menu_shown;
        }
    }

    Connections {
        property int        index       : 0
        property int        debugIndex  : 0
        property variant    debugNodes  : []
        property int        debugMargin : 0

        target: eventsManager;

        onVerificationReady: {
            Utils.GUI.hidePreviousSelectors();
            var i = 1;
            for ( ; i <= numTests; i++ ) {
                var newCheckbox = Qt.createQmlObject(
                            "import QtQuick 2.4
                             import QtQuick.Controls.Styles 1.3;
                             import QtQuick.Controls 1.3;
                             Rectangle {
                             height: testSelector.height; width: 140; anchors { left: parent.left; leftMargin: "+(i-1)+"*140; }
                                color: 'transparent'
                                CheckBox {
                                exclusiveGroup: testSelection

                                style: CheckBoxStyle {
                                        indicator: Rectangle {
                                            visible: false
                                        }

                                        background: Rectangle {
                                            implicitWidth: 140
                                            implicitHeight: testSelector.height
                                            color: 'transparent'
                                        }

                                        label: Text {
                                            color: control.checked ? 'white' : '#D0201A'
                                            font.pixelSize: 18
                                            text: 'prueba "+i+" '
                                            anchors.centerIn: parent
                                        }
                                    }
                                }
                                MouseArea {
                                    anchors.fill: parent;
                                    onClicked: eventsManager.requestTest("+i+");
                                }
                            }", testSelector
                );

                Utils.GUI.saveSelector(newCheckbox);
            }

            Utils.DebuggerSetup.initDebug();
            Utils.DebuggerSetup.setTests(numTests);
            Utils.Debugger.newVerification();
        }

        onTestReady: {
            index = 0;
            debugMargin = 0;
            Utils.Tree.clearData();
            Utils.Debugger.newTest(numTest);
        }

        onTreeNode: {
            Utils.Tree.repaint();
            if ( index == 0 )
                Utils.Tree.drawRoot(formula, isLiteral);
            else
                Utils.Tree.drawNode(formula, y, x, isLiteral, parentX, parentY);
            index++;
        }

        onDebugNode: {
            var _replaceChars = function (formula) {
                return formula
                        .split("<=").join("≤")
                        .split(">=").join("≥")
                        .split("->").join("⇒")
                        .split("<->").join("⇔")
                        .split("&&").join(" ⋀ ")
                        .split("||").join(" ⋁ ")
                        .split("!").join("¬")
                        .split("@").join("∀")
                        .split("#").join("∃")
                        .split("{").join("")
                        .split("}").join("")
            };

            var scene = Utils.DebuggerSetup.getScene()
              , margin = debugMargin * 90 + 10
              , background = (debugIndex % 2 == 0) ? "#0EA46E" : "#2CC185"
              , newElement = Qt.createQmlObject("import QtQuick 2.4
                                import QtQuick.Controls.Styles 1.3;
                                import QtQuick.Controls 1.3;

                                Rectangle {
                                    width: 90
                                    height: 40
                                    color: '"+background+"'
                                    id: debug_node_"+debugMargin+"

                                    anchors.left: parent.left
                                    anchors.leftMargin: "+ margin +"

                                    Text {
                                        text: '"+_replaceChars(croppedFormula)+"'
                                        anchors.centerIn: parent
                                        color: 'white'
                                        font.pixelSize: 17
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            eventsManager.setClickedDebugNode("+debugIndex+",'"+formula+"');
                                        }
                                    }
                                    Connections {
                                        target: eventsManager;
                                    }
                                }", scene);
            debugNodes.push(newElement);
            debugMargin++;
            debugIndex++;
        }

        onNotify: {
            Utils.Debugger.onNodeClicked(id, receivedFormula);
        }

        onDebugBranchReady: {
            for ( var i = 0; i < debugNodes.length; i++ )
                debugNodes[i].visible = false;
            debugMargin = 0;
            //debugIndex = 0;
        }

        onDebugTestEnd: {
            console.log("TERMINO.");
            for ( var i = 0; i < debugNodes.length; i++ )
                debugNodes[i].visible = false;
            debugMargin = 0;
            //debugIndex = 0;
            Utils.Debugger.endTest();
        }

        onResults: {
            Utils.Debugger.printTestResults(first, second, third, fourth, fifth);
        }

        onClickedEndedTest: {
            Utils.Debugger.onClickedEndedTest(requestedTest);
            Utils.Tree.onClickedEndedTest();
        }

        onPreconditionError: Utils.Error.errorState(0);
        onPostconditionError: Utils.Error.errorState(1);
        onInvariantError: Utils.Error.errorState(2);
        onBoundError: Utils.Error.errorState(3);
        onAlgorithmError: Utils.Error.errorState(4);
    }
}
