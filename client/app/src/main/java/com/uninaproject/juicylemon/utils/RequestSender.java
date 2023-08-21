package com.uninaproject.juicylemon.utils;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.Pair;
import android.widget.ImageView;

import com.android.volley.Response;
import com.android.volley.toolbox.ImageRequest;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.StringRequest;

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

    public static void sendRequestForString(Context context, String url, int method, JSONObject body, RequestListeners<String> listeners) {
        StringRequest jsonObjectRequest = new StringRequest(
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

    public static void sendRequestForImage(Context context, String url, int method, JSONObject body, Pair<Integer, Integer> maxImageSize, RequestListeners<Bitmap> listeners) {
        ImageRequest jsonObjectRequest = new ImageRequest(
                url, listeners.responseListener, maxImageSize.first, maxImageSize.second, ImageView.ScaleType.CENTER, null, listeners.errorListener) {
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
