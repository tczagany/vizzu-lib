var selenium = require('selenium-webdriver');
var chrome = require('selenium-webdriver/chrome');
var chromedriver = require('chromedriver');



async function getTitle(port){
       
       
      
       let driver = new selenium.Builder()
           .forBrowser('chrome')
           .setChromeOptions(new chrome.Options().headless())
           .build();
 
        await driver.get('http://127.0.0.1:' + port + '/index.html');

        var title = await driver.getTitle();
        console.log('Title is:',title);
        
        await driver.quit();
}


(async () => {
    try {
        var port = process.argv[2]
        await getTitle(port);
    } catch (e) {
        console.log(e);
    }
})();

