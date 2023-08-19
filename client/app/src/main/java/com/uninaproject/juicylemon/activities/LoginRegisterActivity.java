package com.uninaproject.juicylemon.activities;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;

import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.daos.TokenPayload;
import com.uninaproject.juicylemon.daos.UserDAO;
import com.uninaproject.juicylemon.events.UserAuthErrorEvent;
import com.uninaproject.juicylemon.events.UserLoginEvent;
import com.uninaproject.juicylemon.events.UserRegisterEvent;
import com.uninaproject.juicylemon.lemonExceptions.UserException;
import com.uninaproject.juicylemon.model.User;
import com.uninaproject.juicylemon.utils.Utils;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import java.util.ArrayList;
import java.util.List;

public class LoginRegisterActivity extends AppCompatActivity {

    List<EditText> loginFields = new ArrayList<>();
    List<EditText> registerFields = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login_register);

        Button btnLogin = findViewById(R.id.register_login_button);

        // Login Fields
        loginFields.add(findViewById(R.id.email_login_edittext));
        loginFields.add(findViewById(R.id.password_login_edittext));

        // Register Fields
        registerFields.add(findViewById(R.id.email_register_edittext));
        registerFields.add(findViewById(R.id.password_register_edittext));
        registerFields.add(findViewById(R.id.conferma_password_register_edittext));

        btnLogin.setOnClickListener(v -> {

            long emptyFieldsLogin = Utils.getAllFieldsNotEmpty(loginFields);
            long emptyFieldsRegister = Utils.getAllFieldsNotEmpty(registerFields);
            UserDAO userDAO = new UserDAO();

            // REGISTER ROUTE
            if (emptyFieldsLogin > 0 && emptyFieldsRegister == 0) {
                if (!registerFields.get(1).getText().toString().equals(registerFields.get(2).getText().toString())) {
                    Utils.showAlert(this, "Le password non coincidono");
                    return;
                }

                try {
                    userDAO.register(
                            registerFields.get(0).getText().toString(),
                            registerFields.get(1).getText().toString(),
                            this
                    );


                    Utils.showAlert(this, "Registrazione avvenuta con successo");
                } catch (UserException e) {
                    Utils.showAlert(this, e.getMessage());
                    return;
                }
            }

            // LOGIN ROUTE
            if (emptyFieldsLogin == 0 && emptyFieldsRegister > 0) {
                try {
                     userDAO.login(
                            loginFields.get(0).getText().toString(),
                            loginFields.get(1).getText().toString(),
                            this);

                } catch (UserException e) {
                    Utils.showAlert(this, e.getMessage());
                    return;
                }
            }

            if (emptyFieldsLogin > 0 && emptyFieldsRegister > 0) {
                Utils.showAlert(this, "Compila i campi");
            }
        });

    }

    private void clearFields(List<EditText> fields) {

        for (EditText field : fields) {
            field.setText("");
        }
    }

    @Subscribe
    public void onMessageEvent(UserLoginEvent event) {
        System.out.println("EVENTO: " + event.tokenPayload.rawToken);
        LoginManager.getInstance().setTokenPayload(event.tokenPayload);

        Intent intent = new Intent(this, DashboardActivity.class);
        startActivity(intent);
    }

    @Subscribe
    public void onMessageEvent(UserRegisterEvent event) {
        Utils.showAlert(this, "Registrazione avvenuta con successo");
        clearFields(registerFields);
    }

    @Subscribe
    public void onMessageEvent(UserAuthErrorEvent event) {
        Utils.showAlert(this, "Errore", event.getMessage());
        clearFields(loginFields);
        clearFields(registerFields);
    }

    @Override
    protected void onStart() {
        super.onStart();
        EventBus.getDefault().register(this);
    }

    @Override
    protected void onStop() {
        super.onStop();
        EventBus.getDefault().unregister(this);
    }
}