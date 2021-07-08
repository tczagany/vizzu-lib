let AggregateError = require('aggregate-error');

let fs = require('fs');
let path = require('path');
let webdriver = require('selenium-webdriver');

let Workspace = require('./workspace.js');
let Chrome = require('./browser/chrome.js');


class TestSuite {
    
    #testCasesPath;
    #testCases;

    #testResults = { 'passed': [], 'failed': [] };

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
        try {
            this.#startTestSuite();
            for (let i = 0; i < this.#testCases.length; i++) {
                await this.#runTestCase(i);
            }
        } catch (error) {
            throw error;
        } finally {
            this.#finishTestSuite();
        }
        
    }


    #setTestCases() {
        this.#testCases = fs.readdirSync(this.#testCasesPath);
        console.log('Test Cases: ' + this.#testCases);
    }

    async #runTestCase(index) {
        await this.#browser.getUrl('http://127.0.0.1:' + String(this.#workspace.getWorkspacePort()) + '/index.html' + '?testCase=' + this.#testCases[index]);
        const now = Date.now();
        const timeout = 60000;
        while (true) {
            if (Date.now() > now + timeout) {
                console.error(this.#testCases[index] + ' : ' + 'FAILED - TIMEOUT');
                this.#testResults.failed.push(this.#testCases[index]);
                break;
            }
            let testResult= await this.#browser.getDriver().executeScript('if (window.hasOwnProperty("result")) { return result } else { return \'PENDING\' }');
            if (testResult == 'PASSED' || testResult == 'FAILED') {
                if (testResult == 'PASSED') {
                    console.log(this.#testCases[index] + ' : ' + testResult);
                    this.#testResults.passed.push(this.#testCases[index]);
                } else {
                    console.error(this.#testCases[index] + ' : ' + testResult);
                    this.#testResults.failed.push(this.#testCases[index]);
                }
                break;
            }
        }
    }

    #startTestSuite() {
        this.#workspace = new Workspace(this.#testCasesPath + '/../');
        this.#workspace.openWorkspace();
        console.log('Listening at http://127.0.0.1:' + String(this.#workspace.getWorkspacePort()) + '/');
        this.#browser = new Chrome();
        this.#browser.openBrowser(false);
    }

    #logTestSuiteResults() {
        const sum = this.#testResults.passed.length + this.#testResults.failed.length
        if (this.#testResults.passed.length != sum) {
            console.error('PASSED : ' + sum + '/' + this.#testResults.passed.length + 
                    ', FAILED : ' + sum + '/' + this.#testResults.failed.length)
            process.exitCode = 1;
        } else {
            console.log('PASSED : ' + sum + '/' + this.#testResults.passed.length + 
                    ', FAILED : ' + sum + '/' + this.#testResults.failed.length)
        }
    }

    #finishTestSuite() {
        let errors = [];
        try {
            this.#logTestSuiteResults();
        } catch (error) {
            errors.push(error);
        }
        try {
            this.#browser.closeBrowser();
        } catch (error) {
            errors.push(error);
        }
        try {
            this.#workspace.closeWorkspace();
        } catch (error) {
            errors.push(error);
        }
        if (errors.length > 0) {
            throw new AggregateError(errors);
        }
    }
}


try {
    let test = new TestSuite('../testCases');
    test.runTestSuite();
} catch (error) {
    console.error(error);
    process.exitCode = 1;
}  
