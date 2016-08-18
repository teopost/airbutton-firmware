#include "webserver.h"

void handleNotFound() {
    String s = "<h2>Configuration Mode</h2>\n";
    s += "<p>\n<a href='/wifi'>WiFi</a></p>\n";
    s += "<p>\n<a href='/ifttt'>IFTTT</a></p>\n";
    s += "<p>\n<a href='/customurl'>Custom URL</a></p>\n";
    s += "<p>All done! Please <a href='/reboot'>Reboot</a></p>\n";
    WEB_SERVER.send(200, "text/html", makePage(DEVICE_TITLE,"Configuration mode", s));
}

void handleReboot(){
    String s = "<h2>Rebooting!</h2>\n";
    WEB_SERVER.send(200, "text/html", makePage(DEVICE_TITLE,"Rebooting", s));
    Serial.println("Rebooting...");
    WiFi.mode(WIFI_OFF);
    delay(500);
    ESP.restart();
    //NOTE only the first time after flash Reboot will stuck the chip
}

void handleWiFi() {
    String s = "<h2>Wi-Fi Settings</h2>\n";
    s += "<p>Please select an SSID from the list or add your own.</p>\n";
    s += "<p>Enter the password and submit.</p>\n";
    s += "<form method='get' action='setwifi'>\n";
    s += "<label>SSID: </label>\n<select name='ssid'>";
    s += SSID_LIST;
    s += "\n</select>\n";
    s += "<br><label>or</label>\n<input name='dssid' maxlength='32' placeholder='SSID'/>\n";
    s += "<br><br><label>Password: </label>\n<input name='pass' maxlength='64' type='password' placeholder='password'>\n";
    /*  s += "<br><br><h2>IFTTT Settings</h2>\n";
    s += "<label>IFTTT Key: </label><input name='IFTTT_KEY' maxlenght='32'><br>\n";
    s += "<br><label>IFTTT Event: </label><input name='IFTTT_EVENT' maxlenght='32'><br>\n";
*/  s += "<br><br><input type='submit' value='Submit'>\n</form>";
    WEB_SERVER.send(200, "text/html", makePage(DEVICE_TITLE,"Wi-Fi Settings", s));
}

void handleSetWiFi() {

    ABconfigs.delParam(WIFI);

    String ssid = urlDecode(WEB_SERVER.arg("ssid"));
    String dssid = urlDecode(WEB_SERVER.arg("dssid"));
    if (!dssid.equals("")){
        ssid = dssid;
    }
    Serial.print("SSID: ");
    Serial.println(ssid);

    String pass = urlDecode(WEB_SERVER.arg("pass"));
    Serial.print("Password: ");
    Serial.println(pass);

    Serial.println("Writing SSID and Password to EEPROM...");
    ABconfigs.setParam(WIFI,ssid,pass);

    Serial.println("WiFi settings write to EEPROM done!");
    String s = "<h1>Wifi Setup complete.</h1>\n";
    s += "<p>At restart airbutton will try to connected to <b>";
    s += ssid;
    s += "</b> net, with <b>";
    s += pass;
    s += "</b> as password.\n";
    s +="<br><a href='/'>Back</a></p>";
    WEB_SERVER.send(200, "text/html", makePage(DEVICE_TITLE,"Write Wi-Fi Settings", s));
}
