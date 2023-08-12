package com.uninaproject.juicylemon;

import android.os.Bundle;
import android.widget.FrameLayout;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;


public class DashboardActivity  extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView welcome_text = findViewById(R.id.navbar_text_welcome);

        String email = LoginManager.getInstance().getUser().email;
        welcome_text.setText(getString(R.string.welcome_text, Utils.capitalize(Utils.separateEmail(email).first)));


        FrameLayout frullati_button = findViewById(R.id.frullati_button);
        frullati_button.setOnClickListener(v -> {
            Utils.showAlert(this, "Frullati");
        });

        FrameLayout cocktails_button = findViewById(R.id.cocktails_button);
        cocktails_button.setOnClickListener(v -> {
            Utils.showAlert(this, "Cocktails");
        });


    }
}
