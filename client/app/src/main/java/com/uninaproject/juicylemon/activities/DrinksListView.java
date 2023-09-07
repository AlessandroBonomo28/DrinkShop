package com.uninaproject.juicylemon.activities;

import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.widget.EditText;
import android.widget.ListView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.lifecycle.ViewModelProvider;

import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.adapters.DrinkItemAdapter;
import com.uninaproject.juicylemon.events.DrinkFetchedEvent;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.model.DrinkType;
import com.uninaproject.juicylemon.view_models.DrinksListViewModel;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import java.util.List;
import java.util.stream.Collectors;

public class DrinksListView  extends AppCompatActivity {

    ListView drinksListView;

    DrinkType selectedDrinkType;

    DrinksListViewModel model;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_drinks_list);

        Toolbar toolbar = findViewById(R.id.drink_list_view_navbar);
        setSupportActionBar(toolbar);
        toolbar.setNavigationIcon(R.drawable.ic_back_arrow_24);
        toolbar.setNavigationOnClickListener(v -> onBackPressed());


        model = new ViewModelProvider(this).get(DrinksListViewModel.class);

        selectedDrinkType = (DrinkType) getIntent().getSerializableExtra("drinkType");

        // LIST VIEW
        drinksListView = findViewById(R.id.drinks_list_view);
        drinksListView.setOnItemClickListener((parent, view, position, id) -> {
            Drink drink = (Drink) drinksListView.getItemAtPosition(position);
            Intent intent = new Intent(DrinksListView.this, DetailActivity.class);
            intent.putExtra("drink", drink);
            startActivity(intent);
        });

        // EDIT TEXT
        EditText searchEditText = findViewById(R.id.drinks_list_edit_text);
        searchEditText.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {}

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                model.setSearchQuery(s.toString());
            }

            @Override
            public void afterTextChanged(Editable s) {}
        });

        // MODEL OBSERVERS
        model.getSearchQuery().observe(this, query -> {
            if (query.isEmpty()) {
                model.getDrinks().setValue(model.getDefaultDrinks());
            } else {
                if (model.getDrinks().getValue() == null)
                    return;

                List<Drink> filteredDrinks = model.getDrinks().getValue().stream().filter(drink -> drink.getName().toLowerCase().contains(query.toLowerCase())).collect(Collectors.toList());
                model.getDrinks().setValue(filteredDrinks);
            }
        });

        model.getDrinks().observe(this, drinks -> {
            drinksListView.setAdapter(new DrinkItemAdapter(this, drinks));
        });

        // FETCH DRINKS
        Controller.getInstance().getDrinkDAO().getDrinks(this);
    }

    @Subscribe
    public void onDrinksFetched(DrinkFetchedEvent event) {
        if (selectedDrinkType != null) {
            event.getDrinks().removeIf(drink -> !drink.getType().equals(selectedDrinkType));
        }

        model.getDrinks().setValue(event.getDrinks());
        model.setDefaultDrinks(event.getDrinks());
        drinksListView.setAdapter(new DrinkItemAdapter(this, model.getDrinks().getValue(), false));
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
