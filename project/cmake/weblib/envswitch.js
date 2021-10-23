var fs = require('fs');
var readline = require('readline');

var srcFolder = process.argv.slice(2)[0];
console.log('Js enviroment switching...');
console.log('   Source folder:', srcFolder);
fs.readdirSync(srcFolder).forEach(fileName => {
    console.log(fileName);
    let cntout = "";
    const cntin = fs.readFileSync(srcFolder + "/" + fileName, "utf8");
    const lines = cntin.split(/\r\n|\r|\n/);
    let pass = false;
    let inside = false;
    for (const line of lines) {
        const word = line.split(/(\s+)/);
        if (word[0] == "//$$CMAKE-ENVIRONMENT-SWITCH") {
            inside = word[2] != undefined;
            pass = word[2] == process.argv.slice(2)[1];
            continue;
        }
        if (pass || !inside)
            cntout += line + "\n";
    }
    try {
        fs.unlinkSync(srcFolder + "/_" + fileName);
    }
    catch(e) {
    }
    fs.writeFileSync(srcFolder + "/_" + fileName, cntout);
});
console.log('done');