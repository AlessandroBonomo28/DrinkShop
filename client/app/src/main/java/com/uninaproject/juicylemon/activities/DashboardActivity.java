package com.uninaproject.juicylemon.activities;

import android.content.Intent;
import android.os.Bundle;
import android.widget.FrameLayout;
import android.widget.ListView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.adapters.DrinkItemAdapter;
import com.uninaproject.juicylemon.events.DrinkFetchedEvent;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.model.DrinkType;
import com.uninaproject.juicylemon.utils.Utils;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import java.util.Objects;


public class DashboardActivity  extends AppCompatActivity {
    ListView listView;


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
            intent.putExtra("drinkType", DrinkType.FRULLATO);
            startActivity(intent);
        });

        FrameLayout cocktails_button = findViewById(R.id.cocktails_button);
        cocktails_button.setOnClickListener(v -> {
            Intent intent = new Intent(DashboardActivity.this, DrinksListView.class);
            intent.putExtra("drinkType", DrinkType.COCKTAIL);
            startActivity(intent);
        });

    }

}
