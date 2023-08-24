package com.uninaproject.juicylemon.activities;

import android.content.Intent;
import android.os.Bundle;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.adapters.DrinkItemAdapter;
import com.uninaproject.juicylemon.daos.drinks.DrinkDAO;
import com.uninaproject.juicylemon.daos.drinks.DrinkDAOImpl;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.utils.LoginManager;
import com.uninaproject.juicylemon.utils.Utils;

import org.json.JSONException;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;


public class DashboardActivity  extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Toolbar toolbar = findViewById(R.id.navbar_dashboard);
        setSupportActionBar(toolbar);
        Objects.requireNonNull(getSupportActionBar()).setDisplayShowTitleEnabled(false);

        FrameLayout frullati_button = findViewById(R.id.frullati_button);
        frullati_button.setOnClickListener(v -> {
            Intent intent = new Intent(DashboardActivity.this, DrinksListView.class);
            startActivity(intent);
        });

        FrameLayout cocktails_button = findViewById(R.id.cocktails_button);
        cocktails_button.setOnClickListener(v -> {
            Utils.showAlert(this, "Cocktails");
        });


        ListView listView = findViewById(R.id.list_view);

        listView.setOnItemClickListener((parent, view, position, id) -> {
            Drink drink = (Drink) listView.getItemAtPosition(position);
            Intent intent = new Intent(DashboardActivity.this, DetailActivity.class);
            intent.putExtra("drink", drink);
            startActivity(intent);
        });

        DrinkDAO drinkDAO = new DrinkDAOImpl();

        List<Drink> drinks = new ArrayList<>();

        drinkDAO.getDrinks(this, arrayOfDrinks -> {
            int sizeOfList = arrayOfDrinks.length();
            for (int i = 0; i < sizeOfList; i++) {
                try {
                    drinks.add(Drink.fromJSON(arrayOfDrinks.getJSONObject(i)));
                } catch (JSONException e) {
                    throw new RuntimeException(e);
                }
                System.out.println(drinks);
            }
            listView.setAdapter(new DrinkItemAdapter(this, drinks));
        });


    }

}
