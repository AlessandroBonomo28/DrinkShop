package com.uninaproject.juicylemon.daos.drinks;

import static com.uninaproject.juicylemon.utils.Utils.API_BASE_URL;

import android.content.Context;
import android.util.Pair;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.JsonObjectRequest;
import com.uninaproject.juicylemon.events.DrinkFetchedEvent;
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


    public List<Drink> getDrinks(Context context) {

        RequestSender.sendRequestForJsonArray(context, API_BASE_URL + "drinks", Request.Method.GET, null,new RequestSender.RequestListeners<>(response -> {
            EventBus.getDefault().post(new DrinkFetchedEvent(Drink.fromJsonArray(response)));
        }, System.out::println));

        return null;
    }


    @Override
    public Drink getDrink(int id, Context context, Response.Listener<JSONObject> listener) {

        String token = LoginManager.getInstance().getTokenPayload().rawToken;
        RequestSender.sendRequestForJsonArray(context, API_BASE_URL + "drinks" + id, Request.Method.GET, null,new RequestSender.RequestListeners<>(response -> {
            EventBus.getDefault().post(new DrinkFetchedEvent(Drink.fromJsonArray(response)));
        }, System.out::println));

        return null;
    }

    @Override
    public void getDrinkImage(int id, Context context) {

        RequestSender.sendRequestForImage(context, API_BASE_URL + "drink/image/" + id, Request.Method.GET, null, new Pair<>(1000,300),new RequestSender.RequestListeners<>(response -> {
            EventBus.getDefault().post(new DrinkImageEvent(response, id));
        }, System.out::println));

    }
}
