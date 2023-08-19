package com.uninaproject.juicylemon.utils;

import android.content.Context;

import com.android.volley.Response;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.JsonObjectRequest;

import org.json.JSONArray;
import org.json.JSONObject;

public class RequestSender {

    public static class RequestListeners<T> {

        Response.Listener<T> responseListener;

        Response.ErrorListener errorListener;

        public RequestListeners(Response.Listener<T> resposneListener, Response.ErrorListener errorListener) {
            this.responseListener = resposneListener;
            this.errorListener = errorListener;
        }

    }

    public static void sendRequestForJsonObject(Context context, String url, int method, JSONObject body, RequestListeners<JSONObject> listeners) {
        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(
                url, listeners.responseListener, listeners.errorListener) {

            @Override
            public int getMethod() {
                return method;
            }

            @Override
            public byte[] getBody() {
                return body.toString().getBytes();
            }
        };

        VolleyRequestHandler.getInstance(context).addToRequestQueue(jsonObjectRequest);
    }

    public static void sendRequestForJsonArray(Context context, String url, int method, JSONObject body, RequestListeners<JSONArray> listeners) {
        JsonArrayRequest jsonObjectRequest = new JsonArrayRequest(
                url, listeners.responseListener, listeners.errorListener) {

            @Override
            public int getMethod() {
                return method;
            }

            @Override
            public byte[] getBody() {
                return body.toString().getBytes();
            }
        };

        VolleyRequestHandler.getInstance(context).addToRequestQueue(jsonObjectRequest);
    }

}
