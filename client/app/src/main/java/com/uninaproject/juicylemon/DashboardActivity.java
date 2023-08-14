package com.uninaproject.juicylemon;

import android.os.Bundle;
import android.widget.FrameLayout;
import android.widget.ListView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.uninaproject.juicylemon.adapters.DrinkItemAdapter;
import com.uninaproject.juicylemon.components.DrinkItem;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.model.DrinkType;

import java.util.ArrayList;
import java.util.List;


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


        ListView listView = findViewById(R.id.list_view);
        List<Drink> drinks = new ArrayList<>();

        drinks.add(new Drink("Cockatail di Mango", DrinkType.COCKTAIL, 4.5f));
        drinks.add(new Drink("Cockatail di Mango", DrinkType.COCKTAIL, 4.5f));
        drinks.add(new Drink("Cockatail di Mango", DrinkType.COCKTAIL, 4.5f));
        drinks.add(new Drink("Cockatail di Mango", DrinkType.COCKTAIL, 4.5f));

        listView.setAdapter(new DrinkItemAdapter(this, drinks));
    }
}
