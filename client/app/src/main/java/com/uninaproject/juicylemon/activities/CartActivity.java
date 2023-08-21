package com.uninaproject.juicylemon.activities;

import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.model.Drink;

import java.util.List;

public class CartActivity extends AppCompatActivity {

    List<Drink> drinkList;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cart);

    }
}
