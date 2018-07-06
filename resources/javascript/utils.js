/**
 * GUI class.
 */

var GUI = { };

GUI.selectors = [];

GUI.setInputErrorState = function(_input) {
    _input.parent.color = "#e74c3c";
}

GUI.cleanInputErrorState = function (_input, index) {
    _input.parent.color = "transparent";
}

GUI.saveSelector = function (selector) {
    this.selectors.push(selector);
}

GUI.hidePreviousSelectors = function () {
    for ( var i = 0; i < this.selectors.length; i++ )
        this.selectors[i].visible = false;
    this.selectors = [];
}

GUI.assignLabel = function ( _index ) {
    if ( _index == 0 )
        return "PRECONDICIÓN";
    if ( _index == 1 )
        return "POSTCONDICIÓN";
    if ( _index == 2 )
        return "INVARIANTE";
    if ( _index == 3 )
        return "COTA";

    return "ALGORITMO";
}

GUI.textAreaSize = function ( _index ) {
    if ( _index == 4 )
        return 250;
    return 40;
}

GUI.rectangleContainerSize = function ( _index ) {
    if ( _index == 4 )
        return 300;
    return 70;
}


/**
 * InputCollection class.
 */
var InputCollection = { };

InputCollection.inputs = [ ];

InputCollection._algorithm = null;
InputCollection._preCondition = null;
InputCollection._postCondition = null;
InputCollection._invariant = null;
InputCollection._bound = null;

InputCollection._associateInput = function (_index, _input) {
    if ( _index == 0 )
        this._preCondition = _input;
    if ( _index == 1 )
        this._postCondition = _input;
    if ( _index == 2 )
        this._invariant = _input;
    if ( _index == 3 )
        this._bound = _input;
    if ( _index == 4 )
        this._algorithm = _input;
}

InputCollection.getId = function (_index, _QMLComponent) {
    this.inputs.push(_QMLComponent);
    this._associateInput(_index, _QMLComponent);
    return "input-" + _index;
}


/**
 * Error class.
 */
var Error = { };

Error.emptyData = function() {
    InputCollection.inputs.forEach(function(_input) {
        if ( Verifier.inputIsNull(_input) ) {
            GUI.setInputErrorState(_input);
        }
    });
}

Error.errorState = function (index) {
    GUI.setInputErrorState(InputCollection.inputs[index]);
}

/**
 * Verifier class.
 */

String.prototype.isEmpty = function() {
    return (this.length === 0 || !this.trim());
};

var Verifier = { }

Verifier.inputIsNull = function ( input ) {
    var text = input.getText(0,input.length);
    return (text.isEmpty());
};

Verifier.fullInputs = function() {
    var _self = this
      , value = true
      ;

    InputCollection.inputs.some( function ( input ) {
        if ( value )
            value = !(_self.inputIsNull(input));
    });

    return value;
};

var Events = { };

Events.sendData = function() {
    console.log("Se envian todos los datos.");
    eventsManager.setData(
                InputCollection._algorithm.getText(
                    0, InputCollection._algorithm.length
                ),
                InputCollection._preCondition.getText(
                    0, InputCollection._preCondition.length
                ),
                InputCollection._postCondition.getText(
                    0, InputCollection._postCondition.length
                ),
                InputCollection._invariant.getText(
                    0, InputCollection._invariant.length
                ),
                InputCollection._bound.getText(
                    0, InputCollection._bound.length
                )
    );
};


var Tree = { };

var TreeSetup = { };

TreeSetup.canvas = null;
TreeSetup.context = null;
TreeSetup.canvasWidth = null;

TreeSetup.setValues = function (canvas, canvasWidth, context) {
    this.canvas = canvas;
    this.context = context;
    this.canvasWidth = canvasWidth;

    Tree = new TreeDrawer();
}

