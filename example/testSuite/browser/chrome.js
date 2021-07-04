let webdriver = require('selenium-webdriver');
let chrome = require('selenium-webdriver/chrome');
let chromedriver = require('chromedriver');


class Chrome {

    #driver;
    

    constructor() {}


    openBrowser(headless=true) {
        /*const { Preferences, Type, Level } = require('selenium-webdriver/lib/logging')
        const { By, Key, Capabilities, until } = require('selenium-webdriver');
        const caps = webdriver.Capabilities.chrome();
        const logPrefs = new Preferences();
        logPrefs.setLevel(Type.BROWSER, Level.ALL);
        caps.setLoggingPrefs(logPrefs);
        caps.set('goog:loggingPrefs', logPrefs);
        const options = new chrome.Options(caps);*/

        const options = new chrome.Options();
        if (headless) {
            options.headless();
        }
        this.#driver = new webdriver.Builder()
                .forBrowser('chrome')
                .setChromeOptions(options)
                .withCapabilities(webdriver.Capabilities.chrome())
                .build();

        /*this.#driver.manage().logs()
            .get(Type.BROWSER)
            .then(v => v && v.length && console.log(v));*/
    }

    closeBrowser() {
        this.#driver.quit();
    }

    getDriver() {
        return this.#driver;
    }

    async getUrl(url) {
        await this.#driver.get(url);
    }
}



module.exports = Chrome;
