package com.uninaproject.juicylemon.daos.order;

import android.content.Context;

import com.android.volley.Request;
import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.events.CartPushErrorEvent;
import com.uninaproject.juicylemon.events.CartPushedEvent;
import com.uninaproject.juicylemon.model.Cart;
import com.uninaproject.juicylemon.utils.LoginManager;
import com.uninaproject.juicylemon.utils.RequestSender;
import com.uninaproject.juicylemon.utils.Utils;

import org.greenrobot.eventbus.EventBus;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

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

                RequestSender.sendRequestForString(context, Utils.API_BASE_URL + "order/drink", Request.Method.POST, body, headers, new RequestSender.RequestListeners<String>(
                        response -> {
                            EventBus.getDefault().post(new CartPushedEvent());
                        },
                        error -> {
                            // TODO: ricevo sempre un errore 401, da rivedere con Alessandro
                            EventBus.getDefault().post(new CartPushErrorEvent("Errore while pushing cart to server"));
                        }
                ));

            } catch (JSONException e) {
                EventBus.getDefault().post(new CartPushErrorEvent("Error while parsing JSON"));
            }


        });
    }

    @Override
    public void fetchOrdersFromServer(Context context) {

    }
}
