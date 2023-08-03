package com.uninaproject.juicylemon;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;

import java.util.ArrayList;
import java.util.List;

public class LoginRegisterActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login_register);

        Button btnLogin = findViewById(R.id.register_login_button);

        // Login Fields
        List<EditText> loginFields = new ArrayList<>();
        loginFields.add(findViewById(R.id.email_login_edittext));
        loginFields.add(findViewById(R.id.password_login_edittext));

        // Register Fields
        List<EditText> registerFields = new ArrayList<>();
        registerFields.add(findViewById(R.id.email_register_edittext));
        registerFields.add(findViewById(R.id.password_register_edittext));
        registerFields.add(findViewById(R.id.conferma_password_register_edittext));

        btnLogin.setOnClickListener(v -> {
            LoginManager loginManager = new LoginManager();

            long emptyFieldsLogin = Utils.getAllFieldsNotEmpty(loginFields);
            long emptyFieldsRegister = Utils.getAllFieldsNotEmpty(registerFields);

            // If there are empty fields in login and no empty fields in register, then isLogging is false
            // REGISTER ROUTE
            if (emptyFieldsLogin > 0 && emptyFieldsRegister == 0) {
                if (!registerFields.get(1).getText().toString().equals(registerFields.get(2).getText().toString())) {
                    Utils.showAlert(this, "Le password non coincidono");
                    return;
                }

                loginManager.register(
                        registerFields.get(0).getText().toString(),
                        registerFields.get(1).getText().toString()
                );
            }

            // LOGIN ROUTE
            if (emptyFieldsLogin == 0 && emptyFieldsRegister > 0) {
                loginManager.login(
                        loginFields.get(0).getText().toString(),
                        loginFields.get(1).getText().toString()
                );
            }

            if (emptyFieldsLogin > 0 && emptyFieldsRegister > 0) {
                Utils.showAlert(this, "Compila i campi");
                return;
            }




            Intent intent = new Intent(this, DashboardActivity.class);
            startActivity(intent);
        });

    }


}