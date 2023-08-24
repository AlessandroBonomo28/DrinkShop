package com.uninaproject.juicylemon.utils;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Pair;
import android.view.View;
import android.widget.EditText;

import androidx.appcompat.app.AlertDialog;
import androidx.coordinatorlayout.widget.CoordinatorLayout;

import com.google.android.material.snackbar.Snackbar;
import com.uninaproject.juicylemon.daos.TokenPayload;
import com.uninaproject.juicylemon.model.User;

import java.util.Collection;
import java.util.Optional;

public class Utils {

    /**
     * The base url of the API
     */
    public final static String API_BASE_URL = "http://192.168.1.36:80/api/";


    /**
     * Check if all the fields are not empty
     * @param fields the fields to check
     * @return true if all the fields are not empty, false otherwise
     */
    public static long getAllFieldsNotEmpty(Collection<? extends EditText> fields) {
        return fields.stream().filter(field -> field.getText().toString().isEmpty()).count();
    }

    /**
     * Show an alert with a custom message
     * @param context the context
     * @param message the message to show
     */
    public static void showAlert(Context context, String message) {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("Attenzione");
        builder.setMessage(message);
        builder.setPositiveButton("OK", null);
        builder.show();
    }

    /**
     * Show an alert with a custom title and message
     * @param context the context
     * @param title the title to be shown
     * @param message the message to show
     */
    public static void showAlert(Context context, String title, String message) {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle(title);
        builder.setMessage(message);
        builder.setPositiveButton("OK", null);
        builder.show();
    }

    public static User extractUserFromTokenPayload(TokenPayload tokenPayload) {
        return new User(tokenPayload.id, tokenPayload.email);
    }

    public static Pair<String, String> separateEmail(String email) {
        String[] splitted = email.split("@");
        return new Pair<>(splitted[0], splitted[1]);
    }

    public static String capitalize(String str) {
        return str.substring(0, 1).toUpperCase() + str.substring(1);
    }

    public static String addCurrencySymbol(String str) {
        return str + "€";
    }

    public static void addTokenToCache(String token, Context context ) {
        SharedPreferences sharedPref = context.getSharedPreferences("credentials", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPref.edit();

        editor.putString("token", token);
        editor.apply();
    }

    public static Optional<String> getTokenFromCache(Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("credentials", Context.MODE_PRIVATE);
        String token = sharedPref.getString("token", null);

        return Optional.ofNullable(token);
    }

    public static void showSnackbar(
            CoordinatorLayout coordinatorLayout,
            String message,
            int duration,
            String actionText,
            View.OnClickListener actionListener) {

        Snackbar snackbar = Snackbar.make(coordinatorLayout, message, duration);

        if (actionText != null && actionListener != null) {
            snackbar.setAction(actionText, actionListener);
        }

        snackbar.show();
    }

    public static String normalizePrice(String price) {
        String[] splittedPrice = price.split("\\.");

        // se non ci sono decimali aggiungo 00
        if (splittedPrice.length == 1)
            return price + "00";
        // se ci sono 2 decimali
        // e se il secondo decimale è vuoto aggiungo uno 0
        else if (splittedPrice[1].length() == 1)
            return price + "0";
        else
            return price;
    }
}
