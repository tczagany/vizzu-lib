var fs = require('fs');
var path = require('path');

var Workspace = require('./workspace.js')
var Chrome = require('./browser/chrome.js')


class TestSuite {
    
    #testCasesPath;
    #testCases;

    #workspace;
    #browser;


    constructor(testCasesPath) {
        if(path.isAbsolute(testCasesPath)) {
            this.#testCasesPath = testCasesPath;
        } else {
            this.#testCasesPath = __dirname + '/' + testCasesPath;
        }
        this.#setTestCases();
    }
  

    getTestCasesPath() {
        return this.#testCasesPath;
    }

    getTestCases() {
        return this.#testCases;
    }


    async runTestSuite() {
        this.#startTestSuite();
        for (let i = 0; i < this.#testCases.length; i++) {
            await this.#runTestCase(i)
        }
        this.#finishTestSuite();
    }


    #setTestCases() {
        this.#testCases = fs.readdirSync(this.#testCasesPath);
        console.log('Test Cases: ' + this.#testCases);
    }

    async #runTestCase(index) {
        console.log('Running Test Case ' + index + ' : ' + this.#testCases[index]);
        await this.#browser.getUrl('http://127.0.0.1:' + String(this.#workspace.getWorkspacePort()) + '/index.html' + '?testCase=' + this.#testCases[index])
    }

    #startTestSuite() {
        this.#workspace = new Workspace(this.#testCasesPath + '/../')
        this.#workspace.openWorkspace()
        console.log('Listening at http://127.0.0.1:' + String(this.#workspace.getWorkspacePort()) + '/')
        this.#browser = new Chrome()
        this.#browser.openBrowser(false)
    }

    #finishTestSuite() {
        this.#browser.closeBrowser()
        this.#workspace.closeWorkspace()
    }
}


let test = new TestSuite('../testCases')
test.runTestSuite()
