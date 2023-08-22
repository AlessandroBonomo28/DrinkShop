package com.uninaproject.juicylemon.activities;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.coordinatorlayout.widget.CoordinatorLayout;
import androidx.core.content.res.ResourcesCompat;
import androidx.lifecycle.ViewModel;
import androidx.lifecycle.ViewModelProvider;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.snackbar.Snackbar;
import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.daos.drinks.DrinkDAO;
import com.uninaproject.juicylemon.daos.drinks.DrinkDAOImpl;
import com.uninaproject.juicylemon.events.DrinkImageEvent;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.utils.Utils;
import com.uninaproject.juicylemon.view_models.DetailActivityViewModel;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import java.util.Objects;

public class DetailActivity extends AppCompatActivity {

    private ImageView drinkImage;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_detail);

        DetailActivityViewModel model = new ViewModelProvider(this).get(DetailActivityViewModel.class);

        Toolbar toolbar = findViewById(R.id.toolbar_detail);
        setSupportActionBar(toolbar);
        toolbar.setNavigationIcon(R.drawable.ic_back_arrow_24);
        toolbar.setNavigationOnClickListener(v -> onBackPressed());


        Controller controller = Controller.getInstance();

        TextView drinkName = findViewById(R.id.drinkDetailName);
        TextView drinkDescription = findViewById(R.id.drink_detail_description);
        CoordinatorLayout coordinatorLayout = findViewById(R.id.coordinator_detail);
        FloatingActionButton fabAddToCart = findViewById(R.id.fab_detail_add_to_cart);
        drinkImage = findViewById(R.id.drink_image_view_detail);

        Drink selectedDrink = (Drink) Objects.requireNonNull(getIntent().getExtras()).getSerializable("drink");

        if (selectedDrink == null)
            throw new RuntimeException("Drink is null");

        // get the drink image asynchronously
        controller.getDrinkDAO().getDrinkImage(selectedDrink.getId(), this);

        // setting variables
        drinkName.setText(selectedDrink.getName());
        drinkDescription.setText(selectedDrink.getDescription());

        fabAddToCart.setOnClickListener(v -> {
            model.addDrinkToCart(selectedDrink);
            Snackbar.make(coordinatorLayout, "Drink added to cart", Snackbar.LENGTH_SHORT).show();
        });
    }

    @Subscribe
    public void onDrinkImageEvent(DrinkImageEvent drinkImageEvent) {
        Bitmap image = drinkImageEvent.image;
        drinkImage.setImageBitmap(image);

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
