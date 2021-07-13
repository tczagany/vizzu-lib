const AggregateError = require('aggregate-error');

const fs = require('fs');
const path = require('path');
const yargs = require('yargs');

const Workspace = require('./modules/host/workspace.js');
const Chrome = require('./modules/browser/chrome.js');



class TestSuite {

    #workspacePath = __dirname + '/../..';
    #workspace;
    
    #testCasesPath;
    #testCases;

    #testResults = { 'passed': [], 'failed': [] };

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


    async runTestSuite(filters) {
        try {
            let testCases = this.#filterTestCases(filters)
            console.log('Selected Test Cases: ' + testCases);
            if (testCases.length > 0) {
                this.#startTestSuite();
                for (let i = 0; i < testCases.length; i++) {
                    await this.#runTestCase(testCases[i]);
                }
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

    #filterTestCases(filters) {
        let ans = [];
        if (filters.length == 0) {
            ans = this.#testCases;
        } else {
            filters.forEach(filter => {
                let testCase = path.parse(filter).base;
                if (this.#testCases.includes(testCase)) {
                    if (path.relative(filter, this.#testCasesPath + '/' + testCase) == '') {
                        if (!ans.includes(testCase)) {
                            ans.push(testCase);
                        }
                    }
                }
            });
        }
        return ans;
    }

    async #runTestCase(testCase) {
        await this.#browser.getUrl('http://127.0.0.1:' + String(this.#workspace.getWorkspacePort()) + '/test/integration/modules/client/index.html' + '?testCase=' + testCase);

        const now = Date.now();
        const timeout = 60000;
        while (true) {
            if (Date.now() > now + timeout) {
                console.error(testCase + ' : ' + 'FAILED - TIMEOUT');
                this.#testResults.failed.push(testCase);
                break;
            }
            let testResult= await this.#browser.executeScript('if (window.hasOwnProperty("results")) { return results } else { return \'undefined\' }');
            if (testResult != 'undefined') {
                if (testResult.result == 'PASSED') {
                    console.log(testCase + ' : ' + testResult.result);
                    this.#testResults.passed.push(testCase);
                } else {
                    console.error(testCase + ' : ' + testResult.result);
                    this.#testResults.failed.push(testCase);
                }
                if (testResult.result == 'FAILED') { //if (testResult.result != 'ERROR') {
                    let testResultPath = __dirname + '/testReport/' + testCase;
                    fs.mkdirSync(testResultPath, { recursive: true });
                    for (let i = 0; i < testResult.seeks.length; i++) {
                        for (let j = 0; j < testResult.seeks[i].length; j++) {
                            fs.writeFile(testResultPath + '/' + i + '_' + testResult.seeks[i][j] + ".png", testResult.images[i][j].substring(22), 'base64', error => {
                                if (error) {
                                    throw error;
                                }
                            });
                        }
                    }
                }
                break;
            }
            await new Promise(resolve => setTimeout(resolve, 50));
        }
    }

    #startTestSuite() {
        this.#workspace = new Workspace(__dirname + '/../../');
        this.#workspace.openWorkspace();
        console.log('Listening at http://127.0.0.1:' + String(this.#workspace.getWorkspacePort()));
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
            if(typeof this.#browser !== 'undefined') {
                this.#browser.closeBrowser();
            }
        } catch (error) {
            errors.push(error);
        }
        try {
            if(typeof this.#workspace !== 'undefined') {
                this.#workspace.closeWorkspace();
            }
        } catch (error) {
            errors.push(error);
        }
        if (errors.length > 1) {
            throw new AggregateError(errors);
        } else if (errors.length == 1) {
            throw errors[0];
        }
    }
}


try {
    const argv = yargs
        .usage('Usage: $0 [testCases] [options]')
        .example('$0 testCases/test1.js testCases/test2.js', 'Run test1.js and test2.js')
        .example('$0 testCases/test?.js', 'Run test1.js and test2.js')
        .example('$0 testCases/*', 'Run test1.js and test2.js')
        .help('h')
        .alias('h', 'help')
        .version('0.0.1')
        .alias('v', 'version')
        .argv;

    let test = new TestSuite(__dirname + '/testCases');
    test.runTestSuite(argv._);
} catch (error) {
    console.error(error);
    process.exitCode = 1;
}  
