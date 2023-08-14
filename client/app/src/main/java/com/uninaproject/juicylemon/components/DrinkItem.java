package com.uninaproject.juicylemon.components;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.core.content.res.ResourcesCompat;

import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.Utils;
import com.uninaproject.juicylemon.model.Drink;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Optional;

public class DrinkItem extends ConstraintLayout {

    TextView drinkName;
    TextView drinkPrice;
    TextView drinkDate;
    Drink drink;
    ImageButton addToCartButton;

    public DrinkItem(@NonNull Context context, @Nullable AttributeSet attrs, Optional<Drink> drink) {
        super(context, attrs);

        // style the component
        setBackground(ResourcesCompat.getDrawable(getResources(), R.drawable.edit_text_border, null));

        // inflate the component
        initializeViews(context);

        // set the drink
        drink.ifPresent(this::setDrink);

    }

    private void initializeViews(Context context) {
        LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        inflater.inflate(R.layout.drink_button, this);

        drinkName = findViewById(R.id.drink_name);
        drinkPrice = findViewById(R.id.drink_price);
        drinkDate = findViewById(R.id.drink_date);

        addToCartButton = findViewById(R.id.add_to_cart_button);

    }

    public void setDrink(Drink drink) {
        drinkName.setText(drink.getName());

        drinkPrice.setText(Utils.addCurrencySymbol(String.valueOf(drink.getPrice())));

        SimpleDateFormat formatter = new SimpleDateFormat("dd/MM/yyyy", Locale.ITALY);
        String date = formatter.format(new Date());
        drinkDate.setText("In Data: \n" +date);

        addToCartButton.setOnClickListener(v -> {
            Utils.showAlert(getContext(), "Aggiunto al carrello!!");
        });
    }
}
