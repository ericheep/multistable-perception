inlets = 1
outlets = 2

var axis1 = 1
var axis2 = 1
var axis3 = 1

var positionX = 0
var positionY = 0
var positionZ = 0

var freq = 440

function reset() {
	positionX = 0
	positionY = 0
	positionZ = 0
	freq = 440	
}

function setFreq(f) {
	freq = f
}

function setAxis() {
	var l = arrayfromargs(messagename, arguments)
	l.splice(0, 1)

	axis1 = l[0]
	axis2 = l[1]
	axis3 = l[2]
}

function bang() {
	var axisChoice = Math.floor(Math.random() * 3)
	var directionChoice = Math.floor(Math.random() * 2)
		
	if (axisChoice == 0) {
		if (directionChoice == 0) {
			freq = freq * axis1
			positionX = positionX + 1
		} else {
			freq = freq / axis1
			positionX = positionX - 1
		}	
	} else if (axisChoice == 1) {
		if (directionChoice == 0) {
			freq = freq * axis2
			positionY = positionY + 1
		} else {
			freq = freq / axis2
			positionY = positionY - 1
		}
	} else {
		if (directionChoice == 0) {
			freq = freq * axis3
			positionZ = positionZ + 1
		} else {
			freq = freq / axis3
			positionZ = positionZ - 1
		}
	}
		
	outlet(0, freq)
	outlet(1, [positionX, positionY, positionZ])
}

