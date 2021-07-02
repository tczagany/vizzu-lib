import Vizzu from './lib/vizzu.js';


async function digestMessage(message) {
	const msgUint8 = new TextEncoder().encode(message);
  	const hashBuffer = await crypto.subtle.digest('SHA-256', msgUint8);
 	const hashArray = Array.from(new Uint8Array(hashBuffer));
 	const hashHex = hashArray.map(b => b.toString(16).padStart(2, '0')).join('');
 	return hashHex;
}


function onLoaded()
{
	let data = {
			series: [
				{
					name: 'Colors',
					type: 'categories',
					values: ['red', 'green', 'blue']
				},
				{
					name: 'Val',
					type: 'values',
					values: [ 3, 5, 4 ]
				}
			]
		};

	chart.animate(
	{
		data: data,
		descriptor : {
			channels: {
				x: { attach: [ 'Colors'] },
			},
			title: null,
			legend: null,
		}
	}).then(chart =>
		chart.animate({
			descriptor : {
				channels: {
					x: { detach: [ 'Colors'] },
					y: { attach: [ 'Colors' ]}
				},
			}
		})
	).then(chart =>
		chart.animate({
			descriptor : {
				channels: {
					color: { attach: [ 'Colors' ]}
				}
			}
		})
	).then(chart =>
		chart.animate({
			descriptor : {
				channels: {
					color: { detach: [ 'Colors' ]},
					lightness: { attach: [ 'Colors' ]}
				}
			}
		})
	).then(chart =>
		chart.animate({
			descriptor : {
				channels: {
					lightness: { detach: [ 'Colors' ]},
					label: { attach: [ 'Colors' ]}
				}
			}
		})
	).then(async () => {
		var cavasElement = document.getElementById("canvas");

		//var canvasElementContext = cavasElement.getContext("2d");
		//var ImageData = canvasElementContext.getImageData(60, 60, 200, 100);
		//canvasElementContext.putImageData(ImageData, 150, 10);
		//console.log(ImageData);

		var dataURL = cavasElement.toDataURL();
		const digestBuffer = await digestMessage(dataURL);
		var ref = '83013e50a4a1b650440fe7cdde5a0d107e0948ffe99d57c735e035328b488e37'
		if (ref == digestBuffer) {
			console.log('equal');
		} else {
			console.log('diff');
		}
		
		}).catch((err) =>
	{
		console.log(err);
	});
}

const queryString = window.location.search;
const urlParams = new URLSearchParams(queryString);
const testCase = urlParams.get('testCase')
console.log(testCase);
let chart = new Vizzu('canvas', onLoaded);
