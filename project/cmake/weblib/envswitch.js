var fs = require('fs');
var readline = require('readline');

var srcFolder = process.argv.slice(2)[0];
console.log('Js enviroment switching...');
console.log('   Source folder:', srcFolder);
if (!fs.existsSync(srcFolder + "/tmp")){
    fs.mkdirSync(srcFolder + "/tmp");
}
fs.readdirSync(srcFolder).forEach(fileName => {
    if (fs.lstatSync(srcFolder + "/" + fileName).isFile()) {
        let cntout = "";
        const cntin = fs.readFileSync(srcFolder + "/" + fileName, "utf8");
        let lines = cntin.split(/\r\n|\r|\n/);
        let pass = false;
        let inside = false;
        for (let line of lines) {
            const word = line.split(/(\s+)/);
            if (word[0] == "//$$CMAKE-ENVIRONMENT-SWITCH") {
                inside = word[2] != undefined;
                pass = word[2] == process.argv.slice(2)[1];
                continue;
            }
            if (pass && inside)
                line = line.replace("//", "");
            if (pass || !inside)
                cntout += line + "\n";
        }
        if (fs.existsSync(srcFolder + "/tmp/" + fileName))
            fs.unlinkSync(srcFolder + "/tmp/" + fileName);
        fs.writeFileSync(srcFolder + "/tmp/" + fileName, cntout);
    }
});
console.log('done');