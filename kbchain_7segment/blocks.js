Blockly.Blocks['ht16k33_show'] = {
	init: function() {
		this.jsonInit({
			"message0": "7Segment show %1 decimal %2",
			"args0": [{
				"type": "input_value",
				"name": "value",
				"check": "Number"
			}, {
				"type": "field_number",
				"name": "decimal",
				"value": 0,
				"min": 0,
				"max": 3
			  }],
			"inputsInline": true,
			"previousStatement": null,
			"nextStatement": null,
			"colour": 315,
			"tooltip": "",
			"helpUrl": ""
		});
	}
};

Blockly.Blocks['ht16k33_show_time'] = {
	init: function() {
		this.jsonInit({
			"message0": "7Segment show time   Hour %1 Minute %2 show colon %3",
			"args0": [{
				"type": "input_value",
				"name": "hour",
				"check": "Number"
			}, {
				"type": "input_value",
				"name": "min",
				"check": "Number"
			}, {
				"type": "field_checkbox",
				"name": "colon",
				"checked": true
			}],
			"inputsInline": true,
			"previousStatement": null,
			"nextStatement": null,
			"colour": 315,
			"tooltip": "",
			"helpUrl": ""
		});
	}
};
