package com.uninaproject.juicylemon.daos.order;

import static com.uninaproject.juicylemon.utils.Utils.API_BASE_URL;

import android.content.Context;
import android.util.Log;

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
    // TODO: forse da fare un refactor
    @Override
    public void pushCurrentCartToServer(Cart cart, Context context) {
        if (cart.getDrinks().isEmpty())
            return;

        Runnable runnable = () -> {
            JSONObject body = new JSONObject();
            try {
                body.put("amount", 1000);
                body.put("card_holder", "alex");
                body.put("card_number", "1234567891112130");
                body.put("CVV", "123");
                body.put("expiration_date", "01/21");

                Map<String, String> headers = new HashMap<>();
                headers.put("Authorization", "Bearer " + LoginManager.getInstance().getTokenPayload().rawToken);


                RequestSender.sendRequestForJsonObject(context, API_BASE_URL + "pay", Request.Method.POST, body, headers, new RequestSender.RequestListeners<>(
                                response -> {
                                    Log.d("CartViewModel", "sendOrder: " + response.toString());
                                },
                                error -> {
                                    Log.d("CartViewModel", "sendOrder: " + error.toString());
                                }
                        )
                );

            } catch (JSONException ignored) {
            }
        };

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

        Thread thread = new Thread(runnable);
        thread.start();
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

                    if (drinks == null) {
                        EventBus.getDefault().post(new FetchedLastOrderFromServer(new ArrayList<>()));
                        return;
                    }

                    List<Drink> drinksList = new ArrayList<>();
                    Stream.iterate(0, i -> i + 1).limit(drinks.length()).forEach(i -> {
                        JSONObject drink = drinks.optJSONObject(i);
                        drinksList.add(Drink.fromJSON(drink, "id_drink"));
                    });

                    System.out.println(drinksList);
                    EventBus.getDefault().post(new FetchedLastOrderFromServer(drinksList));
                },
                error -> {
                    EventBus.getDefault().post(new FetchedLastOrderFromServer(new ArrayList<>()));
                }
        ));
    }
}
