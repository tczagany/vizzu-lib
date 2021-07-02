var selenium = require('selenium-webdriver');
var chrome = require('selenium-webdriver/chrome');
var chromedriver = require('chromedriver');


class Chrome {

    #browser;
    

    constructor() {}


    openBrowser(headless=true) {
        /*const { Preferences, Type, Level } = require('selenium-webdriver/lib/logging')
        const { By, Key, Capabilities, until } = require('selenium-webdriver');
        const caps = selenium.Capabilities.chrome();
        const logPrefs = new Preferences();
        logPrefs.setLevel(Type.BROWSER, Level.ALL);
        caps.setLoggingPrefs(logPrefs);
        caps.set('goog:loggingPrefs', logPrefs);
        const options = new chrome.Options(caps);*/

        const options = new chrome.Options();
        if (headless) {
            options.headless();
        }
        this.#browser = new selenium.Builder()
                .forBrowser('chrome')
                .setChromeOptions(options)
                .build();

        /*this.#browser.manage().logs()
            .get(Type.BROWSER)
            .then(v => v && v.length && console.log(v));*/
    }

    closeBrowser() {
        this.#browser.quit();
    }

    async getUrl(url) {
        await this.#browser.get(url);        
        await this.#browser.wait(selenium.until.elementLocated(selenium.By.id('vizzuCanvas')), 1000);
        await this.#browser.wait(selenium.until.elementTextIs(this.#browser.findElement(selenium.By.id('vizzuCanvas')), 'PASSED'), 10000);
    }
}


module.exports = Chrome;
