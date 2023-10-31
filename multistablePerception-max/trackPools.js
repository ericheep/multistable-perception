inlets = 1
outlets = 2

var pools = []
var radius = 0.1
var maxPools = 3

function addPool() {
	if (pools.length >= maxPools) {
		return 0
	}
	
	var coordinates = arrayfromargs(arguments);
	
	var inc = 0
	
	var taken = true
	
	while (taken) {		
		taken = false
		for (var i = 0; i < pools.length; i++) {
			if (inc == pools[i].index) {
				taken = true;
			} 
		}
		
		if (taken == true) {
			inc++
		}
	}

	var pool = {
		x: coordinates[0],
		y: coordinates[1],
		index: inc,
	}	
		
	pools.push(pool)
	
	outlet(0, pool.index, pool.x, pool.y)
}

function calculateDistance(x1, y1, x2, y2) {
	return Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2))
}

function setMaxPools(_maxPools) {
	maxPools = _maxPools
}


function checkWithinRange() {
	var coordinates = arrayfromargs(arguments)
	
	var x1 = coordinates[0]
	var y1 = coordinates[1]

	for (var i = 0; i < pools.length; i++) {
		var x2 = pools[i].x
		var y2 = pools[i].y
		
		var distance = calculateDistance(x1, y1, x2, y2)
		
		if (distance < radius) {
			outlet(1, pools[i].index)
			pools.splice(i, 1)
		}
	}
}