input = '10.000500'

var decimal = 0;

whole_number = Number(input).toFixed(0).toString(); 
 
if (Number(input.substring (0, input.indexOf('.')) != 0)) { 
	var decimal = (Number(input) % Number(input.substring (0, input.indexOf('.')))).toFixed (input.substring (input.indexOf('.'), input.length).length - 1).toString();
}



console.log ('whole_number: ' + whole_number + '\ndecimal: ' + decimal)