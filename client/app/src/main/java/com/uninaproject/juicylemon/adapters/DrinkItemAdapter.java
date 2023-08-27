package com.uninaproject.juicylemon.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.uninaproject.juicylemon.components.DrinkItem;
import com.uninaproject.juicylemon.model.Drink;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class DrinkItemAdapter extends ArrayAdapter<Drink> {

    private boolean isDataAcquistoVisible = true;

    public DrinkItemAdapter(Context context, List<Drink> drinks) {
        super(context, 0, drinks);
    }

    public DrinkItemAdapter(Context context, List<Drink> drinks, boolean isDataAcquistoVisible) {
        super(context, 0, drinks);
        this.isDataAcquistoVisible = isDataAcquistoVisible;
    }

    @NonNull
    @Override
    public View getView(int position, View convertView, @NonNull ViewGroup parent) {
        Drink drink = getItem(position);

        return new DrinkItem(getContext(), null, Optional.ofNullable(drink), isDataAcquistoVisible);
    }
}
