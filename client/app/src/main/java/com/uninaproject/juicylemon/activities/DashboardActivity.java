package com.uninaproject.juicylemon.activities;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.FrameLayout;
import android.widget.ListView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.adapters.DrinkItemAdapter;
import com.uninaproject.juicylemon.daos.drinks.DrinkDAO;
import com.uninaproject.juicylemon.daos.drinks.DrinkDAOImpl;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.utils.Utils;

import org.json.JSONException;

import java.util.ArrayList;
import java.util.List;


public class DashboardActivity  extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView welcome_text = findViewById(R.id.navbar_text_welcome);

        String email = LoginManager.getInstance().getUser().email;
        welcome_text.setText(getString(R.string.welcome_text, Utils.capitalize(Utils.separateEmail(email).first)));


        FrameLayout frullati_button = findViewById(R.id.frullati_button);
        frullati_button.setOnClickListener(v -> {
            Utils.showAlert(this, "Frullati");
        });

        FrameLayout cocktails_button = findViewById(R.id.cocktails_button);
        cocktails_button.setOnClickListener(v -> {
            Utils.showAlert(this, "Cocktails");
        });


        ListView listView = findViewById(R.id.list_view);

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                Drink drink = (Drink) listView.getItemAtPosition(position);
                Intent intent = new Intent(DashboardActivity.this, DetailActivity.class);
                intent.putExtra("drink", drink);
                startActivity(intent);
            }
        });

        DrinkDAO drinkDAO = new DrinkDAOImpl();

        List<Drink> drinks = new ArrayList<>();

        drinkDAO.getDrinks(this, arrayOfDrinks -> {
            int sizeOfList = arrayOfDrinks.length();
            for (int i = 0; i < sizeOfList; i++) {
                try {
                    drinks.add(Drink.fromJSON(arrayOfDrinks.getJSONObject(i)));
                } catch (JSONException e) {
                    throw new RuntimeException(e);
                }
                System.out.println(drinks);
            }
            listView.setAdapter(new DrinkItemAdapter(this, drinks));
        });


    }
}
