//import Vizzu from 'https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.js';
import Vizzu from '/example/lib/vizzu.js'


function catchError(err) {
    console.log(err)
    //window.results = { result: 'ERROR' };
}

function digestMessage(message) {
    let msgUint8 = new TextEncoder().encode(message);
    return crypto.subtle.digest('SHA-256', msgUint8).then(hashBuffer => {
        let hashArray = Array.from(new Uint8Array(hashBuffer));
        let hashHex = hashArray.map(b => b.toString(16).padStart(2, '0')).join('');
        return hashHex;
    });
}


let queryString = window.location.search;
let urlParams = new URLSearchParams(queryString);
let testCase = urlParams.get('testCase');
let results = { result: 'PASSED', seeks: [], images: [], hashes: [] };

import('/test/integration/testCases/' + testCase).then((module) => {
    let chart = new Vizzu('vizzuCanvas');
    return chart.initializing.then((chart) => {
        let promise = Promise.resolve(chart);
        let promises = [];
        for (let i = 0; i < module.default.testSteps.length; i++) {
            promise = promise.then((chart) => {
                let prom = module.default.testSteps[i].task(chart)
                let anim = chart.animation;
                anim.pause();
                let seeks = Object.keys(module.default.testSteps[i]['ref'])
                seeks.sort(function(a, b) {
                    return parseInt(a.replace('%', '')) - parseInt(b.replace('%', ''));
                });
                results.seeks[i] = [];
                results.images[i] = [];
                results.hashes[i] = {ref: [], now: []};
                seeks.forEach(key => {
                    let seek = key.replace('%', '') + '%'
                    results.seeks[i].push(seek);
                    let hash = module.default.testSteps[i]['ref'][key]
                    anim.seek(seek);
                    chart.render.updateFrame(true);
                    let cavasElement = document.getElementById('vizzuCanvas');
                    let dataURL = cavasElement.toDataURL();
                    results.images[i].push(dataURL);
                    let digest = digestMessage(dataURL);
                    digest = digest.then(digestBuffer => {
                        results.hashes[i].ref.push(hash);
                        results.hashes[i].now.push(digestBuffer);
                        if (hash == digestBuffer) {
                            console.log(testCase + ' : ' + i + ' : ' + seek + ' : ' + 'PASSED');
                        } else {
                            results.result = 'FAILED';
                            console.error(testCase + ' : ' + i + ' : ' + seek + ' : ' + 'FAILED' + ' : ' + digestBuffer);
                        }
                    });
                    promises.push(digest);
                });
                anim.play();
                return prom 
            });
        }
        return promise.then(() => {
            return Promise.all(promises).then(() => {
                if (typeof window.results === 'undefined') {
                    window.results = results;
                }
            });
        });
    });
}).catch((err) => { catchError(err) });
