
String response;
int statusCode = 0;

const char* getApiStatus(HttpClient client) {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Reading status from API...");

        client.get("/api/datacollector/status");

        // read the status code and body of the response
        statusCode = client.responseStatusCode();
        response = client.responseBody();

        Serial.print("Status code: ");
        Serial.println(statusCode);
        Serial.print("Response: ");
        Serial.println(response);

        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, response);

        // Test if parsing succeeds.
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return "error";
        }

        const char* dateTime = doc["dateTime"];

        return dateTime;
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
