var selenium = require('selenium-webdriver');
var chrome = require('selenium-webdriver/chrome');
var chromedriver = require('chromedriver');


class Chrome {

    #browser;
    

    constructor() {}


    openBrowser(headless=true) {
        if (headless) {
            this.#browser = new selenium.Builder()
                .forBrowser('chrome')
                .setChromeOptions(new chrome.Options().headless())
                .build();
        } else {
            this.#browser = new selenium.Builder()
                .forBrowser('chrome')
                .build();
        }
    }

    closeBrowser() {
        this.#browser.quit();
    }

    async getUrl(url) {
        await this.#browser.get(url);
        await this.#browser.wait(selenium.until.elementLocated(selenium.By.id('vizzuResult')), 1000);
        await this.#browser.wait(selenium.until.elementTextIs(this.#browser.findElement(selenium.By.id('vizzuResult')), 'PASSED'), 1000);
    }
}


module.exports = Chrome;
