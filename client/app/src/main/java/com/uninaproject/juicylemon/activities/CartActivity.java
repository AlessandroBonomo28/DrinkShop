package com.uninaproject.juicylemon.activities;

import android.os.Bundle;
import android.widget.ListView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.lifecycle.ViewModelProvider;

import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.adapters.CartListViewAdapter;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.view_models.CartViewModel;

import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

public class CartActivity extends AppCompatActivity {


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cart);

        CartViewModel model = new ViewModelProvider(this).get(CartViewModel.class);

        // Toolbar setup
        Toolbar toolbar = findViewById(R.id.cart_navbar);
        setSupportActionBar(toolbar);
        toolbar.setNavigationIcon(R.drawable.ic_back_arrow_24);
        toolbar.setNavigationOnClickListener(v -> onBackPressed());

        // get the drink list from the controller
        ListView listView = findViewById(R.id.cart_list_view);

        List<Drink> drinks = (List<Drink>) Objects.requireNonNull(model.getDrinks().getValue()).keySet().stream().map(drink -> (Drink) drink).collect(Collectors.toList());
        listView.setAdapter(new CartListViewAdapter(this, R.layout.cart_list_item, drinks));
    }
}
