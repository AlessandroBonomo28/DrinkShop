package com.uninaproject.juicylemon.activities;

import android.os.Bundle;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.model.Drink;

import java.util.Objects;

public class DetailActivity extends AppCompatActivity {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_detail);

        TextView drinkName = findViewById(R.id.drinkDetailName);

        Drink selectedDrink = (Drink) Objects.requireNonNull(getIntent().getExtras()).getSerializable("drink");
        drinkName.setText(selectedDrink.getName());

    }
}
