import Vizzu from './lib/vizzu.js';
import data from './data.js';

let slider = document.getElementById("myRange");

const chart = new Vizzu(document.getElementById("vizzuCanvas"));

slider.oninput = (e)=>
{
	let t = e.target.value;
	let anim = chart.animation;
	anim.pause();
	anim.seek(t/10 + '%');
};

chart.initializing.then(chart =>
	chart.animate({
		data,
		descriptor: {
			channels: {
				x: { attach: ['cat0', 'cat2', 'cat3', 'cat4', 'cat5','cat6'] },
				y: { attach: ['val1', 'cat1']},
			}/*,
			filter: [
				{ 'cat1' : 'C'},
				{ 'cat1' : 'F'},
				'push'
			]*/
		}
	})
).then( chart =>
	chart.animate({ descriptor: {
		orientation: 'horizontal'
	}
	})
).catch(console.log);
