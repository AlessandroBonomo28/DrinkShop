package com.uninaproject.juicylemon.daos;

import static com.uninaproject.juicylemon.utils.Utils.API_BASE_URL;

import android.content.Context;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.toolbox.JsonObjectRequest;
import com.uninaproject.juicylemon.events.UserAuthErrorEvent;
import com.uninaproject.juicylemon.events.UserLoginEvent;
import com.uninaproject.juicylemon.events.UserRegisterEvent;
import com.uninaproject.juicylemon.lemonExceptions.UserException;
import com.uninaproject.juicylemon.utils.RequestSender;
import com.uninaproject.juicylemon.utils.VolleyRequestHandler;

import org.greenrobot.eventbus.EventBus;
import org.json.JSONObject;

public class UserDAO implements IUserDAO {
    @Override
    public TokenPayload login(String email, String password, Context context) throws UserException {

        System.out.println("LOGIN");

        if (email.isEmpty() || password.isEmpty())
            throw new UserException("Utente inesistente");

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(
                API_BASE_URL + "login", obj -> {
            try {
                TokenPayload tokenPayload = new TokenPayload(obj.getString("token"));
                UserLoginEvent userLoginEvent = new UserLoginEvent(tokenPayload);

                EventBus.getDefault().post(userLoginEvent);
            } catch (Exception e) {
                EventBus.getDefault().post(new UserAuthErrorEvent(e.getMessage()));

            }
        }, (error) -> {
            EventBus.getDefault().post(new UserAuthErrorEvent("Non è stato possibile effettuare il login"));
        }) {

            @Override
            public int getMethod() {
                return Method.POST;
            }

            @Override
            public byte[] getBody() {
                JSONObject body = new JSONObject();
                try {
                    body.put("email", email);
                    body.put("password", password);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                return body.toString().getBytes();
            }
        };

        VolleyRequestHandler.getInstance(context).addToRequestQueue(jsonObjectRequest);
        return null;
    }

    @Override
    public void register(String email, String password, Context context) throws UserException {

        RequestSender.RequestListeners<JSONObject> requestListeners = new RequestSender.RequestListeners<>(obj -> {
            EventBus.getDefault().post(new UserRegisterEvent());
        }, (error) -> {
            EventBus.getDefault().post(new UserRegisterEvent());
        });


        JSONObject body = new JSONObject();

        try {
            body.put("email", email);
            body.put("password", password);
        } catch (Exception e) {
            e.printStackTrace();
        }

        RequestSender.sendRequestForJsonObject(context, API_BASE_URL + "register", Request.Method.POST, body, null ,requestListeners);

    }
}