var TreeDrawer = (function(){
    var _depth   = 13
      , _breadth = 16
      , _rootX   = 500
      , _rootY   = 60
      , _context = TreeSetup.context
      , _canvas  = TreeSetup.canvas
      , previousX = 0
      , previousY = 0
      , possibleX = 0
      , possibleY = 0
      , positionY = 0
      ;

    _context.textAlign = "center";

    var _drawText = function(partialText, x, y, strong) {
        if ( strong )
            _context.font = "20px Verdana";
        else
            _context.font = "14px Verdana";
        _context.fillStyle = "black";
        _context.fillText(partialText, x, y);
    };

    var _drawError = function (error, x, y) {
        _context.fillStyle = "red";
        _context.fillText(error,x,y);
    };

    var _clear = function() {
        previousX = 0;
        previousY = 0;
        possibleX = 0;
        possibleY = 0;
        positionY = 0;
        _context.clearRect(0, 0, _canvas.width, _canvas.height);
    }

    var _drawSheet = function(sourceX, sourceY, destinyX, destinyY) {
        _context.beginPath();
        _context.moveTo(sourceX, sourceY);
        _context.lineTo(destinyX, destinyY);
        _context.stroke();
        _context.closePath();
    };

    var _replaceChars = function (formula) {
        return formula
                .split("==").join("=")
                .split("<=").join("≤")
                .split(">=").join("≥")
                .split("->").join("⇒")
                .split("<->").join("⇔")
                .split("&&").join(" ⋀ ")
                .split("||").join(" ⋁ ")
                .split("!=").join("≠")
                .split("!").join("¬")
                .split("@").join("∀")
                .split("#").join("∃")
                .split("{").join("")
                .split("}").join("")

    };

    return {
        drawRoot: function (value, isLiteral) {
            _drawText(_replaceChars(value), _rootX, _rootY);
        },
        drawNode: function(value, row, position, isLiteral, parentX, parentY) {
            var stepX = 56;
            var stepY = 45;

            var x = _rootX + (stepX * position);
            var y = _rootY + (stepY * (row-1));

            _drawText((isLiteral) ? _replaceChars(value) : "♦", x, y+10, !isLiteral);

            console.log("Grafico desde (" + x + "," + y + ") hasta (" + parentX + "," + parentY + ")" );

            var xf = _rootX + (stepX * parentX);
            var yf = _rootY +10 + (stepY * (parentY-1));

            _drawSheet(x,y,xf,yf);

        },
        onClickedEndedTest: function() {
            _clear();
            _drawText("Ya se ha realizado este test antes.", _rootX, _rootY);
        },
        clearData: function() {
            _clear();
        },
        repaint : function () {
            _canvas.requestPaint();
        }
    }
});

var DebuggerSetup = { };
var Debugger = { };

DebuggerSetup.scene = null;
DebuggerSetup.formulaHeader = null;
DebuggerSetup.resultsSection = null;
DebuggerSetup.tests = 0;

DebuggerSetup.setScene = function (context) {
    this.scene = context;
}

DebuggerSetup.setFormulaHeader = function (rectangle) {
    this.formulaHeader = rectangle;
}

DebuggerSetup.setResultsSection = function (section) {
    this.resultsSection = section;
}

DebuggerSetup.setTests = function (num) {
    this.tests = num;
}

DebuggerSetup.initDebug = function() {
    Debugger = new Debugger_();
}

DebuggerSetup.getScene = function() {
    return this.scene;
}

DebuggerSetup.getFormulaHeader = function (rectangle) {
    return this.formulaHeader;
}

DebuggerSetup.getResultsSection = function() {
    return this.resultsSection;
}

DebuggerSetup.getTests = function() {
    return this.tests;
}

