package com.uninaproject.juicylemon.daos.drinks;

import static com.uninaproject.juicylemon.utils.Utils.API_BASE_URL;

import android.content.Context;
import android.util.Pair;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.JsonObjectRequest;
import com.uninaproject.juicylemon.utils.LoginManager;
import com.uninaproject.juicylemon.events.DrinkImageEvent;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.utils.RequestSender;
import com.uninaproject.juicylemon.utils.VolleyRequestHandler;

import org.greenrobot.eventbus.EventBus;
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
    public void getDrinkImage(int id, Context context) {

        RequestSender.sendRequestForImage(context, API_BASE_URL + "drink/image/" + id, Request.Method.GET, null, new Pair<>(1000,300),new RequestSender.RequestListeners<>(response -> {
            EventBus.getDefault().post(new DrinkImageEvent(response));
        }, System.out::println));

    }
}
