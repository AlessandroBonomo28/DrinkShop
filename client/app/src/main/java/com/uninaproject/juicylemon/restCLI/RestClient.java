package com.uninaproject.juicylemon.restCLI;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class RestClient {
    private static RestClient instance;
    private static final String BASE_URL = "http://ratatouille.bonomo.cloud:3000";

    private RestClient() {
        // Private constructor to prevent direct instantiation
    }

    public static RestClient getInstance() {
        if (instance == null) {
            synchronized (RestClient.class) {
                if (instance == null) {
                    instance = new RestClient();
                }
            }
        }
        return instance;
    }

    public String sendRequest(String method, String endpoint, String bearerToken, String body) throws IOException {
        HttpURLConnection connection = null;
        BufferedReader reader = null;

        try {
            // Create the full URL of the endpoint
            URL url = new URL(BASE_URL + endpoint);

            // Open the HTTP connection
            connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod(method);

            // Set the Authorization header if present
            if (bearerToken != null && !bearerToken.isEmpty()) {
                connection.setRequestProperty("Authorization", "Bearer " + bearerToken);
            }

            // Set the content type of the request as application/json
            connection.setRequestProperty("Content-Type", "application/json");

            // Check if the request requires a body
            if (body != null && !body.isEmpty()) {
                connection.setDoOutput(true);
                OutputStream outputStream = connection.getOutputStream();
                outputStream.write(body.getBytes("UTF-8"));
                outputStream.flush();
                outputStream.close();
            }

            // Check the response status code
            int responseCode = connection.getResponseCode();
            if (responseCode >= 200 && responseCode < 300) {
                // Read the response from the server
                reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
                StringBuilder response = new StringBuilder();
                String line;
                while ((line = reader.readLine()) != null) {
                    response.append(line);
                }

                // Return the response as a string
                return response.toString();
            } else {
                // Read the error from the response
                reader = new BufferedReader(new InputStreamReader(connection.getErrorStream()));
                StringBuilder errorResponse = new StringBuilder();
                String line;
                while ((line = reader.readLine()) != null) {
                    errorResponse.append(line);
                }

                // Throw an exception with the error message
                throw new IOException("Request failed with HTTP error code: " + responseCode +
                        "\nError response: " + errorResponse.toString());
            }
        } catch (IOException e) {
            throw e;
        } finally {
            // Close resources
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (connection != null) {
                connection.disconnect();
            }
        }
    }
}
