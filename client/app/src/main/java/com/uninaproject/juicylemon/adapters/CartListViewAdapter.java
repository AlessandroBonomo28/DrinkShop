package com.uninaproject.juicylemon.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.model.Drink;

import java.util.List;

public class CartListViewAdapter extends ArrayAdapter<Drink> {

    public CartListViewAdapter(@NonNull Context context, int resource, @NonNull List<Drink> drinks) {
        super(context, resource, drinks);
    }


    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        Drink drink = getItem(position);

        if (convertView == null) {
            convertView = LayoutInflater.from(getContext()).inflate(R.layout.cart_list_item, parent, false);
        }

        TextView drinkName = convertView.findViewById(R.id.cart_list_item_drink_name);
        TextView drinkCounter = convertView.findViewById(R.id.cart_list_item_count);

        if (drink == null) {
            return convertView;
        }

        drinkName.setText(drink.getName());
        drinkCounter.setText(String.valueOf(Controller.getInstance().getCart().getDrinks().get(drink)));

        return convertView;
    }
}
