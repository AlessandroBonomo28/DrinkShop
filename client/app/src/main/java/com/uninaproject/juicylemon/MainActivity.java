package com.uninaproject.juicylemon;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login_register);

        Button btnLogin = findViewById(R.id.register_login_button);

        btnLogin.setOnClickListener( v -> setContentView(R.layout.activity_main));
    }


}