var Debugger_ = (function(){
    var formulaHeader   = DebuggerSetup.getFormulaHeader()
      , branch          = DebuggerSetup.getScene()
      , resultsContainer = DebuggerSetup.getResultsSection()
      ;

    var firstClickedNode      = null
      , secondClickedNode     = null
      , selectedLength        = 0
      , firstSelectedFormula  = null
      , secondSelectedFormula = null
      , currentTest           = 0
      , endedTests            = []
      ;

    var _replaceChars = function (formula) {
        return formula
                .split("==").join("=")
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
                .split("!=").join("≠")
        }
      , showFormula = function (formula) {
            formulaHeader.color = "#333";
            formulaHeader.text = _replaceChars(formula);
        }
      , notifyError = function (error) {
            formulaHeader.color = "red";
            formulaHeader.text = error;
        }
      , notifySuccess = function (message) {
            formulaHeader.color = "green";
            formulaHeader.text = message;
        }
      , cleanMessage = function() {
            formulaHeader.text = "";
        }
      , isSelected = function (node) {
            return ( firstClickedNode == node || secondClickedNode == node );
        }
      , clearData = function (node, id) {
            if ( firstClickedNode == node ) {
                firstClickedNode = null;
                firstSelectedFormula = null;
            }
            else {
                secondClickedNode = null;
                secondSelectedFormula = null;
            }
            node.color = (id % 2 == 0) ? "#0EA46E" : "#2CC185";
        }
      , setSelectedColor = function (node) {
            node.color = "#09F";
        }
      , setSelection = function (node, formula) {
          if ( selectedLength == 0 ) {
              firstClickedNode = node;
              firstSelectedFormula = formula;
              setSelectedColor(firstClickedNode);
          }
          else {
              secondClickedNode = node;
              secondSelectedFormula = formula;
              setSelectedColor(secondClickedNode);
          }
        }
      , unselect = function (node,id) {
            clearData(node,id);
            selectedLength--;
        }
      , select = function (node, formula) {
            setSelection(node, formula);
            selectedLength++;
        }
      , cleanAllData = function (cleanMsg) {
            firstClickedNode = null;
            secondClickedNode = null;
            firstSelectedFormula = null;
            secondSelectedFormula = null;
            selectedLength = 0;
            if (cleanMsg)
                cleanMessage();
        }
      , getState = function(isClosed) {
            return (isClosed) ? "closed" : "open"
        }
      , printResult = function(numTest, isClosed) {
            resultsContainer.children[numTest].state = getState(isClosed);
        }
      , hideResults = function(onlyTitle) {
            if ( onlyTitle )
                resultsContainer.children[0].visible = false;
            else {
                resultsContainer.children[0].visible = true;
                for ( var i = 1; i < resultsContainer.children.length; i++ ) {
                    resultsContainer.children[i].state = "";
                }
            }
        }
      , printConclusion = function(isVerified) {
            if ( isVerified )
                resultsContainer.children[6].state = "verified";
            else
                resultsContainer.children[6].state = "notVerified";
        }
      ;

    return {
        onNodeClicked: function(id, formula) {
            var node = branch.children[id]
              , isSelectedNode = false
              ;

            showFormula(formula);
            isSelectedNode = isSelected(node);

            if ( isSelectedNode )
                unselect(node,id);
            else
                if ( selectedLength == 2 )
                    notifyError("No se pueden seleccionar más nodos.");
                else
                    select(node, formula);
        },
        closeBranch: function() {
            if ( selectedLength == 0 )
                notifyError("No se ha seleccionado ningún nodo.");
            else
                eventsManager.closeBranch(firstSelectedFormula, secondSelectedFormula, (selectedLength == 2));

            cleanAllData(false);
        },
        skipBranch: function() {
            eventsManager.skipBranch();
            cleanAllData(false);
        },
        endTest: function() {
            eventsManager.endedTest(currentTest);
            endedTests.push(currentTest);
            if ( endedTests.length < DebuggerSetup.getTests() )
                notifySuccess("El test ha finalizado, ya podes pasar al siguiente test." );
            else {
                notifySuccess("Las pruebas han finalizado.");
                eventsManager.requestResults();
            }
            cleanAllData(false);
        },
        newTest: function (numTest) {
            currentTest = numTest;
            cleanAllData(true);
        },
        onClickedEndedTest: function (numTest) {
            notifyError("El test " + numTest + " ya se ha realizado antes.");
        },
        printTestResults: function (first, second, third, fourth, fifth) {
            hideResults(true);
            printResult(1, first);
            printResult(2, second);
            printResult(3, third);
            printResult(4, fourth);
            printResult(5, fifth);
            printConclusion(first && second && third && fourth && fifth);
        },
        newVerification: function() {
            cleanAllData(true);
            hideResults(false);
        }
    }
});
