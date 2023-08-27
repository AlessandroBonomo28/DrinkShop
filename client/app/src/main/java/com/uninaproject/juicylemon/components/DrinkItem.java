package com.uninaproject.juicylemon.components;

import android.annotation.SuppressLint;
import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.core.content.res.ResourcesCompat;

import com.google.android.material.snackbar.Snackbar;
import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.events.DrinkImageEvent;
import com.uninaproject.juicylemon.model.DrinkType;
import com.uninaproject.juicylemon.utils.Utils;
import com.uninaproject.juicylemon.model.Drink;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

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

    ImageView drinkImage;

    private boolean isDataAcquistoVisible = true;

    public DrinkItem(@NonNull Context context, @Nullable AttributeSet attrs, Optional<Drink> drink, boolean isDataAcquistoVisible) {
        super(context, attrs);
        this.isDataAcquistoVisible = isDataAcquistoVisible;

        // style the component
        setBackground(ResourcesCompat.getDrawable(getResources(), R.drawable.edit_text_border, null));

        // set android:descendantFocusability="blocksDescendants"
        setDescendantFocusability(FOCUS_BLOCK_DESCENDANTS);


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
        drinkImage = findViewById(R.id.drink_image);

        if (!isDataAcquistoVisible)
            drinkDate.setVisibility(GONE);

        addToCartButton = findViewById(R.id.add_to_cart_button);
    }

    @SuppressLint("SetTextI18n")
    public void setDrink(Drink drink) {
        drinkName.setText(drink.getName());

        String price = Utils.normalizePrice(String.valueOf(drink.getPrice()));
        drinkPrice.setText("€" + price);

        SimpleDateFormat formatter = new SimpleDateFormat("dd/MM/yyyy", Locale.ITALY);
        String date = formatter.format(new Date());
        drinkDate.setText("In Data: \n" + date);

        addToCartButton.setOnClickListener(v -> {
            Controller.getInstance().addDrinkToCart(drink);
            playAnimation();
            Snackbar.make(this, drink.getName() + " aggiunto al carrello", Snackbar.LENGTH_SHORT).show();
        });

        // set the drink icon
        setDrinkIcon(drink);
    }

    public void hideDataAcquisto() {
        drinkDate.setVisibility(GONE);
    }

    private void playAnimation() {
        Animation colorAnimation = AnimationUtils.loadAnimation(getContext(), R.anim.rotate);
        colorAnimation.setDuration(1000);
        colorAnimation.setRepeatMode(Animation.RESTART);
        addToCartButton.startAnimation(colorAnimation);
    }

    private void setDrinkIcon(Drink drink) {
        if (drink.getType() == DrinkType.COCKTAIL)
            drinkImage.setImageDrawable(ResourcesCompat.getDrawable(getResources(), R.drawable.image_cocktails, null));
        else
            drinkImage.setImageDrawable(ResourcesCompat.getDrawable(getResources(), R.drawable.image_tre_frullati, null));

    }

}
