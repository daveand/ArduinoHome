
String configUrl;
String response;
int statusCode = 0;

String getArduinoConfig(HttpClient client, String arduinoId) {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Reading status from API...");

        configUrl = "/api/datacollector/getarduinoconfig/" + arduinoId;
        
        client.get(configUrl);

        // read the status code and body of the response
        statusCode = client.responseStatusCode();
        response = client.responseBody();

        Serial.print("Status code: ");
        Serial.println(statusCode);
        // Serial.print("Response: ");
        // Serial.println(response);


        return response;
    }
}

void postDataToApi(HttpClient client, StaticJsonDocument<200> doc) {

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Posting data to API");

    String contentType = "application/json";


    String requestBody;
    serializeJson(doc, requestBody);

    client.post("/api/datacollector/addhomeclimatelog", contentType, requestBody);

    int httpResponseCode = client.responseStatusCode();

    String response = client.responseBody();                       

    Serial.println("Response:");  
    Serial.println(httpResponseCode);   
    Serial.println(response);

  }
  else {
    Serial.println("Could not post data: Not connected to wifi");
    Serial.println("Trying to reconnect...");
    setup();
  }
}
