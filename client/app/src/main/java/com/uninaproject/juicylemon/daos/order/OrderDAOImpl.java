package com.uninaproject.juicylemon.daos.order;

import android.content.Context;

import com.android.volley.Request;
import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.events.CartPushErrorEvent;
import com.uninaproject.juicylemon.events.CartPushedEvent;
import com.uninaproject.juicylemon.events.FetchedLastOrderFromServer;
import com.uninaproject.juicylemon.model.Cart;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.utils.LoginManager;
import com.uninaproject.juicylemon.utils.RequestSender;
import com.uninaproject.juicylemon.utils.Utils;

import org.greenrobot.eventbus.EventBus;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Stream;

public class OrderDAOImpl implements OrderDAO {
    @Override
    public void pushCurrentCartToServer(Cart cart, Context context) {
        if (cart.getDrinks().isEmpty())
            return;

        String token = LoginManager.getInstance().getTokenPayload().rawToken;
        Map<String, String> headers = new HashMap<>();

        headers.put("Authorization", "Bearer " + token);

        cart.getDrinks().forEach((drink, amount) -> {
            JSONObject body = new JSONObject();

            try {
                body.put("id_drink", drink.getId());
                body.put("quantity", amount);

                RequestSender.sendRequestForString(context, Utils.API_BASE_URL + "order/drink", Request.Method.POST, body, headers, new RequestSender.RequestListeners<>(
                        response -> {
                            System.out.println(response);
                        },
                        error -> {
                            throw new RuntimeException(error.getMessage());
                        }
                ));

            } catch (JSONException e) {
                EventBus.getDefault().post(new CartPushErrorEvent("Error while parsing JSON"));
            } catch (RuntimeException e) {
                EventBus.getDefault().post(new CartPushErrorEvent(e.getMessage()));
            }


        });

        EventBus.getDefault().post(new CartPushedEvent());
    }

    @Override
    public void fetchDrinksLastOrderFromServer(Context context) {
        String token = LoginManager.getInstance().getTokenPayload().rawToken;
        Map<String, String> headers = new HashMap<>();

        headers.put("Authorization", "Bearer " + token);

        RequestSender.sendRequestForJsonObject(context, Utils.API_BASE_URL + "order/last", Request.Method.GET, null, headers, new RequestSender.RequestListeners<>(
                response -> {
                    JSONArray drinks = response.optJSONArray("drinks");

                    if (drinks == null)
                        return;

                    List<Drink> drinksList = new ArrayList<>();
                    Stream.iterate(0, i -> i + 1).limit(drinks.length()).forEach(i -> {
                        JSONObject drink = drinks.optJSONObject(i);

                        if (drink == null)
                            return;
                        //TODO: controllare se esiste il campo drink_type
                        drinksList.add(Drink.fromJSON(drink));

                    });


                    EventBus.getDefault().post(new FetchedLastOrderFromServer(drinksList));
                },
                error -> {
                    throw new RuntimeException(error.getMessage());
                }
        ));
    }
}
