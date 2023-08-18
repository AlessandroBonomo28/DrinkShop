package com.uninaproject.juicylemon.daos.drinks;

import android.content.Context;

import com.android.volley.Response;
import com.uninaproject.juicylemon.model.Drink;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.List;
import java.util.function.Function;

public interface DrinkDAO {
    public List<Drink> getDrinks(Context context, Response.Listener<JSONArray> listener);

    public Drink getDrink(int id, Context context, Response.Listener<JSONObject> listener);

    public String getDrinkImage(int id, Context context);
}
