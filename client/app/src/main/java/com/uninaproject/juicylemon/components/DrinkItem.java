package com.uninaproject.juicylemon.components;

import static com.uninaproject.juicylemon.utils.Utils.API_BASE_URL;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.util.AttributeSet;
import android.util.Log;
import android.util.Pair;
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

import com.android.volley.Request;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.ImageLoader;
import com.google.android.material.snackbar.Snackbar;
import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.utils.RequestSender;
import com.uninaproject.juicylemon.utils.Utils;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.utils.VolleyRequestHandler;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Objects;
import java.util.Optional;

public class DrinkItem extends ConstraintLayout {

    TextView drinkName;
    TextView drinkPrice;
    TextView drinkDate;
    Drink drink;
    ImageButton addToCartButton;

    ImageView drinkImageView;

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
        drinkImageView = findViewById(R.id.drink_image);

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

        VolleyRequestHandler.getInstance(getContext()).getImageLoader().get(API_BASE_URL + "drink/image/" + drink.getId(), new ImageLoader.ImageListener() {
            @Override
            public void onResponse(ImageLoader.ImageContainer response, boolean isImmediate) {
                Bitmap bitmap = response.getBitmap();
                if (bitmap != null)
                    drinkImageView.setImageBitmap(bitmap);
            }

            @Override
            public void onErrorResponse(VolleyError error) {
            }
        });

    }


}
