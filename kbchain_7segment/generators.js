Blockly.JavaScript['ht16k33_show'] = function(block) {
	var value_value = Blockly.JavaScript.valueToCode(block, 'value', Blockly.JavaScript.ORDER_ATOMIC);
	var number_decimal = block.getFieldValue('decimal');
	var code = 'DEV_I2C1.KBChain_7segment(0, 0x71).print((double)(' + value_value + '), ' + number_decimal + '); DEV_I2C1.KBChain_7segment(0, 0x71).writeDisplay();\n';
	return code;
};

Blockly.JavaScript['ht16k33_show_time'] = function(block) {
	var value_hour = Blockly.JavaScript.valueToCode(block, 'hour', Blockly.JavaScript.ORDER_ATOMIC);
	var value_min = Blockly.JavaScript.valueToCode(block, 'min', Blockly.JavaScript.ORDER_ATOMIC);
	var checkbox_colon = block.getFieldValue('colon') == 'TRUE';
	var code = 'DEV_I2C1.KBChain_7segment(0, 0x71).print((long)(' + value_hour + ' * 100 + ' + value_min + '), DEC); DEV_I2C1.KBChain_7segment(0, 0x71).drawColon(' + (checkbox_colon ? 'true' : 'false') + '); DEV_I2C1.KBChain_7segment(0, 0x71).writeDisplay();\n';
	return code;
};
