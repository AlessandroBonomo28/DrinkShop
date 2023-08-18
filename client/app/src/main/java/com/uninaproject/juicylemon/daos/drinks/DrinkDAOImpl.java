package com.uninaproject.juicylemon.daos.drinks;

import static com.uninaproject.juicylemon.utils.Utils.API_BASE_URL;

import android.content.Context;

import com.android.volley.Response;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.JsonObjectRequest;
import com.uninaproject.juicylemon.activities.LoginManager;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.utils.VolleyRequestHandler;

import org.json.JSONArray;
import org.json.JSONObject;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class DrinkDAOImpl implements DrinkDAO {


    public List<Drink> getDrinks(Context context, Response.Listener<JSONArray> listener) {
        String token = LoginManager.getInstance().getTokenPayload().rawToken;
        System.out.println(token);
        List<Drink> drinks = new ArrayList<>();


        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(
                API_BASE_URL + "drinks", listener, error -> {
            System.out.println("ERRORE: " + error.toString());
        }) {
            @Override
            public Map<String, String> getHeaders()  {
                Map<String, String> headers = new HashMap<>();

                headers.put("Authorization", "Bearer " + token);
                return headers;
            }
        };

        VolleyRequestHandler.getInstance(context).addToRequestQueue(jsonArrayRequest);


        return drinks;
    }


    @Override
    public Drink getDrink(int id, Context context, Response.Listener<JSONObject> listener) {

        String token = LoginManager.getInstance().getTokenPayload().rawToken;
        JsonObjectRequest jsonArrayRequest = new JsonObjectRequest(
                API_BASE_URL + "drink/" + id, listener, error -> {
            System.out.println("ERRORE: " + error.toString());
        }) {
            @Override
            public Map<String, String> getHeaders()  {
                Map<String, String> headers = new HashMap<>();

                headers.put("Authorization", "Bearer " + token);
                return headers;
            }
        };

        VolleyRequestHandler.getInstance(context).addToRequestQueue(jsonArrayRequest);

        return null;
    }

    @Override
    public String getDrinkImage(int id, Context context) {
        return null;
    }
}
