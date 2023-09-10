package com.uninaproject.juicylemon.activities;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.widget.FrameLayout;
import android.widget.ListView;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.adapters.DrinkItemAdapter;
import com.uninaproject.juicylemon.events.FetchedLastOrderFromServer;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.model.DrinkType;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import java.util.Objects;


public class DashboardActivity  extends AppCompatActivity {
    ListView listView;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        listView = findViewById(R.id.dashboard_list_view);

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

        Controller.getInstance().getOrderDAO().fetchDrinksLastOrderFromServer(this);

        listView.setOnItemClickListener((parent, view, position, id) -> {
            Drink drink = (Drink) parent.getItemAtPosition(position);
            Intent intent = new Intent(DashboardActivity.this, DetailActivity.class);
            intent.putExtra("drink", drink);
            startActivity(intent);
        });

    }

    @Subscribe
    public void onFetchedDrinks(FetchedLastOrderFromServer event) {
        System.out.println("Fetched drinks");
        DrinkItemAdapter adapter = new DrinkItemAdapter(this, event.getDrinksFetched(), true);
        listView.setAdapter(adapter);
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

    @Override
    protected void onRestart() {
        super.onRestart();
        Controller.getInstance().getOrderDAO().fetchDrinksLastOrderFromServer(this);
    }
}
