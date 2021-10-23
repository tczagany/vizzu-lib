"use strict";

//$$CMAKE-ENVIRONMENT-SWITCH NODE
module.exports = class AnimControl
//$$CMAKE-ENVIRONMENT-SWITCH ES6
//export default class AnimControl
//$$CMAKE-ENVIRONMENT-SWITCH
{
	constructor(chart) {
		this.chart = chart;
	}

	seek(value) {
		this.animControl('seek', value);
	}

	pause() {
		this.animControl('pause');
	}

	play() {
		this.animControl('play');
	}

	stop() {
		this.animControl('stop');
	}

	reverse() {
		this.animControl('reverse');
	}

	animControl(command, param = '')
	{
		let ccommand = this.chart.toCString(command);
		let cparam = this.chart.toCString(param);

		try {
			this.chart.call(this.chart.module._anim_control)(ccommand, cparam);
		} finally
		{
			this.chart.module._free(cparam);
			this.chart.module._free(ccommand);
		}
	}
}

