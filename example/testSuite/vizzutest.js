let fs = require('fs');
let path = require('path');
let webdriver = require('selenium-webdriver');

let Workspace = require('./workspace.js')
let Chrome = require('./browser/chrome.js')


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
        //console.log('Running Test Case ' + index + ' : ' + this.#testCases[index]);
        await this.#browser.getUrl('http://127.0.0.1:' + String(this.#workspace.getWorkspacePort()) + '/index.html' + '?testCase=' + this.#testCases[index])
        let testResultId = 'vizzuTestResult'
        const element = await this.#browser.getDriver().findElement(webdriver.By.id(testResultId));
        let testResult;
        // todo: timeout break
        while (true) {
            testResult = await this.#browser.getDriver().executeScript('return ' + testResultId, element);
            if (testResult == 'PASSED' || testResult == 'FAILED') {
                break;
            }
        }
        if (testResult == 'PASSED') {
            console.log(this.#testCases[index] + ' : ' + testResult)
        } else {
            console.error(this.#testCases[index] + ' : ' + testResult)
        }
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
