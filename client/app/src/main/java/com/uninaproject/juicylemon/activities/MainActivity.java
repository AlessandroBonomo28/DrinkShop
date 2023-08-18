package com.uninaproject.juicylemon.activities;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;

import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.restCLI.RestClient;

import android.widget.Toast;
public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login_register);

        Button btnLogin = findViewById(R.id.register_login_button);

        btnLogin.setOnClickListener( v -> setContentView(R.layout.activity_main));

        // Effettua la richiesta a /api/login
        String endpoint = "/api/";
        String method = "GET";
        String bearerToken = "il_tuo_bearer_token"; // Imposta il bearer token corretto
        String body = null; // Non c'è bisogno di un body per una richiesta GET

        try {
            String response = RestClient.getInstance().sendRequest(method, endpoint, bearerToken, body);
            // Stampa la risposta in un Toast
            Toast.makeText(this, response, Toast.LENGTH_SHORT).show();
        } catch (Exception e) {
            e.printStackTrace();
            // Stampa l'errore in un Toast
            Toast.makeText(this, "Errore nella richiesta: " + e.getMessage(), Toast.LENGTH_SHORT).show();
        }
    }


}